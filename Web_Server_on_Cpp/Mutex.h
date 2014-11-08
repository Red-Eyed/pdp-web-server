#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <stdlib.h>

class Mutex
{

public:

    Mutex();
    ~Mutex();
    void lock();
    void unlock();

private:

    void operator=(const Mutex&);

private:

    pthread_mutex_t m_Mutex;
    bool            m_Locked;
};

class ScopeMutex
{

public:

    explicit ScopeMutex(const Mutex& mx);
    ~ScopeMutex();

private:

    Mutex m_Mutex;

};
#endif // MUTEX_H
