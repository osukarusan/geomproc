#-------------------------------------------------
#
# Project created by QtCreator 2013-02-08T12:55:58
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Base
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    camera.cpp \
    bbox.cpp \
    trianglemesh.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    camera.h \
    bbox.h \
    trianglemesh.h

LIBS += -lGLU

FORMS    += mainwindow.ui
