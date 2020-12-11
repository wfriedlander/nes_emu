#include "interface_input.h"

#include "inputs/humaninterfacedevice.h"

#include <QDebug>


namespace Input {


Interface::Interface(QObject* parent) : QObject(parent)
{
    mDevices.push_back(new HumanInterfaceDevice(this, 0));
    //        mProviders.push_back(new GamepadProvider(this));
}

void Interface::StartImmediate(Receiver *receiver)
{
    mReceiver = receiver;
    for (auto& dev : mDevices) {
        dev->GetImmediate();
    }
}

void Interface::StopImmediate()
{
    mReceiver = nullptr;
    for (auto& dev : mDevices) {
        dev->StopImmediate();
    }
}

void Interface::Activated(Config::Mapping button)
{
    if (mReceiver) {
        mReceiver->Activated(button);
    }
}

QString Interface::MappingName(Config::Mapping m)
{
    return mDevices[m.dev]->CodeToString(m.code);
}

Core::Input::State Interface::State(int controller)
{
    auto& state = mState[controller];
    auto& config = mConfig[controller];
    for (int i = 0; i < 8; i++) {
        auto& m = config[i];
        state[i] = mDevices[m.dev]->Active(m.sub, m.code);
    }
    return state;
}

}


