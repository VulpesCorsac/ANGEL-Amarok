#ifndef AMAROK_H
#define AMAROK_H

#include <QMainWindow>

#include "../ANGEL/Angel.h"

namespace Ui {
    class Amarok;
}

class Amarok : public QMainWindow
{
    Q_OBJECT

private:
    bool constructor = false;
    bool filling = false;

    QStringList programMode;
    QStringList rangeMode;
    QStringList signMode;

    QStringList lockInAmplifierModel;
    QStringList lockInAmplifierCom;
    QStringList GeneratorModel;
    QStringList GeneratorCom;

public:
    explicit Amarok(QWidget *parent = 0);
    ~Amarok();

private:
    Ui::Amarok *ui;
};

#endif // AMAROK_H
