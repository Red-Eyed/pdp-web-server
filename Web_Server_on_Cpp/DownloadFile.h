#ifndef DOWNLOAD_FILE_H
#define DOWNLOAD_FILE_H

#include "iRequestHandler.h"
#include <sys/stat.h>
#include <unistd.h>

class DownloadFile : public iRequestHandler
{
public:
    virtual autoPtrByteArr handleRequest(const std::string& input_str) const;
};

#endif // DOWNLOAD_FILE_H
