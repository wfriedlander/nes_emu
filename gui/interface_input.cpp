#include "interface_input.h"

#include "keyboard.h"
#include "gamepad.h"

#include <QDebug>



InputDevices::InputDevices(QObject *parent) : QObject(parent)
{
    mDevices.push_back(new Keyboard(this));
    mDevices.push_back(new Gamepad(this));

    for (auto& dev : mDevices) {
        connect(dev, &InputDevice::PushInput, this, &InputDevices::ReceiveInput);
    }
}

void InputDevices::GetImmediateInput()
{
    mWaitingForInput = true;
    for (auto& dev : mDevices) {
        dev->EnableImmediateMode(true);
    }
}


void InputDevices::ReceiveInput(InputMap im)
{
    if (mWaitingForInput) {
        for (auto& dev : mDevices) {
            dev->EnableImmediateMode(false);
        }
        emit PushInput(im);
    }
    mWaitingForInput = false;
}












Input::Input(QObject *parent) : QObject(parent)
{

}


ControllerState Input::GetController(int controller)
{
    ControllerState& state = mState[controller];
    ControllerInfo& info = mControllers[controller];
    for (auto& b : {Button::A, Button::B, Button::Start, Button::Select, Button::Up, Button::Down, Button::Left, Button::Right})
    {
        auto& input = info.mapping[b];
        state[b] = input.IsActive();
    }
    return state;
}


void Input::SetController(int controller, ControllerInfo info)
{
    mControllers[controller] = info;
}


