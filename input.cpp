#include "input.h"

#include <QDebug>
#include <QKeyEvent>
#include <QGamepad>
#include <QGamepadManager>


Input::Input(QWidget *parent) : QWidget(parent)
{
    setVisible(false);
    setFocus();



//    qDebug() << "QGamePadManager Demo\n";

//    auto gamepads = QGamepadManager::instance()->connectedGamepads();
//    qDebug() << "Number of gamepads:" << gamepads.size();

//    for (auto i : gamepads) {
//        QGamepad *gamepad = new QGamepad(i);
//        qDebug() << "Gamepad:" << i;
//        qDebug() << "  device id:   " << gamepad->deviceId();
//        qDebug() << "  name:        " << gamepad->name();
//        qDebug() << "  is connected?" << gamepad->isConnected();
//    }

//    qDebug() << "\nMonitoring for events...\n";

//    connect(QGamepadManager::instance(), &QGamepadManager::connectedGamepadsChanged, this,
//        []() { qDebug() << "connected gamepads changed:"; });
//    connect(QGamepadManager::instance(), &QGamepadManager::gamepadConnected, this,
//        [](int deviceId) { qDebug() << "gamepad connected:" << deviceId; });
//    connect(QGamepadManager::instance(), &QGamepadManager::gamepadDisconnected, this,
//        [](int deviceId) { qDebug() << "gamepad disconnected:" << deviceId; });
//    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this,
//        [](int deviceId, QGamepadManager::GamepadButton button, double value) { qDebug() << "button press event:" << deviceId << button << value; });
//    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this,
//        [](int deviceId, QGamepadManager::GamepadButton button) { qDebug() << "button release event:" << deviceId << button; });
//    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this,
//        [](int deviceId, QGamepadManager::GamepadAxis axis, double value) { qDebug() << "axis event:" << deviceId << axis << value; });
//    connect(QGamepadManager::instance(), &QGamepadManager::buttonConfigured, this,
//        [](int deviceId, QGamepadManager::GamepadButton button) { qDebug() << "button configured:" << deviceId << button; });
//    connect(QGamepadManager::instance(), &QGamepadManager::axisConfigured, this,
//        [](int deviceId, QGamepadManager::GamepadAxis axis) { qDebug() << "axis configured:" << deviceId << axis; });
//    connect(QGamepadManager::instance(), &QGamepadManager::configurationCanceled, this,
    //        [](int deviceId) { qDebug() << "configuration canceled:" << deviceId; });
}


const ButtonState &Input::InputUpdate()
{
    return mState;
}


const Input::ButtonMap & Input::MapButton(int key, Input::Button button)
{
//    for (const auto& [k, v] : mMapping)
//    {
//        if (v == button)
//        {
//            qDebug() << k << " " << key;
//            mMapping.erase(k);
//        }
//    }

    for (auto it = mMapping.cbegin(); it != mMapping.cend();)
    {
        if(it->second == button)
        {
            mMapping.erase(it++);
        }
        else
        {
            ++it;
        }

    }
    mMapping[key] = button;

    return mMapping;
}

void Input::keyPressEvent(QKeyEvent *event)
{
    auto it = mMapping.find(event->key());
    if (it == mMapping.end()) return;
    auto [_, button] = *it;

    switch (button) {
    case Button::A:
        mState.a = true;
        break;
    case Button::B:
        mState.b = true;
        break;
    case Button::Start:
        mState.start = true;
        break;
    case Button::Select:
        mState.select = true;
        break;
    case Button::Up:
        mState.up = true;
        break;
    case Button::Down:
        mState.down = true;
        break;
    case Button::Left:
        mState.left = true;
        break;
    case Button::Right:
        mState.right = true;
        break;
    }
}

void Input::keyReleaseEvent(QKeyEvent *event)
{
    auto it = mMapping.find(event->key());
    if (it == mMapping.end()) return;
    auto [_, button] = *it;

    switch (button) {
    case Button::A:
        mState.a = false;
        break;
    case Button::B:
        mState.b = false;
        break;
    case Button::Start:
        mState.start = false;
        break;
    case Button::Select:
        mState.select = false;
        break;
    case Button::Up:
        mState.up = false;
        break;
    case Button::Down:
        mState.down = false;
        break;
    case Button::Left:
        mState.left = false;
        break;
    case Button::Right:
        mState.right = false;
        break;
    }
}
