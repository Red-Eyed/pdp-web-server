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
#include <string.h>
#include <cstdlib>
#include <assert.h>

#include "Server.h"
#include "ServerStrs.h"
#include "Thread.h"
#include "ClientConnection.h"

#define __PORT_RESERVED__           1024
#define __QUEUE_OF_CONNECTIONS__    10

Server* Server::m_Self = NULL;

void Server::deleteInstance(){
    m_Self->~Server();
    m_Self = NULL;
}

Server& Server::instance(const in_addr addr, u_int16_t port, const std::string& defaultPage){
    if(!m_Self){
        std::atexit(deleteInstance);
        m_Self = new Server;
        if(port < htons(__PORT_RESERVED__)){
            throw ServerExeption(0, "Bad port", __FUNCTION__, __LINE__ );
        }
        m_Self->m_LocalAddress = addr;
        m_Self->m_Socket = 0;
        m_Self->m_Port = port;
        m_Self->m_Connected = false;
        m_Self->m_DefaultPage = defaultPage;
        m_Self->m_LoopFlag = false;
    }
    return *m_Self;
}

void Server::openConection(){
    doOpenConnection(m_Self);
}

void Server::closeConnection(){
    if(m_Connected){
        m_LoopFlag = true;
    }
}

Server::~Server(){
    closeConnection();
    close(m_Socket);
    std::cout << "\nServer closed!\n";
}

void Server::clearUnusedClients(){
    listShPtrClient::iterator iter = m_Connections.begin();
    for(; iter != m_Connections.end() ;){
        if(iter->get()->isActive() == false){
            iter = m_Connections.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

void Server::closeServer(int){
    if(m_Self){
        m_Self->closeConnection();
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

    //if closeConnection was called
    while (!(ptrSrv->m_LoopFlag)){

        //wait for the new connection
        int fd = 0;

        ptrSrv->getDescriptor(fd);

        ptrSrv->m_Connections.push_back(std::tr1::shared_ptr<ClientConnection>(new ClientConnection(fd, ptrSrv->m_DefaultPage)));
        ptrSrv->m_Connections.back()->ConnectToDescriptorInThread();
        ptrSrv->clearUnusedClients();
    }
    return NULL;
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

void Server::getDescriptor(int& fd){
    struct sockaddr_in RemoteAddr;
    socklen_t AddrLen;

    memset(&RemoteAddr, 0, sizeof(RemoteAddr));
    memset(&AddrLen, 0, sizeof(AddrLen));

    AddrLen = sizeof(RemoteAddr);

    fd = accept(m_Socket, reinterpret_cast<struct sockaddr*>(&RemoteAddr), &AddrLen);

}

