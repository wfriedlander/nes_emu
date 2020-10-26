#include "gamepad.h"

#include <QDebug>
#include <QGamepad>
#include <QGamepadManager>


Gamepad::Gamepad(QObject *parent) : InputDevice(parent)
{
    for (int i = 0; i < 4; i++) {
        mGamepads[i] = new QGamepad(i);
    }
}

QString Gamepad::DeviceName()
{
    return "Gamepad";
}

void Gamepad::EnableImmediateMode(bool enabled)
{
    if (enabled) {

        c0 = connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent,
            [this](int deviceId, QGamepadManager::GamepadButton button, double value) {
                if (value > 0.1) {
                    emit PushInput({this, (deviceId << 8) | static_cast<int>(button)});
                }
            });

        c1 = connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent,
            [this](int deviceId, QGamepadManager::GamepadAxis axis, double value) {
                if (value > 0.3) {
                    int code = 0;
                    switch (axis) {
                    case QGamepadManager::AxisLeftX: code = static_cast<int>(GP::Button::LSXP); break;
                    case QGamepadManager::AxisLeftY: code = static_cast<int>(GP::Button::LSYP); break;
                    case QGamepadManager::AxisRightX: code = static_cast<int>(GP::Button::RSXP); break;
                    case QGamepadManager::AxisRightY: code = static_cast<int>(GP::Button::RSYP); break;
                    default: return;
                    }
                    emit PushInput({this, (deviceId << 8) | code});
                }
                else if (value < -0.3) {
                    int code = 0;
                    switch (axis) {
                    case QGamepadManager::AxisLeftX: code = static_cast<int>(GP::Button::LSXN); break;
                    case QGamepadManager::AxisLeftY: code = static_cast<int>(GP::Button::LSYN); break;
                    case QGamepadManager::AxisRightX: code = static_cast<int>(GP::Button::RSXN); break;
                    case QGamepadManager::AxisRightY: code = static_cast<int>(GP::Button::RSYN); break;
                    default: return;
                    }
                    emit PushInput({this, (deviceId << 8) | code});
                }
            });
    }
    else {
        disconnect(c0);
        disconnect(c1);
    }
}

bool Gamepad::IsCodeActive(int code)
{
    int pad = code >> 8;
    GP::Button b = static_cast<GP::Button>(code & 0xFF);

    switch (b)
    {
    case GP::Button::A: return mGamepads[pad]->buttonA();
    case GP::Button::B: return mGamepads[pad]->buttonB();
    case GP::Button::X: return mGamepads[pad]->buttonX();
    case GP::Button::Y: return mGamepads[pad]->buttonY();
    case GP::Button::BACK: return mGamepads[pad]->buttonSelect();
    case GP::Button::START: return mGamepads[pad]->buttonStart();
    case GP::Button::LB: return mGamepads[pad]->buttonL1();
    case GP::Button::RB: return mGamepads[pad]->buttonR1();
    case GP::Button::LT: return mGamepads[pad]->buttonL2() > 0.1f;
    case GP::Button::RT: return mGamepads[pad]->buttonR2() > 0.1f;
    case GP::Button::L3: return mGamepads[pad]->buttonL3();
    case GP::Button::R3: return mGamepads[pad]->buttonR3();
    case GP::Button::UP: return mGamepads[pad]->buttonUp();
    case GP::Button::DOWN: return mGamepads[pad]->buttonDown();
    case GP::Button::LEFT: return mGamepads[pad]->buttonLeft();
    case GP::Button::RIGHT: return mGamepads[pad]->buttonRight();
    case GP::Button::LSXP: return mGamepads[pad]->axisLeftX() > 0.3f;
    case GP::Button::LSXN: return mGamepads[pad]->axisLeftX() < -0.3f;
    case GP::Button::LSYP: return mGamepads[pad]->axisLeftY() > 0.3f;
    case GP::Button::LSYN: return mGamepads[pad]->axisLeftY() < -0.3f;
    case GP::Button::RSXP: return mGamepads[pad]->axisRightX() > 0.3f;
    case GP::Button::RSXN: return mGamepads[pad]->axisRightX() < -0.3f;
    case GP::Button::RSYP: return mGamepads[pad]->axisRightY() > 0.3f;
    case GP::Button::RSYN: return mGamepads[pad]->axisRightY() < -0.3f;
    default: return false;
    }
}

QString Gamepad::CodeToString(int code)
{
    int pad = (code >> 8) + 1;
    GP::Button b = static_cast<GP::Button>(code & 0xFF);

    switch (b)
    {
    case GP::Button::A: return QString("Pad ") + QString::number(pad) + QString(" A");
    case GP::Button::B: return QString("Pad ") + QString::number(pad) + QString(" B");
    case GP::Button::X: return QString("Pad ") + QString::number(pad) + QString(" X");
    case GP::Button::Y: return QString("Pad ") + QString::number(pad) + QString(" Y");
    case GP::Button::BACK: return QString("Pad ") + QString::number(pad) + QString(" Back");
    case GP::Button::START: return QString("Pad ") + QString::number(pad) + QString(" Start");
    case GP::Button::LB: return QString("Pad ") + QString::number(pad) + QString(" LB");
    case GP::Button::RB: return QString("Pad ") + QString::number(pad) + QString(" RB");
    case GP::Button::LT: return QString("Pad ") + QString::number(pad) + QString(" LT");
    case GP::Button::RT: return QString("Pad ") + QString::number(pad) + QString(" RT");
    case GP::Button::L3: return QString("Pad ") + QString::number(pad) + QString(" L3");
    case GP::Button::R3: return QString("Pad ") + QString::number(pad) + QString(" R3");
    case GP::Button::UP: return QString("Pad ") + QString::number(pad) + QString(" Up");
    case GP::Button::DOWN: return QString("Pad ") + QString::number(pad) + QString(" Down");
    case GP::Button::LEFT: return QString("Pad ") + QString::number(pad) + QString(" Left");
    case GP::Button::RIGHT: return QString("Pad ") + QString::number(pad) + QString(" Right");
    case GP::Button::LSXP: return QString("Pad ") + QString::number(pad) + QString(" LS X+");
    case GP::Button::LSXN: return QString("Pad ") + QString::number(pad) + QString(" LS X-");
    case GP::Button::LSYP: return QString("Pad ") + QString::number(pad) + QString(" LS Y-");
    case GP::Button::LSYN: return QString("Pad ") + QString::number(pad) + QString(" LS Y+");
    case GP::Button::RSXP: return QString("Pad ") + QString::number(pad) + QString(" RS X+");
    case GP::Button::RSXN: return QString("Pad ") + QString::number(pad) + QString(" RS X-");
    case GP::Button::RSYP: return QString("Pad ") + QString::number(pad) + QString(" RS Y-");
    case GP::Button::RSYN: return QString("Pad ") + QString::number(pad) + QString(" RS Y+");
    default: return "Pad Unknown";
    }
}

