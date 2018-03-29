#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T22:55:02
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stick
TEMPLATE = app


SOURCES += main.cpp\
        stick.cpp \
    player.cpp \
    snake.cpp \
    myview.cpp \
    readname.cpp \
    record.cpp \


HEADERS  += stick.h \
    player.h \
    snake.h \
    myview.h \
    readname.h \
    record.h \


RESOURCES += \
    png.qrc

FORMS += \
    readname.ui \
    record.ui
