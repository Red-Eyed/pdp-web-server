#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "ServerExeption.h"

template
<typename funcType, typename funcArg>
class Thread
{
public:

    Thread():m_ThreadID(0)
    {}

    ~Thread(){
        closeThread();
    }

    void createTrhead(funcType foo, funcArg arg){
        int ret =  pthread_create(&m_ThreadID, NULL, reinterpret_cast<void*(*)(void*)>(foo), reinterpret_cast<void*>(arg));
        if(ret){
            throw ServerExeption(ret, "createThread() error ", __FUNCTION__, __LINE__ );
        }
    }

    void closeThread(){
        int ret = pthread_join(m_ThreadID, NULL);
        if(ret){
            std::cerr << "closeTread error\n return vaue = " << ret << "\n";
        }
    }

private:

    Thread(const Thread&);
    void operator=(const Thread&);

private:

    pthread_t m_ThreadID;

};

#endif // THREAD_H
