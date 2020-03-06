#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cstddef>
#include <sys/stat.h>
#include <iostream>
#include <filesystem>

struct file_cache {
    typedef std::size_t size_type;
         
#define DEFAULT_CACHE_SIZE 1048576UL*256UL
#define DEFAULT_MAX_FILE_SIZE 1048576UL*128UL
    
    FILE * curr_file;
    char * cache_buffer;
    std::vector<std::size_t> file_size;
    std::string my_prefix;
    size_type file_no;
    size_type my_max_file_size;
    size_type cache_size;
    bool am_writing;
    bool preserve;
    
    file_cache():curr_file(0x0),cache_buffer(0x0),file_size(),my_prefix(),file_no(0),
        my_max_file_size( DEFAULT_MAX_FILE_SIZE ),
        cache_size( DEFAULT_CACHE_SIZE ),am_writing(true),preserve(false) {
        cache_buffer = new char[cache_size];
        my_prefix = std::string("./tmp.");
    }

    explicit file_cache(const std::string& prefix):
        curr_file(0x0),cache_buffer(0x0),file_size(),my_prefix(prefix),file_no(0),
        my_max_file_size( DEFAULT_MAX_FILE_SIZE ),
        cache_size( DEFAULT_CACHE_SIZE ),am_writing(true),preserve(false)  {
        cache_buffer = new char[cache_size];
    }
    
    
    explicit file_cache(const std::string& prefix,size_type max_file_size,
        size_type cache_size_in= DEFAULT_CACHE_SIZE ):
        curr_file(0x0),cache_buffer(0x0),file_size(),my_prefix(prefix),file_no(0),
        my_max_file_size(max_file_size),
        cache_size(cache_size_in),am_writing(true),preserve(false)  {
        cache_buffer = new char[cache_size];
    }
    
    ~file_cache() {
        fclose(curr_file);
        if (preserve) {
           /// dump cache_buffer to file
            if (file_size.size() > 0) {
                fclose(curr_file);
                std::string fname(make_filename(0));
                std::size_t sz = file_size[0];         
                curr_file = fopen(fname.c_str(),"w");
                fwrite(cache_buffer,sizeof(char),sz,curr_file);
                fflush(curr_file);
                fclose(curr_file);
            }           
        }else{
            if (file_size.size() > 1) {
                for (std::size_t k=0;k<file_size.size();++k) {
                    std::string fname = make_filename(k);
                    /// REMOVE FILE ///
                    std::cerr << "removing file " << fname << "\n";
                }
            }            
        }
        delete [] cache_buffer;
        curr_file=0x0;    
    };
    
    void load_from_files()
    {
        if (curr_file) {
            fclose(curr_file);
        }
        struct stat st;
        std::string fname(make_filename(0));
        int e = stat(fname.c_str(),&st);
        if (e) {
            perror("error opening mem cache file");
            exit(EXIT_FAILURE);
        }
        size_type bsz = st.st_size;
        if (bsz == 0) {
            std::cerr << "error the memory cache is empty\n";  
        }else{
        if (bsz != cache_size) {
            cache_size = bsz;
            if (cache_buffer) delete [] cache_buffer;
            cache_buffer = new char[bsz];
        }
        file_size.clear();
        curr_file = fopen(fname.c_str(),"r");
        long rd= fread(cache_buffer,sizeof(char),bsz,curr_file);
        fclose(curr_file); 
        file_size.push_back(bsz);
        }
        std::size_t k = 1;
        do {
            std::string fname(make_filename(k));
            int e = stat(fname.c_str(),&st);
            if (e) break;
            size_type fsz = st.st_size;            
            file_size.push_back(fsz);
            ++k;
        } while (1); 
        open_for_reading();
    }
    
    void set_preserve() {
        preserve = true;
    }
    

    void rewind() {
        close();         
        if (am_writing) {
            open_for_writing();
        }else{
            open_for_reading();
        }
    }
    
    template < typename T >
    std::size_t read(T *buffer,std::size_t count) {
        size_type rd;
        
        if (curr_file && !am_writing) {
            if (file_no >= file_size.size()) {
                std::cerr << "Reading files that do not exist!\n";
                std::cerr << "file no = " << file_no << " of " << file_size.size() << "\n";
            } 
            int64_t cpos = ftell(curr_file);
            int64_t cend = file_size[file_no];
            std::cerr << "read " << count << " file_pos = " << cpos << " file_end = " << cend << "\n";
            int64_t av_rd = cend - cpos;
            if (av_rd < 0) {
                std::cerr << "past eof exception on read\n";
                std::cerr << "curr pos in file " << file_no << " is " << cpos << "\n";
                std::cerr << "file size is " << cend << "\n";
                exit(1);
            }
            std:size_t av_count = av_rd/sizeof(T);
            if (av_count) {
                av_count = (av_count >= count) ? count:av_count;
                rd = fread(buffer,sizeof(T),av_count,curr_file);
                if (rd==count) return rd;
                if (rd!=av_count) {
                    std::cerr << "did not read all " << rd << " of " << av_count << "\n";
                    return rd;
                }
                count -= rd;
            }
            open_new_file_for_reading();
            rd += this->read(buffer+rd,count);
            return rd;
        }else{
            open_for_reading();
            return this->read(buffer,count);
        } 
    };

    template < typename T >
    std::size_t write(T *buffer,std::size_t count) {
        size_type wr;
        
        size_type tot_sz = sizeof(T)*count;

        std::cerr<<"total size = " << tot_sz << "\n";
        
        if (curr_file && am_writing) {
            int64_t cpos = ftell(curr_file);
            int64_t cend = my_max_file_size;
            std::cerr << "write " << count << " file_pos = " << cpos << " file_end = " << cend << "\n";
            int64_t avail_wr = cend - cpos;
            if (avail_wr < 0) {
                std::cerr << "past eof exception on write\n";
                std::cerr << "curr pos in file " << file_no << " is " << cpos << "\n";
                std::cerr << "file size is " << my_max_file_size << "\n";
                exit(1);
            }
            std:size_t av_count = avail_wr/sizeof(T);
            std::cerr << "can write " << av_count << "\n";
            if (av_count) {
                av_count = (av_count > count) ? count:av_count;
                std::cerr << "should write " << av_count << "\n";
                wr = fwrite(buffer,sizeof(T),av_count,curr_file);
                std::cerr << "wrote " << wr << "\n"; 
                if (wr==count) return wr;
                if (wr!=av_count) {
                    std::cerr << "did not write all " << wr << " of " << av_count << "\n";
                    return wr;
                }
                count -= av_count;
            }
            open_new_file_for_writing();
            wr += this->write(buffer+wr,count);
            std::cerr << "finally wrote " << wr << "\n";
            return wr;
        }else{
            open_for_writing();
            return this->write(buffer,count);
        } 
    };
    
    void close() {
         if (curr_file) {
             if (am_writing)  {
                 size_type p = ftell(curr_file);
                 file_size.push_back(p);
             }
             fclose(curr_file);    
         }   
         am_writing = false;
         file_no = 0;
         curr_file = 0x0;
    }
    void open_for_writing() {
        am_writing = true;
        // reset everything
        file_no = 0;
        file_size.clear();
        curr_file = fmemopen(cache_buffer,cache_size,"w");
    };
    void open_for_reading() {
         if (am_writing && curr_file) {
            size_type p = ftell(curr_file);
            file_size.push_back(p);
            fclose(curr_file);                  
         }
         
         std::cerr << "There are " << file_size.size() << " files to read\n";
         for (size_type k = 0; k< file_size.size(); ++k) {
             std::cerr << "file no = " << k << " size = " << file_size[k] << "\n";
         }
         
         am_writing = false; 
         file_no = 0;
         curr_file = fmemopen(cache_buffer,cache_size,"r");                           
    };
private:
    inline std::string&& make_filename(std::size_t file_no) const noexcept
    {
        std::string fname(my_prefix);
        fname += std::to_string(file_no) + ".cache";
        return std::move(fname);
    }
    
    void open_new_file_for_reading() {
        if (am_writing) return open_for_reading();
        if (curr_file) fclose(curr_file);
        ++file_no;
        std::string fname = make_filename(file_no);
        std::cerr << "opening new file " << file_no << " " << fname << "\n";
        curr_file = fopen(fname.c_str(),"r");
    }
    void open_new_file_for_writing() {
        if (!am_writing) return open_for_writing();
        if (curr_file) {
            size_type p = ftell(curr_file);
            file_size.push_back(p); 
            fclose(curr_file);
        }
        ++file_no;
        if (file_no > 8UL) {
            std::cerr << "error file no > 128\n";
            exit(1);
        }
        std::string fname = make_filename(file_no);
        std::cerr << "opening new file " << file_no << " " << fname << "\n";
        curr_file = fopen(fname.c_str(),"w");
    }
};


int test_it()
{
//    std::filesystem::path tmp_path="./tmp";
//    std::filesystem::create_directories(tmp_path);
    std::string prefix="./tmp/";
    std::size_t max_file_size = 1024UL*12UL; 
    std::size_t cache_size = 1024UL*12UL;
    file_cache fc(prefix,max_file_size,cache_size);
    std::size_t bfsize = max_file_size * 2UL + 51;
    std::size_t bsize = bfsize/sizeof(int32_t);
    int32_t *wbuff = new int32_t[bsize];
    int32_t *rbuff = new int32_t[bsize];
    
    fc.set_preserve();
    std::size_t cnt1 = 100;
    std::cerr << "will write " << cnt1 << " ints\n";
    for (size_t k=0;k<bsize;++k) {
        wbuff[k]=(k+1)%69;
    }
    fc.open_for_writing();
    fc.write(wbuff,cnt1);
    fc.close();
    fc.open_for_reading();
    fc.read(rbuff,cnt1);
    for (size_t k=0;k<100;++k) {
        int32_t e = (k+1)%69;
        if (e!=rbuff[k]) {
            std::cerr << " error in read 1 " << rbuff[k] << " not equal " << e << "\n";
        }
    }     
    std::cerr << "first read done!\n";
    fc.close();
    fc.open_for_writing();
    fc.write(wbuff,bsize);
    fc.close();
    fc.open_for_reading();
    fc.read(rbuff,bsize);
    for (size_t k=0;k<bsize;++k) {
        int32_t e = (k+1)%69;
        if (e!=rbuff[k]) {
            std::cerr << " error in read 2 " << rbuff[k] << " not equal " << e << "\n";
        }
    }
    std::cerr << "second read done\n";
    return 0;     
}

int test_it2()
{
    std::string prefix="./tmp/";
    std::size_t max_file_size = 1024UL*12UL; 
    std::size_t cache_size = 1024UL*12UL;
    file_cache fc(prefix,max_file_size,cache_size);
    std::size_t bfsize = max_file_size * 2UL + 51;
    std::size_t bsize = bfsize/sizeof(int32_t);
    int32_t *wbuff = new int32_t[bsize];
    int32_t *rbuff = new int32_t[bsize];
    
    fc.set_preserve();
    fc.load_from_files();
    fc.open_for_reading();
    fc.read(rbuff,bsize);
    for (size_t k=0;k<bsize;++k) {
        int32_t e = (k+1)%69;
        if (e!=rbuff[k]) {
            std::cerr << " error in read 3 " << rbuff[k] << " not equal " << e << "\n";
        }
    }
    std::cerr << "third read done\n";
    return 0;     


}


int main()
{      
   int e = test_it();
   return (test_it2()+e);
}