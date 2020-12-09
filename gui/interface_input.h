#ifndef INPUT_H
#define INPUT_H

#include "core/interface.h"

#include <chrono>
#include <vector>
#include <QObject>


QString ButtonToString(Button button);

////////////////////////////////////////////////////////
// ENUM FOR DIFFERENT CONTROLLER TYPES
/////////////////////////////
enum class ControllerType
{
    None,
    Standard,
    Zapper
};


////////////////////////////////////////////////////////
// STRUCT THAT DEFINES A SERIALIZABLE FORMAT TO BE
// CONVERTED TO/FROM A USABLE CONTROLLER MAP
// CONFIG SYSTEM SHOULD USE THIS TO SAVE/LOAD MAP
/////////////////////////////
struct InputConfig
{
    struct ButtonConfig {
        int provider = 0;
        int device = 0;
        int code = 0;
    };

    ControllerType type = ControllerType::None;
    ButtonConfig& operator[](Button b) {return cfg[static_cast<int>(b)];}
private:
    ButtonConfig cfg[8];

};


////////////////////////////////////////////////////////
// FORWARD DECLARATION OF INPUTMAP
/////////////////////////////
struct InputMap;


////////////////////////////////////////////////////////
// BASE CLASS REPRESENTING AN ACTUAL INPUT DEVICE
// LIKE KEYBOARD OR GAMEPAD
/////////////////////////////
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


////////////////////////////////////////////////////////
// STRUCT THAT REPRESENTS A SINGLE BUTTON
// CONTAINS THE PHYSICAL DEVICE AND CODE THAT
// DEVICE USES TO REPRESENT THE PHYSICAL BUTTON
/////////////////////////////
struct InputMap
{
    InputDevice* device = nullptr;
    int code = 0;

    bool IsActive() {return device == nullptr ? false : device->IsCodeActive(code);}
    QString Name() {return device == nullptr ? "" : device->CodeToString(code);}
};


////////////////////////////////////////////////////////
// STRUCT FOR AN ENTIRE NES CONTROLLER
// SPECIFIES CONTROLLER TYPE AND WHAT PHYSICAL
// DEVICE AND BUTTON REPRESENT ALL 8 NES PAD BUTTONS
/////////////////////////////
struct ControllerInfo
{
    ControllerType type = ControllerType::None;
    InputMap& operator[](Button b) {return dev[static_cast<int>(b)];}
private:
    InputMap dev[8];
};


////////////////////////////////////////////////////////
// CONTAINER CLASS FOR ALL PHYSICAL INPUT DEVICES
// MAIN APPLICATION USES THIS TO INTERFACE WITH
// THOSE DEVICES
/////////////////////////////
class InputDevices : public QObject
{
    Q_OBJECT
public:
    InputDevices(QObject* parent);

public:
    ControllerInfo ControllerFromConfig(InputConfig config);
    InputConfig ConfigFromInfo(ControllerInfo info);

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


////////////////////////////////////////////////////////
// NES CORE INTERFACE
/////////////////////////////
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
