#ifndef AMAROK_H
#define AMAROK_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

#include "../ANGEL/Angel.h"

namespace Ui {
    class Amarok;
}

class Amarok : public QMainWindow
{
    Q_OBJECT

private:
    Generator *generator;
    LockInAmplifier *lockInAmplifier;

    bool singleFrequencyMode = false;

    bool listLoad = false;

    bool pause = false;
    bool stop  = false;

    double xMin1, xMax1, yMin1, yMax1;
    double xMin2, xMax2, yMin2, yMax2;
    double xMin3, xMax3, yMin3, yMax3;

    QString absolutePath       = "";
    QString dataPath           = "";
    QString reservePath        = "";
    QString kineticPath        = "";
    QString currentKineticPath = "";

    QString reserveFileName = "Experiment_Reserve_File";
    QString userFileName    = "";

    double addUp   = 0.0001;
    double subDown = 0.0001;

public:
    explicit Amarok(QWidget *parent = 0);
    ~Amarok();

private slots:
    void initPlot(int i = 0);

    void on_plot_1_Clicked(QMouseEvent *event) const;
    void on_plot_2_Clicked(QMouseEvent *event) const;
    void on_plot_3_Clicked(QMouseEvent *event) const;

    void on_actionCOM_Upd_triggered();

    void lockInAmplifierShow(bool show = true) const;
    void on_comboBoxModelLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void on_comboBoxSerialPortLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void currentConnectionTextChangedLockInAmplifier() const;
    void on_pushButtonActionLockInAmplifier_clicked();
    void fillButtonsLockInAmplifier();
    void on_comboBoxTimeConstantLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void on_pushButtonAutoPresettingsLockInAmplifier_clicked();

    void generatorShow(bool show = true) const;
    void on_comboBoxModelGenerator_currentTextChanged(const QString &arg1) const;
    void on_comboBoxSerialPortGenerator_currentTextChanged(const QString &arg1) const;
    void currentConnectionTextChangedGenerator() const;
    void on_pushButtonActionGenerator_clicked();
    void fillButtonsGenerator();
    void on_comboBoxAmplitudeUnitGenerator_currentTextChanged(const QString &arg1);
    void on_doubleSpinBoxAmplitudeGenerator_valueChanged(double arg1);
    void on_doubleSpinBoxOffsetGenerator_valueChanged(double arg1);
    void on_doubleSpinBoxFrequencyFromGenerator_valueChanged(double arg1);

    void on_comboBoxExperimentMode_currentTextChanged(const QString &arg1) const;


private:
    Ui::Amarok *ui;
};

#endif // AMAROK_H
