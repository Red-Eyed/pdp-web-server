#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdexcept>

#include "ViewContentDir.h"
#include "ServerStrs.h"
#include "Utils.h"

#define __DIR__ 4
#define __LINK__ 10

static void viewFolders(const std::string& path, const std::string& tabs, std::vector<char>& out);
static void viewFiles(const std::string& path, const std::string& tabs, std::vector<char>& out);

void ViewContentDir::handleRequest(const std::string& inputStr, std::vector<char>& out) const{
    if(opendir(inputStr.c_str()) > NULL){
        std::vector<char> folders, files;

        try{
            viewFolders(inputStr, "", folders);
            viewFiles(inputStr, "", files);
        }
        catch(const std::exception& e){
            std::cerr << e.what();
        }

        size_t size = pageEnd.size() + pageStart.size() + folders.size() + files.size();
        out.resize(size);
        out.insert(out.begin(), pageStart.begin(), pageStart.end());
        out.insert(out.end(), folders.begin(), folders.end());
        out.insert(out.end(), files.begin(), files.end());
        out.insert(out.end(), pageEnd.begin(), pageEnd.end());
    }
    else{
        throw std::runtime_error(createString(-1, "Open dir error ", __FUNCTION__, __LINE__ ));
    }
}

void viewFolders(const std::string& path, const std::string& tabs, std::vector<char>& out){

    DIR * dir = NULL;
    struct dirent* de = NULL;
    std::string foldersList;
    size_t sizeOfLinkPath = 256;
    std::vector<char> linkPath;
    linkPath.reserve(sizeOfLinkPath);
    linkPath.assign(sizeOfLinkPath, 0);

    if ((dir = opendir(path.c_str())) == NULL) {
        throw std::runtime_error(createString(0, "opendir failed! ", __FUNCTION__, __LINE__ ));
    }
    else {
        do{
            de = readdir(dir);
            if(!de){
                break;
            }

            if (de->d_type == __DIR__) {
                if (de->d_name[0] == '.' && de->d_name[1] == '\0') {
                    continue;
                }
                foldersList += tabs +
                        std::string("<a href=\"") +
                        path +
                        std::string(de->d_name) +
                        std::string("/\">[Dir] ") +
                        de->d_name +
                        std::string("</a> \n");
            }
            if(de->d_type == __LINK__){
                size_t size = path.length() + strlen(de->d_name) + 1;
                std::vector<char> absPath;
                absPath.reserve(size);
                absPath.assign(size, 0);
                sprintf(&absPath[0], "%s%s", path.c_str(), de->d_name);
                absPath[size] = '\0';
                int end_symbols = readlink(&absPath[0], &linkPath[0], sizeOfLinkPath);
                linkPath[end_symbols] = '\0';
                foldersList += tabs +
                        std::string("<a href=\"") +
                        std::string(&absPath[0]) +
                        std::string("/\">[Link] ") +
                        std::string(de->d_name) +
                        std::string("</a> \n");
            }
        }while (de);

        closedir(dir);
    }
    out.reserve(foldersList.size());
    out.insert(out.begin(), foldersList.begin(), foldersList.end());
}

void viewFiles(const std::string& path, const std::string& tabs, std::vector<char>& out){
    DIR* dir = NULL;
    struct dirent* de = NULL;
    std::string filesList;
    if ((dir = opendir(path.c_str())) == NULL) {
        throw std::runtime_error(createString(0, "opendir failed! ", __FUNCTION__, __LINE__ ));
    }
    else {
        do {
            de = readdir(dir);
            if(!de){
                break;
            }

            if (de->d_type == __DIR__ || de->d_type == __LINK__) {
                continue;
            }
            else {
                filesList += tabs + "<a href=\"" +
                        path +
                        std::string(de->d_name) +
                        "\" >[File] " +
                        std::string(de->d_name) +
                        " </a> \n";
            }
        }while (de);

        closedir(dir);
    }
    out.reserve(filesList.size());
    out.insert(out.begin(), filesList.begin(), filesList.end());
}
