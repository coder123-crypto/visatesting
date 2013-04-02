#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T21:02:09
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VISA
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ImageLabel.cpp \
    Question.cpp \
    DialogAuthorization.cpp \
    DialogResults.cpp \
    DialogSelectAnswer.cpp \
    DialogNote.cpp \
    QuestionDialog.cpp

HEADERS  += MainWindow.h \
    ImageLabel.h \
    Question.h \
    DialogAuthorization.h \
    DialogResults.h \
    DialogSelectAnswer.h \
    DialogNote.h \
    QuestionDialog.h

RESOURCES +=
