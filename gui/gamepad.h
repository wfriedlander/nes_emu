#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QObject>

#include "interface_input.h"


class Gamepad : public InputDevice
{
    Q_OBJECT
public:
    explicit Gamepad(QObject *parent = nullptr);

public:
    bool IsCodeActive(int code);
    void EnableImmediateMode(bool enabled);
    QString CodeToString(int code);
    QString DeviceName();

};

#endif // GAMEPAD_H
