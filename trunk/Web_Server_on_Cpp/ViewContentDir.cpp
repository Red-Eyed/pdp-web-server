#include "ViewContentDir.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

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

static const std::string pageEnd =
        "\t\t</pre>\n"
        "\t</body>\n"
        "</html>\n\n";

const autoPtrStr viewFolders(const std::string& path, const std::string& tabs);
const autoPtrStr viewFiles(const std::string& path, const std::string& tabs);

autoPtrStr ViewContentDir::handleRequest(const std::string& inputStr) const{

    if(opendir(inputStr.c_str()) > 0){
        autoPtrStr retStr(new std::string);

        *retStr += pageStart;
        try{
            *retStr += *viewFolders(inputStr, "");
            *retStr += *viewFiles(inputStr, "");
        }
        catch(std::exception& e){
            std::cerr << e.what();
        }

        *retStr += pageEnd;
        return retStr;
    }
    else{
        throw ServerExeption(-1, "Open dir error");
    }

}

const autoPtrStr viewFolders(const std::string& path, const std::string& tabs){
    DIR * dir;
    struct dirent* de;
    autoPtrStr foldersList(new std::string);
    size_t sizeOfLinkPath = 256;
    char linkPath[sizeOfLinkPath];
    memset(linkPath, 0, sizeOfLinkPath);
    if ((dir = opendir(path.c_str())) == NULL) {
        throw(ServerExeption(0, "opendir failed!"));
    }
    else {
        while (de = readdir(dir)) {
            if (de->d_type == __DIR__) {
                if (de->d_name[0] == '.' && de->d_name[1] == '\0') {
                    continue;
                }
                *foldersList += tabs +"<a href=\"" +
                        path +
                        std::string(de->d_name) +
                        "/\">[Dir] " +
                        de->d_name +
                        "</a> \n";
            }
            if(de->d_type == __LINK__){
                size_t size = path.length() + strlen(de->d_name) + 1;
                char absPath[size];
                sprintf(absPath, "%s%s", path.c_str(), de->d_name);
                absPath[size] = '\0';
                int end_symbols = readlink(absPath, linkPath, sizeOfLinkPath);
                linkPath[end_symbols] = '\0';
                *foldersList += tabs + "<a href=\"" +
                        std::string(absPath) +
                        "/\">[Link] " +
                        std::string(de->d_name) +
                        "</a> \n";
            }
        }
        closedir(dir);
    }
    return foldersList;
}

const autoPtrStr viewFiles(const std::string& path, const std::string& tabs){
    DIR* dir;
    struct dirent* de;
    autoPtrStr filesList(new std::string);
    if ((dir = opendir(path.c_str())) == NULL) {
        throw(ServerExeption(0, "opendir failed!"));
    }
    else {
        while (de = readdir(dir)) {
            if (de->d_type == __DIR__ || de->d_type == __LINK__) {
                continue;
            }
            else {
                *filesList += tabs + "<a href=\"" +
                        path +
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
