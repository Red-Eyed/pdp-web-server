#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <getopt.h>
#include <netdb.h>

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
    void handleConnection(int fd);

private:

    Thread                              m_Threads;
    std::auto_ptr<iRequestHandler>      m_SrvRequestOperations;
    struct sockaddr_in                  m_SrvSocketAddress;
    struct in_addr                      m_SrvLocalAddress;
    int                                 m_SrvSocket;
    bool                                m_Connected;
    u_int16_t                           m_Port;
    int                                 m_Fd;
};


void fs_browse(int fd, const char* page);

#endif // SERVER_H
