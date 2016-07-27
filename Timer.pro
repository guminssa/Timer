#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T15:04:20
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Timer
TEMPLATE = app


SOURCES += main.cpp\
        timer.cpp \
    timermanagerui.cpp \
    internaltimer.cpp

HEADERS  += timer.h \
    timermanagerui.h \
    internaltimer.h

FORMS    += timer.ui

RESOURCES += \
    resources.qrc
