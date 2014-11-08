#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <exception>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>

#include "Server.h"
#include "ServerStrs.h"
#include "Thread.h"
#include "Mutex.h"
#include "FileDescriptor.h"

#define __PORT_RESERVED__ 1024
#define __QUEUE_OF_CONNECTIONS__ 10


Server::Server(const in_addr addr, u_int16_t port, const std::string& defaultPage):
    m_RequestOperations(NULL),
    m_LocalAddress(addr),
    m_Socket(0),
    m_Port(port),
    m_Connected(false),
    m_FileDescriptor(0),
    m_DefaultPage(defaultPage),
    m_LoopFlag(false)
{
    if(port < htons(__PORT_RESERVED__)){
        throw ServerExeption(0, "Bad port", __FUNCTION__, __LINE__ );
    }
}

Server::~Server(){
    if(m_Connected)
        closeConnection();
}

void Server::openConectionInThread(){
    Thread<void*(*)(Server*), Server*> thread;
    thread.createTrhead(doOpenConnection, this);
}

void Server::openConection(){
    doOpenConnection(this);
}

void Server::closeConnection(){
    if(m_Connected){
        m_LoopFlag = true;
    }
}

void* Server::doOpenConnection(Server* ptrSrv){
    if(ptrSrv->m_Connected){
        throw ServerExeption(0, "connections has already existed ", __FUNCTION__, __LINE__ );
    }
    else{
        ptrSrv->m_Connected = true;
    }

    ptrSrv->bindToSocket();

    while (1){
        //if server get string "/kill_server" - break loop and Destroy object Server
        if(ptrSrv->m_LoopFlag){
            break;
        }
        //wait for the new connection
        ptrSrv->getDescriptor();
        Thread<void*(*)(Server*), Server*> thread;
        try {
            thread.createTrhead(handleConnection, ptrSrv);
        } catch (const std::exception& e) {
            std::cerr << e.what();
        }
    }
    close(ptrSrv->m_FileDescriptor);
    close(ptrSrv->m_Socket);
    return NULL;
}

void* Server::handleConnection(Server* srv){
    std::vector<char> buffer;
    size_t sizeReserve = 256;
    buffer.reserve(sizeReserve);
    ssize_t bytesRead = 0;
    FileDescriptor fd(srv->m_FileDescriptor);
    //get data from client
    bytesRead = read(fd.getFd(), &buffer[0], buffer.capacity());
    if (bytesRead > 0){
        std::vector<char> method;
        std::vector<char> path;
        std::vector<char> protocol;

        method.reserve(sizeReserve);

        path.reserve(sizeReserve);

        protocol.reserve(sizeReserve);

        sscanf(&buffer[0], "%s %s %s", &method[0], &path[0], &protocol[0]);


        while (strstr(&buffer[0], "\r\n\r\n") == NULL){
            bytesRead = read(fd.getFd(), &buffer[0], buffer.capacity());
        }

        if (bytesRead == -1) {
            fd.fdClose();
            return NULL;
        }

        if (strcmp(&protocol[0], "HTTP/1.0") && strcmp(&protocol[0], "HTTP/1.1")) {
            size_t writeSize = 0;
            writeSize = write(fd.getFd(), badRequestResponse.c_str(), badRequestResponse.size() );
            if(writeSize != badRequestResponse.size()){
                throw ServerExeption("write error ", __FUNCTION__, __LINE__ );
            }
        }
        else if (strcmp(&method[0], "GET")) {

            std::vector<char> response;
            sizeReserve = 1024;
            response.reserve(sizeReserve);

            snprintf(&response[0], response.capacity(), badMethodResponseTemplate.c_str(), &method[0]);
            size_t writeSize = 0;
            writeSize = write(fd.getFd(), &response[0], strlen(&response[0]));
            if(writeSize != badRequestResponse.size()){
                throw ServerExeption("write error ", __FUNCTION__, __LINE__ );
            }
        }
        else{
            std::string strPath(&path[0]);
            if(strPath == "/kill_server"){
                srv->closeConnection();
                return NULL;
            }
            try{
                srv->fsBrowse(strPath);
            }
            catch(const std::exception& e){
                std::cerr << e.what();
            }
        }
    }
    return NULL;
}

void Server::fsBrowse(std::string& path){

    //replace %20 to space
    std::size_t found = 0;
    while((found = path.find("%20", found) ) != std::string::npos){
        path.erase(found, 2);
        path[found] = ' ';
    }

    if(path == "/start"){
        path = m_DefaultPage;
    }
    struct stat fsStat;
    if(stat(path.c_str(), &fsStat) == 0){
        if(S_ISDIR(fsStat.st_mode) || S_ISBLK(fsStat.st_mode)){//view fs
            m_RequestOperations = std::auto_ptr<iRequestHandler> (new ViewContentDir);
        }
        else if(S_ISREG(fsStat.st_mode)){//download file
            m_RequestOperations = std::auto_ptr<iRequestHandler> (new DownloadFile);
        }
        if(m_RequestOperations.get()){
            try{
                writeToDescriptor(path);
            }catch (const std::exception& e){
                std::cerr << e.what();
            }
        }
    }
    else{
        FileDescriptor fd(m_FileDescriptor);
        write(fd.getFd(), pathNotFound.c_str(), pathNotFound.size());
    }

}

void Server::writeToDescriptor(const std::string& path) const{
    std::vector<char> buf;
    size_t fileSize = 0;

    m_RequestOperations->handleRequest(path, buf);
    Mutex mx;
    mx.lock();
    FileDescriptor fd(m_FileDescriptor);
    mx.unlock();
    fileSize = write(fd.getFd(), &buf[0], buf.capacity());
    if(fileSize != buf.capacity()){
        throw ServerExeption(fileSize, "write error ", __FUNCTION__, __LINE__ );
    }
}

void Server::bindToSocket(){
    m_Socket = socket(PF_INET, SOCK_STREAM, 0);
    if(m_Socket == -1){
        throw ServerExeption(m_Socket, "socket error ", __FUNCTION__, __LINE__ );
    }

    memset(&m_SocketAddress, 0, sizeof(m_SocketAddress));
    m_SocketAddress.sin_family = AF_INET;
    m_SocketAddress.sin_port = m_Port;
    m_SocketAddress.sin_addr = m_LocalAddress;

    int rval = 0;
    rval = bind(m_Socket, reinterpret_cast<struct sockaddr*>(&m_SocketAddress), sizeof(m_SocketAddress));
    if(rval){
        throw ServerExeption(rval, "bind error ", __FUNCTION__, __LINE__ );
    }

    rval = listen(m_Socket, __QUEUE_OF_CONNECTIONS__);
    if (rval)
        throw ServerExeption(rval, "listen error ", __FUNCTION__, __LINE__ );
}

void Server::getDescriptor(){
    struct sockaddr_in RemoteAddr;
    socklen_t AddrLen;

    memset(&RemoteAddr, 0, sizeof(RemoteAddr));
    memset(&AddrLen, 0, sizeof(AddrLen));

    AddrLen = sizeof(RemoteAddr);
    Mutex mx;
    ScopeMutex scMx(mx);
    m_FileDescriptor = accept(m_Socket, (struct sockaddr*) &RemoteAddr, &AddrLen);
    if(m_FileDescriptor <= -1) {
        //If system call was interrupted by signal continue executing
        if (errno != EINTR){
            throw ServerExeption(errno, "accept error ", __FUNCTION__, __LINE__ );
        }
    }
}

