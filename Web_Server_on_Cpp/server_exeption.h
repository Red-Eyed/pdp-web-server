#ifndef SERVER_EXEPTION_H
#define SERVER_EXEPTION_H

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

class server_exeption: public std::exception
{
private:
    int rval;
    std::string message;
    int connection_fd;
public:
    server_exeption(const int inp_int, const std::string str);
    server_exeption();
    virtual ~server_exeption() throw() {};
    virtual const char* what() const throw();
};

#endif // SERVER_EXEPTION_H
