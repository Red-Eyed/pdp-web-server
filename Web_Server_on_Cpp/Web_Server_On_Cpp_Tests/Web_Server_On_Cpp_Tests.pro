TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
    WebServerTest.cpp

INCLUDEPATH += ./gtest/include ../ ./


OTHER_FILES += \
    Makefile
