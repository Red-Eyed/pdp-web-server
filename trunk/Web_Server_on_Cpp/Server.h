#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <getopt.h>
#include <netdb.h>
#include <sys/wait.h>

#include "ServerExeption.h"
#include "DownloadFile.h"
#include "ViewContentDir.h"

class Server
{
public:

    Server(const struct in_addr addr, u_int16_t port, const std::string& defaultPage = "");
    ~Server();

    void openConectionInThread();
    void openConection();
    void closeConnection();

private:

    static void* doOpenConnection(Server* ptrSrv);
    void bindToSocket();
    void getDescriptor();
    static void* handleConnection(Server* srv);
    void fsBrowse(std::string& path);
    void writeToDescriptor(const std::string& path) const;

private:

    std::auto_ptr<iRequestHandler>      m_RequestOperations;
    struct sockaddr_in                  m_SocketAddress;
    const struct in_addr                m_LocalAddress;
    int                                 m_Socket;
    const u_int16_t                     m_Port;
    bool                                m_Connected;
    int                                 m_FileDescriptor;
    const std::string                   m_DefaultPage;
    bool                                m_LoopFlag;
};

#endif // SERVER_H
