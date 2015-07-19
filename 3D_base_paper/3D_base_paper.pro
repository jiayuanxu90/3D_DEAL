#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T13:04:34
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D_base_paper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    filedispose.cpp \
    model.cpp \
    fobj.cpp \
    halfedge.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    filedispose.h \
    model.h \
    fobj.h \
    halfedge.h
