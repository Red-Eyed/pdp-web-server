#ifndef SERVER_EXEPTION_H
#define SERVER_EXEPTION_H

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <string>

class ServerExeption: public std::exception
{
public:
    ServerExeption();
    ServerExeption(long long inpInt, const std::string& str, const std::string& fun, unsigned long int line);
    ServerExeption(const std::string& str, const std::string& fun, unsigned long int line);
    virtual ~ServerExeption() throw();
    virtual const char* what() const throw();

private:
    const int             m_Rval;
    std::string           m_Message;
};

#endif // SERVER_EXEPTION_H
