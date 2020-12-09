#include "interface_input.h"

#include "keyboard.h"
#include "gamepad.h"

#include <QDebug>



QString ButtonToString(Button button)
{
    switch (button)
    {
    case Button::Up: return "Up";
    case Button::Down: return "Down";
    case Button::Left: return "Left";
    case Button::Right: return "Right";
    case Button::Start: return "Start";
    case Button::Select: return "Select";
    case Button::A: return "A";
    case Button::B: return "B";
    default: return "Unknown";
    }
}




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


ControllerState Input::GetState(int controller)
{
    ControllerState& state = mState[controller];
    ControllerInfo& info = mControllers[controller];
    for (auto& b : {Button::A, Button::B, Button::Start, Button::Select, Button::Up, Button::Down, Button::Left, Button::Right})
    {
        state[b] = info[b].IsActive();
    }
    return state;
}


ControllerInfo* Input::Controllers()
{
    return mControllers;
}


