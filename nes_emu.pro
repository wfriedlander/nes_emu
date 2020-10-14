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
    core/apu.cpp \
    core/blipbuffer.cpp \
    core/bus.cpp \
    core/cartridge.cpp \
    core/controller.cpp \
    core/cpu.cpp \
    core/mappers.cpp \
    core/nes.cpp \
    core/ppu.cpp \
    core/serializer.cpp \
    gui/audio.cpp \
    gui/controllerinput.cpp \
    gui/gameloader.cpp \
    gui/input.cpp \
    gui/keyboard.cpp \
    gui/main.cpp \
    gui/mainwindow.cpp \
    gui/settingscontroller.cpp \
    gui/video.cpp

HEADERS += \
    core/apu.h \
    core/bus.h \
    core/cartridge.h \
    core/controller.h \
    core/cpu.h \
    core/mappers.h \
    core/ppu.h \
    core/serializer.h \
    gui/audio.h \
    gui/controllerinput.h \
    gui/gameloader.h \
    gui/input.h \
    gui/keyboard.h \
    gui/mainwindow.h \
    gui/settingscontroller.h \
    gui/video.h \
    include/blipbuffer.h \
    include/common.h \
    include/interface.h \
    include/json.h \
    include/nes.h

FORMS += \
    gui/controllerinput.ui \
    gui/test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -luser32 -ladvapi32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/extlib/windows/x86/release/ -lportaudio_static_x86
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/extlib/windows/x86/debug/ -lportaudio_static_x86
else:unix: LIBS += -L$$PWD/extlib/linux/x86/ -lportaudio_static_x86

INCLUDEPATH += $$PWD/extlib/include $$PWD/include
DEPENDPATH += $$PWD/extlib/include

RESOURCES += \
    resources/resources.qrc

RC_ICONS = nes_icon.ico
