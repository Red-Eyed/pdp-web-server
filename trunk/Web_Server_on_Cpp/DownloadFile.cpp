#include "DownloadFile.h"
#include "FileDescriptor.h"
#include <fcntl.h>
#include <stdio.h>

void DownloadFile::handleRequest(const std::string& inputStr, std::vector<char>& out) const
{
    struct stat fsStat;
    int ret = access( inputStr.c_str(), F_OK );
    if( ret == -1 ) {
        throw ServerExeption(ret, "file not exist ", __FUNCTION__, __LINE__ );
    }

    stat(inputStr.c_str(), &fsStat);
    if ((fsStat.st_mode & S_IFMT) == S_IFDIR) {
        throw ServerExeption(0, "it is a directory not a file ", __FUNCTION__, __LINE__ );
    }

    FileDescriptor fdFile(inputStr, O_RDONLY);

    stat(inputStr.c_str(), &fsStat);
    out.reserve(fsStat.st_size);
    size_t readRet = 0;
    readRet = read(fdFile.getFd(), &out[0], fsStat.st_size);

    if(readRet == static_cast<size_t>(-1) || readRet != static_cast<size_t>(fsStat.st_size)){
        throw ServerExeption(readRet, "read error ", __FUNCTION__, __LINE__ );
    }
}
