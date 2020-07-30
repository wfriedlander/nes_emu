#ifndef CONTROLLERINPUT_H
#define CONTROLLERINPUT_H

#include <QWidget>
#include "input.h"
namespace Ui {
class ControllerInput;
}

class ControllerInput : public QWidget
{
    Q_OBJECT

public:
    explicit ControllerInput(QWidget *parent = nullptr);
    void getInput(Input *input);
    ~ControllerInput();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    char state;


private slots:
    void on_A_clicked();
    void on_B_clicked();
    void on_Up_clicked();
    void on_Down_clicked();
    void on_Left_clicked();
    void on_Right_clicked();
    void on_Start_clicked();
    void on_Select_clicked();

private:
    Ui::ControllerInput *ui;
    Input *mInput;
};

#endif // CONTROLLERINPUT_H
