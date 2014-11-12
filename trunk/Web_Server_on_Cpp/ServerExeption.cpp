#include "ServerExeption.h"

ServerExeption::ServerExeption(long long inpInt, const std::string& str, const std::string& fun, unsigned long int line):
    m_Rval(inpInt), m_Message(str + "function(" + fun + ") line(")
{
    std::ostringstream ss;
    ss << line;
    m_Message += ss.str() + ")";
}

ServerExeption::ServerExeption(const std::string& str, const std::string& fun, unsigned long line):
    m_Rval(0), m_Message(str + "function(" + fun + ") line(")
{
    std::ostringstream ss;
    ss << line;
    m_Message += ss.str() + ")";
}

ServerExeption::ServerExeption():
    m_Rval(0), m_Message("Error")
{}

ServerExeption::~ServerExeption() throw(){

}

const char* ServerExeption::what() const throw(){
    std::ostringstream ss;
    ss << m_Rval;
    std::string TmpStr("Error code(" + ss.str() + ") " + m_Message + "\n");
    return TmpStr.c_str();
}

