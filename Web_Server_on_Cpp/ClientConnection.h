#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <auto_ptr.h>

#include "FileDescriptor.h"
#include "iRequestHandler.h"
#include "Thread.h"


class ClientConnection;

typedef Thread<void*(*)(ClientConnection*), ClientConnection*> ClientThread;

class ClientConnection
{

public:

    ClientConnection(int fd, const std::string& startPage);
    ClientConnection(FileDescriptor& fd, const std::string& startPage);
    void ConnectToDescriptorInThread();
    ~ClientConnection();
    bool isActive() const;

private:

    static void* handleConnection(ClientConnection* objPtr);
    void fsBrowse();
    void writeToDescriptor(const std::auto_ptr<iRequestHandler>& ptrInterface) const;

private:

    std::string                         m_Path;
    const std::string                   m_StartPage;
    bool                                m_IsActive;
    ClientThread                        m_Thread;
    FileDescriptor                      m_Fd;
};


#endif // CLIENT_H
