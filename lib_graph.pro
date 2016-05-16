#-------------------------------------------------
#
# Project created by QtCreator 2016-03-20T21:25:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lib_graph
TEMPLATE = app
CONFIG +=console


SOURCES += main.cpp\
        mainwindow.cpp \
    graph.cpp \
    node.cpp \
    transition.cpp

HEADERS  += mainwindow.h \
    graph.h \
    node.h \
    transition.h

FORMS    += mainwindow.ui
