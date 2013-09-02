#-------------------------------------------------
#
# Project created by QtCreator 2013-08-28T16:24:25
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CODECFORTR		= UTF-8
CODECFORSRC		= UTF-8

DESTDIR +=  output/bin-release
TARGET = JoomlaTranslator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32 {
    RC_FILE += version_info.rc
}

RESOURCES += \
    res.qrc
