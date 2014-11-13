#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <getopt.h>
#include <netdb.h>
#include <sys/wait.h>
#include <list>
#include <tr1/memory>
#include <tr1/shared_ptr.h>

#include "ServerExeption.h"
#include "DownloadFile.h"
#include "ViewContentDir.h"
#include "FileDescriptor.h"
#include "ClientConnection.h"

typedef std::list<std::tr1::shared_ptr<ClientConnection> > listShPtrClient;

class Server
{
public:


    static Server& instance(const struct in_addr addr, u_int16_t port, const std::string& defaultPage = "");
    static void deleteInstance();
    void openConection();
    void closeConnection();
    static void closeServer(int);

private:

    Server() {}
    Server(const Server&) {}
    ~Server();
    void* doOpenConnection(Server* ptrSrv);
    void bindToSocket();
    void getDescriptor(int& fd);
    void fsBrowse(std::string& path);
    void writeToDescriptor(const std::string& path);
    void clearUnusedClients();

private:

    static Server*                      m_Self;
    struct sockaddr_in                  m_SocketAddress;
    struct in_addr                      m_LocalAddress;
    int                                 m_Socket;
    u_int16_t                           m_Port;
    bool                                m_Connected;
    std::string                         m_DefaultPage;
    bool                                m_LoopFlag;
    listShPtrClient                     m_Connections;
};

#endif // SERVER_H
