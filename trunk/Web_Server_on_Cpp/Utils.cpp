#include <sstream>

#include "Utils.h"

std::string createString(int err, const std::string& msg, const std::string& func, int line){
    std::ostringstream ss;
    ss << line;
    std::string strLine = ss.str();
    ss << err;
    std::string strError = ss.str();
    std::string message = "Error code(" + strError + ") " + msg + " function(" + func + ") " + "line(" + strLine + ")\n";
    return message;
}
