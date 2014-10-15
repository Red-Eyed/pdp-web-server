#include "server_exeption.h"

server_exeption::server_exeption(const int inp_int, const std::string str):
    rval(inp_int), message(str)
{}

server_exeption::server_exeption():
    rval(0), message("Error")
{}

const char* server_exeption::what() const throw(){
    std::ostringstream ss;
    ss << rval;
    std::string temp_str("Error code( " + ss.str() + " ) " + message + "\n");
    return temp_str.c_str();
}
