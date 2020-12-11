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
    void SetButtonState(int button, bool state);

signals:
    void ButtonPressed(int, QPushButton*);

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
    explicit ControllerPanel(int id, QWidget *parent = nullptr);

public:
    void ResetDisplayButtons();
    void SetDisplayButton(int button, bool active);

public:
    QPushButton* operator[](int i) {return mButtons[i];}

private slots:
    void DisplayPressed(int button, QPushButton* pushbutton);

signals:
    void TypeChanged(QString);
    void SavePreset(int);
    void LoadPreset(int);
    void AssignButton(int, QPushButton*);

private:
    ControllerDisplay* mDisplay = nullptr;
    QPushButton* mButtons[8] = { };
    QComboBox* mPreset = nullptr;
};



///////////////////////////////////////////////////////////////////////////////////////////
/// \brief The SettingsController class
///

class ControllerWindow : public QDialog, public Input::Receiver
{
    Q_OBJECT
public:
    ControllerWindow(Input::Interface* iface, QWidget *parent = nullptr);

public:
    void Activated(Input::Config::Mapping m) override;

private slots:
    void AssignButton(int controller, int button, QPushButton* pbutton);
    void EndAssign();
    void SavePreset(int controller, int preset);
    void LoadPreset(int controller, int preset);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    int poll_id = -1;
    QTimer* mTimer;
    Input::Interface* mInterface;
    Input::Config mConfig[2];
    Input::Config mOldConfig[2];
    ControllerPanel* mPanel[2];
    QPushButton* mSelected = nullptr;
    int mAssignController = 0;
    int mAssignButton = 0;
};

#endif // SETTINGSCONTROLLER_H
