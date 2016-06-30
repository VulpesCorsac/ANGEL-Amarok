#ifndef AMAROK_H
#define AMAROK_H

#include <QMainWindow>

//#include <QSerialPort>
//#include <QSerialPortInfo>

//#include <QDateTime>
//#include <QTime>
//#include <QTimer>

//#include <QDir>

//#include <stdio.h>
//#include <iostream>
//#include <iomanip>

//#include "qcustomplot.h"

#include "../ANGEL/Angel.h"

namespace Ui {
    class Amarok;
}

class Amarok : public QMainWindow
{
    Q_OBJECT

private:
    bool constructor = false;

    QStringList experimentMode;
    QStringList autoKineticsJumpMode;
    QStringList signMode;

    QStringList replotMode;
    QStringList xAxisNormalMode;
    QStringList yAxisNormalMode;

    QStringList lockInAmplifierModel;
    QStringList generatorModel;
    QStringList lockInAmplifierCom;
    QStringList generatorCom;
    QStringList allCom;

    Generator* generator;
    LockInAmplifier* lockInAmplifier;

    QString absolutePath = "";
    QString dataPath = "";
    QString reservePath = "";
    QString kineticPath = "";
    QString currentKineticPath = "";

    QString reserveFileNameHeader = "Experiment_Reserve_File";
    QString userFileNameHeader    = "";

    double addUp   = 0.0001;
    double subDown = 0.0001;

    int plotUpdateTime = 100;

    bool generatorActive = false;
    bool generatorPoll = false;
    bool continuousMode = false;
    bool lockInAmplifierAutosettings = false;

    int analizingPointsForLockInAmplifierAutosettings = 100;

    int waitBeforeExperiment = 100;
    int waitAfterNewFrequency = 150;

    int generatorSendTimes = 5;
    double eps = 1e-3;

    int invalidPoints = 3;
    int averageOfPoints = 1;

    double from = 0;
    double to   = 0;
    double step = 0;

    SimpleExperimentData simpleExperimentData;
    KineticThetaData kineticThetaData;
    KineticFrequencyData kineticFrequencyData;

public:
    explicit Amarok(QWidget *parent = 0);
    ~Amarok();

private slots:
//  Hiding and Showing
    void hideGraph(const int &i = 0) const;
    void hideAxises(const int &i = 0) const;
    void hideCurrentReadingsAndTiming() const;
    void hideExperimentMainSettings() const;
    void hideExperimentKinetics() const;
    void hideGroupBoxes() const;
    void hideLockInAmplifier() const;
    void hideGenerator() const;
    void hideTest() const;
    void hideGeneratorPolling() const;
    void hideExperimentMode() const;
    void hideAll() const;

    void showGraph(const int &i = 0) const;
    void showAxises(const int &i = 0) const;
    void showCurrentReadingsAndTiming() const;
    void showExperimentMainSettings() const;
    void showExperimentKinetics() const;
    void showGroupBoxes() const;
    void showLockInAmplifier() const;
    void showGenerator() const;
    void showTest() const;
    void showGeneratorPolling() const;
    void showExperimentMode() const;
    void showAll() const;

// Plotting
    void initPlot(const int &i = 0) const;

    void updateAxisesLabels(const int &i = 0) const;

    void on_plot_1_Clicked(QMouseEvent *event) const;
    void on_plot_2_Clicked(QMouseEvent *event) const;

    void updatePlot(const int &i = 0) const;
    void replotPlot(const int &i = 0) const;

    void pushPointOnPlot(const SimpleExperimentPoint &point) const;
    void pushPointOnPlot(const KineticThetaPoint &point) const;
    void pushPointOnPlot(const KineticFrequencyPoint &point) const;

// Lock-in Amplifier
    double getMaxValueInRange(const double &min, const double &max) const;

// Generator
    void changeGeneratorSettings(const QString &str = "") const;
    void setFrequency(const double &new_frequency, bool confirm) const;
    void setFrequency(const double &new_frequency) const;

// Experiment
    void changeCurrentExperimentSettings();
    void changeCurrentExperimentSettings(const QString &str); // Calling changeCurrentSettings();
    void changeCurrentExperimentSettings(const bool &flag);   // Calling changeCurrentSettings();

    void exportSettings(const QString &fileName) const; // TODO

    QString getFileNamePathPrefix(const QString &str = "") const;
    QString getFileNameTimePostfix(const QString &str = "") const;
    QString getFileNameExtension(const QString &str = "", const QString &mode = "SimpleExperiment") const;

    void timerStart(); // TODO
    void timerPause(); // TODO
    void timerStop();  // TODO

    void experimentInit(); // TODO
    void experimentExport(const QString &fileName = ""); // TODO

    void simpleExperimentRun();  // TODO
    void kineticExperimentRun(); // TODO

// Buttons react
    void on_comboBoxExperimentMode_currentTextChanged(const QString &arg1);

    void on_comboBoxReplot_1_currentTextChanged(const QString &arg1);
    void on_comboBoxReplot_2_currentTextChanged(const QString &arg1);

    void on_comboBoxXAxisValue_1_currentTextChanged(const QString &arg1);
    void on_comboBoxYAxisValue_1_currentTextChanged(const QString &arg1);
    void on_comboBoxXAxisValue_2_currentTextChanged(const QString &arg1);
    void on_comboBoxYAxisValue_2_currentTextChanged(const QString &arg1);

    void on_pushButtonRangeManualReplot_1_clicked();
    void on_pushButtonRangeManualReplot_2_clicked();

    void on_comboBoxTimeConstantLockInAmplifier_currentTextChanged(const QString &arg1); // TODO
    void on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1);   // TODO
    void on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1);    // TODO

    void on_pushButtonExport_clicked(); // TODO

    void on_pushButtonStart_clicked(); // TODO
    void on_pushButtonPause_clicked(); // TODO
    void on_pushButtonStop_clicked();  // TODO

    void on_comboBoxModelLockInAmplifier_currentTextChanged(const QString &arg1); // TODO
    void on_comboBoxSerialPortLockInAmplifier_currentTextChanged(const QString &arg1); // TODO
    void on_comboBoxModelGenerator_currentTextChanged(const QString &arg1); // TODO
    void on_comboBoxSerialPortGenerator_currentTextChanged(const QString &arg1); // TODO

private:
    Ui::Amarok *ui;
};

#endif // AMAROK_H
