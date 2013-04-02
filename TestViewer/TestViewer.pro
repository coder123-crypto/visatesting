#-------------------------------------------------
#
# Project created by QtCreator 2013-03-24T10:01:02
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestViewer
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    DelegateTestResult.cpp \
    DialogDatabaseParams.cpp

HEADERS  += MainWindow.h \
    DelegateTestResult.h \
    DialogDatabaseParams.h
