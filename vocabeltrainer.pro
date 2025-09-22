QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
LIBS += -luser32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configmanager.cpp \
    gameselection.cpp \
    main.cpp \
    signin.cpp \
    signup.cpp \
    startwindow.cpp \
    vocabeltrainer.cpp

HEADERS += \
    configmanager.h \
    gameselection.h \
    signin.h \
    signup.h \
    startwindow.h \
    vocabeltrainer.h

FORMS += \
    gameselection.ui \
    signin.ui \
    signup.ui \
    startwindow.ui \
    vocabeltrainer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc \
    load.qrc
