#include "ServerExeption.h"

ServerExeption::ServerExeption(int inp_int, const std::string& str):
    m_Rval(inp_int), m_Message(str)
{}

ServerExeption::ServerExeption():
    m_Rval(0), m_Message("Error")
{}

ServerExeption::~ServerExeption() throw(){

}


const char* ServerExeption::what() const throw(){
    std::ostringstream ss;
    ss << m_Rval;
    std::string TmpStr("Error code( " + ss.str() + " ) " + m_Message + "\n");
    return TmpStr.c_str();
}
