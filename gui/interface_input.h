#ifndef INPUT_H
#define INPUT_H

#include "interface.h"

#include <chrono>
#include <vector>
#include <QObject>


QString ButtonToString(Button button);


struct InputMap;

class InputDevice : public QObject
{
    Q_OBJECT
public:
    InputDevice(QObject *parent) : QObject(parent) {}
public:
    virtual bool IsCodeActive(int code) = 0;
    virtual void EnableImmediateMode(bool enabled) = 0;
    virtual QString CodeToString(int code) = 0;
    virtual QString DeviceName() = 0;

signals:
    void PushInput(InputMap im);
};



class InputDevices : public QObject
{
    Q_OBJECT
public:
    InputDevices(QObject* parent);

public:
    void GetImmediateInput();

private slots:
    void ReceiveInput(InputMap im);

signals:
    void PushInput(InputMap im);

private:
    std::vector<InputDevice*> mDevices;
    bool mWaitingForInput = false;
};



struct InputMap
{
    InputDevice* device = nullptr;
    int code = 0;

    bool IsActive() {return device == nullptr ? false : device->IsCodeActive(code);}
    QString Name() {return device == nullptr ? "" : device->CodeToString(code);}
};


enum class ControllerType
{
    None,
    Standard,
    Zapper
};

struct ControllerInfo
{
    ControllerType type = ControllerType::None;
    InputMap& operator[](Button b) {return dev[static_cast<int>(b)];}
private:
    InputMap dev[8];
};






class Input : public QObject, public IInput
{
    Q_OBJECT

public:
    explicit Input(QObject *parent = nullptr);

public:
    ControllerState GetState(int controller) override;

public:
    ControllerInfo* Controllers();

private:
    ControllerInfo mControllers[2];
    ControllerState mState[2];

};

#endif // INPUT_H
