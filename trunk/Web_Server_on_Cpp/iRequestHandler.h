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

private:
    void setBit(char &src, char index, bool val);
    bool getBit(char src, char index);
    std::string fromBoolVectorToSting(const std::vector<bool>& vec);
    std::vector<bool> fromStringToBoolVector(const std::string& str);
};

#endif // IREQUEST_HANDLER_H

