//#include "controllerinput.h"
//#include "ui_controllerinput.h"
//#include <QDebug>
//#include <QKeyEvent>


//#include "interface_input.h"
//#include "mainwindow.h"
//#include "settings_controller.h"


//ControllerInput::ControllerInput(QWidget *parent) : QWidget(parent)//, ui(new Ui::ControllerInput)
//{
//    qDebug() << "test";
////    resize(700+64, 300+64);
////    setFixedSize(764, 364);
//   // mInput.MapButton(66, Input::Button::A);
//    state = '0';
////    auto a = new ControllerPanel(this);
////    ui->setupUi(this);
//}

//void ControllerInput::getInput(Input *input)
//{
//    mInput = input;
//}

//ControllerInput::~ControllerInput()
//{
//   delete ui;
//}

//void ControllerInput::on_A_clicked()
//{
//    state = 'A';
//}

//void ControllerInput::on_B_clicked()
//{
//    state = 'B';
//}

//void ControllerInput::on_Up_clicked()
//{
//    state = 'U';
//}

//void ControllerInput::on_Down_clicked()
//{
//    state = 'D';
//}

//void ControllerInput::on_Left_clicked()
//{
//    state = 'L';
//}

//void ControllerInput::on_Right_clicked()
//{
//    state = 'R';
//}

//void ControllerInput::on_Start_clicked()
//{
//    state = 'S';
//}

//void ControllerInput::on_Select_clicked()
//{
//    state = 'T';
//}

//void ControllerInput::keyPressEvent(QKeyEvent *event)
//{
//    switch(state)
//    {
//    case '0':
//        break;

//    case 'A':
//        mInput->MapButton(event->key(), Input::Button::A);
//        state = '0';
//        break;
//    case 'B':
//        mInput->MapButton(event->key(), Input::Button::B);
//        state = '0';
//        break;
//    case 'U':
//        mInput->MapButton(event->key(), Input::Button::Up);
//        state = '0';
//        break;
//    case 'D':
//        mInput->MapButton(event->key(), Input::Button::Down);
//        state = '0';
//        break;
//    case 'L':
//        mInput->MapButton(event->key(), Input::Button::Left);
//        state = '0';
//        break;
//    case 'R':
//        mInput->MapButton(event->key(), Input::Button::Right);
//        state = '0';
//        break;
//    case 'S':
//        mInput->MapButton(event->key(), Input::Button::Start);
//        state = '0';
//        break;
//    case 'T':
//        mInput->MapButton(event->key(), Input::Button::Select);
//        state = '0';
//        break;

//    }
////    qDebug() << event->text();
////    qDebug() << event->key();
//}


