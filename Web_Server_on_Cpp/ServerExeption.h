#ifndef SERVER_EXEPTION_H
#define SERVER_EXEPTION_H

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

class ServerExeption: public std::exception
{
public:
    ServerExeption(int inp_int, const std::string& str);
    ServerExeption();
    virtual ~ServerExeption() throw();
    virtual const char* what() const throw();

private:
    const int             m_Rval;
    const std::string     m_Message;
};

#endif // SERVER_EXEPTION_H
