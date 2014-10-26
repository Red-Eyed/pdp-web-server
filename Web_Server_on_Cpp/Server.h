#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <getopt.h>
#include <netdb.h>
#include <sys/wait.h>

#include "ServerExeption.h"
#include "DownloadFile.h"
#include "ViewContentDir.h"
#include "Thread.h"

class Server
{
public:

    Server(const struct in_addr addr, u_int16_t port);
    void openConection();
    void closeConnection();

private:

    void bindToSocket();
    void getDescriptor();
    void handleConnection();
    void fsBrowse(const std::string& path);
    void writeToDescriptor(const std::string& path);
    void writeDirTreeToDescriptor(const std::string& path);

private:

    Thread                              m_Threads;
    std::auto_ptr<iRequestHandler>      m_RequestOperations;
    struct sockaddr_in                  m_SocketAddress;
    struct in_addr                      m_LocalAddress;
    int                                 m_Socket;
    u_int16_t                           m_Port;
    bool                                m_Connected;
    int                                 m_FileDescriptor;
};


#endif // SERVER_H
