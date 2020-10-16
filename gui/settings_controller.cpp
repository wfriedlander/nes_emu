#include "settings_controller.h"
#include "keyboard.h"

#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QButtonGroup>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QDebug>


ControllerDisplay::ControllerDisplay(QWidget *parent) : QWidget(parent)
{
    up = new QPushButton(this);
    up->setGeometry(96, 108, 56, 42);
    up->setStyleSheet(R"ss(
        QPushButton         {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover   {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:1, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:1, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
    )ss");


    down = new QPushButton(this);
    down->setGeometry(96, 206, 56, 42);
    down->setStyleSheet(R"ss(
        QPushButton         {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover   {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:0, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed {background: qlineargradient(x1:0, y1:0.5, x2:0, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        )ss");

    left = new QPushButton(this);
    left->setGeometry(54, 150, 56, 56);
    left->setStyleSheet(R"ss(
        QPushButton         {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover   {background: qlineargradient(x1:0.5, y1:0, x2:0.8, y2:0, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed {background: qlineargradient(x1:0.5, y1:0, x2:0.8, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        )ss");

    right = new QPushButton(this);
    right->setGeometry(138, 150, 56, 56);
    right->setStyleSheet(R"ss(
        QPushButton         {background: rgb(0, 0, 0); border-radius: 0px; border-style: none;}
        QPushButton:hover   {background: qlineargradient(x1:0.5, y1:0, x2:0.2, y2:0, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(0, 0, 0, 255));}
        QPushButton:pressed {background: qlineargradient(x1:0.5, y1:0, x2:0.2, y2:0, stop:0 rgba(80, 80, 80, 255), stop:1 rgba(0, 0, 0, 255));}
        )ss");

    select = new QPushButton(this);
    select->setGeometry(249, 200, 55, 20);
    select->setStyleSheet(R"ss(
        QPushButton {background: rgb(60, 60, 60); border-radius: 6px; border-style: none;}
        QPushButton:hover {background: rgb(40, 40, 40);}
        QPushButton:pressed {background: rgb(20, 20, 20);}
        )ss");

    start = new QPushButton(this);
    start->setGeometry(344, 200, 55, 20);
    start->setStyleSheet(R"ss(
        QPushButton {background: rgb(60, 60, 60); border-radius: 6px; border-style: none;}
        QPushButton:hover {background: rgb(40, 40, 40);}
        QPushButton:pressed {background: rgb(20, 20, 20);}
        )ss");

    b = new QPushButton(this);
    b->setGeometry(456, 174, 74, 74);
    b->setStyleSheet(R"ss(
        QPushButton {background: rgb(196, 16, 16); border-radius: 37px; border-style: none;}
        QPushButton:hover {background: rgb(176, 16, 16);}
        QPushButton:pressed {background: rgb(150, 8, 8);}
        )ss");

    a = new QPushButton(this);
    a->setGeometry(548, 174, 74, 74);
    a->setStyleSheet(R"ss(
        QPushButton {background: rgb(196, 16, 16); border-radius: 37px; border-style: none;}
        QPushButton:hover {background: rgb(176, 16, 16);}
        QPushButton:pressed {background: rgb(150, 8, 8);}
        )ss");
}

void ControllerDisplay::PressButton(int button)
{
    qDebug() << "PRESS" << button << up;
    switch (button)
    {
    case 0: up->animateClick(100000); break;
    case 1: down->animateClick(100000); break;
    case 2: left->animateClick(100000); break;
    case 3: right->animateClick(100000); break;
    case 4: start->animateClick(100000); break;
    case 5: select->animateClick(100000); break;
    case 6: a->animateClick(100000); break;
    case 7: b->animateClick(100000); break;
    }
}

void ControllerDisplay::ReleaseButton(int button)
{
    qDebug() << "RELEASE" << button;
    switch (button)
    {
    case 0: up->animateClick(0); break;
    case 1: down->animateClick(0); break;
    case 2: left->animateClick(0); break;
    case 3: right->animateClick(0); break;
    case 4: start->animateClick(0); break;
    case 5: select->animateClick(0); break;
    case 6: a->animateClick(0); break;
    case 7: b->animateClick(0); break;
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


static QString ButtonToString(Button button)
{
    switch (button)
    {
    case Button::Up: return "Up";
    case Button::Down: return "Down";
    case Button::Left: return "Left";
    case Button::Right: return "Right";
    case Button::Start: return "Start";
    case Button::Select: return "Select";
    case Button::A: return "A";
    case Button::B: return "B";
    }
}


ControllerPanel::ControllerPanel(const QString& title, QWidget *parent) : QWidget(parent)
{
    auto main_layout = new QHBoxLayout(this);
    auto main_box = new QGroupBox(title, this);
    auto main_box_layout = new QHBoxLayout(main_box);
    auto settings = new QWidget(main_box);

    auto settings_layout = new QVBoxLayout(settings);

    auto type_layout = new QHBoxLayout();
    auto type_label = new QLabel("Type", settings);
    type_layout->addWidget(type_label);
    auto type_combo = new QComboBox(settings);
    type_combo->addItems({"None", "Standard", "Zapper"});
    type_layout->addWidget(type_combo);
    settings_layout->addLayout(type_layout);

    auto preset_layout = new QHBoxLayout();
    auto preset_label = new QLabel("Preset", settings);
    preset_layout->addWidget(preset_label);
    auto preset_combo = new QComboBox(settings);
    preset_combo->addItems({"1", "2", "3", "4", "5"});
    preset_layout->addWidget(preset_combo);
    settings_layout->addLayout(preset_layout);

    auto save_load_layout = new QHBoxLayout();
    auto preset_load = new QPushButton("Load", settings);
    preset_load->setAutoDefault(false);
    save_load_layout->addWidget(preset_load);
    auto preset_save = new QPushButton("Save", settings);
    preset_save->setAutoDefault(false);
    save_load_layout->addWidget(preset_save);
    settings_layout->addLayout(save_load_layout);

//    auto button_group = new QButtonGroup(settings);
    auto button_box = new QGroupBox("Buttons", settings);
    auto button_box_layout = new QGridLayout(button_box);



    for (auto& b : {Button::Up, Button::Down, Button::Left, Button::Right, Button::Start, Button::Select, Button::A, Button::B})
    {
        int i = static_cast<int>(b);
        auto label = new QLabel(ButtonToString(b), button_box);
        button_box_layout->addWidget(label, i, 0, 1, 1);
        auto button = new QPushButton(button_box);
        button->setAutoDefault(false);
        button->setCheckable(true);
        button_box_layout->addWidget(button, i, 1, 1, 1);
//        button_group->addButton(button);
        connect(button, &QPushButton::toggled, [=](bool t) { if (t) emit AssignButton(b, button);});
    }

    settings_layout->addWidget(button_box);
    main_box_layout->addWidget(settings);

    ControllerDisplay* display = new ControllerDisplay(main_box);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    display->setSizePolicy(sizePolicy);
    display->setMinimumSize(QSize(700, 300));
    display->setMaximumSize(QSize(700, 300));
    main_box_layout->addWidget(display);

    main_layout->addWidget(main_box);
}




SettingsController::SettingsController(InputDevices* devices, QWidget *parent) : QDialog(parent), mDevices(devices)
{
    setWindowTitle("Controller Settings");
    resize(1000, 875);

    auto main_layout = new QVBoxLayout(this);
    auto controller1 = new ControllerPanel("Controller 1", this);
    main_layout->addWidget(controller1);
    auto controller2 = new ControllerPanel("Controller 2", this);
    main_layout->addWidget(controller2);
    auto dialog_layout = new QHBoxLayout();
    auto spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    dialog_layout->addItem(spacer);
    auto accept = new QPushButton("Accept", this);
    dialog_layout->addWidget(accept);
    accept->setDefault(true);
    auto cancel = new QPushButton("Cancel", this);
    dialog_layout->addWidget(cancel);
    main_layout->addLayout(dialog_layout);

    connect(accept, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);

    connect(controller1, &ControllerPanel::AssignButton, this, &SettingsController::AssignButton);

//    connect(mDevices, &InputDevices::PushInput, this, &SettingsController::ReceiveInput);
//    auto k = new Keyboard(this);
//    connect(k, &Keyboard::KeyDown, [controller1](KEY key){if (key == KEY::A) controller1->display->PressButton(0);});
//    connect(k, &Keyboard::KeyUp, [controller1](KEY key){if (key == KEY::A) controller1->display->ReleaseButton(0);});
    //    controller1->display->PressButton(0);
}

void SettingsController::ReceiveInput(InputDevice *device, int code)
{
//    qDebug() << "Receive" << device << code << mSelected;


//    ButtonMap bm {device, code};



//    mSelected->setChecked(false);
//    mSelected = nullptr;
}

void SettingsController::AssignButton(Button button, QPushButton *pbutton)
{
    if (mSelected) {
        mSelected->setChecked(false);
    }
    qDebug() << "Assign" << ButtonToString(button) << pbutton;
    mSelected = pbutton;


    auto const connection = new QMetaObject::Connection;
    *connection = connect(mDevices, &InputDevices::PushInput, [=](InputMap im){
        qDebug() << "Receive" << im.device << im.code << (int)button << pbutton;
        pbutton->setChecked(false);
        disconnect(*connection);
        delete connection;
    });




    mDevices->GetImmediateInput();

//    startTimer(5000);
}


