QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Benötigt für PostgreSQL-Anbindung
QT += sql
unix:LIBS += -lpq
unix:LIBS += -L/Users/carlo/PostgreSQL/16/lib
win32: {
    # nur getestet mit mingw-64
    LIBS += -L'C:\Program Files\PostgreSQL\16\bin'
    LIBS += -llibpq
}

SOURCES += \
    actor.cpp \
    controller.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    movie.cpp \
    provider.cpp

HEADERS += \
    actor.h \
    controller.h \
    mainwindow.h \
    model.h \
    movie.h \
    provider.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../OneDrive/Desktop/Semester 6/Datenbanken/Abgabe/Bilder/logo_nontransparent.png

RESOURCES += \
    bilder.qrc
