#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include <string>
#include <unistd.h>
#include <sys/fcntl.h>

class FileDescriptor
{
public:

    explicit FileDescriptor(int fd);
    explicit FileDescriptor(const FileDescriptor& fd);
    FileDescriptor(const std::string& path, int flags);
    ~FileDescriptor();

    int getFd() const;
    void fdClose();

    const FileDescriptor&  operator= (int fd);
    bool operator< (int fd) const;
    bool operator> (int fd) const;
    bool operator>= (int fd) const;
    bool operator<= (int fd) const;
    bool operator== (int fd) const;
    bool operator!= (int fd) const;

private:

    bool isOpen() const;
    void fdOpen(const std::string& path, int flags);
    void fdOpen();

public:

    bool m_IsOpen;
    int  m_Fd;
};

#endif // FILEDESCRIPTOR_H
