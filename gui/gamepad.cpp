#include "gamepad.h"

//#include <QGamepad>
//#include <QGamepadManager>

Gamepad::Gamepad(QObject *parent) : InputDevice(parent)
{
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

QString Gamepad::DeviceName()
{
    return "Gamepad";
}

void Gamepad::EnableImmediateMode(bool enabled)
{

}

bool Gamepad::IsCodeActive(int code)
{
    return false;
}

QString Gamepad::CodeToString(int code)
{
    return "";
}


