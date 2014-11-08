#include "Mutex.h"
#include "ServerExeption.h"
#include <string>

Mutex::Mutex(): m_Locked(false){
    int ret = pthread_mutex_init(&m_Mutex, NULL);
    if(ret){
        throw ServerExeption(ret, "Constructor error ", __FUNCTION__, __LINE__ );
    }
}

Mutex::~Mutex(){
    try {
        unlock();
    } catch (const std::exception& e) {
        std::cerr << e.what();
    }

    int ret = pthread_mutex_destroy(&m_Mutex);
    if(ret){
        std::cerr << "~Mutex() error\n";
        std::cerr << "Error code of pthread_mutex_destroy() = " << ret << "\n";
    }
}

void Mutex::lock(){
    if(!m_Locked){
        int ret = pthread_mutex_lock(&m_Mutex);
        if(ret){
            throw ServerExeption(ret, "lock error ", __FUNCTION__, __LINE__ );
        }
        m_Locked = true;
    }
}

void Mutex::unlock(){
    if(m_Locked){
        int ret = pthread_mutex_unlock(&m_Mutex);
        if(ret){
            throw ServerExeption(ret, "unlock error ", __FUNCTION__, __LINE__ );
        }
        m_Locked = false;
    }
}

ScopeMutex::ScopeMutex(const Mutex& mx): m_Mutex(mx){
    m_Mutex.lock();
}

ScopeMutex::~ScopeMutex(){
    m_Mutex.unlock();
}
