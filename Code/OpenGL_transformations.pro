#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T12:26:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_transformations
TEMPLATE = app
CONFIG += c++14

SOURCES += main.cpp\
    mainwindow.cpp \
    mainview.cpp \
    user_input.cpp \
    model.cpp \
    shapes/polygon.cpp \
    shapes/multipolygon.cpp \
    shapes/shape.cpp

HEADERS  += mainwindow.h \
    mainview.h \
    model.h \
    shapes/polygon.h \
    shapes/vertex.h \
    shapes/multipolygon.h \
    shapes/shape.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
