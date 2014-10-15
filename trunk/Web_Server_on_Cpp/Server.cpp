#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
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



#define __DIR__ 4
#define __LINK__ 10

static const char* page_start =
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\"\n\"http://www.w3.org/TR/html4/strict.dtd\">\n"
        "<html>\n"
        "\t<head>\n"
        "\t<title>!DOCTYPE</title>\n"
        "\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
        "\t</head>\n"
        "\t<body>\n"
        "\t\t<pre>\n";

static const char* page_end = "\t\t</pre>\n"
        "\t</body>\n"
        "</html>\n";

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







Server::Server(const in_addr addr, u_int16_t port):m_Connected(0), m_SrvLocalAddress(addr), m_Port(port){
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
            close(m_SrvSocket);

            try{
                handleConnection(m_Fd);
            }
            catch(std::exception& e){
                std::cerr << e.what();
            }

            close(m_Fd);
            exit(0);
        }
        else if (child_pid > 0){
            close(m_Fd);
            waitpid(child_pid, NULL, 0);
        }
        else{
            throw(ServerExeption(child_pid, "fork"));
        }
    }
}

void Server::closeConnection(){
    if(!m_Connected){
        throw ServerExeption();
    }
    else{
        m_Connected = 0;
    }
}

void Server::handleConnection(int connection_fd){
    char buffer[256];
    ssize_t bytes_read;

    bytes_read = read(connection_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0){
        char method[sizeof(buffer)];
        char url[sizeof(buffer)];
        char protocol[sizeof(buffer)];

        buffer[bytes_read] = '\0';

        sscanf(buffer, "%s %s %s", method, url, protocol);
        std::cerr << "***************************************\n";
        std::cerr << "Buffer\n" << buffer << "\n";
        std::cerr << "***************************************\n";
        std::cerr << "Method\n" << method << "\n";
        std::cerr << "***************************************\n";
        std::cerr << "URL\n" << url << "\n";
        std::cerr << "***************************************\n";
        std::cerr << "Protocol\n" << protocol << "\n";
        std::cerr << "***************************************\n";
        while (strstr(buffer, "\r\n\r\n") == NULL){
            bytes_read = read(connection_fd, buffer, sizeof(buffer));
        }

        if (bytes_read == -1) {
            close(connection_fd);
            return;
        }

        if (strcmp(protocol, "HTTP/1.0") && strcmp(protocol, "HTTP/1.1")) {

            write(connection_fd, bad_request_response,
                  sizeof(bad_request_response));

        }
        else if (strcmp(method, "GET")) {

            char response[1024];

            snprintf(response, sizeof(response), bad_method_response_template,
                     method);
            write(connection_fd, response, strlen(response));

        }
        else{
            try{
                fs_browse(connection_fd, url);
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

void fs_browse(int fd, const char* page){
    pid_t child_pid;
    int rval;
    size_t len = strlen(page);

    std::auto_ptr<char> local_page(new char [len + 1]);
    strcpy(local_page.get(), page);
    local_page.get()[len] = '\0';

    child_pid = fork();
    if (child_pid == 0) {
        rval = dup2(fd, STDOUT_FILENO);
        if (rval == -1){
            throw(ServerExeption(rval, "dup2"));
        }

        rval = dup2(fd, STDERR_FILENO);
        if (rval == -1){
            throw(ServerExeption(rval, "dup2"));
        }

        try{
            struct stat s;
            if(stat(local_page.get(), &s) == 0){
                if(S_ISDIR(s.st_mode)){//view fs
                    write(fd, page_start, strlen(page_start));
                    view_folder(local_page.get());
                    view_files(local_page.get());
                    write(fd, page_end, strlen(page_end));
                }
                else if(S_ISREG(s.st_mode)){//download file
                    int fd_file = open(local_page.get(), O_RDONLY);
                    struct stat st;
                    stat(local_page.get(), &st);
                    char* buf = new char[st.st_size];
                    read(fd_file, buf, st.st_size);
                    write(fd, buf, st.st_size);
                    delete[] buf;
                }
            }
        }
        catch(std::exception& e){
            std::cerr << e.what();
        }

    } else if (child_pid > 0) {
        waitpid(child_pid, NULL, 0);
    }
}

void Server::bindToSocket(){
    m_SrvSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(m_SrvSocket == -1){
        throw(ServerExeption(m_SrvSocket, "socket"));
    }

    memset(&m_SrvSocketAddress, 0, sizeof(m_SrvSocketAddress));
    m_SrvSocketAddress.sin_family = AF_INET;
    m_SrvSocketAddress.sin_port = m_Port;
    m_SrvSocketAddress.sin_addr = m_SrvLocalAddress;

    int rval = 0;
    rval = bind(m_SrvSocket, reinterpret_cast<struct sockaddr*>(&m_SrvSocketAddress), sizeof(m_SrvSocketAddress));
    if(rval){
        throw(ServerExeption(rval, "bind"));
    }

    rval = listen(m_SrvSocket, 10);
    if (rval)
        throw(ServerExeption(rval, "listen"));
}

void Server::getDescriptor(){
    struct sockaddr_in RemoteAddr;
    socklen_t AddrLen;

    AddrLen = sizeof(RemoteAddr);
    m_Fd = accept(m_SrvSocket, (struct sockaddr*) &RemoteAddr, &AddrLen);
    if (m_Fd == -1) {
        if (errno != EINTR){
            throw(ServerExeption(errno, "accept"));
        }
    }
}


