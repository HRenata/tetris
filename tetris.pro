QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
OUTPUT += Console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ClientStuff.cpp \
    Controller.cpp \
    Figure.cpp \
    Game.cpp \
    View.cpp \
    main.cpp

HEADERS += \
    ClientStuff.h \
    Controller.h \
    Figure.h \
    Game.h \
    ICallbackFigureWatcher.h \
    ICallbackGameStateWatcher.h \
    View.h\
    interface.h \
    ../helper_class/helper_class.h \
    ../helper_class/helper_class_global.h \
    ../helper/helper.h \
    ../helper/helper_global.h \
    ../About/about.h \
    ../About/About_global.h


FORMS += \
    view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32{
    CONFIG(release, debug|release) {
        LIBS += \
            -L../helper_class/release/ -lhelper_class \
            -L../helper/release/ -lhelper \
            -L../About/release/ -labout
    }
    else {
        LIBS += \
            -L../helper_class/debug/ -lhelper_class \
            -L../helper/debug/ -lhelper \
            -L../About/debug/ -labout
    }
}

INCLUDEPATH += \
    ../helper_class \
    ../helper \
    ../About

DEPENDPATH += \
    ../helper_class \
    ../helper \
    ../About

DISTFILES += \
    client.json
