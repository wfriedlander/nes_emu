#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "interface_input.h"

#include <QWidget>
#include <QDialog>
#include <QPushButton>


class ControllerDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ControllerDisplay(QWidget *parent = nullptr);

public slots:
    void PressButton(int button);
    void ReleaseButton(int button);

signals:
    void ButtonPressed(int button);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPushButton* up;
    QPushButton* down;
    QPushButton* left;
    QPushButton* right;
    QPushButton* start;
    QPushButton* select;
    QPushButton* a;
    QPushButton* b;
};


class ControllerPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControllerPanel(const QString& title, QWidget *parent = nullptr);

public:
    void ShowButton(int pad, int button, bool active);

public:


signals:
    void TypeChanged(QString);
    void SavePreset(int);
    void LoadPreset(int);
    void AssignButton(Button, QPushButton*);

};


class SettingsController : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsController(InputDevices* devices, QWidget *parent = nullptr);

private slots:
    void ReceiveInput(InputDevice* device, int code);
    void AssignButton(Button button, QPushButton* pbutton);

private:
    InputDevices* mDevices;
    ControllerInfo mInfo[2];



    QPushButton* mSelected = nullptr;
};

#endif // SETTINGSCONTROLLER_H
