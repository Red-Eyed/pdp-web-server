#ifndef IREQUEST_HANDLER_H
#define IREQUEST_HANDLER_H

#include <vector>
#include <memory>
#include "ServerExeption.h"

class iRequestHandler
{
public:
    virtual void handleRequest(const std::string& input_str, std::vector<char>& out) const = 0;
    virtual ~iRequestHandler() throw() {}

};

#endif // IREQUEST_HANDLER_H

