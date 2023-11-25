
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pac-Man
TEMPLATE = app



CONFIG += c++11

SOURCES += \
        espacio.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        espacio.h \
        espacio.h \
        mainwindow.h

FORMS += \
        mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
        espacio.h \
        mainwindow.h

FORMS += \
        mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
