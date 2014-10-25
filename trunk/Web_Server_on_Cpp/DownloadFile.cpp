#include "DownloadFile.h"
#include <fcntl.h>

autoPtrStr DownloadFile::handleRequest(const std::string& input_str) const
{
    struct stat sb;
    if( access( input_str.c_str(), F_OK ) == -1 ) {
        throw ServerExeption();
    }
    stat(input_str.c_str(), &sb);
    switch (sb.st_mode & S_IFMT) {
        case S_IFDIR:  throw ServerExeption();break;
    }
    int fd_file = open(input_str.c_str(), O_RDONLY);
    struct stat st;
    stat(input_str.c_str(), &st);
    std::auto_ptr<char> buf(new char[st.st_size]);
    read(fd_file, buf.get(), st.st_size);
    return autoPtrStr(new std::string(buf.get(), st.st_size));
}
