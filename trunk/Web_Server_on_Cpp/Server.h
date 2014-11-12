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


    static const Server& instance(const struct in_addr addr, u_int16_t port, const std::string& defaultPage = "");
    static void deleteInstance();
    static void openConectionInThread();
    static void openConection();
    static void closeConnection();
    static void closeServer(int);

private:

    Server() {}
    Server(const Server&) {}
    ~Server();
    static void* doOpenConnection(Server* ptrSrv);
    static void bindToSocket();
    static void getDescriptor(int& fd);
    static void fsBrowse(std::string& path);
    static void writeToDescriptor(const std::string& path);
    static void clearUnusedClients();

private:

    static Server*                             m_Self;
    static struct sockaddr_in                  m_SocketAddress;
    static struct in_addr                      m_LocalAddress;
    static int                                 m_Socket;
    static u_int16_t                           m_Port;
    static bool                                m_Connected;
    static std::string                         m_DefaultPage;
    static bool                                m_LoopFlag;
    static listShPtrClient                     m_Connections;
};

#endif // SERVER_H
