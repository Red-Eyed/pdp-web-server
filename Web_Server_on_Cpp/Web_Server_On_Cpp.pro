TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -pthread -Werror
LIBS += -pthread

SOURCES += \
    main.cpp \
    DownloadFile.cpp \
    Server.cpp \
    ServerExeption.cpp \
    ViewContentDir.cpp \
    FileDescriptor.cpp \
    Mutex.cpp

HEADERS += \
    DownloadFile.h \
    iRequestHandler.h \
    Server.h \
    ServerExeption.h \
    ViewContentDir.h \
    FileDescriptor.h \
    ServerStrs.h \
    Mutex.h \
    Thread.h

