#include "ViewContentDir.h"

autoPtrByteArr ViewContentDir::handleRequest(const std::string& input_str) const{
    //    struct stat sb;
    //    if( access( input_str.c_str(), F_OK ) == -1 ) {
    //        throw ServerExeption();
    //    }
    //    stat(input_str.c_str(), &sb);
    //    switch (sb.st_mode & S_IFMT) {
    //        case S_IFREG:  throw ServerExeption();break;
    //        case S_IFLNK:  throw ServerExeption();break;
    //    }
}

void view_folder(const char *folder){
    DIR * dir;
    struct dirent* de;
    size_t size_of_link_path = 256;
    char link_path[size_of_link_path];
    if ((dir = opendir(folder)) == NULL) {
        throw(ServerExeption(0, "opendir failed!"));
    }
    else {
        while (de = readdir(dir)) {
            if (de->d_type == __DIR__) {
                if (de->d_name[0] == '.' && de->d_name[1] == '\0') {
                    continue;
                }
                printf("<a href=\"%s/%s\">[Dir] %s</a> \n", folder, de->d_name,
                       de->d_name);
            }
            if(de->d_type == __LINK__){
                size_t size = strlen(folder) + strlen(de->d_name) + 1;
                char abs_path[size];
                sprintf(abs_path, "%s%s", folder, de->d_name);
                abs_path[size] = 0;
                int end_symbols = readlink(abs_path, link_path, size_of_link_path);
                link_path[end_symbols] = '\0';
                printf("<a href=\"%s\">[Link]%s</a> \n", link_path,
                       de->d_name);
            }
        }
        closedir(dir);
    }
}

void view_files(const char *folder){
    DIR * dir;
    struct dirent * de;
    if ((dir = opendir(folder)) == NULL) {
        throw(ServerExeption(0, "opendir failed!"));
    }
    else {
        while (de = readdir(dir)) {
            if (de->d_type == __DIR__ || de->d_type == __LINK__) {
                continue;
            }
            else {
                printf("<a href=\"%s/%s\" >[File] %s</a> \n", folder, de->d_name,
                       de->d_name);
            }
        }
        closedir(dir);
    }
}
