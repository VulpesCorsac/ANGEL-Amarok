#ifndef AMAROK_H
#define AMAROK_H

#include <QMainWindow>

//#include <QSerialPort> // NOT NEEDED
//#include <QSerialPortInfo> // NOT NEEDED

//#include <QDateTime> // NOT NEEDED
//#include <QTime> // NOT NEEDED
#include <QTimer>

//#include <QDir> // NOT NEEDED

//#include <stdio.h> // NOT NEEDED
#include <iostream>
#include <iomanip>

//#include "qcustomplot.h"

#include "../ANGEL/Angel.h"

namespace Ui {
    class Amarok;
}

struct KineticRoundReturn {
    double Time;
    double Fext;
    double ThetaPeak;
};

class Amarok : public QMainWindow
{
    Q_OBJECT

private:

    bool constructor = false;
    bool listLoad = false;

    bool roundRun = false;
    bool kineticRun = false;

    bool pause = true;
    bool stop = true;

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

    QString reserveFileName = "Experiment_Reserve_File";
    QString userFileName    = "";

    double addUp   = 0.0001;
    double subDown = 0.0001;

    int plotUpdateTime = 100;

    bool generatorActive = false;
    bool generatorPoll = false;
    bool continuousMode = false;
    bool lockInAmplifierAutosettings = false;

    int analizingPointsForLockInAmplifierAutosettings = 100;

    const int minWait = 10;
    int waitBeforeExperiment = 100;
    int waitAfterNewFrequency = 150;
    int waitAfterNewFrequencyKinetic = 50;

    int generatorSendTimes = 5;
    double eps = 1e-3;

    int invalidPoints = 3;
    int averageOfPoints = 1;

    double from = 0;
    double to   = 0;
    double step = 0;

    QTimer replotTimer;
    QTime allTime;

    double startTime = 0;
    double newRoundTime = 0;

    const int maxPointForReplot = 3;

    SimpleExperimentData simpleExperimentData;
    KineticThetaData kineticThetaData;
    KineticFrequencyData kineticFrequencyData;

    std::vector < double > RSDvector;
    std::vector < double > ThetaSDvector;
    std::vector < double > FSDvector;

    QString kineticTask = "";
    int kineticRound = 0;

    const int minNormalPointNumber = 5;

    const double thetaMaxDeviation = 3;
    double thetaExtremumDepth = 0;
    double thetaLine = 0;

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
    void setBoundaries(const int &i = 0) const;
    void fillAxisesLabels(const QString &experimentMode = "Normal") const;

    void on_plot_1_Clicked(QMouseEvent *event) const;
    void on_plot_2_Clicked(QMouseEvent *event) const;

    void updatePlot(const int &i) const;
    void updatePlot() const;
    void replotPlot(const int &i);
    void replotPlot();

    void pushPointOnPlot(const SimpleExperimentPoint &point) const;
    void pushPointOnPlot(const KineticThetaPoint &point) const;
    void pushPointOnPlot(const KineticFrequencyPoint &point) const;

// Lock-in Amplifier
    double getMaxValueInRange(const double &min, const double &max) const;
    void fillLockInAmplifierButtons();
    double getR() const;
    void getR(double &R) const;
    double getTheta() const;
    void getTheta(double &Theta) const;
    double getFext() const;
    void getFext(double &Fext) const;
    void getRThetaF(double &R, double &Theta, double &Fext) const;
    void getSDAll(double &R, double &RSD, double &Theta, double &ThetaSD, double &Fext, double &FextSD, const int &average);

// Generator
    void changeGeneratorSettings(const QString &str = "") const;
    void fillGeneratorButtons();
    void setFrequency(const double &new_frequency, bool confirm) const;
    void setFrequency(const double &new_frequency) const;

// Experiment
    bool inRange(const double &min, const double &max, const double &value) const;

    void changeCurrentExperimentSettings();
    void changeCurrentExperimentSettings(const QString &str); // Calling changeCurrentSettings();
    void changeCurrentExperimentSettings(const bool &flag);   // Calling changeCurrentSettings();

    void exportSettings(const QString &fileName = "") const;

    QString getFileNamePathPrefix(const QString &str = "") const;
    QString getFileNameTimePostfix(const QString &str = "") const;
    QString getFileNameExtension(const QString &str = "", const QString &mode = "SimpleExperiment") const;

    void timerStart();
    void timerPause();
    void timerStop();

    void experimentInit() const;
    void experimentExport(const QString &fileName = "") const;

    SimpleExperimentPoint fillPoint(const double &Fgen, const int &average);
    void addNewPoint(const double &Fgen);

    void simpleExperimentRun(const QString &runtimeExportDir = "", const QString &runtimeExportFile = "");

    void kineticPreScanRun();
    KineticRoundReturn kineticRoundRun(const QString &fileName = "");
    void kineticExperimentRun();

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

    void on_comboBoxTimeConstantLockInAmplifier_currentTextChanged(const QString &arg1);
    void on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1);
    void on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1);

    void on_pushButtonExport_clicked();

    void on_pushButtonStart_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();

    void on_comboBoxModelLockInAmplifier_currentTextChanged(const QString &arg1);
    void on_comboBoxSerialPortLockInAmplifier_currentTextChanged(const QString &arg1);
    void on_comboBoxModelGenerator_currentTextChanged(const QString &arg1);
    void on_comboBoxSerialPortGenerator_currentTextChanged(const QString &arg1);

    void on_pushButtonTestEquipment_clicked();

    void on_comboBoxJumpAfterExtremum_currentTextChanged(const QString &arg1);

private:
    Ui::Amarok *ui;
};

#endif // AMAROK_H
