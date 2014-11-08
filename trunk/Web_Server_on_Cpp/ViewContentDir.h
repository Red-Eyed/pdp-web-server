#ifndef VIEW_CONTENT_DIR_H
#define VIEW_CONTENT_DIR_H

#include "iRequestHandler.h"
#include <sys/stat.h>
#include <unistd.h>

class ViewContentDir : public iRequestHandler
{
public:

    virtual void handleRequest(const std::string& input_str, std::vector<char>& out) const;
};


#endif // VIEW_CONTENT_DIR_H
