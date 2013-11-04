#-------------------------------------------------
#
# Project created by QtCreator 2013-02-08T12:55:58
#
#-------------------------------------------------

QT += core gui opengl

TARGET = Base
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    camera.cpp \
    bbox.cpp \
    trianglemesh.cpp \
    cornertable.cpp \
    dialogcollapse.cpp \
    dialogsmoothing.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    camera.h \
    bbox.h \
    trianglemesh.h \
    cornertable.h \
    dialogcollapse.h \
    dialogsmoothing.h

macx:LIBS += -glu
!macx:LIBS += -lGLU

FORMS    += mainwindow.ui \
    dialogcollapse.ui \
    dialogsmoothing.ui

macx:DEFINES += MACOSX
