#include "DownloadFile.h"

autoPtrByteArr DownloadFile::handleRequest(const std::string& input_str) const
{
    struct stat sb;
    if( access( input_str.c_str(), F_OK ) == -1 ) {
        throw ServerExeption();
    }
    stat(input_str.c_str(), &sb);
    switch (sb.st_mode & S_IFMT) {
        case S_IFDIR:  throw ServerExeption();break;
    }
}
