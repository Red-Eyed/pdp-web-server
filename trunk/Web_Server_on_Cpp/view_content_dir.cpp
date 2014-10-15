#include "view_content_dir.h"

std::auto_ptr<std::vector<bool> > View_content_dir::handle_request(const std::string& input_str) const{
    std::ifstream f(input_str.c_str());
    if (!f.good()){
        throw server_exeption();
    }
}
