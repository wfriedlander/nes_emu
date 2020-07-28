QT       += core gui gamepad multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    apu.cpp \
    audio.cpp \
    blipbuffer.cpp \
    bus.cpp \
    cartridge.cpp \
    controller.cpp \
    controllerinput.cpp \
    cpu.cpp \
    gameloader.cpp \
    gamewindow.cpp \
    input.cpp \
    main.cpp \
    mainwindow.cpp \
    mappers.cpp \
    nes.cpp \
    ppu.cpp \
    serializer.cpp \
    video.cpp

HEADERS += \
    apu.h \
    audio.h \
    blipbuffer.h \
    bus.h \
    cartridge.h \
    common.h \
    controller.h \
    controllerinput.h \
    cpu.h \
    gameloader.h \
    gamewindow.h \
    input.h \
    interface.h \
    json.h \
    mainwindow.h \
    mappers.h \
    nes.h \
    ppu.h \
    serializer.h \
    video.h

FORMS += \
    controllerinput.ui \
    test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../PortAudio/Release/ -lportaudio_x86
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../PortAudio/Debug/ -lportaudio_x86

INCLUDEPATH += $$PWD/../PortAudio/include
DEPENDPATH += $$PWD/../PortAudio/include
