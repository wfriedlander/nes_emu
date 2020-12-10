#include "interface_input.h"

//#include "keyboard.h"
//#include "gamepad.h"

#include "inputs/humaninterfacedevice.h"

#include <QDebug>







namespace Input1 {


    void DeviceProvider::Activated(Config::Mapping button)
    {
        button.provider = mID;
        mReceiver->Activated(button);
    }


    Interface::Interface(QObject* parent) : QObject(parent)
    {
        mProviders.push_back(new HIDProvider(this, 0));
//        mProviders.push_back(new GamepadProvider(this));
    }


    Core::Input::State Interface::State(int controller)
    {
        auto& state = mState[controller];
        auto& config = mConfig[controller];
        for (int i = 0; i < 8; i++) {
            auto& m = config[i];
            state[i] = mProviders[m.provider]->Device(m.device)->Active(m.code);
        }
        return state;
    }



}










//QString ButtonToString(Button button)
//{
//    switch (button)
//    {
//    case Button::Up: return "Up";
//    case Button::Down: return "Down";
//    case Button::Left: return "Left";
//    case Button::Right: return "Right";
//    case Button::Start: return "Start";
//    case Button::Select: return "Select";
//    case Button::A: return "A";
//    case Button::B: return "B";
//    default: return "Unknown";
//    }
//}




//InputDevices::InputDevices(QObject *parent) : QObject(parent)
//{
//    mDevices.push_back(new Keyboard(this));
//    mDevices.push_back(new Gamepad(this));

//    for (auto& dev : mDevices) {
//        connect(dev, &InputDevice::PushInput, this, &InputDevices::ReceiveInput);
//    }
//}

//ControllerInfo InputDevices::ControllerFromConfig(InputConfig config)
//{
//    ControllerInfo info;
//    info.type = config.type;

//    for (int i = 0; i < 8; i++)
//    {
//        Button b = static_cast<Button>(i);
//        info[b] = {mDevices[config[b].device], config[b].code};
//    }
//    return info;
//}

//InputConfig InputDevices::ConfigFromInfo(ControllerInfo info)
//{
//    return InputConfig();
//}

//void InputDevices::GetImmediateInput()
//{
//    mWaitingForInput = true;
//    for (auto& dev : mDevices) {
//        dev->EnableImmediateMode(true);
//    }
//}


//void InputDevices::ReceiveInput(InputMap im)
//{
//    if (mWaitingForInput) {
//        for (auto& dev : mDevices) {
//            dev->EnableImmediateMode(false);
//        }
//        emit PushInput(im);
//    }
//    mWaitingForInput = false;
//}





//Input::Input(QObject *parent) : QObject(parent)
//{

//}


//ControllerState Input::GetState(int controller)
//{
//    ControllerState& state = mState[controller];
//    ControllerInfo& info = mControllers[controller];
//    for (auto& b : {Button::A, Button::B, Button::Start, Button::Select, Button::Up, Button::Down, Button::Left, Button::Right})
//    {
//        state[b] = info[b].IsActive();
//    }
//    return state;
//}


//ControllerInfo* Input::Controllers()
//{
//    return mControllers;
//}


