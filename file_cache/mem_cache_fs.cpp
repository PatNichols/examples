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
    
    file_cache():curr_file(nullptr),cache_buffer(nullptr),file_size(),my_prefix(),file_no(0),
        my_max_file_size( DEFAULT_MAX_FILE_SIZE ),
        cache_size( DEFAULT_CACHE_SIZE ),am_writing(true),preserve(false) {
        cache_buffer = new char[cache_size];
        my_prefix = std::string("./tmp/tmp_file.");
    }

    explicit file_cache(const std::string& prefix):
        curr_file(nullptr),cache_buffer(nullptr),file_size(),my_prefix(prefix),file_no(0),
        my_max_file_size( DEFAULT_MAX_FILE_SIZE ),
        cache_size( DEFAULT_CACHE_SIZE ),am_writing(true),preserve(false)  {
        cache_buffer = new char[cache_size];
        my_prefix += "/tmp_file.";
    }
    
    
    explicit file_cache(const std::string& prefix,size_type max_file_size,
        size_type cache_size_in= DEFAULT_CACHE_SIZE ):
        curr_file(nullptr),cache_buffer(nullptr),file_size(),my_prefix(prefix),file_no(0),
        my_max_file_size(max_file_size),
        cache_size(cache_size_in),am_writing(true),preserve(false)  {
        cache_buffer = new char[cache_size];
        my_prefix += "/tmp_file.";
    }
    
    ~file_cache() {
        fclose(curr_file);
        if (preserve) {
           /// dump cache_buffer to file
            if (file_size.size() > 0) {
                fclose(curr_file);
                std::string fname = my_prefix + std::to_string(0);
                std::size_t sz = file_size[0];         
                curr_file = fopen(fname.c_str(),"w");
                fwrite(cache_buffer,sizeof(char),sz,curr_file);
                fflush(curr_file);
                fclose(curr_file);
            }           
        }else{
            if (file_size.size() > 1) {
                for (std::size_t k=0;k<file_size.size();++k) {
                    std::string fname = my_prefix + std::to_string(k);
                    /// REMOVE FILE ///
                    std::cerr << "removing file " << fname << "\n";
                }
            }            
        }
        delete [] cache_buffer;
        curr_file=nullptr;    
    };
    
    
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


    int64_t Fread(void *ptr,size_type sz,size_type cnt,FILE *fp)
    {
        int64_t nr = fread(ptr,sz,cnt,fp);
        if (nr!=cnt) {
            std::cerr << "could not read " << cnt << " only read " << nr << "\n";
        }
        return nr;
    }
    int64_t Fwrite(const void *ptr,size_type sz,size_type cnt,FILE *fp)
    {
        int64_t nw = fwrite(ptr,sz,cnt,fp);
        if (nw!=cnt) {
            std::cerr << "could not write " << cnt << " only wrote " << nw << "\n";
        }
        return nw;
    }

     
    template < typename T >
    std::size_t read(T *buffer,std::size_t count) {
        size_type rd=0;
        
        if (curr_file && !am_writing) {
            if (file_no >= file_size.size()) {
                std::cerr << "Reading files that do not exist!\n";
                std::cerr << "file no = " << file_no << " of " << file_size.size() << "\n";
                exit(EXIT_FAILURE);
            } 
            int64_t cpos = ftell(curr_file);
            int64_t cend = file_size[file_no];
            std::cerr << "read " << count << " file_pos = " << cpos << " file_end = " << cend << "\n";
            int64_t av_count = (cend - cpos)/sizeof(T);
            if (av_count < 0) {
                std::cerr << "past eof exception on read\n";
                std::cerr << "curr pos in file " << file_no << " is " << cpos << "\n";
                std::cerr << "file size is " << cend << "\n";
                exit(EXIT_FAILURE);
            }
            if (av_count >= count) {
                std::cerr << "reading all of the objects " << count << "\n";
                return Fread(buffer,sizeof(T),count,curr_file);
            }            
            if (av_count) {
                rd = Fread(buffer,sizeof(T),av_count,curr_file);
                if (rd!=av_count) {
                    std::cerr << "did not read all " << rd << " of " << av_count << "\n";
                    return rd;
                }
            }
            open_new_file_for_reading();
            rd += this->read(buffer+rd,count-rd);
            return rd;
        }else{
            std::cerr << "error trying to read with no current file or in write mode\n";
            std::cerr << "am writing = " << am_writing << "\n";
            exit(EXIT_FAILURE);            
        } 
    };

    template < typename T >
    std::size_t write(T *buffer,std::size_t count) {
        size_type wr;
        

        if (curr_file && am_writing) {
            int64_t cpos = ftell(curr_file);
            int64_t cend = my_max_file_size;
            std::cerr << "write " << count << " file_pos = " << cpos << " file_end = " << cend << "\n";
            int64_t av_wr = (cend - cpos)/sizeof(T);
            if (av_wr < 0) {
                std::cerr << "past eof exception on write\n";
                std::cerr << "curr pos in file " << file_no << " is " << cpos << "\n";
                std::cerr << "file size is " << my_max_file_size << "\n";
                exit(1);
            }
            if (av_wr >= count) {
                wr = fwrite(buffer,sizeof(T),count,curr_file);            
                if (wr!=count) {
                    std::cerr << "did not write all " << wr << " of " << count << "\n";
                }
                return wr;                
            }
            if (av_wr) {
                std::cerr << "should write " << av_wr << "\n";
                wr = fwrite(buffer,sizeof(T),av_wr,curr_file);
                std::cerr << "wrote " << wr << "\n"; 
                if (wr!=av_wr) {
                    std::cerr << "did not write all " << wr << " of " << av_wr << "\n";
                    return wr;
                }
            }
            open_new_file_for_writing();
            wr += this->write(buffer+wr,count-wr);
            std::cerr << "finally wrote " << wr << "\n";
            return wr;
        }else{
            std::cerr << "error trying to write with no current file or out of write mode\n";
            std::cerr << "am writing = " << am_writing << "\n";
            exit(EXIT_FAILURE);
        } 
    };
    
    void close() {
         if (curr_file) {
             if (am_writing)  {
                 file_size.push_back(ftell(curr_file));
             }
             fclose(curr_file);    
         }   
         am_writing = false;
         file_no = 0;
         curr_file = nullptr;
    }
    void open_for_writing() {
        if (curr_file) fclose(curr_file);
        am_writing = true;
        // reset everything
        file_no = 0;
        file_size.clear();
        curr_file = fmemopen(cache_buffer,cache_size,"w");
    };
    void open_for_reading() {
         if (curr_file) close();
         std::cerr << "There are " << file_size.size() << " files to read\n";
         for (size_type k = 0; k< file_size.size(); ++k) {
             std::cerr << "file no = " << k << " size = " << file_size[k] << "\n";
         }         
         am_writing = false; 
         file_no = 0;
         curr_file = fmemopen(cache_buffer,cache_size,"r");                           
    };   
    void load_from_files()
    {
        file_size.clear();
        close();
        std::string fname = my_prefix + std::to_string(0);
        curr_file = fopen(fname.c_str(),"r");
        if (!curr_file) {
            std::cerr << "could not open the mem cache file " << fname << "\n";
            exit(EXIT_FAILURE);
        }
        fseek(curr_file,0,SEEK_END);
        size_t p = ftell(curr_file);
        cache_buffer = new char[p];
        fseek(curr_file,0,SEEK_SET);
        fread(cache_buffer,sizeof(char),p,curr_file);
        fclose(curr_file);
        file_size.push_back(p);
        int64_t k = 1;
        while (1) {
            fname = my_prefix + std::to_string(k);
            curr_file = fopen(fname.c_str(),"r");
            if (!curr_file) {
                std::cerr << "could not open the file " << fname << "\n";
                errno = 0;
                curr_file = nullptr;
                std::cerr << "breaking now\n";
                break;
            }
            fseek(curr_file,0,SEEK_END);
            p = ftell(curr_file);
            file_size.push_back(p); 
            fclose(curr_file);
            std::cerr << " file " << fname << " size = " << p << "\n";
            ++k;
        }
        std::cerr << "there are " << file_size.size() << " files\n";
        open_for_reading();
    }
    
    
private:    
    void open_new_file_for_reading() {
        if (am_writing) return open_for_reading();
        if (curr_file) fclose(curr_file);
        ++file_no;
        std::string fname = my_prefix + std::to_string(file_no);
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
        std::string fname = my_prefix + std::to_string(file_no);
        std::cerr << "opening new file " << file_no << " " << fname << "\n";
        curr_file = fopen(fname.c_str(),"w");
    }
};


int test_it()
{
    std::string prefix="./tmp";
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
    std::cerr << "first read done and test passed!\n";
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
    std::cerr << "TEST2 !!!!!!!\n";
    std::string prefix="./tmp";
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