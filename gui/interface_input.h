#ifndef INPUT_H
#define INPUT_H

#include "core/interface.h"

#include <chrono>
#include <vector>
#include <QObject>




namespace Input {


struct Config
{
    struct Mapping
    {
        int dev = 0;
        int sub = 0;
        int code = 0;
    };
    Mapping& operator[](int i) {return button[i];}
private:
    Mapping button[8];
};



class Receiver
{
public:
    virtual void Activated(Config::Mapping button) = 0;
};



class Device
{
public:
    Device(Receiver* receiver, int id) : mReceiver(receiver), mId(id) {}
    virtual ~Device() {}
public:
    virtual bool Active(int sub, int code) = 0;
    virtual void GetImmediate() = 0;
    virtual void StopImmediate() = 0;
    virtual QString CodeToString(int code) = 0;
    virtual QString Name(int sub) = 0;
protected:
    Receiver* mReceiver;
    int mId = -1;
};



class Interface : public QObject, public Receiver, public Core::Input::Interface
{
    Q_OBJECT

public:
    explicit Interface(QObject *parent = nullptr);

public:
    void StartImmediate(Receiver* receiver);
    void StopImmediate();
    void Activated(Config::Mapping button) override;
    QString MappingName(Config::Mapping m);
    Config GetConfig(int n) {return mConfig[n];}
    void SetConfig(int n, Config c) {mConfig[n] = c;}
    Core::Input::State State(int controller) override;


private:
    Config mConfig[2];
    Core::Input::State mState[2];
    std::vector<Device*> mDevices;
    Receiver* mReceiver = nullptr;
};


}



#endif // INPUT_H
