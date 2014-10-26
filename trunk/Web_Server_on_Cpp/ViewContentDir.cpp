#include "ViewContentDir.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#define __DIR__ 4
#define __LINK__ 10

static const std::string pageStart =
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\"\n\"http://www.w3.org/TR/html4/strict.dtd\">\n"
        "<html>\n"
        "\t<head>\n"
        "\t<title>!DOCTYPE</title>\n"
        "\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
        "\t</head>\n"
        "\t<body>\n"
        "\t\t<pre>\n";

static const std::string pageEnd = "\t\t</pre>\n"
        "\t</body>\n"
        "</html>\n\n";


autoPtrStr ViewContentDir::handleRequest(const std::string& input_str) const{

    autoPtrStr retStr(new std::string);

    *retStr += pageStart;
    try{
        *retStr += *viewFolders(input_str);
        *retStr += *viewFiles(input_str);
    }
    catch(std::exception& e){
        std::cerr << e.what();
    }

    *retStr += pageEnd;
    return retStr;

}

const autoPtrStr viewFolders(const std::string& folder){
    DIR * dir;
    struct dirent* de;
    autoPtrStr foldersList(new std::string);
    size_t sizeOfLinkPath = 256;
    char linkPath[sizeOfLinkPath];
    memset(linkPath, 0, sizeOfLinkPath);
    if ((dir = opendir(folder.c_str())) == NULL) {
        throw(ServerExeption(0, "opendir failed!"));
    }
    else {
        while (de = readdir(dir)) {
            if (de->d_type == __DIR__) {
                if (de->d_name[0] == '.' && de->d_name[1] == '\0') {
                    continue;
                }
                *foldersList += "<a href=\"" +
                        folder +
                        std::string(de->d_name) +
                        "/\">[Dir] " +
                        de->d_name +
                        "</a> \n";
            }
            if(de->d_type == __LINK__){
                size_t size = folder.length() + strlen(de->d_name) + 1;
                char abs_path[size];
                sprintf(abs_path, "%s%s", folder.c_str(), de->d_name);
                abs_path[size] = '\0';
                int end_symbols = readlink(abs_path, linkPath, sizeOfLinkPath);
                linkPath[end_symbols] = '\0';
                *foldersList += "<a href=\"" +
                        std::string(abs_path) +
                        "/\">[Link] " +
                        std::string(de->d_name) +
                        "</a> \n";
            }
        }
        closedir(dir);
    }
    return foldersList;
}

const autoPtrStr viewFiles(const std::string& folder){
    DIR* dir;
    struct dirent* de;
    autoPtrStr filesList(new std::string);
    if ((dir = opendir(folder.c_str())) == NULL) {
        throw(ServerExeption(0, "opendir failed!"));
    }
    else {
        while (de = readdir(dir)) {
            if (de->d_type == __DIR__ || de->d_type == __LINK__) {
                continue;
            }
            else {
                *filesList += "<a href=\"" +
                        folder +
                        std::string(de->d_name) +
                        "\" >[File] " +
                        std::string(de->d_name) +
                        " </a> \n";
            }
        }
        closedir(dir);
    }
    return filesList;
}
