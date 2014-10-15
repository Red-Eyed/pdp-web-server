#include "download_file.h"

std::auto_ptr<std::vector<bool> > Download_file::handle_request(const std::string& input_str) const
{
    std::ifstream f(input_str.c_str());
    if (!f.good()){
        throw server_exeption();
    }
}
