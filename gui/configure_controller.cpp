#include "configure_controller.h"

#include <QApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QDebug>



QString ButtonToString(int button)
{
    switch (button)
    {
    case Core::Input::Up: return "Up";
    case Core::Input::Down: return "Down";
    case Core::Input::Left: return "Left";
    case Core::Input::Right: return "Right";
    case Core::Input::Start: return "Start";
    case Core::Input::Select: return "Select";
    case Core::Input::A: return "A";
    case Core::Input::B: return "B";
    default: return "Unknown";
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class ControllerDisplay
/// This is the drawing of the NES controller
///


ControllerDisplay::ControllerDisplay(QWidget *parent) : QWidget(parent)
{
    up = new QPushButton(this);
    up->setGeometry(96, 108, 56, 42);
    up->setFocusPolicy(Qt::NoFocus);
    up->setStyleSheet(R"ss(
        QPushButton          {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover    {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:1, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed  {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:1, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:disabled {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:1, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
    )ss");
    connect(up, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::Up, up);});


    down = new QPushButton(this);
    down->setGeometry(96, 206, 56, 42);
    down->setFocusPolicy(Qt::NoFocus);
    down->setStyleSheet(R"ss(
        QPushButton          {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover    {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:0, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed  {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:disabled {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        )ss");
    connect(down, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::Down, down);});

    left = new QPushButton(this);
    left->setGeometry(54, 150, 56, 56);
    left->setFocusPolicy(Qt::NoFocus);
    left->setStyleSheet(R"ss(
        QPushButton          {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover    {background: qlineargradient(x1:0.5, y1:0, x2:0.8, y2:0, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed  {background: qlineargradient(x1:0.5, y1:0, x2:0.8, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:disabled {background: qlineargradient(x1:0.5, y1:0, x2:0.8, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        )ss");
    connect(left, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::Left, left);});

    right = new QPushButton(this);
    right->setGeometry(138, 150, 56, 56);
    right->setFocusPolicy(Qt::NoFocus);
    right->setStyleSheet(R"ss(
        QPushButton          {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover    {background: qlineargradient(x1:0.5, y1:0, x2:0.2, y2:0, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed  {background: qlineargradient(x1:0.5, y1:0, x2:0.2, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:disabled {background: qlineargradient(x1:0.5, y1:0, x2:0.2, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        )ss");
    connect(right, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::Right, right);});

    select = new QPushButton(this);
    select->setGeometry(249, 200, 55, 20);
    select->setFocusPolicy(Qt::NoFocus);
    select->setStyleSheet(R"ss(
        QPushButton          {background: rgb(60, 60, 60); border-radius: 6px; border-style: none;}
        QPushButton:hover    {background: rgb(40, 40, 40);}
        QPushButton:pressed  {background: rgb(20, 20, 20);}
        QPushButton:disabled {background: rgb(20, 20, 20);}
        )ss");
    connect(select, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::Select, select);});

    start = new QPushButton(this);
    start->setGeometry(344, 200, 55, 20);
    start->setFocusPolicy(Qt::NoFocus);
    start->setStyleSheet(R"ss(
        QPushButton          {background: rgb(60, 60, 60); border-radius: 6px; border-style: none;}
        QPushButton:hover    {background: rgb(40, 40, 40);}
        QPushButton:pressed  {background: rgb(20, 20, 20);}
        QPushButton:disabled {background: rgb(20, 20, 20);}
        )ss");
    connect(start, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::Start, start);});

    b = new QPushButton(this);
    b->setGeometry(456, 174, 74, 74);
    b->setFocusPolicy(Qt::NoFocus);
    b->setStyleSheet(R"ss(
        QPushButton          {background: rgb(196, 16, 16); border-radius: 37px; border-style: none;}
        QPushButton:hover    {background: rgb(176, 16, 16);}
        QPushButton:pressed  {background: rgb(150, 8, 8);}
        QPushButton:disabled {background: rgb(150, 8, 8);}
        )ss");
    connect(b, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::B, b);});

    a = new QPushButton(this);
    a->setGeometry(548, 174, 74, 74);
    a->setFocusPolicy(Qt::NoFocus);
    a->setStyleSheet(R"ss(
        QPushButton          {background: rgb(196, 16, 16); border-radius: 37px; border-style: none;}
        QPushButton:hover    {background: rgb(176, 16, 16);}
        QPushButton:pressed  {background: rgb(150, 8, 8);}
        QPushButton:disabled {background: rgb(150, 8, 8);}
        )ss");
    connect(a, &QPushButton::clicked, [this](){emit ButtonPressed(Core::Input::A, a);});
}

void ControllerDisplay::ResetButtonState()
{
    up->setDisabled(false);
    down->setDisabled(false);
    left->setDisabled(false);
    right->setDisabled(false);
    start->setDisabled(false);
    select->setDisabled(false);
    a->setDisabled(false);
    b->setDisabled(false);
    qApp->postEvent(up, new QFocusEvent(QEvent::Leave));
    qApp->postEvent(down, new QFocusEvent(QEvent::Leave));
    qApp->postEvent(left, new QFocusEvent(QEvent::Leave));
    qApp->postEvent(right, new QFocusEvent(QEvent::Leave));
    qApp->postEvent(start, new QFocusEvent(QEvent::Leave));
    qApp->postEvent(select, new QFocusEvent(QEvent::Leave));
    qApp->postEvent(a, new QFocusEvent(QEvent::Leave));
    qApp->postEvent(b, new QFocusEvent(QEvent::Leave));
}

void ControllerDisplay::SetButtonState(int button, bool state)
{
    switch (button)
    {
    case Core::Input::Up: up->setDisabled(state); break;
    case Core::Input::Down: down->setDisabled(state); break;
    case Core::Input::Left: left->setDisabled(state); break;
    case Core::Input::Right: right->setDisabled(state); break;
    case Core::Input::Start: start->setDisabled(state); break;
    case Core::Input::Select: select->setDisabled(state); break;
    case Core::Input::A: a->setDisabled(state); break;
    case Core::Input::B: b->setDisabled(state); break;
    }
}



void ControllerDisplay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QColor dark(60, 60, 60);
    QColor mid(160, 160, 160);
    QColor light(205, 205, 205);
    QColor black(0, 0, 0);
    QColor red(196, 16, 16);

    // CONTROLLER BODY
    painter.fillRect(0, 0, 700, 300, light);
    painter.fillRect(24, 48, 652, 228, dark);
    painter.drawRect(0, 0, 699, 299);

    // CENTER LINES
    painter.fillRect(224, 48, 200, 32, mid);
    painter.fillRect(224, 92, 200, 32, mid);
    painter.fillRect(224, 136, 200, 32, mid);
    painter.fillRect(224, 252, 200, 24, mid);
    painter.fillRect(224, 180, 200, 60, light);

    // A/B BUTTONS
    painter.setBrush(QBrush(red));
    painter.fillRect(224+200+28, 170, 80, 80, light);
    painter.fillRect(224+200+28+92, 170, 80, 80, light);
    painter.drawEllipse(224+200+32, 174, 72, 72);
    painter.drawEllipse(224+200+32+92, 174, 72, 72);

    // D-PAD
    auto pen = painter.pen();
    pen.setColor(light);
    pen.setWidth(5);
    painter.setPen(pen);
    painter.drawRoundedRect(54, 150, 140, 56, 2, 2);
    painter.drawRoundedRect(96, 108, 56, 140, 2, 2);
    painter.fillRect(54, 150, 140, 56, black);
    painter.fillRect(96, 108, 56, 140, black);

    // TEXT
    painter.setPen(red);
    painter.setFont(QFont("Pretendo", 24));
    painter.drawText(452, 92, 172, 32, Qt::AlignHCenter, "Nintendo");
    painter.setFont(QFont("Pretendo", 14));
    painter.drawText(232, 136, 200-16, 32, Qt::AlignVCenter | Qt::AlignLeft, "SELECT");
    painter.drawText(232, 136, 200-16, 32, Qt::AlignVCenter | Qt::AlignRight, "START");
    painter.drawText(452, 250, 80, 26, Qt::AlignVCenter | Qt::AlignRight, "B");
    painter.drawText(544, 250, 80, 26, Qt::AlignVCenter | Qt::AlignRight, "A");
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class ControllerPanel
/// Settings panel for a single controller
/// Has presets/button assignment/controller diagram
///


ControllerPanel::ControllerPanel(int id, QWidget *parent) : QWidget(parent)
{
    auto main_layout = new QHBoxLayout(this);
    auto main_box = new QGroupBox(QString("Controller ") + QString::number(id), this);
    auto main_box_layout = new QHBoxLayout(main_box);
    auto settings = new QWidget(main_box);

    auto settings_layout = new QVBoxLayout(settings);

    auto type_layout = new QHBoxLayout();
    auto type_label = new QLabel("Type", settings);
    type_layout->addWidget(type_label);
    auto type_combo = new QComboBox(settings);
    type_combo->addItems({"None", "Standard", "Zapper"});
    type_combo->setFocusPolicy(Qt::NoFocus);
    type_layout->addWidget(type_combo);
    settings_layout->addLayout(type_layout);

    auto preset_layout = new QHBoxLayout();
    auto preset_label = new QLabel("Preset", settings);
    preset_layout->addWidget(preset_label);
    mPreset = new QComboBox(settings);
    mPreset->addItems({"1", "2", "3", "4", "5"});
    mPreset->setFocusPolicy(Qt::NoFocus);
    preset_layout->addWidget(mPreset);
    settings_layout->addLayout(preset_layout);

    auto save_load_layout = new QHBoxLayout();
    auto preset_load = new QPushButton("Load", settings);
    preset_load->setAutoDefault(false);
    preset_load->setFocusPolicy(Qt::NoFocus);
    save_load_layout->addWidget(preset_load);
    auto preset_save = new QPushButton("Save", settings);
    preset_save->setAutoDefault(false);
    preset_save->setFocusPolicy(Qt::NoFocus);
    save_load_layout->addWidget(preset_save);
    settings_layout->addLayout(save_load_layout);

    auto button_box = new QGroupBox("Buttons", settings);
    auto button_box_layout = new QGridLayout(button_box);
    for (int i = 0; i < 8; i++)
    {
        auto label = new QLabel(ButtonToString(i), button_box);
        button_box_layout->addWidget(label, i, 0, 1, 1);
        mButtons[i] = new QPushButton(button_box);
        mButtons[i]->setAutoDefault(false);
        mButtons[i]->setCheckable(true);
        mButtons[i]->setFocusPolicy(Qt::NoFocus);
        button_box_layout->addWidget(mButtons[i], i, 1, 1, 1);
        connect(mButtons[i], &QPushButton::toggled, [=](bool t) { if (t) emit AssignButton(i, mButtons[i]);});
    }
    settings_layout->addWidget(button_box);
    main_box_layout->addWidget(settings);

    mDisplay = new ControllerDisplay(main_box);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mDisplay->setSizePolicy(sizePolicy);
    mDisplay->setMinimumSize(QSize(700, 300));
    mDisplay->setMaximumSize(QSize(700, 300));
    main_box_layout->addWidget(mDisplay);

    main_layout->addWidget(main_box);

    connect(type_combo, &QComboBox::currentTextChanged, this, &ControllerPanel::TypeChanged);
    connect(preset_save, &QPushButton::clicked, [this](){emit SavePreset(mPreset->currentIndex());});
    connect(preset_load, &QPushButton::clicked, [this](){emit LoadPreset(mPreset->currentIndex());});
    connect(mDisplay, &ControllerDisplay::ButtonPressed, this, &ControllerPanel::DisplayPressed);
}

void ControllerPanel::ResetDisplayButtons()
{
    mDisplay->ResetButtonState();
}

void ControllerPanel::SetDisplayButton(int button, bool active)
{
    mDisplay->SetButtonState(button, active);
}

void ControllerPanel::DisplayPressed(int button, QPushButton *pushbutton)
{
    mDisplay->ResetButtonState();
    pushbutton->setDisabled(true);
    mButtons[static_cast<int>(button)]->setChecked(true);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class SettingsController
/// Main window for controller settings
/// Has two ControllerPanels and accept/reject
/// Handles all of the actual input config stuff
/// todo: give this a better name
///


ControllerWindow::ControllerWindow(Input::Interface* interface, QWidget *parent) : QDialog(parent), mInterface(interface)
{
    setWindowTitle("Controller Settings");
    resize(1000, 875);

    auto main_layout = new QVBoxLayout(this);
    for (int i = 0; i < 2; i++) {
        mPanel[i] = new ControllerPanel(i+1, this);
        main_layout->addWidget(mPanel[i]);
        mConfig[i] = interface->GetConfig(i);
        mOldConfig[i] = interface->GetConfig(i);
        for (int b = 0; b < 8; b++) {
            (*mPanel[i])[b]->setText(interface->MappingName(mConfig[i][b]));
        }
    }
    auto dialog_layout = new QHBoxLayout();
    auto spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    dialog_layout->addItem(spacer);
    auto accept = new QPushButton("Accept", this);
    accept->setFocusPolicy(Qt::NoFocus);
    dialog_layout->addWidget(accept);
    accept->setAutoDefault(false);
    auto cancel = new QPushButton("Cancel", this);
    cancel->setFocusPolicy(Qt::NoFocus);
    cancel->setAutoDefault(false);
    dialog_layout->addWidget(cancel);
    main_layout->addLayout(dialog_layout);

    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    connect(mTimer, &QTimer::timeout, this, &ControllerWindow::EndAssign);

    connect(accept, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, [this]() {
        mInterface->SetConfig(0, mOldConfig[0]);
        mInterface->SetConfig(1, mOldConfig[1]);
        reject();
    });

    connect(mPanel[0], &ControllerPanel::AssignButton, [this](auto a, auto b) {AssignButton(0, a, b);});
    connect(mPanel[1], &ControllerPanel::AssignButton, [this](auto a, auto b) {AssignButton(1, a, b);});

//    connect(mPanel[0], &ControllerPanel::TypeChanged, [this](auto t) {
//        auto s = ControllerType::None;
//        if (t == "Standard")
//            s = ControllerType::Standard;
//        else if (t == "Zapper")
//            s = ControllerType::Zapper;
//        mInfo[0].type = s;
//    });
//    connect(mPanel[1], &ControllerPanel::TypeChanged, [this](auto t) {
//        auto s = ControllerType::None;
//        if (t == "Standard")
//            s = ControllerType::Standard;
//        else if (t == "Zapper")
//            s = ControllerType::Zapper;
//        mInfo[0].type = s;
//    });

    poll_id = startTimer(50, Qt::TimerType::PreciseTimer);
}

void ControllerWindow::AssignButton(int controller, int button, QPushButton *pbutton)
{
    if (poll_id != -1) {
        killTimer(poll_id);
        poll_id = -1;
    }

    if (mSelected) {
        mSelected->setChecked(false);
    }

    mSelected = pbutton;
    pbutton->setText("...");

    grabKeyboard();
    grabMouse();

    mAssignController = controller;
    mAssignButton = button;

    mTimer->start(5000);
    mInterface->StartImmediate(this);
}

void ControllerWindow::EndAssign()
{
    mInterface->StopImmediate();
    releaseKeyboard();
    releaseMouse();
    mSelected->setChecked(false);
    mSelected->setText(mInterface->MappingName(mConfig[mAssignController][mAssignButton]));
    qApp->postEvent(mSelected, new QFocusEvent(QEvent::Leave));
    mSelected = nullptr;
    mPanel[mAssignController]->ResetDisplayButtons();
    poll_id = startTimer(50, Qt::TimerType::PreciseTimer);
}

void ControllerWindow::Activated(Input::Config::Mapping m)
{
    mTimer->stop();
    mConfig[mAssignController][mAssignButton] = m;
    mInterface->SetConfig(mAssignController, mConfig[mAssignController]);
    EndAssign();
}


void ControllerWindow::SavePreset(int controller, int preset)
{
    Q_UNUSED(controller)
    Q_UNUSED(preset)
}

void ControllerWindow::LoadPreset(int controller, int preset)
{
    Q_UNUSED(controller)
    Q_UNUSED(preset)
}

void ControllerWindow::timerEvent(QTimerEvent*)
{
    auto a = mInterface->State(0);
    auto b = mInterface->State(1);

    for (int i = 0; i < 8; i++)
    {
        mPanel[0]->SetDisplayButton(i, a[i]);
        mPanel[1]->SetDisplayButton(i, b[i]);
    }
}


