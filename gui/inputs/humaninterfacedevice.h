#ifndef HUMANINTERFACEDEVICE_H
#define HUMANINTERFACEDEVICE_H


#include <QObject>
#include <QAbstractNativeEventFilter>

#include "gui/interface_input.h"


namespace Input1 {



class HIDProvider : public QObject, public DeviceProvider
{
public:
    HIDProvider(Interface* interface, int device_number);

public:
    QString Name();
    Input1::Device* Device(int i);

private:
    Input1::Device* mDevice = nullptr;
};




class HumanInterfaceDevice : public QObject, public Device, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit HumanInterfaceDevice(HIDProvider* provider);

public:
    bool Active(int code) override;
    void GetImmediate() override;
    void StopImmediate() override;
    QString CodeToString(int code) override;
    QString Name() override;

protected:
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

private:
    bool mImmediate = false;
};


}


#endif // HUMANINTERFACEDEVICE_H
