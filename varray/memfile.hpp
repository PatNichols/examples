


///
//  A file with a cache that can be filled before actually touching file
//
//
///
template < class char_t >
struct memfile {
    typedef char_t char_type;
    typedef std::size_t size_type;
    typedef std::
    
    std:;stringstream sbuffer;
    std::fstream sfile;
    size_type max_size;
    bool write_to_file;
    
    memfile():sbuffer(),sfile(),max_size(DEFAULT_BUFFER_SIZE),write_to_file(false) {}
    memfile(const memfile& mfile):sbuffer(mfile.sbuffer),sfile(mfile.sfile),max_size(mfile.max_size),write_to_file(mfile.write_to_file){}
    
    void SetBufferSize(size_type bsize) { max_size = bsize;}
    
    memfile& write(void * data, size_type data_size) {
        if (write_to_file) {
            sfile.write(data,data_size);
            return *this;
        }else{
            size_type end = sbuffer.tellg() + data_size;
            if (end >= max_size) {
               write_to_file = true;
               sfile.write(data,data_size);
            }else{
               sbuffer.write(data,data_size);
            }
        } 
        return *this;
    }
        
    memfile& read(void * data, size_type data_size) {
        if (write_to_file) {
            sfile.read(data,data_size);
            return *this;
        }else{
            size_type end = sbuffer.tellp() + data_size;
            if (end >= max_size) {
               write_to_file = true;
               sfile.write(data,data_size);
            }else{
               sbuffer.write(data,data_size);
            }
        } 
        return *this;
    }
        


};