#include <stdexcept>
#include <fcntl.h>
#include <stdio.h>

#include "Utils.h"
#include "DownloadFile.h"
#include "FileDescriptor.h"

void DownloadFile::handleRequest(const std::string& inputStr, std::vector<char>& out) const
{
    struct stat fsStat;
    int ret = access( inputStr.c_str(), F_OK );
    if( ret == -1 ) {
        throw std::runtime_error(createString(ret, "file not exist ", __FUNCTION__, __LINE__ ));
    }

    stat(inputStr.c_str(), &fsStat);
    if ((fsStat.st_mode & S_IFMT) == S_IFDIR) {
        throw std::runtime_error(createString(0, "it is a directory not a file ", __FUNCTION__, __LINE__) );
    }

//    fdFile (inputStr, O_RDONLY);
    int fd = open(inputStr.c_str(), O_RDONLY);
    out.resize(2);
    out[0] = 0;
    out[1] = static_cast<char>(fd);

//    stat(inputStr.c_str(), &fsStat);
//    out.resize(fsStat.st_size);
//    size_t readRet = 0;
//    readRet = read(fdFile.getFd(), &out[0], fsStat.st_size);

//    if(readRet == static_cast<size_t>(-1) || readRet != static_cast<size_t>(fsStat.st_size)){
//        throw std::runtime_error(createString(readRet, "read error ", __FUNCTION__, __LINE__ ));
//    }
}
