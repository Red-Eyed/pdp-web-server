#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Server.h"

#include "gtest/gtest.h"



//Test VarDownloadFile handle_request()
TEST(DownloadFileHandleRequest, UnExistPath){
    DownloadFile VarDownloadFile;
    iRequestHandler* PtrDownloadFile = &VarDownloadFile;
    std::string UnExistPath = "unexist";

    EXPECT_THROW(PtrDownloadFile->handleRequest(UnExistPath), ServerExeption);
}

TEST(DownloadFileHandleRequest, File){
    DownloadFile VarDownloadFile;
    iRequestHandler* PtrDownloadFile = &VarDownloadFile;

    std::string FilePath = "/home/vadimstupakov/test_file";
    open(FilePath.c_str(), O_CREAT);

    EXPECT_NO_THROW(PtrDownloadFile->handleRequest(FilePath));

    remove(FilePath.c_str());
}

TEST(DownloadFileHandleRequest, Directory){
    DownloadFile VarDownloadFile;
    iRequestHandler* PtrDownloadFile = &VarDownloadFile;

    std::string DirPath = "/home/vadimstupakov/test_dir";
    mkdir(DirPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    EXPECT_THROW(PtrDownloadFile->handleRequest(DirPath), ServerExeption);

    rmdir(DirPath.c_str());
}


TEST(DownloadFileHandleRequest, SymLink){
    DownloadFile VarDownloadFile;
    iRequestHandler* PtrDownloadFile = &VarDownloadFile;

    std::string FilePath = "/home/vadimstupakov/test_file";
    open(FilePath.c_str(), O_CREAT);

    std::string  LinkPath = "/home/vadimstupakov/test_file";
    symlink(FilePath.c_str(), LinkPath.c_str());

    EXPECT_NO_THROW(PtrDownloadFile->handleRequest(LinkPath));

    remove(FilePath.c_str());
    unlink(LinkPath.c_str());
}

TEST(DownloadFileHandleRequest, EmptyString){
    DownloadFile VarDownloadFile;
    iRequestHandler* PtrDownloadFile = &VarDownloadFile;
    std::string EmptyStr = "";
    EXPECT_THROW(PtrDownloadFile->handleRequest(EmptyStr), ServerExeption);
}

//Test ViewContentDir handle_request()
TEST(ViewContentDirHandleRequest, UnExistPath){
    ViewContentDir ViewContentDir;

    iRequestHandler* PtrViewContentDir = &ViewContentDir;
    std::string UnExistPath = "unexist";

    EXPECT_THROW(PtrViewContentDir->handleRequest(UnExistPath), ServerExeption);
}

TEST(ViewContentDirHandleRequest, ExistPath){
    ViewContentDir ViewContentDir;
    iRequestHandler* PtrViewContentDir = &ViewContentDir;
    std::string ExistPath = "/home";
    EXPECT_NO_THROW(PtrViewContentDir->handleRequest(ExistPath));
}

TEST(ViewContentDirHandleRequest, File){
    ViewContentDir ViewContentDir;
    iRequestHandler* PtrViewContentDir = &ViewContentDir;

    std::string FilePath = "/home/vadimstupakov/test_file";
    open(FilePath.c_str(), O_CREAT);

    EXPECT_THROW(PtrViewContentDir->handleRequest(FilePath), ServerExeption);

    remove(FilePath.c_str());
}

TEST(ViewContentDirHandleRequest, Directory){
    ViewContentDir ViewContentDir;
    iRequestHandler* PtrViewContentDir = &ViewContentDir;

    std::string DirPath = "/home/vadimstupakov/test_dir";
    mkdir(DirPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    EXPECT_NO_THROW(PtrViewContentDir->handleRequest(DirPath));

    rmdir(DirPath.c_str());
}


TEST(ViewContentDirHandleRequest, SymLink){
    ViewContentDir ViewContentDir;
    iRequestHandler* PtrViewContentDir = &ViewContentDir;

    std::string FilePath = "/home/vadimstupakov/test_file";
    open(FilePath.c_str(), O_CREAT);

    std::string  LinkPath = "/home/vadimstupakov/test_file";
    symlink(FilePath.c_str(), LinkPath.c_str());

    EXPECT_THROW(PtrViewContentDir->handleRequest(LinkPath), ServerExeption);

    remove(FilePath.c_str());
    unlink(LinkPath.c_str());
}

TEST(ViewContentDirHandleRequest, EmptyString){
    ViewContentDir ViewContentDir;
    iRequestHandler* PtrViewContentDir = &ViewContentDir;
    std::string EmptyStr = "";
    EXPECT_THROW(PtrViewContentDir->handleRequest(EmptyStr), ServerExeption);
}

//Test Thread create Thread
TEST(createThread, TestCreate_Thread){
    Thread thread;
    EXPECT_NO_THROW(thread.createThread(););
}

//Test Thread close Thread
TEST(closeThread, TestClose_Thread){
    Thread thread;
    EXPECT_NO_THROW(thread.closeThread(););
}

//Test Constructor
TEST(Contructor, Good_addr){
    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    uint16_t Port = 4000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    EXPECT_NO_THROW(Server srv(LocalAddr, Port));
}

TEST(Contructor, Bad_addr){
    struct hostent* LocalHostName = gethostbyname ("1234");
    struct in_addr LocalAddr;
    uint16_t Port = 4000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    EXPECT_THROW(Server srv(LocalAddr, Port), ServerExeption);
}

TEST(Contructor, Good_Port){
    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    uint16_t Port = 4000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    EXPECT_NO_THROW(Server srv(LocalAddr, Port));
}

TEST(Constructor, Bad_Port){
    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    uint16_t Port = 1000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    EXPECT_THROW(Server srv(LocalAddr, Port), ServerExeption);
}

//Test openConnection
TEST(openConection, OnceOpen){
    //good data
    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    uint16_t Port = 4000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    Server srv(LocalAddr, Port);

    EXPECT_NO_THROW(srv.openConection());
}

TEST(openConection, TwiceOpen){
    //good data
    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    uint16_t Port = 4000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    Server srv(LocalAddr, Port);
    srv.openConection();

    EXPECT_THROW(srv.openConection(), ServerExeption);
}


//Test closeConnection
TEST(closeConection, OnceClose){
    //good data
    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    uint16_t Port = 4000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    Server srv(LocalAddr, Port);
    srv.openConection();

    EXPECT_NO_THROW(srv.closeConnection());
}

TEST(closeConection, TwiceClose){
    //good data
    struct hostent* LocalHostName = gethostbyname ("localhost");
    struct in_addr LocalAddr;
    uint16_t Port = 4000;
    LocalAddr.s_addr = *((int*) (LocalHostName->h_addr_list[0]));
    Server srv(LocalAddr, Port);
    srv.openConection();
    srv.closeConnection();

    EXPECT_THROW(srv.closeConnection(), ServerExeption);
}
