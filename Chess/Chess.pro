#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T15:52:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chessmain.cpp \
    pieces.cpp \
    board.cpp

HEADERS  += mainwindow.h \
    chessmain.h \
    pieces.h \
    board.h

FORMS    += mainwindow.ui
