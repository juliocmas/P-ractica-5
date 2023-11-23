
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pac-Man
TEMPLATE = app



CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    field.cpp

HEADERS += \
        mainwindow.h \
    field.h \
    field.h

FORMS += \
        mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
