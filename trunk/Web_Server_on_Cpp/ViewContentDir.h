#ifndef VIEW_CONTENT_DIR_H
#define VIEW_CONTENT_DIR_H

#include "iRequestHandler.h"
#include <sys/stat.h>
#include <unistd.h>

class ViewContentDir : public iRequestHandler
{
public:

    virtual autoPtrByteArr handleRequest(const std::string& input_str) const;

private:

    void view_files(const char *folderPath);
    void view_folder(const char *filePath);
};

#endif // VIEW_CONTENT_DIR_H
