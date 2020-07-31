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
    src/apu.cpp \
    src/audio.cpp \
    src/blipbuffer.cpp \
    src/bus.cpp \
    src/cartridge.cpp \
    src/controller.cpp \
    src/controllerinput.cpp \
    src/cpu.cpp \
    src/gameloader.cpp \
    src/gamewindow.cpp \
    src/input.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mappers.cpp \
    src/nes.cpp \
    src/ppu.cpp \
    src/serializer.cpp \
    src/video.cpp

HEADERS += \
    src/apu.h \
    src/audio.h \
    src/blipbuffer.h \
    src/bus.h \
    src/cartridge.h \
    src/common.h \
    src/controller.h \
    src/controllerinput.h \
    src/cpu.h \
    src/gameloader.h \
    src/gamewindow.h \
    src/input.h \
    src/interface.h \
    src/json.h \
    src/mainwindow.h \
    src/mappers.h \
    src/nes.h \
    src/ppu.h \
    src/serializer.h \
    src/video.h

FORMS += \
    src/controllerinput.ui \
    src/test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -luser32 -ladvapi32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/extlib/windows/x86/release/ -lportaudio_static_x86
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/extlib/windows/x86/debug/ -lportaudio_static_x86
else:unix: LIBS += -L$$PWD/extlib/linux/x86/ -lportaudio_static_x86

INCLUDEPATH += $$PWD/extlib/include
DEPENDPATH += $$PWD/extlib/include
