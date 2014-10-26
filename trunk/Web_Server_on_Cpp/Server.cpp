#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <exception>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Server.h"



static const char* bad_request_response = "HTTP/1.0 400 Bad Request\n"
        "Content-type: text/html\n"
        "\n"
        "<html>\n"
        " <body>\n"
        "  <h1>Bad Request</h1>\n"
        "  <p>This server did not understand your request.</p>\n"
        " </body>\n"
        "</html>\n";

static const char* bad_method_response_template =
        "HTTP/1.0 501 Method Not Implemented\n"
        "Content-type: text/html\n"
        "\n"
        "<html>\n"
        " <body>\n"
        "  <h1>Method Not Implemented</h1>\n"
        "  <p>The method %s is not implemented by this server.</p>\n"
        " </body>\n"
        "</html>\n";


Server::Server(const in_addr addr, u_int16_t port):m_LocalAddress(addr), m_Port(port), m_Connected(0){
    if(port < 1024){
        throw ServerExeption();
    }

    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));

    if(addr.s_addr != LocalAddr.s_addr){
        throw ServerExeption();
    }
}

void Server::openConection(){
    if(m_Connected){
        throw ServerExeption();
        return;
    }
    else{
        m_Connected = 1;
    }
    bindToSocket();
    while (1){
        getDescriptor();
        pid_t child_pid = fork();
        if (child_pid == 0){
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(m_Socket);

            try{
                handleConnection();
            }
            catch(std::exception& e){
                std::cerr << e.what();
            }

            close(m_FileDescriptor);
            exit(0);
        }
        else if (child_pid > 0){
            waitpid(child_pid, NULL, 0);
            close(m_FileDescriptor);
        }
        else{
            throw(ServerExeption(child_pid, "fork"));
        }
    }
}

void Server::closeConnection(){
    if(!m_Connected){
        throw ServerExeption(0, "closeConnection error");
    }
    else{
        m_Connected = 0;
        close(m_FileDescriptor);
        close(m_Socket);
    }
}

void Server::handleConnection(){
    char buffer[256];
    ssize_t bytes_read;

    //get data from client
    bytes_read = read(m_FileDescriptor, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0){
        char method[sizeof(buffer)];
        char path[sizeof(buffer)];
        char protocol[sizeof(buffer)];

        buffer[bytes_read] = '\0';

        sscanf(buffer, "%s %s %s", method, path, protocol);

        while (strstr(buffer, "\r\n\r\n") == NULL){
            bytes_read = read(m_FileDescriptor, buffer, sizeof(buffer));
        }

        if (bytes_read == -1) {
            close(m_FileDescriptor);
            return;
        }

        if (strcmp(protocol, "HTTP/1.0") && strcmp(protocol, "HTTP/1.1")) {

            write(m_FileDescriptor, bad_request_response,
                  sizeof(bad_request_response));

        }
        else if (strcmp(method, "GET")) {

            char response[1024];

            snprintf(response, sizeof(response), bad_method_response_template,
                     method);
            write(m_FileDescriptor, response, strlen(response));

        }
        else{
            try{
                fsBrowse(std::string(path));
            }
            catch(std::exception& e){
                std::cerr << e.what();
            }
        }
    }
    else{
        throw(ServerExeption(bytes_read, "read"));
    }
}

void Server::fsBrowse(const std::string& path){
    try{
        struct stat s;
        if(stat(path.c_str(), &s) == 0){
            if(S_ISDIR(s.st_mode) || S_ISBLK(s.st_mode)){//view fs
                m_RequestOperations = std::auto_ptr<iRequestHandler> (new ViewContentDir);
                writeDirTreeToDescriptor(path);
            }
            else if(S_ISREG(s.st_mode)){//download file
                m_RequestOperations = std::auto_ptr<iRequestHandler> (new DownloadFile);
                writeToDescriptor(path);
            }
        }
    }
    catch(std::exception& e){
        std::cerr << e.what();
    }

}

void Server::writeToDescriptor(const std::string& path){
    try{
        autoPtrStr toWrite = m_RequestOperations->handleRequest(path);
        const size_t& size = toWrite->size();
        write(m_FileDescriptor, toWrite->c_str(), size);
    }
    catch(std::exception& e){
        std::cerr << e.what();
        return;
    }
}

void Server::writeDirTreeToDescriptor(const std::string& path){

    struct stat st;
    fstat(m_FileDescriptor, &st);
    char buffer[st.st_size];
    read(m_FileDescriptor, buffer, st.st_size);
    std::string htmlPage(buffer, st.st_size);
    autoPtrStr getHTMLPage = m_RequestOperations->handleRequest(path);
    if(htmlPage.empty()){
        if(htmlPage.find(path) != std::string::npos){
            for(int i = 0; i < getHTMLPage->length(); ++i){
                if(getHTMLPage->begin()[i] == '\n'){
                    getHTMLPage->insert(getHTMLPage->begin() + i + 1, '\t');
                }
            }
        }
    }
    write(m_FileDescriptor, getHTMLPage->c_str(), getHTMLPage->length());
}

void Server::bindToSocket(){
    m_Socket = socket(PF_INET, SOCK_STREAM, 0);
    if(m_Socket == -1){
        throw(ServerExeption(m_Socket, "socket"));
    }

    memset(&m_SocketAddress, 0, sizeof(m_SocketAddress));
    m_SocketAddress.sin_family = AF_INET;
    m_SocketAddress.sin_port = m_Port;
    m_SocketAddress.sin_addr = m_LocalAddress;

    int rval = 0;
    rval = bind(m_Socket, reinterpret_cast<struct sockaddr*>(&m_SocketAddress), sizeof(m_SocketAddress));
    if(rval){
        throw(ServerExeption(rval, "bind"));
    }

    rval = listen(m_Socket, 10);
    if (rval)
        throw(ServerExeption(rval, "listen"));
}

void Server::getDescriptor(){
    struct sockaddr_in RemoteAddr;
    socklen_t AddrLen;

    AddrLen = sizeof(RemoteAddr);
    m_FileDescriptor = accept(m_Socket, (struct sockaddr*) &RemoteAddr, &AddrLen);
    if (m_FileDescriptor == -1) {
        if (errno != EINTR){
            throw(ServerExeption(errno, "accept"));
        }
    }
}


