#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "ClientConnection.h"
#include "Utils.h"
#include "DownloadFile.h"
#include "ViewContentDir.h"
#include "ServerStrs.h"
#include "Thread.h"


ClientConnection::ClientConnection(int fd, const std::string& startPage):
    m_Path(""),
    m_StartPage(startPage),
    m_IsActive(true),
    m_Fd(fd)
{
}

ClientConnection::ClientConnection(FileDescriptor& fd, const std::string& startPage):
    m_Path(""),
    m_StartPage(startPage),
    m_IsActive(true),
    m_Fd(fd)
{
}

void ClientConnection::ConnectToDescriptorInThread(){
    try {
        m_Thread.createTrhead(ClientConnection::handleConnection, this);
    } catch (const std::exception& e) {
        std::cerr << e.what();
    }
}

ClientConnection::~ClientConnection(){
    m_IsActive = false;
}

bool ClientConnection::isActive() const {
    return m_IsActive;
}

void* ClientConnection::handleConnection(ClientConnection* objPtr){

    if(objPtr->m_Fd == -1){
        objPtr->m_IsActive = false;
        return NULL;
    }
    std::vector<char> buffer;
    size_t size = 256;
    buffer.resize(size);
    ssize_t bytesRead = 0;
    //get data from client
    bytesRead = read(objPtr->m_Fd.getFd(), &buffer[0], buffer.size());
    if (bytesRead > 0){
        std::vector<char> method;
        std::vector<char> path;
        std::vector<char> protocol;

        method.resize(size);

        path.resize(size);

        protocol.resize(size);

        sscanf(&buffer[0], "%s %s %s", &method[0], &path[0], &protocol[0]);

        objPtr->m_Path = std::string(&path[0]);

        while (strstr(&buffer[0], "\r\n\r\n") == NULL){
            bytesRead = read(objPtr->m_Fd.getFd(), &buffer[0], buffer.size());
        }

        if (bytesRead == -1) {
            objPtr->m_Fd.fdClose();
            return NULL;
        }

        if (strcmp(&protocol[0], "HTTP/1.0") && strcmp(&protocol[0], "HTTP/1.1")) {
            size_t writeSize = 0;
            writeSize = write(objPtr->m_Fd.getFd(), badRequestResponse.c_str(), badRequestResponse.size() );
            if(writeSize != badRequestResponse.size()){
                throw std::runtime_error(createString(errno, "write error ", __FUNCTION__, __LINE__ ));
            }
        }
        else if (strcmp(&method[0], "GET")) {

            std::vector<char> response;
            size = 1024;
            response.resize(size);

            snprintf(&response[0], response.size(), badMethodResponseTemplate.c_str(), &method[0]);
            size_t writeSize = 0;
            writeSize = write(objPtr->m_Fd.getFd(), &response[0], strlen(&response[0]));
            if(writeSize != badRequestResponse.size()){
                throw std::runtime_error(createString(errno, "write error ", __FUNCTION__, __LINE__ ));
            }
        }
        else{
            try{
                objPtr->fsBrowse();
            }
            catch(const std::exception& e){
                std::cerr << e.what();
            }
        }
    }
    else if (bytesRead < 0){
        throw std::runtime_error(createString(errno, "read error ", __FUNCTION__, __LINE__));
    }
    objPtr->m_IsActive = false;
    return NULL;
}

void ClientConnection::fsBrowse(){

    //replace %20 to space
    std::size_t found = 0;
    while((found = m_Path.find("%20", found) ) != std::string::npos){
        m_Path.erase(found, 2);
        m_Path[found] = ' ';
    }

    if(m_Path == "/start"){
        if(!m_StartPage.empty())
            m_Path = m_StartPage;
    }
    struct stat fsStat;
    std::auto_ptr<iRequestHandler> interface;
    if(stat(m_Path.c_str(), &fsStat) == 0){
        if(S_ISDIR(fsStat.st_mode) || S_ISBLK(fsStat.st_mode)){//view fs
            interface = std::auto_ptr<iRequestHandler> (new ViewContentDir);
        }
        else if(S_ISREG(fsStat.st_mode)){//download file
            interface = std::auto_ptr<iRequestHandler> (new DownloadFile);
        }
        if(interface.get()){
            try{
                writeToDescriptor(interface);
            }catch (const std::exception& e){
                std::cerr << e.what();
            }
        }
    }
    else{
        write(m_Fd.getFd(), pathNotFound.c_str(), pathNotFound.size());
    }

}

void ClientConnection::writeToDescriptor(const std::auto_ptr<iRequestHandler>& ptrInterface) const{
    std::vector<char> buf;

    ptrInterface->handleRequest(m_Path, buf);

    size_t writeSize = write(m_Fd.getFd(), &buf[0], buf.size());
    if(writeSize != buf.size()){
        throw std::runtime_error(createString(writeSize, "write error ", __FUNCTION__, __LINE__ ));
    }
}

