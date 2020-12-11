#ifndef HUMANINTERFACEDEVICE_H
#define HUMANINTERFACEDEVICE_H


#include <QObject>
#include <QAbstractNativeEventFilter>

#include "gui/interface_input.h"


namespace Input {



class HumanInterfaceDevice : public QObject, public Device, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit HumanInterfaceDevice(Interface* parent, int id);

public:
    bool Active(int sub, int code) override;
    void GetImmediate() override;
    void StopImmediate() override;
    QString CodeToString(int code) override;
    QString Name(int sub) override;

protected:
    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

private:
    bool mImmediate = false;
};


}


#endif // HUMANINTERFACEDEVICE_H
