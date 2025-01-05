QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    info_profile.cpp \
    lostpassword.cpp \
    main.cpp \
    mainwindow.cpp \
    openwall_crypt/crypt_blowfish.c \
    openwall_crypt/crypt_gensalt.c \
    openwall_crypt/wrapper.c \
    qtbcrypt.cpp \
    windows_reg.cpp

HEADERS += \
    info_profile.h \
    lostpassword.h \
    main.h \
    mainwindow.h \
    openwall_crypt/crypt.h \
    openwall_crypt/crypt_blowfish.h \
    openwall_crypt/crypt_gensalt.h \
    openwall_crypt/ow-crypt.h \
    qtbcrypt.h \
    windows_reg.h

FORMS += \
    info_profile.ui \
    lostpassword.ui \
    mainwindow.ui \
    windows_reg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    openwall_crypt/x86.S
