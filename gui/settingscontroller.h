#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

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
    ControllerDisplay* display;
};


class SettingsController : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsController(QWidget *parent = nullptr);


};

#endif // SETTINGSCONTROLLER_H
