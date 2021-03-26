#-------------------------------------------------
#
# Project created by QtCreator 2021-01-12T15:26:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_play
TEMPLATE = app


SOURCES += main.cpp\
        qtmainwindow.cpp \
    usb/usb_util.cpp \
    log/log.cpp \
    finance/finance.cpp \
    nl_del/nl_del.cpp \
    config/appsettings.cpp \
    edit/text2htmldlg.cpp

HEADERS  += qtmainwindow.h \
    usb/usb_util.h \
    log/log.h \
    finance/finance.h \
    nl_del/nl_del.h \
    config/appsettings.h \
    edit/text2htmldlg.h

FORMS    += qtmainwindow.ui \
    finance/finance.ui \
    nl_del/nl_del.ui \
    edit/text2htmldlg.ui

win32: LIBS += -L$$PWD/libs/ -llibusb-1.0

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

RESOURCES += \
    resources.qrc
