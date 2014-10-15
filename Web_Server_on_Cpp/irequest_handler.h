#ifndef IREQUEST_HANDLER_H
#define IREQUEST_HANDLER_H

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "server_exeption.h"


class iRequest_handler
{
public:
    virtual std::auto_ptr<std::vector<bool> > handle_request(const std::string& input_str) const = 0;
};

#endif // IREQUEST_HANDLER_H
