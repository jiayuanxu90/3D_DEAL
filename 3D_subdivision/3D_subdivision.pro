#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T14:41:12
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D_subdivision
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    model.cpp \
    filedispose.cpp \
    fdobj.cpp \
    fdply.cpp \
    meshdispose.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    model.h \
    filedispose.h \
    fdobj.h \
    fdply.h \
    meshdispose.h
