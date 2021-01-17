#-------------------------------------------------
#
# Project created by QtCreator 2021-01-12T15:26:03
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_play
TEMPLATE = app


SOURCES += main.cpp\
        qtmainwindow.cpp \
    usb/usb_util.cpp \
    log/log.cpp \
    camera/uvc_camera.cpp \
    camera/image_settings.cpp \
    camera/video_settings.cpp

HEADERS  += qtmainwindow.h \
    usb/usb_util.h \
    log/log.h \
    camera/uvc_camera.h \
    camera/image_settings.h \
    camera/video_settings.h

FORMS    += qtmainwindow.ui \
    camera/video_settings.ui \
    camera/uvc_camera.ui \
    camera/image_settings.ui

win32: LIBS += -L$$PWD/libs/ -llibusb-1.0

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

RESOURCES += \
    resources.qrc
