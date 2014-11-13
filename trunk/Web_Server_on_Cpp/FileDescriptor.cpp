#include <stdexcept>

#include "Utils.h"
#include "FileDescriptor.h"
#include <iostream>
FileDescriptor::FileDescriptor():
    m_IsOpen(false),
    m_Fd(0){std::cerr << "FileDescriptor()\n";}

FileDescriptor::FileDescriptor(int fd):
    m_IsOpen(true),
    m_Fd(fd)
{}

FileDescriptor::FileDescriptor(const std::string& path, int flags):m_IsOpen(false), m_Fd(-1){
    fdOpen(path, flags);
}

FileDescriptor::FileDescriptor(const FileDescriptor& fd):
    m_IsOpen(fd.m_IsOpen),
    m_Fd(fd.m_Fd)

{}

FileDescriptor::~FileDescriptor(){
    fdClose();
}

int FileDescriptor::getFd() const{
    return m_Fd;
}

const FileDescriptor& FileDescriptor::operator=(int fd){
    m_Fd = fd;
    m_IsOpen = true;
    return *this;
}

bool FileDescriptor::operator<(int fd) const{
    return (m_Fd < fd ? true : false);
}

bool FileDescriptor::operator>(int fd) const{
    return (m_Fd > fd ? true : false);
}

bool FileDescriptor::operator>=(int fd) const{
    return (m_Fd >= fd ? true : false);
}

bool FileDescriptor::operator<=(int fd) const{
    return (m_Fd <= fd ? true : false);
}

bool FileDescriptor::operator==(int fd) const{
    return (m_Fd == fd ? true : false);
}

bool FileDescriptor::operator!=(int fd) const{
    return (m_Fd != fd ? true : false);
}

bool FileDescriptor::isOpen() const{
    return m_IsOpen;
}

void FileDescriptor::fdClose(){
    if(isOpen()){
        close(m_Fd);
        m_IsOpen = false;
    }
}

void FileDescriptor::fdOpen(const std::string& path, int flags){
    if(!isOpen()){
        m_Fd = open(path.c_str(), flags);
        if(m_Fd <= 0){
            throw std::runtime_error(createString(m_Fd, "bad descriptor, open() error", __FUNCTION__, __LINE__) );
        }
        m_IsOpen = true;
    }
}
