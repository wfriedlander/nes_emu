#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include <QWidget>

namespace Ui {
class ControllerSettings;
}

class ControllerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ControllerSettings(QWidget *parent = nullptr);
    ~ControllerSettings();

private:
    Ui::ControllerSettings *ui;
};

#endif // CONTROLLERSETTINGS_H
