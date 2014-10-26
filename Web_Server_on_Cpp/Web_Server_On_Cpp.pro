TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
    main.cpp \
    DownloadFile.cpp \
    Server.cpp \
    ServerExeption.cpp \
    Thread.cpp \
    ViewContentDir.cpp \
    iRequestHandler.cpp

HEADERS += \
    DownloadFile.h \
    iRequestHandler.h \
    Server.h \
    ServerExeption.h \
    Thread.h \
    ViewContentDir.h
