#ifndef AMAROK_H
#define AMAROK_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <string>

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

    bool generatorActive        = false;
    bool lockInAmplifierActive  = false;

    bool listLoad = false;

    bool pause = true;
    bool stop  = true;

    double xMin1 = 0, xMax1 = 0, yMin1 = 0, yMax1 = 0;
    double xMin2 = 0, xMax2 = 0, yMin2 = 0, yMax2 = 0;

    int doubleClick1 = 0, doubleClick2 = 0;
    double xSelectionMin1 = 0, xSelectionMax1 = 0, ySelectionMin1 = 0, ySelectionMax1 = 0;
    double xSelectionMin2 = 0, xSelectionMax2 = 0, ySelectionMin2 = 0, ySelectionMax2 = 0;

    bool autoPlotUpdate1 = true;
    bool autoPlotUpdate2 = true;

    double frequencyFrom = 0;
    double frequencyTo   = 0;
    double frequencyStep = 0;

    int firstInvalidPoints = 3;
    int averageOfPoints    = 1;

    int waitBeforeExperimentStart       = 100;
    int waitAfterNewFrequency           = 150;
    int waitAfterNewSettings            = 1000;
    int waitAfterNewFrequencyKinetic    = 50;
    int minWait                         = 10;
    double waitBetweenRoundsMultiplier  = 1.5;

    double timeStart    = 0;
    double timeNewRound = 0;

    bool calculateDebugTime = true;
    double debugTime1 = 0;
    double debugTime2 = 0;
    double debugTime3 = 0;
    double debugTime4 = 0;
    double debugTime5 = 0;
    double debugTime6 = 0;

    QString absolutePath       = "";
    QString dataPath           = "";
    QString reservePath        = "";
    QString logPath            = "";
    QString kineticPath        = "";
    QString currentKineticPath = "";

    QString reserveFileName = "Experiment_Reserve_File";
    QString userFileName    = "";

    bool logExport        = true;
    FILE* logFile         = nullptr;
    QString logMode       = "All";
    QString logFileName   = "RunLog";

    double addUp   = 0.0001;
    double subDown = 0.0001;

    SimpleExperimentData experimentData;
    KineticThetaData kineticRoundData;
    KineticFrequencyData kineticData;

    std::vector < double > RSDvector;
    std::vector < double > ThetaSDvector;
    std::vector < double > XSDvector;
    std::vector < double > YSDvector;
    std::vector < double > FextSDvector;

    int intervalsAmmount = 0;
    std::vector < double > intervalFrom;
    std::vector < double > intervalTo;
    std::vector < double > intervalStep;

    bool nextKineticRound      = false;
    bool kineticThetaRPlotting = false;

    int kineticRoundNumber = 0;

    double jumpValue = 0;
    double jumpDivider = 0;
    int jumpSign = 0;

    QString currentKineticTask = "";

    QStringList experimentMode;
    QStringList signMode;
    QStringList extremumMode;
    QStringList xAxisNormalMode;
    QStringList yAxisNormalMode;
    QStringList xAxisKineticsModeNormalLeft;
    QStringList yAxisKineticsModeNormalLeft;
    QStringList xAxisKineticsModeNormalRight;
    QStringList yAxisKineticsModeNormalRight;
    QStringList xAxisKineticsModeRTheta;
    QStringList yAxisKineticsModeRTheta;

public:
    explicit Amarok(QWidget *parent = 0);
    ~Amarok();

private slots:
// PLOTTING
    void samplePlot() const;
    void initPlot(const int &i = 0) const;

    void on_plot_1_Moved(QMouseEvent *event) const;
    void on_plot_2_Moved(QMouseEvent *event) const;

    void on_plot_1_Pressed(QMouseEvent *event);
    void on_plot_2_Pressed(QMouseEvent *event);

    void on_plot_1_Released(QMouseEvent *event);
    void on_plot_2_Released(QMouseEvent *event);

    void on_plot_1_DoubleClicked(QMouseEvent *event);
    void on_plot_2_DoubleClicked(QMouseEvent *event);

    void on_checkBoxFixPlot_1_stateChanged(int arg1);
    void on_checkBoxFixPlot_2_stateChanged(int arg1);

    void updateAxisesLabels(const int &i = 0) const;
    void clearPlot(const int &i = 0) const;
    void updatePlot(const int &i = 0) const;
    void replotPlot(const int &i = 0);

    void pushPointOnPlot(const SimpleExperimentPoint &point, const int &plot = 0, const bool &changeRange = true);
    void pushPointOnPlot(const KineticThetaPoint &point);
    void pushPointOnPlot(const KineticFrequencyPoint &point);

    void on_comboBoxXAxisValue_1_currentTextChanged(const QString &arg1);
    void on_comboBoxYAxisValue_1_currentTextChanged(const QString &arg1);
    void on_comboBoxXAxisValue_2_currentTextChanged(const QString &arg1);
    void on_comboBoxYAxisValue_2_currentTextChanged(const QString &arg1);

// ADDITIONAL ACTIONS
    void init();
    void on_actionCOM_Upd_triggered();
    void importConfig();
    void exportConfig() const;
    void on_actionExport_Config_triggered() const;
    void on_actionAllow_Frequency_Step_be_Zero_triggered() const;
    void printLogMsg(const QString& msg, const bool printToUI = true) const;

// LOCK-IN AMPLIFIER
    void lockInAmplifierShow(const bool &show = true) const;
    void on_comboBoxModelLockInAmplifier_currentTextChanged() const;
    void on_comboBoxSerialPortLockInAmplifier_currentTextChanged() const;
    void currentConnectionTextChangedLockInAmplifier() const;
    void on_pushButtonActionLockInAmplifier_clicked();
    void fillButtonsLockInAmplifier();
    void on_comboBoxTimeConstantLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1) const;
    void on_pushButtonAutoPresettingsLockInAmplifier_clicked();
    bool checkRange(const double &R) const;
    void printCurrentReadingsR(const double &R, const double &RSD = 0) const;
    void printCurrentReadingsTheta(const double &Theta, const double &ThetaSD = 0) const;
    void printCurrentReadingsFext(const double &Fext, const double &FextSD = 0) const;
    double getR() const;
    void getR(double &R) const;
    double getTheta() const;
    void getTheta(double &Theta) const;
    double getFext() const;
    void getFext(double &Fext) const;
    void getRTheta(double &R, double &Theta) const;
    void calculateXY(const double R, const double Theta, double &X, double &Y) const;
    void getRThetaXYF(double &R, double &Theta, double &X, double &Y, double &Fext) const;
    void getSDAll(double &R, double &RSD, double &Theta, double &ThetaSD, double &X, double &XSD, double &Y, double &YSD, double &Fext, double &FextSD, const int &average);

// GENERATOR
    void generatorShow(const bool &show = true) const;
    void on_comboBoxModelGenerator_currentTextChanged() const;
    void on_comboBoxSerialPortGenerator_currentTextChanged() const;
    void currentConnectionTextChangedGenerator() const;
    void on_pushButtonActionGenerator_clicked();
    void fillButtonsGenerator(const QString &amplitudeUnit = "VR");
    void on_comboBoxAmplitudeUnitGenerator_currentTextChanged(const QString &arg1);
    void on_doubleSpinBoxAmplitudeGenerator_valueChanged(const double &arg1) const;
    void on_doubleSpinBoxOffsetGenerator_valueChanged(const double &arg1) const;
    void on_doubleSpinBoxFrequencyFromGenerator_valueChanged(const double &arg1);
    void on_doubleSpinBoxFrequencyToGenerator_valueChanged(const double &arg1);
    void on_doubleSpinBoxFrequencyStepGenerator_valueChanged();
    void on_checkBoxMultiIntervalMode_stateChanged(int arg1);
    void on_pushButtonGeneratorAddInterval_clicked();
    void printCurrentReadingsFgen(const double &Fgen) const;
    void setFrequency(const double &frequency) const;

// MAIN ACTIONS
    void on_comboBoxExperimentMode_currentTextChanged(const QString &arg1);

    void on_spinBoxAverageOfPoints_valueChanged();
    void on_spinBoxWait_valueChanged();

    void on_checkBoxKineticsSecondPlotEnableRTheta_stateChanged(int arg1);

    void experimentSettingsChange();

    bool inRange(const double &min, const double &max, const double &value) const;

    void on_pushButtonStart_clicked();
    void Start();
    void on_pushButtonPause_clicked();
    void Pause();
    void on_pushButtonStop_clicked();
    void Stop();

    QString getFileNamePathPrefix(const QString &str = "") const;
    QString getFileNameTimePostfix(const QString &str = "") const;

    void on_pushButtonExport_clicked() const;
    void exportData(const QString &fileName = "") const;
    void exportSettings(const QString &fileName = "") const;

    SimpleExperimentPoint getSimpleExperimentPoint(const double &currentFgen, const int &currentInterval, const int &currentRound);
    void experimentRun(const int &currentInterval, const int &currentRound);
    void prepareIntervalList();
    void simpleExperimentRun();
    void multiIntervalExperimentRun();

    void kineticPrescanRun();
    KineticFrequencyPoint kineticRoundRun();
    void kineticExperimentRun();

    void on_comboBoxDFresDT_currentTextChanged(const QString &arg1);

    void on_doubleSpinBoxJump_valueChanged(double arg1);
    void on_doubleSpinBoxJumpDivider_valueChanged(double arg1);

private:
    Ui::Amarok *ui;
};

#endif // AMAROK_H
