#include "Amarok.h"
#include "ui_Amarok.h"

Amarok::Amarok(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Amarok)
{
    this->constructor = true;

    this->programMode.push_back("Single");
    this->programMode.push_back("Continuous");
    this->programMode.push_back("Kinetics");

    this->rangeMode.push_back("Auto");
    this->rangeMode.push_back("Manual");

    this->signMode.push_back(" > 0");
    this->signMode.push_back(" < 0");

    ui->setupUi(this);

    ui->comboBoxMode->addItems(this->programMode);

    ui->comboBoxRange_1->addItems(this->rangeMode);
    ui->comboBoxRange_2->addItems(this->rangeMode);

    ui->comboBoxDFresDT->addItems(this->signMode);

    this->constructor = false;
}

Amarok::~Amarok()
{
    delete ui;
}
