#ifndef CONFIGURE_CONTROLLER_H
#define CONFIGURE_CONTROLLER_H

#include "interface_input.h"

#include <QWidget>
#include <QDialog>

class QPushButton;
class QComboBox;
class QTimer;

///////////////////////////////////////////////////////////////////////////////////////////
/// \brief The ControllerDisplay class
///


class ControllerDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ControllerDisplay(QWidget *parent = nullptr);

public slots:
    void ResetButtonState();
    void SetButtonState(Button button, bool state);

signals:
    void ButtonPressed(Button, QPushButton*);

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


///////////////////////////////////////////////////////////////////////////////////////////
/// \brief The ControllerPanel class
///

class ControllerPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControllerPanel(int id, ControllerInfo& info, QWidget *parent = nullptr);

public:
    void ResetDisplayButtons();
    void SetDisplayButton(Button button, bool active);

private slots:
    void DisplayPressed(Button button, QPushButton* pushbutton);

signals:
    void TypeChanged(QString);
    void SavePreset(int);
    void LoadPreset(int);
    void AssignButton(Button, QPushButton*);

private:
    ControllerDisplay* mDisplay = nullptr;
    QPushButton* mButtons[8] = { };
    QComboBox* mPreset = nullptr;
};



///////////////////////////////////////////////////////////////////////////////////////////
/// \brief The SettingsController class
///

class ControllerWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ControllerWindow(InputDevices* devices, ControllerInfo* controllers, QWidget *parent = nullptr);

public:
    ControllerInfo* Controllers();

private slots:
    void AssignButton(int controller, Button button, QPushButton* pbutton);
    void SavePreset(int controller, int preset);
    void LoadPreset(int controller, int preset);

protected:
    void timerEvent(QTimerEvent* event);

private:
    int poll_id = -1;
    QTimer* mTimer;
    InputDevices* mDevices;
    ControllerInfo mInfo[2];

    ControllerPanel* mPanel[2];


    QPushButton* mSelected = nullptr;
};

#endif // SETTINGSCONTROLLER_H
