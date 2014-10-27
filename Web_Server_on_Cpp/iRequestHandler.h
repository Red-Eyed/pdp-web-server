#ifndef IREQUEST_HANDLER_H
#define IREQUEST_HANDLER_H

#include <vector>
#include <memory>
#include "ServerExeption.h"


typedef std::auto_ptr<std::string> autoPtrStr;

class iRequestHandler
{
public:
    virtual autoPtrStr handleRequest(const std::string& input_str) const = 0;
    virtual ~iRequestHandler() throw() {}

};

#endif // IREQUEST_HANDLER_H

