#-------------------------------------------------
#
# Project created by QtCreator 2018-07-26T14:17:43
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = Light_System_prototype
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
    device.cpp \
    tcp.cpp \
    person.cpp \
    point.cpp \
    map.cpp \
    devices.cpp \
    persons.cpp \
    client.cpp \
    coder.cpp

HEADERS += \
        widget.h \
    device.h \
    tcp.h \
    person.h \
    point.h \
    map.h \
    devices.h \
    persons.h \
    client.h \
    coder.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
