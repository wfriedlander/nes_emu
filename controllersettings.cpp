#include "controllersettings.h"
#include "ui_controllersettings.h"

ControllerSettings::ControllerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControllerSettings)
{
    ui->setupUi(this);
}

ControllerSettings::~ControllerSettings()
{
    delete ui;
}
