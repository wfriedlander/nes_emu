#ifndef INPUT_H
#define INPUT_H

#include <QWidget>
#include <unordered_map>

#include "interface.h"



class Input : public QWidget, public IInput
{
    Q_OBJECT

public:
    enum class Button { A, B, Start, Select, Up, Down, Left, Right };
    using ButtonMap = std::unordered_map<int, Button>;

public:
    explicit Input(QWidget *parent = nullptr);

public:
    const ButtonState & InputUpdate() override;
    const ButtonMap & MapButton(int key, Button button);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    ButtonMap mMapping = {
        {Qt::Key_F, Button::A},
        {Qt::Key_D, Button::B},
        {Qt::Key_Return, Button::Start},
        {Qt::Key_Shift, Button::Select},
        {Qt::Key_Up, Button::Up},
        {Qt::Key_Down, Button::Down},
        {Qt::Key_Left, Button::Left},
        {Qt::Key_Right, Button::Right}
    };

private:
    ButtonState mState = { };

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;


signals:

};

#endif // INPUT_H
