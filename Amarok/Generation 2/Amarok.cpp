#include "Amarok.h"
#include "ui_Amarok.h"

Amarok::Amarok(QWidget *parent) : QMainWindow(parent), ui(new Ui::Amarok)
{
    ui->setupUi(this);

    init();

/*
    ui->comboBoxSerialPortGenerator->setCurrentText("COM4");
//    on_pushButtonActionGenerator_clicked();

    ui->comboBoxSerialPortLockInAmplifier->setCurrentText("COM3");
//    on_pushButtonActionLockInAmplifier_clicked();

    ui->doubleSpinBoxFrequencyFromGenerator->setValue(225000);
    ui->doubleSpinBoxFrequencyToGenerator->setValue(225200);
    ui->doubleSpinBoxFrequencyStepGenerator->setValue(5);

    ui->comboBoxExperimentMode->setCurrentText("Kinetic");
    ui->doubleSpinBoxJump->setValue(100);
    ui->doubleSpinBoxJumpDivider->setValue(2);
    ui->comboBoxDFresDT->setCurrentText(" > 0");

    this->lockInAmplifier->setReadWaitTimeout(50);

//    on_pushButtonStart_clicked();
//*/
}

Amarok::~Amarok()
{
    exportConfig();

    delete ui;

//    this->lockInAmplifier->~LockInAmplifier();
//    delete this->lockInAmplifier;

//    this->generator->~Generator();
//    delete this->lockInAmplifier;
}

// PLOTTING

void Amarok::samplePlot() const
{
    // ------------------------------------------------ //
    // create graph and assign data to it:
    ui->customPlot_1->addGraph();

    QVector < double > x(101), y2(101), y3(101), y4(101);
    for (int i = 0; i < 101; ++i) {
        x[i] = i/50.0 - 1;
        y2[i] = x[i]*x[i];
        y3[i] = x[i]*x[i]*x[i];
        y4[i] = x[i]*x[i]*x[i]*x[i];
        ui->customPlot_1->graph(0)->addData(x[i], y2[i]);
        ui->customPlot_1->graph(1)->addData(x[i], y4[i]);
        ui->customPlot_2->graph(0)->addData(x[i], y3[i]);
    }
    // give the axes some labels:
    ui->customPlot_1->xAxis->setLabel("x");
    ui->customPlot_1->yAxis->setLabel("y = x ^ 2 && y = x ^ 4 ");
    ui->customPlot_2->xAxis->setLabel("x");
    ui->customPlot_2->yAxis->setLabel("y = x ^ 3");
    // set axes ranges, so we see all data:
    ui->customPlot_1->xAxis->setRange(-1, 1);
    ui->customPlot_1->yAxis->setRange(0, 1);
    ui->customPlot_1->replot();
    ui->customPlot_2->xAxis->setRange(-1, 1);
    ui->customPlot_2->yAxis->setRange(-1, 1);
    ui->customPlot_2->replot();

    // ------------------------------------------------ //
}

void Amarok::initPlot(const int &i) const
{
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);

    if (i == 0 || i == 1) {
        ui->customPlot_1->setNotAntialiasedElements(QCP::aeAll);
        ui->customPlot_1->xAxis->setTickLabelFont(font);
        ui->customPlot_1->yAxis->setTickLabelFont(font);
        ui->customPlot_1->legend->setFont(font);

        ui->customPlot_1->clearGraphs();
        ui->customPlot_1->addGraph();

        ui->customPlot_1->axisRect()->setupFullAxesBox();

        ui->customPlot_1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }
    if (i == 0 || i == 2) {
        ui->customPlot_2->setNotAntialiasedElements(QCP::aeAll);
        ui->customPlot_2->xAxis->setTickLabelFont(font);
        ui->customPlot_2->yAxis->setTickLabelFont(font);
        ui->customPlot_2->legend->setFont(font);

        ui->customPlot_2->clearGraphs();
        ui->customPlot_2->addGraph();

        ui->customPlot_2->axisRect()->setupFullAxesBox();

        ui->customPlot_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }

    return;
}

void Amarok::on_plot_1_Clicked(QMouseEvent *event) const
{
    ui->lineEditPointX_1->setText(QString::number(ui->customPlot_1->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditPointY_1->setText(QString::number(ui->customPlot_1->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Amarok::on_plot_2_Clicked(QMouseEvent *event) const
{
    ui->lineEditPointX_2->setText(QString::number(ui->customPlot_2->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditPointY_2->setText(QString::number(ui->customPlot_2->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Amarok::on_checkBoxFixPlot_1_stateChanged(int arg1)
{
    this->autoPlotUpdate1 = (arg1 == Qt::Unchecked);

    if (arg1 == Qt::Checked)
        printLogMsg("Plot 1 is now fixed",     this->logMode == "All");
    if (arg1 == Qt::Unchecked)
        printLogMsg("Plot 1 is now not fixed", this->logMode == "All");

    return;
}

void Amarok::on_checkBoxFixPlot_2_stateChanged(int arg1)
{
    this->autoPlotUpdate2 = (arg1 == Qt::Unchecked);

    if (arg1 == Qt::Checked)
        printLogMsg("Plot 2 is now fixed",     this->logMode == "All");
    if (arg1 == Qt::Unchecked)
        printLogMsg("Plot 2 is now not fixed", this->logMode == "All");

    return;
}

void Amarok::updateAxisesLabels(const int &i) const
{
    if (i == 1 || i == 0) {
        ui->customPlot_1->xAxis->setLabel(ui->comboBoxXAxisValue_1->currentText());
        ui->customPlot_1->yAxis->setLabel(ui->comboBoxYAxisValue_1->currentText());

        ui->customPlot_1->replot();
    }
    if (i == 2 || i == 0) {
        ui->customPlot_2->xAxis->setLabel(ui->comboBoxXAxisValue_2->currentText());
        ui->customPlot_2->yAxis->setLabel(ui->comboBoxYAxisValue_2->currentText());

        ui->customPlot_2->replot();
    }

    return;
}

void Amarok::clearPlot(const int &i) const
{
    if (i == 1 || i == 0)
        ui->customPlot_1->clearGraphs();
    if (i == 2 || i == 0)
        ui->customPlot_2->clearGraphs();

    updateAxisesLabels(i);

    return;
}

void Amarok::updatePlot(const int &i) const
{
    if (i == 1 || i == 0) {
        if (this->autoPlotUpdate1) {
            ui->customPlot_1->xAxis->setRange(this->xMin1 * (1 - _sgn(this->xMin1) * this->subDown),
                                              this->xMax1 * (1 + _sgn(this->xMax1) * this->addUp));
            ui->customPlot_1->yAxis->setRange(this->yMin1 * (1 - _sgn(this->yMin1) * this->subDown),
                                              this->yMax1 * (1 + _sgn(this->yMax1) * this->addUp));
            ui->customPlot_1->replot();
        }
    }
    if (i == 2 || i == 0) {
        if (this->autoPlotUpdate2) {
            ui->customPlot_2->xAxis->setRange(this->xMin2 * (1 - _sgn(this->xMin2) * this->subDown),
                                              this->xMax2 * (1 + _sgn(this->xMax2) * this->addUp));
            ui->customPlot_2->yAxis->setRange(this->yMin2 * (1 - _sgn(this->yMin2) * this->subDown),
                                              this->yMax2 * (1 + _sgn(this->yMax2) * this->addUp));
            ui->customPlot_2->replot();
        }
    }

    return;
}

void Amarok::replotPlot(const int &i)
{
    if (this->experimentData.isEmpty())
        return;

    if (i == 1 || i == 0)
        ui->customPlot_1->clearGraphs();
    if (i == 2 || i == 0)
        ui->customPlot_2->clearGraphs();

    bool flag = false;
    if (!this->stop || !this->pause)
        flag = true;

    if (flag)
        on_pushButtonPause_clicked();

    for (int point = 0; point < this->experimentData.getSize(); point++) {
        SimpleExperimentPoint pointT = this->experimentData.getPointat(point);
        pushPointOnPlot(pointT, i, false);
    }
    pushPointOnPlot(this->experimentData.getPointat(this->experimentData.getSize() - 1), i);

    updatePlot(i);

    if (flag)
        on_pushButtonPause_clicked();

    return;
}

void Amarok::pushPointOnPlot(const SimpleExperimentPoint &point, const int &plot, const bool &changeRange)
{
    int currentInterval = point.IntervalNumber;
    int currentRound = point.RoundNumber;

    int plotNumber = (currentRound - 1) * this->intervalsAmmount + currentInterval - 1;

    if (plot == 1 || plot == 0) {
        while (ui->customPlot_1->graphCount() < plotNumber + 1)
            ui->customPlot_1->addGraph();

        if (ui->comboBoxXAxisValue_1->currentText() == "FGen") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fgen, point.R);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFgenMin();
                    this->xMax1 = this->experimentData.getFgenMax();

                    this->yMin1 = this->experimentData.getRMin();
                    this->yMax1 = this->experimentData.getRMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fgen, point.Theta);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFgenMin();
                    this->xMax1 = this->experimentData.getFgenMax();

                    this->yMin1 = this->experimentData.getThetaMin();
                    this->yMax1 = this->experimentData.getThetaMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "X") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fgen, point.X);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFgenMin();
                    this->xMax1 = this->experimentData.getFgenMax();

                    this->yMin1 = this->experimentData.getXMin();
                    this->yMax1 = this->experimentData.getXMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Y") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fgen, point.Y);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFgenMin();
                    this->xMax1 = this->experimentData.getFgenMax();

                    this->yMin1 = this->experimentData.getYMin();
                    this->yMax1 = this->experimentData.getYMax();
                }
            }
        }
        if (ui->comboBoxXAxisValue_1->currentText() == "FExt") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fext, point.R);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFextMin();
                    this->xMax1 = this->experimentData.getFextMax();

                    this->yMin1 = this->experimentData.getRMin();
                    this->yMax1 = this->experimentData.getRMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fext, point.Theta);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFextMin();
                    this->xMax1 = this->experimentData.getFextMax();

                    this->yMin1 = this->experimentData.getThetaMin();
                    this->yMax1 = this->experimentData.getThetaMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "X") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fext, point.X);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFextMin();
                    this->xMax1 = this->experimentData.getFextMax();

                    this->yMin1 = this->experimentData.getXMin();
                    this->yMax1 = this->experimentData.getXMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Y") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Fext, point.Y);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getFextMin();
                    this->xMax1 = this->experimentData.getFextMax();

                    this->yMin1 = this->experimentData.getYMin();
                    this->yMax1 = this->experimentData.getYMax();
                }
            }
        }
        if (ui->comboBoxXAxisValue_1->currentText() == "Time") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Time, point.R);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getTimeMin();
                    this->xMax1 = this->experimentData.getTimeMax();

                    this->yMin1 = this->experimentData.getRMin();
                    this->yMax1 = this->experimentData.getRMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Time, point.Theta);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getTimeMin();
                    this->xMax1 = this->experimentData.getTimeMax();

                    this->yMin1 = this->experimentData.getThetaMin();
                    this->yMax1 = this->experimentData.getThetaMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "X") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Time, point.X);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getTimeMin();
                    this->xMax1 = this->experimentData.getTimeMax();

                    this->yMin1 = this->experimentData.getXMin();
                    this->yMax1 = this->experimentData.getXMax();
                }
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Y") {
                ui->customPlot_1->graph(plotNumber)->addData(point.Time, point.Y);

                if (changeRange) {
                    this->xMin1 = this->experimentData.getTimeMin();
                    this->xMax1 = this->experimentData.getTimeMax();

                    this->yMin1 = this->experimentData.getYMin();
                    this->yMax1 = this->experimentData.getYMax();
                }
            }
        }
    }

    if (plot == 2 || plot == 0) {
        while (ui->customPlot_2->graphCount() < plotNumber + 1)
            ui->customPlot_2->addGraph();

        if (ui->comboBoxXAxisValue_2->currentText() == "FGen") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fgen, point.R);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFgenMin();
                    this->xMax2 = this->experimentData.getFgenMax();

                    this->yMin2 = this->experimentData.getRMin();
                    this->yMax2 = this->experimentData.getRMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fgen, point.Theta);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFgenMin();
                    this->xMax2 = this->experimentData.getFgenMax();

                    this->yMin2 = this->experimentData.getThetaMin();
                    this->yMax2 = this->experimentData.getThetaMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "X") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fgen, point.X);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFgenMin();
                    this->xMax2 = this->experimentData.getFgenMax();

                    this->yMin2 = this->experimentData.getXMin();
                    this->yMax2 = this->experimentData.getXMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Y") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fgen, point.Y);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFgenMin();
                    this->xMax2 = this->experimentData.getFgenMax();

                    this->yMin2 = this->experimentData.getYMin();
                    this->yMax2 = this->experimentData.getYMax();
                }
            }
        }
        if (ui->comboBoxXAxisValue_2->currentText() == "FExt") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fext, point.R);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFextMin();
                    this->xMax2 = this->experimentData.getFextMax();

                    this->yMin2 = this->experimentData.getRMin();
                    this->yMax2 = this->experimentData.getRMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fext, point.Theta);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFextMin();
                    this->xMax2 = this->experimentData.getFextMax();

                    this->yMin2 = this->experimentData.getThetaMin();
                    this->yMax2 = this->experimentData.getThetaMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "X") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fext, point.X);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFextMin();
                    this->xMax2 = this->experimentData.getFextMax();

                    this->yMin2 = this->experimentData.getXMin();
                    this->yMax2 = this->experimentData.getXMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Y") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Fext, point.Y);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getFextMin();
                    this->xMax2 = this->experimentData.getFextMax();

                    this->yMin2 = this->experimentData.getYMin();
                    this->yMax2 = this->experimentData.getYMax();
                }
            }
        }
        if (ui->comboBoxXAxisValue_2->currentText() == "Time") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Time, point.R);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getTimeMin();
                    this->xMax2 = this->experimentData.getTimeMax();

                    this->yMin2 = this->experimentData.getRMin();
                    this->yMax2 = this->experimentData.getRMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Time, point.Theta);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getTimeMin();
                    this->xMax2 = this->experimentData.getTimeMax();

                    this->yMin2 = this->experimentData.getThetaMin();
                    this->yMax2 = this->experimentData.getThetaMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "X") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Time, point.X);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getTimeMin();
                    this->xMax2 = this->experimentData.getTimeMax();

                    this->yMin2 = this->experimentData.getXMin();
                    this->yMax2 = this->experimentData.getXMax();
                }
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Y") {
                ui->customPlot_2->graph(plotNumber)->addData(point.Time, point.Y);

                if (changeRange) {
                    this->xMin2 = this->experimentData.getTimeMin();
                    this->xMax2 = this->experimentData.getTimeMax();

                    this->yMin2 = this->experimentData.getYMin();
                    this->yMax2 = this->experimentData.getYMax();
                }
            }
        }
    }

    return;
}

void Amarok::pushPointOnPlot(const KineticThetaPoint &point)
{
    ui->customPlot_1->graph(0)->addData(point.TimeRound, point.Theta);

    this->xMin1 = this->kineticRoundData.getTimeRoundMin();
    this->xMax1 = this->kineticRoundData.getTimeRoundMax();

    this->yMin1 = this->kineticRoundData.getThetaMin();
    this->yMax1 = this->kineticRoundData.getThetaMax();

    if (ui->checkBoxKineticsSecondPlotEnableRTheta->checkState() == Qt::Checked) {
        ui->customPlot_2->graph(0)->addData(point.TimeRound, point.R);

        this->xMin2 = this->kineticRoundData.getTimeRoundMin();
        this->xMax2 = this->kineticRoundData.getTimeRoundMax();

        this->yMin2 = this->kineticRoundData.getRMin();
        this->yMax2 = this->kineticRoundData.getRMax();
    }

    return;
}

void Amarok::pushPointOnPlot(const KineticFrequencyPoint &point)
{
    ui->customPlot_2->graph(0)->addData(point.Time, point.Fext);

    this->xMin2 = this->kineticData.getTimeMin();
    this->xMax2 = this->kineticData.getTimeMax();

    this->yMin2 = this->kineticData.getFextMin();
    this->yMax2 = this->kineticData.getFextMax();

    return;
}

void Amarok::on_comboBoxXAxisValue_1_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    printLogMsg("Plot 1 X axis changed to " + arg1, this->logMode == "All");

    updateAxisesLabels(1);
    replotPlot(1);

    return;
}

void Amarok::on_comboBoxYAxisValue_1_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    printLogMsg("Plot 1 Y axis changed to " + arg1, this->logMode == "All");

    updateAxisesLabels(1);
    replotPlot(1);

    return;
}

void Amarok::on_comboBoxXAxisValue_2_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    printLogMsg("Plot 2 X axis changed to " + arg1, this->logMode == "All");


    updateAxisesLabels(2);
    replotPlot(2);

    return;
}

void Amarok::on_comboBoxYAxisValue_2_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    printLogMsg("Plot 2 Y axis changed to " + arg1, this->logMode == "All");


    updateAxisesLabels(2);
    replotPlot(2);

    return;
}

// ADDITIONAL ACTIONS

void Amarok::init()
{
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    QDir currentFolder = QDir();
    this->absolutePath = currentFolder.absolutePath() + "\\";
    this->dataPath = this->absolutePath + "DataAmarok\\";
    QDir dataFolder = QDir(this->dataPath);
    if (!dataFolder.exists())
        dataFolder.mkpath(".");
    this->reservePath = this->dataPath + "Reserve\\";
    QDir reserveDataFolder = QDir(this->reservePath);
    if (!reserveDataFolder.exists())
        reserveDataFolder.mkpath(".");
    this->kineticPath = this->dataPath + "Kinetic\\";
    QDir kineticsDataFolder = QDir(kineticPath);
    if (!kineticsDataFolder.exists())
        kineticsDataFolder.mkpath(".");
    this->logPath = this->dataPath + "Logs\\";
    QDir logFolder = QDir(this->logPath);
    if (!logFolder.exists())
        logFolder.mkpath(".");

    this->logFileName = this->logPath + this->logFileName + "_" + getFileNameTimePostfix().left(10) + ".log";
    this->logFile = fopen(this->logFileName.toStdString().c_str(), "a+");

    printLogMsg("Program started at " + getFileNameTimePostfix(), true);

    initPlot();

    samplePlot();

    connect(ui->customPlot_1, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plot_1_Clicked(QMouseEvent*)));
    connect(ui->customPlot_2, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(on_plot_2_Clicked(QMouseEvent*)));

    ui->pushButtonActionLockInAmplifier->setText("Connect");
    ui->pushButtonActionGenerator->setText("Connect");

    this->listLoad = true;

    this->experimentMode.clear();
    this->experimentMode.push_back("Single");
    this->experimentMode.push_back("Continuous");
    this->experimentMode.push_back("Kinetic");
    ui->comboBoxExperimentMode->clear();
    ui->comboBoxExperimentMode->addItems(this->experimentMode);
    ui->comboBoxExperimentMode->setCurrentText("Single");

    this->signMode.clear();
    this->signMode.push_back(" > 0");
    this->signMode.push_back(" < 0");
    ui->comboBoxDFresDT->clear();
    ui->comboBoxDFresDT->addItems(this->signMode);
    ui->comboBoxDFresDT->setCurrentText(" < 0");

    this->extremumMode.clear();
    this->extremumMode.push_back("Min");
    this->extremumMode.push_back("Max");
    ui->comboBoxExtremumMode->addItems(this->extremumMode);

    this->xAxisNormalMode.clear();
    this->xAxisNormalMode.push_back("FGen");
    this->xAxisNormalMode.push_back("FExt");
    this->xAxisNormalMode.push_back("Time");
    ui->comboBoxXAxisValue_1->clear();
    ui->comboBoxXAxisValue_2->clear();
    ui->comboBoxXAxisValue_1->addItems(this->xAxisNormalMode);
    ui->comboBoxXAxisValue_2->addItems(this->xAxisNormalMode);
    ui->comboBoxXAxisValue_1->setCurrentText("Time");
    ui->comboBoxXAxisValue_2->setCurrentText("Time");

    this->yAxisNormalMode.clear();
    this->yAxisNormalMode.push_back("R");
    this->yAxisNormalMode.push_back("Theta");
    this->yAxisNormalMode.push_back("X");
    this->yAxisNormalMode.push_back("Y");
    ui->comboBoxYAxisValue_1->clear();
    ui->comboBoxYAxisValue_2->clear();
    ui->comboBoxYAxisValue_1->addItems(this->yAxisNormalMode);
    ui->comboBoxYAxisValue_2->addItems(this->yAxisNormalMode);
    ui->comboBoxYAxisValue_1->setCurrentText("R");
    ui->comboBoxYAxisValue_2->setCurrentText("R");

    this->xAxisKineticsModeRTheta.clear();
    this->xAxisKineticsModeRTheta.push_back("Time");

    this->yAxisKineticsModeRTheta.clear();
    this->yAxisKineticsModeRTheta.push_back("R");
    this->yAxisKineticsModeRTheta.push_back("Theta");

    this->xAxisKineticsModeNormalLeft.clear();
    this->xAxisKineticsModeNormalLeft.push_back("Time");

    this->yAxisKineticsModeNormalLeft.clear();
    this->yAxisKineticsModeNormalLeft.push_back("Theta");

    this->xAxisKineticsModeNormalRight.clear();
    this->xAxisKineticsModeNormalRight.push_back("Time");

    this->yAxisKineticsModeNormalRight.clear();
    this->yAxisKineticsModeNormalRight.push_back("FGen");
    this->yAxisKineticsModeNormalRight.push_back("FExt");

    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);

    ui->lcdNumberCurrentInterval->display(0);
    ui->lcdNumberCurrentRound->display(0);
    ui->progressBarExperiment->setValue(0);

    ui->textEditGeneratorScanningIntervals->clear();

    lockInAmplifierShow(false);
    generatorShow(false);

    on_actionCOM_Upd_triggered();

    this->lockInAmplifier = new LockInAmplifier();
    ui->comboBoxModelLockInAmplifier->addItems(this->lockInAmplifier->getSupportedModels());

    this->generator = new Generator();
    ui->comboBoxModelGenerator->addItems(this->generator->getSupportedModels());

    this->experimentData.setIncludeXY(true);

    this->listLoad = false;

    ui->comboBoxModelLockInAmplifier->setCurrentText("AUTO");
    ui->comboBoxModelGenerator->setCurrentText("AUTO");

    ui->comboBoxExperimentMode->setCurrentText("Kinetic");
    ui->comboBoxExperimentMode->setCurrentText("Single");

    ui->groupBoxPlot_1->hide();
    ui->customPlot_1->hide();
    ui->groupBoxPlot_2->hide();
    ui->customPlot_2->hide();
    ui->groupBoxCurrentReadingsAndTiming->hide();
    ui->groupBoxGeneratorScanningIntervals->hide();
    ui->groupBoxExprimentMainSettings->hide();
    ui->groupBoxResonanceData->hide();

    ui->textEditProgramLog->clear();

    this->experimentData.setIncludeXY(true);
    this->experimentData.setIncludeIntervalNumber(true);
    this->experimentData.setIncludeRoundNumber(true);

    ui->lineEditCurrentReadingsFrequencyGenerator->setText("UNKNOWN");
    ui->lineEditCurrentReadingsExternalFrequencyLockInAmplifier->setText("UNKNOWN");
    ui->lineEditCurrentReadingsRLockInAmplifier->setText("UNKNOWN");
    ui->lineEditCurrentReadingsThetaLockInAmplifier->setText("UNKNOWN");

    ui->lineEditTimePassed->setText("UNKNOWN");
    ui->lineEditTimeToRun->setText("UNKNOWN");

    importConfig();

    printLogMsg("Log mode: " + this->logMode, true);

    std::ios_base::sync_with_stdio(0);

    return;
}

void Amarok::on_actionCOM_Upd_triggered()
{
    QStringList allCom;

    allCom.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        allCom.push_back(info.portName());
    allCom.push_back("SetIt");

    this->listLoad = true;

    ui->comboBoxSerialPortLockInAmplifier->clear();
    ui->comboBoxSerialPortLockInAmplifier->addItems(allCom);
    ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SetIt");

    ui->comboBoxSerialPortGenerator->clear();
    ui->comboBoxSerialPortGenerator->addItems(allCom);
    ui->comboBoxSerialPortGenerator->setCurrentText("SetIt");

    this->listLoad = false;

    printLogMsg("COM list UPD", this->logMode == "All");

    return;
}

void Amarok::importConfig()
{
    if (freopen("ConfigAmarok.conf", "r+", stdin) != nullptr) {
        std::string buffer;
        while (getline(std::cin, buffer)) {
            QString command = QString(buffer.c_str());
            if (command.startsWith("LIA")) {
                if (command.contains("Com")) {
                    ui->comboBoxSerialPortLockInAmplifier->setCurrentText(command.split(" ")[1]);
                    if (ui->comboBoxSerialPortLockInAmplifier->currentText() == command.split(" ")[1])
                        on_pushButtonActionLockInAmplifier_clicked();
                }
                if (command.contains("ReadWait")) {
                    this->lockInAmplifier->setReadWaitTimeout((command.split(" ")[1]).toInt());
                }
            }
            if (command.startsWith("GEN")) {
                if (command.contains("Com")) {
                    ui->comboBoxSerialPortGenerator->setCurrentText(command.split(" ")[1]);
                    if (ui->comboBoxSerialPortGenerator->currentText() == command.split(" ")[1])
                        on_pushButtonActionGenerator_clicked();
                }
                if (command.contains("ReadWait")) {
                    this->generator->setReadWaitTimeout((command.split(" ")[1]).toInt());
                }
            }
            if (command.startsWith("EXP")) {
                if (command.contains("InvalidPoints"))
                    this->firstInvalidPoints           = (command.split(" ")[1]).toInt();
                if (command.contains("WaitBeforeExperimentStart"))
                    this->waitBeforeExperimentStart    = (command.split(" ")[1]).toInt();
                if (command.contains("WaitAfterNewFrequency"))
                    this->waitAfterNewFrequency        = (command.split(" ")[1]).toInt();
                if (command.contains("WaitAfterNewSettings"))
                    this->waitAfterNewSettings         = (command.split(" ")[1]).toInt();
                if (command.contains("WaitAfterNewFrequencyKinetic"))
                    this->waitAfterNewFrequencyKinetic = (command.split(" ")[1]).toInt();
            }
            if (command.startsWith("PLT")) {
                if (command.contains("AddUp"))
                    this->addUp   = (command.split(" ")[1]).toInt();
                if (command.contains("SubDown"))
                    this->subDown = (command.split(" ")[1]).toInt();
            }
            if (command.startsWith("LOG")) {
                if (command.contains("Msg")) {
                    this->logMode = (command.split(" ")[1]);
                    if (!(this->logMode == "All" ||
                          this->logMode == "Critical" ||
                          this->logMode == "Off"))
                            this->logMode = "All";
                }
                if (command.startsWith("Export")) {
                    if (command.split(" ")[1] == "On")
                        this->logExport = true;
                    else
                        this->logExport = false;
                }
            }

        }
        fclose(stdin);
        if (this->logMode == "Off")
            ui->textEditProgramLog->clear();

        printLogMsg("Config loaded", true);
    } else {
        printLogMsg("Config file not found", true);
    }

    return;
}

void Amarok::exportConfig() const
{
    if (freopen("ConfigAmarok.conf", "w+", stdout) != nullptr) {
        if (this->lockInAmplifierActive) {
            std::cout << "LIACom: " << ui->comboBoxSerialPortLockInAmplifier->currentText().toStdString() << std::endl;
            std::cout << "LIAReadWait: " << this->lockInAmplifier->getReadWaitTimeout() << std::endl;
        }
        if (this->generatorActive) {
            std::cout << "GENCom: " << ui->comboBoxSerialPortGenerator->currentText().toStdString() << std::endl;
            std::cout << "GENReadWait: " << this->generator->getReadWaitTimeout() << std::endl;
        }
        std::cout << "EXPInvalidPoints: " << this->firstInvalidPoints << std::endl;
        std::cout << "EXPWaitBeforeExperimentStart: " << this->waitBeforeExperimentStart << std::endl;
        std::cout << "EXPWaitAfterNewFrequency: " << this->waitAfterNewFrequency << std::endl;
        std::cout << "EXPWaitAfterNewSettings: " << this->waitAfterNewSettings << std::endl;
        std::cout << "EXPWaitAfterNewFrequencyKinetic: " << this->waitAfterNewFrequencyKinetic << std::endl;
        std::cout << "PLTAddUp: " << this->addUp << std::endl;
        std::cout << "PLTSubDown: " << this->subDown << std::endl;
        std::cout << "LOGMsg: " << this->logMode.toStdString() << std::endl;
        std::cout << "LOGExport: On" << std::endl;

        fclose(stdout);
        printLogMsg("Config saved", this->logMode == "All");
    } else {
        printLogMsg("Unable to save config", this->logMode == "All");
    }

    return;
}

void Amarok::on_actionExport_Config_triggered() const
{
    exportConfig();

    return;
}

void Amarok::on_actionAllow_Frequency_Step_be_Zero_triggered() const
{
    ui->doubleSpinBoxFrequencyStepGenerator->setMinimum(0);

    printLogMsg("Minimal frequency step is now 0", this->logMode == "All");

    return;
}

void Amarok::printLogMsg(const QString &msg, const bool printToUI) const
{
    QString currentLogMsg = msg.trimmed() + "\n";

    if (currentLogMsg.startsWith("Program started at "))
        currentLogMsg = "\n" + currentLogMsg;

    if (printToUI)
        ui->textEditProgramLog->append(currentLogMsg.trimmed());
    if (this->logExport)
        fprintf(this->logFile, currentLogMsg.toStdString().c_str());

    return;
}

// LOCK-IN AMPLIFIER

void Amarok::lockInAmplifierShow(const bool &show) const
{
    ui->labelTimeConstantLockInAmplifier->setVisible(show);
    ui->comboBoxTimeConstantLockInAmplifier->setVisible(show);
    ui->pushButtonAutoPresettingsLockInAmplifier->setVisible(show);
    ui->labelInputRangeLockInAmplifier->setVisible(show);
    ui->comboBoxInputRangeLockInAmplifier->setVisible(show);
    ui->labelSensivityLockInAmplifier->setVisible(show);
    ui->comboBoxSensivityLockInAmplifier->setVisible(show);

    if (show) {
        ui->checkBoxMultiIntervalMode->setCheckState(Qt::Unchecked);
        ui->checkBoxMultiIntervalMode->setCheckState(Qt::Checked);
        ui->checkBoxMultiIntervalMode->setCheckState(Qt::Unchecked);
    }

    return;
}

void Amarok::on_comboBoxModelLockInAmplifier_currentTextChanged() const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedLockInAmplifier();
}

void Amarok::on_comboBoxSerialPortLockInAmplifier_currentTextChanged() const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedLockInAmplifier();
}

void Amarok::currentConnectionTextChangedLockInAmplifier() const
{
    if (ui->comboBoxModelLockInAmplifier->currentText() == "AUTO") {
        if (ui->comboBoxSerialPortLockInAmplifier->currentText() == "SetIt") {
            ui->pushButtonActionLockInAmplifier->setText("Fill Smth");
            ui->pushButtonActionLockInAmplifier->setEnabled(false);
        } else {
            ui->pushButtonActionLockInAmplifier->setText("Connect");
            ui->pushButtonActionLockInAmplifier->setEnabled(true);
        }
    } else {
        ui->pushButtonActionLockInAmplifier->setEnabled(true);
        if (ui->comboBoxSerialPortLockInAmplifier->currentText() == "SetIt") {
            ui->pushButtonActionLockInAmplifier->setText("Scan");
        } else {
            ui->pushButtonActionLockInAmplifier->setText("Connect");
        }
    }

    return;
}

void Amarok::on_pushButtonActionLockInAmplifier_clicked()
{     if (ui->pushButtonActionLockInAmplifier->text() == "Fill Smth") {
        ui->comboBoxModelLockInAmplifier->setEnabled(true);
        ui->comboBoxSerialPortLockInAmplifier->setEnabled(true);

        printLogMsg("LIA cannot be connected: some info must be given", this->logMode == "All");

        return;
    }
    if (ui->pushButtonActionLockInAmplifier->text() == "Scan") {
        this->listLoad = true;

        ui->comboBoxSerialPortLockInAmplifier->clear();

        QStringList allCom;
        for (int i = 0; i < ui->comboBoxSerialPortLockInAmplifier->count(); i++)
            allCom.push_back(ui->comboBoxSerialPortLockInAmplifier->itemText(i));

        foreach (const QString &com, allCom) {
            if (com != "SetIt") {
                this->lockInAmplifier->autoSetLockInAmplifierModel(com);
                if (this->lockInAmplifier->getLockInAmplifierModel() == ui->comboBoxModelLockInAmplifier->currentText()) {
                    ui->comboBoxSerialPortLockInAmplifier->addItem(com);
                    this->lockInAmplifier->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortLockInAmplifier->addItem("SetIt");
        ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SetIt");

        this->listLoad = false;

        ui->comboBoxModelLockInAmplifier->setEnabled(true);
        ui->comboBoxSerialPortLockInAmplifier->setEnabled(true);

        printLogMsg("LIA porst scanning finished", this->logMode == "All");

        return;
    }
    if (ui->pushButtonActionLockInAmplifier->text() == "Connect") {
        if (this->lockInAmplifier->isActive())
            return;

        if (!this->lockInAmplifier->setConnectionBrute(ui->comboBoxSerialPortLockInAmplifier->currentText()))
            return;

        ui->comboBoxModelLockInAmplifier->setCurrentText(this->lockInAmplifier->getLockInAmplifierModel());

        ui->pushButtonActionLockInAmplifier->setText("Disconnect");

        ui->comboBoxModelLockInAmplifier->setEnabled(false);
        ui->comboBoxSerialPortLockInAmplifier->setEnabled(false);
        lockInAmplifierShow(true);

        fillButtonsLockInAmplifier();

        printLogMsg("LIA connected", this->logMode == "All");

        this->lockInAmplifierActive = true;

        ui->groupBoxPlot_1->show();
        ui->customPlot_1->show();
        ui->groupBoxPlot_2->show();
        ui->customPlot_2->show();
        ui->groupBoxCurrentReadingsAndTiming->show();
        ui->groupBoxExprimentMainSettings->show();
        ui->groupBoxResonanceData->show();

        return;
    }
    if (ui->pushButtonActionLockInAmplifier->text() == "Disconnect") {
        if (!this->lockInAmplifier->isActive())
            return;

        this->lockInAmplifier->disconnect();

        ui->comboBoxModelLockInAmplifier->setCurrentText("AUTO");

        ui->pushButtonActionLockInAmplifier->setText("Connect");

        ui->comboBoxModelLockInAmplifier->setEnabled(true);
        ui->comboBoxSerialPortLockInAmplifier->setEnabled(true);

        lockInAmplifierShow(false);

        ui->comboBoxTimeConstantLockInAmplifier->clear();
        ui->comboBoxInputRangeLockInAmplifier->clear();
        ui->comboBoxSensivityLockInAmplifier->clear();

        printLogMsg("LIA disconnected", this->logMode == "All");

        this->lockInAmplifierActive = false;

        if (!this->generatorActive) {
            ui->groupBoxPlot_1->hide();
            ui->customPlot_1->hide();
            ui->groupBoxPlot_2->hide();
            ui->customPlot_2->hide();
            ui->groupBoxCurrentReadingsAndTiming->hide();
            ui->groupBoxExprimentMainSettings->hide();
        }
        ui->groupBoxResonanceData->hide();

        return;
    }

    return;
}

void Amarok::fillButtonsLockInAmplifier()
{
    this->listLoad = true;

    ui->comboBoxTimeConstantLockInAmplifier->clear();
    ui->labelTimeConstantLockInAmplifier->setVisible(this->lockInAmplifier->workWithTimeConstant());
    ui->comboBoxTimeConstantLockInAmplifier->setVisible(this->lockInAmplifier->workWithTimeConstant());
    if (this->lockInAmplifier->workWithTimeConstant()) {
        ui->comboBoxTimeConstantLockInAmplifier->addItems(this->lockInAmplifier->getTimeConstantList());
        ui->comboBoxTimeConstantLockInAmplifier->setCurrentText(this->lockInAmplifier->getTimeConstant());
    }

    ui->comboBoxInputRangeLockInAmplifier->clear();
    ui->labelInputRangeLockInAmplifier->setVisible(this->lockInAmplifier->workWithVoltageInputRange());
    ui->comboBoxInputRangeLockInAmplifier->setVisible(this->lockInAmplifier->workWithVoltageInputRange());
    if (this->lockInAmplifier->workWithVoltageInputRange()) {
        ui->comboBoxInputRangeLockInAmplifier->addItems(this->lockInAmplifier->getVoltageInputRangeList());
        ui->comboBoxInputRangeLockInAmplifier->setCurrentText(this->lockInAmplifier->getVoltageInputRange());
    }

    ui->comboBoxSensivityLockInAmplifier->clear();
    ui->labelSensivityLockInAmplifier->setVisible(this->lockInAmplifier->workWithSensivity());
    ui->comboBoxSensivityLockInAmplifier->setVisible(this->lockInAmplifier->workWithSensivity());
    if (this->lockInAmplifier->workWithSensivity()) {
        ui->comboBoxSensivityLockInAmplifier->addItems(this->lockInAmplifier->getSensivityList());
        ui->comboBoxSensivityLockInAmplifier->setCurrentText(this->lockInAmplifier->getSensivity());
    }

    this->listLoad = false;

    QTest::qWait(this->waitAfterNewSettings);
    double R, Theta, Fext;
    getRTheta(R, Theta);
    getFext(Fext);

    return;
}

void Amarok::on_comboBoxTimeConstantLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (!this->listLoad) {
        this->lockInAmplifier->setTimeConstant(arg1);

        QTest::qWait(this->waitAfterNewSettings);
        double R, Theta, Fext;
        getRTheta(R, Theta);
        getFext(Fext);
    }

    if (!arg1.isEmpty())
        printLogMsg("LIA time constant changed to " + arg1, this->logMode == "All");

    return;
}

void Amarok::on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (!this->listLoad) {
        this->lockInAmplifier->setVoltageInputRange(arg1);

        QTest::qWait(this->waitAfterNewSettings);
        double R, Theta, Fext;
        getRTheta(R, Theta);
        getFext(Fext);
    }

    if (!arg1.isEmpty())
        printLogMsg("LIA input range changed to " + arg1, this->logMode == "All");

    return;
}

void Amarok::on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (!this->listLoad) {
        this->lockInAmplifier->setSensivity(arg1);

        QTest::qWait(this->waitAfterNewSettings);
        double R, Theta, Fext;
        getRTheta(R, Theta);
        getFext(Fext);
    }

    if (!arg1.isEmpty())
        printLogMsg("LIA sensivity changed to " + arg1, this->logMode == "All");

    return;
}

void Amarok::on_pushButtonAutoPresettingsLockInAmplifier_clicked()
{
    ui->groupBoxLockInAmplifier->setTitle("Lock-In Amplifier: Autosettings...");

    this->lockInAmplifier->autoAll();

    if (this->lockInAmplifier->getLockInAmplifierModel() != "SR865")
        QTest::qWait(10*1000);

    this->listLoad = true;

    if (this->lockInAmplifier->workWithVoltageInputRange())
        ui->comboBoxInputRangeLockInAmplifier->setCurrentText(this->lockInAmplifier->getVoltageInputRange());
    if (this->lockInAmplifier->workWithSensivity())
        ui->comboBoxSensivityLockInAmplifier->setCurrentText(this->lockInAmplifier->getSensivity());

    this->listLoad = false;

    if (this->lockInAmplifier->workWithVoltageInputRange()) {
        double currentR = this->lockInAmplifier->getR();

        ui->comboBoxInputRangeLockInAmplifier->setCurrentText("1V");
        if (currentR < 0.3)
            ui->comboBoxInputRangeLockInAmplifier->setCurrentText("300M");
        if (currentR < 0.1)
            ui->comboBoxInputRangeLockInAmplifier->setCurrentText("100M");
        if (currentR < 0.03)
            ui->comboBoxInputRangeLockInAmplifier->setCurrentText("30M");
        if (currentR < 0.01)
            ui->comboBoxInputRangeLockInAmplifier->setCurrentText("10M");
    }

    ui->groupBoxLockInAmplifier->setTitle("Lock-In Amplifier");

    printLogMsg("LIA autosettings activated", this->logMode == "All");

    return;
}

bool Amarok::checkRange(const double &R) const
{
    bool ans = true;

    if (this->lockInAmplifier->workWithVoltageInputRange()) {
        QString inputRange = ui->comboBoxInputRangeLockInAmplifier->currentText();

        if (inputRange ==   "1V" && R > 1)
            return false;
        if (inputRange == "300M" && R > 0.3)
            return false;
        if (inputRange == "100M" && R > 0.1)
            return false;
        if (inputRange ==  "30M" && R > 0.03)
            return false;
        if (inputRange ==  "10M" && R > 0.01)
            return false;
    }
    if (this->lockInAmplifier->workWithSensivity()) {
        QString sensivity = ui->comboBoxSensivityLockInAmplifier->currentText();

        if (sensivity ==    "1 V/uA" && R > 1)
            return false;
        if (sensivity == "500 mV/nA" && R > 0.5)
            return false;
        if (sensivity == "300 mV/nA" && R > 0.3)
            return false;
        if (sensivity == "200 mV/nA" && R > 0.2)
            return false;
        if (sensivity == "100 mV/nA" && R > 0.1)
            return false;
        if (sensivity ==  "50 mV/nA" && R > 0.05)
            return false;
        if (sensivity ==  "30 mV/nA" && R > 0.03)
            return false;
        if (sensivity ==  "20 mV/nA" && R > 0.02)
            return false;
        if (sensivity ==  "10 mV/nA" && R > 0.01)
            return false;
        if (sensivity ==   "5 mV/nA" && R > 0.005)
            return false;
        if (sensivity ==   "3 mV/nA" && R > 0.003)
            return false;
        if (sensivity ==   "2 mV/nA" && R > 0.002)
            return false;
        if (sensivity ==   "1 mV/nA" && R > 0.001)
            return false;
        if (sensivity == "500 uV/pA" && R > 0.0005)
            return false;
        if (sensivity == "300 uV/pA" && R > 0.0003)
            return false;
        if (sensivity == "200 uV/pA" && R > 0.0002)
            return false;
        if (sensivity == "100 uV/pA" && R > 0.0001)
            return false;
        if (sensivity ==  "50 uV/pA" && R > 0.00005)
            return false;
        if (sensivity ==  "30 uV/pA" && R > 0.00003)
            return false;
        if (sensivity ==  "20 uV/pA" && R > 0.00002)
            return false;
        if (sensivity ==  "10 uV/pA" && R > 0.00001)
            return false;
        if (sensivity ==   "5 uV/pA" && R > 0.000005)
            return false;
        if (sensivity ==   "3 uV/pA" && R > 0.000003)
            return false;
        if (sensivity ==   "2 uV/pA" && R > 0.000002)
            return false;
        if (sensivity ==   "1 uV/pA" && R > 0.000001)
            return false;
        if (sensivity == "500 nV/fA" && R > 0.0000005)
            return false;
        if (sensivity == "300 nV/fA" && R > 0.0000003)
            return false;
        if (sensivity == "200 nV/fA" && R > 0.0000002)
            return false;
        if (sensivity == "100 nV/fA" && R > 0.0000001)
            return false;
        if (sensivity ==  "50 nV/fA" && R > 0.00000005)
            return false;
        if (sensivity ==  "30 nV/fA" && R > 0.00000003)
            return false;
        if (sensivity ==  "20 nV/fA" && R > 0.00000002)
            return false;
        if (sensivity ==  "10 nV/fA" && R > 0.00000001)
            return false;
        if (sensivity ==   "5 nV/fA" && R > 0.000000005)
            return false;
        if (sensivity ==   "3 nV/fA" && R > 0.000000003)
            return false;
        if (sensivity ==   "2 nV/fA" && R > 0.000000002)
            return false;
        if (sensivity ==   "1 nV/fA" && R > 0.000000001)
            return false;
    }

    return ans;
}

void Amarok::printCurrentReadingsR(const double &R, const double &RSD) const
{
    ui->lineEditCurrentReadingsRLockInAmplifier->setText(QString::number(R, 'g', 6) + " +- " +
                                                         QString::number(100*RSD/R, 'g', 2) + "%");
    if (!checkRange(R)) {
        ui->lineEditCurrentReadingsRLockInAmplifier->setStyleSheet("QLineEdit {background-color: red;}");

        printLogMsg("Input overloaded!", this->logMode != "Off");
    } else {
        ui->lineEditCurrentReadingsRLockInAmplifier->setStyleSheet("QLineEdit {background: rgb(240, 240, 240);}");
    }

    return;
}

void Amarok::printCurrentReadingsTheta(const double &Theta, const double &ThetaSD) const
{
    ui->lineEditCurrentReadingsThetaLockInAmplifier->setText(QString::number(Theta, 'g', 6) + " +- " +
                                                             QString::number(100*ThetaSD/Theta, 'g', 2) + "%");
    return;
}

void Amarok::printCurrentReadingsFext(const double &Fext, const double &FextSD) const
{
    ui->lineEditCurrentReadingsExternalFrequencyLockInAmplifier->setText(QString::number(Fext, 'g', 6) + " +- " +
                                                                         QString::number(100*FextSD/Fext, 'g', 2) + "%");
    return;
}

double Amarok::getR() const
{
    try {
        double R = this->lockInAmplifier->getR();

        printCurrentReadingsR(R);

        return R;
    } catch (...) {
        printLogMsg("Error in getting amplitude from LIA!", this->logMode != "Off");
    }

    return 0;
}

void Amarok::getR(double &R) const
{
    R = getR();

    return;
}

double Amarok::getTheta() const
{
    try {
        double theta = this->lockInAmplifier->getTheta();

        printCurrentReadingsTheta(theta);

        return theta;

    } catch (...) {
        printLogMsg("Error in getting phase from LIA!", this->logMode != "Off");
    }

    return 0;
}

void Amarok::getTheta(double &Theta) const
{
    Theta = getTheta();

    return;
}

double Amarok::getFext() const
{
    try {
        double Fext = this->lockInAmplifier->getExternalFrequency();

        printCurrentReadingsFext(Fext);

        return Fext;
    } catch (...) {
        printLogMsg("Error in getting external frequency from LIA!", this->logMode != "Off");
    }

    return 0;
}

void Amarok::getFext(double &Fext) const
{
    Fext = getFext();

    return;
}

void Amarok::getRTheta(double &R, double &Theta) const
{
    R = Theta = 0;
    try {
        this->lockInAmplifier->getRTheta(R, Theta);

        printCurrentReadingsR(R);
        printCurrentReadingsTheta(Theta);
    } catch (...) {
        printLogMsg("Error in getting R and Theta from LIA!", this->logMode != "Off");
    }

    return;
}

void Amarok::calculateXY(const double &R, const double &Theta, double &X, double &Y) const
{
    X = R * cos(Theta);
    Y = R * sin(Theta);

    return;
}

void Amarok::getRThetaXYF(double &R, double &Theta, double &X, double &Y, double &Fext) const
{
    R = Theta = X = Y = Fext = 0;

    try {
        this->lockInAmplifier->getRThetaFext(R, Theta, Fext);

        printCurrentReadingsR(R);
        printCurrentReadingsTheta(Theta);
        printCurrentReadingsFext(Fext);
    } catch (...) {
        printLogMsg("Error in getting all data from LIA!", this->logMode != "Off");
    }

    calculateXY(R, Theta, X, Y);

    return;
}

void Amarok::getSDAll(double &R, double &RSD, double &Theta, double &ThetaSD, double &X, double &XSD, double &Y, double &YSD, double &Fext, double &FextSD, const int &average)
{
    double temp_R = 0;
    double temp_Theta = 0;
    double temp_X = 0;
    double temp_Y = 0;
    double temp_Fext = 0;

    R = RSD = 0;
    Theta = ThetaSD = 0;
    X = XSD = 0;
    Y = YSD = 0;
    Fext = FextSD = 0;

    if ((int) this->RSDvector.size() != average)
        this->RSDvector.resize(average);
    if ((int) this->ThetaSDvector.size() != average)
        this->ThetaSDvector.resize(average);
    if ((int) this->XSDvector.size() != average)
        this->XSDvector.resize(average);
    if ((int) this->YSDvector.size() != average)
        this->YSDvector.resize(average);
    if ((int) this->FextSDvector.size() != average)
        this->FextSDvector.resize(average);

    for (int point = 0; point < average; point++) {
        getRThetaXYF(temp_R, temp_Theta, temp_X, temp_Y, temp_Fext);

        RSDvector[point] = temp_R;
        ThetaSDvector[point] = temp_Theta;
        XSDvector[point] = temp_X;
        YSDvector[point] = temp_Y;
        FextSDvector[point] = temp_Fext;

        R += temp_R;
        Theta += temp_Theta;
        X += temp_X;
        Y += temp_Y;
        Fext += temp_Fext;
    }

    R /= average;
    Theta /= average;
    X /= average;
    Y /= average;
    Fext /= average;

    for (int point = 0; point < average; point++) {
        RSD += _sqr(RSDvector[point] - R);
        ThetaSD += _sqr(ThetaSDvector[point] - Theta);
        XSD += _sqr(XSDvector[point] - X);
        YSD += _sqr(YSDvector[point] - Y);
        FextSD += _sqr(FextSDvector[point] - Fext);
    }

    RSD = _sqrt(RSD/average);
    ThetaSD = _sqrt(ThetaSD/average);
    XSD = _sqrt(XSD/average);
    YSD = _sqrt(YSD/average);
    FextSD = _sqrt(FextSD/average);

    return;
}

// GENERATOR

void Amarok::generatorShow(const bool &show) const
{
    ui->labelAmplitudeGenerator->setVisible(show);
    ui->comboBoxAmplitudeUnitGenerator->setVisible(show);
    ui->doubleSpinBoxAmplitudeGenerator->setVisible(show);
    ui->labelOffsetGenerator->setVisible(show);
    ui->doubleSpinBoxOffsetGenerator->setVisible(show);
    ui->labelFrequencyFromGenerator->setVisible(show);
    ui->doubleSpinBoxFrequencyFromGenerator->setVisible(show);
    ui->labelFrequencyToGenerator->setVisible(show);
    ui->doubleSpinBoxFrequencyToGenerator->setVisible(show);
    ui->labelFrequencyStepGenerator->setVisible(show);
    ui->doubleSpinBoxFrequencyStepGenerator->setVisible(show);

    ui->groupBoxGeneratorScanningIntervals->setVisible(show);

    return;
}

void Amarok::on_comboBoxModelGenerator_currentTextChanged() const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedGenerator();
}

void Amarok::on_comboBoxSerialPortGenerator_currentTextChanged() const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedGenerator();
}

void Amarok::currentConnectionTextChangedGenerator() const
{
    if (ui->comboBoxModelGenerator->currentText() == "AUTO") {
        if (ui->comboBoxSerialPortGenerator->currentText() == "SetIt") {
            ui->pushButtonActionGenerator->setText("Fill Smth");
            ui->pushButtonActionGenerator->setEnabled(false);
        } else {
            ui->pushButtonActionGenerator->setText("Connect");
            ui->pushButtonActionGenerator->setEnabled(true);
        }
    } else {
        ui->pushButtonActionGenerator->setEnabled(true);
        if (ui->comboBoxSerialPortGenerator->currentText() == "SetIt") {
            ui->pushButtonActionGenerator->setText("Scan");
        } else {
            ui->pushButtonActionGenerator->setText("Connect");
        }
    }

    return;
}

void Amarok::on_pushButtonActionGenerator_clicked()
{
    if (ui->pushButtonActionGenerator->text() == "Fill Smth") {
        ui->comboBoxModelGenerator->setEnabled(true);
        ui->comboBoxSerialPortGenerator->setEnabled(true);

        printLogMsg("GEN cannot be connected: some info must be given", this->logMode == "All");

        return;
    }
    if (ui->pushButtonActionGenerator->text() == "Scan") {
        this->listLoad = true;

        ui->comboBoxSerialPortGenerator->clear();

        QStringList allCom;
        for (int i = 0; i < ui->comboBoxSerialPortGenerator->count(); i++)
            allCom.push_back(ui->comboBoxSerialPortGenerator->itemText(i));

        foreach (const QString &com, allCom) {
            if (com != "SetIt") {
                this->generator->autoSetGeneratorModel(com);
                if (this->generator->getGeneratorModel() == ui->comboBoxModelGenerator->currentText()) {
                    ui->comboBoxSerialPortGenerator->addItem(com);
                    this->generator->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortGenerator->addItem("SetIt");
        ui->comboBoxSerialPortGenerator->setCurrentText("SetIt");

        this->listLoad = false;

        ui->comboBoxModelGenerator->setEnabled(true);
        ui->comboBoxSerialPortGenerator->setEnabled(true);

        printLogMsg("GEN porst scanning finished", this->logMode == "All");

        return;
    }
    if (ui->pushButtonActionGenerator->text() == "Connect") {
        if (this->generator->isActive())
            return;

        if (!this->generator->setConnectionBrute(ui->comboBoxSerialPortGenerator->currentText()))
            return;

        ui->comboBoxModelGenerator->setCurrentText(this->generator->getGeneratorModel());

        ui->pushButtonActionGenerator->setText("Disconnect");

        ui->comboBoxModelGenerator->setEnabled(false);
        ui->comboBoxSerialPortGenerator->setEnabled(false);

        generatorShow(true);

        fillButtonsGenerator();

        printLogMsg("GEN connected", this->logMode == "All");

        ui->checkBoxMultiIntervalMode->setChecked(true);
        ui->checkBoxMultiIntervalMode->setChecked(false);

        this->generatorActive = true;

        ui->groupBoxPlot_1->show();
        ui->customPlot_1->show();
        ui->groupBoxPlot_2->show();
        ui->customPlot_2->show();
        ui->groupBoxCurrentReadingsAndTiming->show();
        ui->groupBoxGeneratorScanningIntervals->show();
        ui->groupBoxExprimentMainSettings->show();

        return;
    }
    if (ui->pushButtonActionGenerator->text() == "Disconnect") {
        if (!this->generator->isActive())
            return;

        this->generator->disconnect();

        ui->comboBoxModelGenerator->setCurrentText("AUTO");

        ui->pushButtonActionGenerator->setText("Connect");

        ui->comboBoxModelGenerator->setEnabled(true);
        ui->comboBoxSerialPortGenerator->setEnabled(true);

        generatorShow(false);

        this->listLoad = true;

        ui->comboBoxAmplitudeUnitGenerator->clear();

        this->listLoad = false;

        printLogMsg("GEN disconnected", this->logMode == "All");

        this->generatorActive = false;

        if (!this->lockInAmplifierActive) {
            ui->groupBoxPlot_1->hide();
            ui->customPlot_1->hide();
            ui->groupBoxPlot_2->hide();
            ui->customPlot_2->hide();
            ui->groupBoxCurrentReadingsAndTiming->hide();
            ui->groupBoxExprimentMainSettings->hide();
        }
        ui->groupBoxGeneratorScanningIntervals->hide();

        return;
    }

    return;
}

void Amarok::fillButtonsGenerator(const QString &amplitudeUnit)
{
    this->listLoad = true;

    ui->labelAmplitudeGenerator->setVisible(this->generator->workWithAmplitude());
    ui->comboBoxAmplitudeUnitGenerator->clear();
    ui->comboBoxAmplitudeUnitGenerator->setVisible(this->generator->workWithAmplitude());
    ui->doubleSpinBoxAmplitudeGenerator->setVisible(this->generator->workWithAmplitude());
    if (this->generator->workWithAmplitude()) {
        ui->comboBoxAmplitudeUnitGenerator->addItems(this->generator->getAmplitudeTypeList());
        ui->comboBoxAmplitudeUnitGenerator->setCurrentText(amplitudeUnit);

        ui->doubleSpinBoxAmplitudeGenerator->setMinimum(this->generator->getMinAmplitudeCurrent(amplitudeUnit));
        ui->doubleSpinBoxAmplitudeGenerator->setMaximum(this->generator->getMaxAmplitudeCurrent(amplitudeUnit));
        ui->doubleSpinBoxAmplitudeGenerator->setSingleStep(this->generator->getStepAmplitudeCurrent(amplitudeUnit));
        ui->doubleSpinBoxAmplitudeGenerator->setDecimals(this->generator->getDecimalsAmplitudeCurrent(amplitudeUnit));
        ui->doubleSpinBoxAmplitudeGenerator->setValue(this->generator->getAmplitude(amplitudeUnit));
    }

    ui->labelOffsetGenerator->setVisible(this->generator->workWithOffset());
    ui->doubleSpinBoxOffsetGenerator->setVisible(this->generator->workWithOffset());
    if (this->generator->workWithOffset()) {
        ui->doubleSpinBoxOffsetGenerator->setMinimum(this->generator->getMinOffset());
        ui->doubleSpinBoxOffsetGenerator->setMaximum(this->generator->getMaxOffset());
        ui->doubleSpinBoxOffsetGenerator->setSingleStep(this->generator->getStepOffset());
        ui->doubleSpinBoxOffsetGenerator->setDecimals(this->generator->getDecimalsOffset());
        ui->doubleSpinBoxOffsetGenerator->setValue(this->generator->getOffset());
    }

    ui->labelFrequencyFromGenerator->setVisible(this->generator->workWithFrequency());
    ui->doubleSpinBoxFrequencyFromGenerator->setVisible(this->generator->workWithFrequency());
    ui->labelFrequencyToGenerator->setVisible(this->generator->workWithFrequency());
    ui->doubleSpinBoxFrequencyToGenerator->setVisible(this->generator->workWithFrequency());
    ui->labelFrequencyStepGenerator->setVisible(this->generator->workWithFrequency());
    ui->doubleSpinBoxFrequencyStepGenerator->setVisible(this->generator->workWithFrequency());
    if (this->generator->workWithFrequency()) {
        ui->doubleSpinBoxFrequencyFromGenerator->setMinimum(this->generator->getMinFrequencyCurrent());
        ui->doubleSpinBoxFrequencyFromGenerator->setMaximum(this->generator->getMaxFrequencyCurrent());
        ui->doubleSpinBoxFrequencyFromGenerator->setSingleStep(this->generator->getStepFrequencyCurrent());
        ui->doubleSpinBoxFrequencyFromGenerator->setDecimals(this->generator->getDecimalsFrequencyCurrent());
        ui->doubleSpinBoxFrequencyFromGenerator->setValue(this->generator->getFrequency());

        printCurrentReadingsFgen(this->generator->getFrequency());

        ui->doubleSpinBoxFrequencyToGenerator->setMinimum(this->generator->getMinFrequencyCurrent());
        ui->doubleSpinBoxFrequencyToGenerator->setMaximum(this->generator->getMaxFrequencyCurrent());
        ui->doubleSpinBoxFrequencyToGenerator->setSingleStep(this->generator->getStepFrequencyCurrent());
        ui->doubleSpinBoxFrequencyToGenerator->setDecimals(this->generator->getDecimalsFrequencyCurrent());
        ui->doubleSpinBoxFrequencyToGenerator->setValue(this->generator->getFrequency());


        ui->doubleSpinBoxFrequencyStepGenerator->setMinimum(this->generator->getMinFrequencyCurrent());
        ui->doubleSpinBoxFrequencyStepGenerator->setMaximum(this->generator->getMaxFrequencyCurrent());
        ui->doubleSpinBoxFrequencyStepGenerator->setSingleStep(this->generator->getStepFrequencyCurrent());
        ui->doubleSpinBoxFrequencyStepGenerator->setDecimals(this->generator->getDecimalsFrequencyCurrent());
        ui->doubleSpinBoxFrequencyStepGenerator->setValue(1);
    }

    printLogMsg("GEN amplitude units changed to " + amplitudeUnit, this->logMode == "All");

    this->listLoad = false;

    return;
}

void Amarok::on_comboBoxAmplitudeUnitGenerator_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    fillButtonsGenerator(arg1);

    return;
}

void Amarok::on_doubleSpinBoxAmplitudeGenerator_valueChanged(const double &arg1) const
{
    if (this->listLoad)
        return;

    this->generator->setAmplitude(arg1, ui->comboBoxAmplitudeUnitGenerator->currentText());

    QTest::qWait(this->waitAfterNewFrequency);
    double R, Theta, Fext;
    getRTheta(R, Theta);
    getFext(Fext);

    printLogMsg("GEN amplitude changed to " + QString::number(arg1) + " " + ui->comboBoxAmplitudeUnitGenerator->currentText(), this->logMode == "All");

    return;
}

void Amarok::on_doubleSpinBoxOffsetGenerator_valueChanged(const double &arg1) const
{
    if (this->listLoad)
        return;

    this->generator->setOffset(arg1);

    QTest::qWait(this->waitAfterNewSettings);
    double R, Theta, Fext;
    getRTheta(R, Theta);
    getFext(Fext);

    printLogMsg("GEN offset changed to " + QString::number(arg1), this->logMode == "All");

    return;
}

void Amarok::on_doubleSpinBoxFrequencyFromGenerator_valueChanged(const double &arg1)
{
    if (this->listLoad)
        return;

    if (this->stop) {
        this->generator->setFrequency(arg1);

        QTest::qWait(this->waitAfterNewSettings);
        double R, Theta, Fext;
        getRTheta(R, Theta);
        getFext(Fext);
    }
    else
        experimentSettingsChange();

    return;
}

void Amarok::on_doubleSpinBoxFrequencyToGenerator_valueChanged(const double &arg1)
{
    if (this->listLoad)
        return;

    if (this->stop) {
        this->generator->setFrequency(arg1);

        QTest::qWait(this->waitAfterNewSettings);
        double R, Theta, Fext;
        getRTheta(R, Theta);
        getFext(Fext);
    }
    else
        experimentSettingsChange();

    return;
}

void Amarok::on_doubleSpinBoxFrequencyStepGenerator_valueChanged()
{
    if (this->listLoad)
        return;

    if (!this->stop)
        experimentSettingsChange();

    return;
}

void Amarok::on_checkBoxMultiIntervalMode_stateChanged(int arg1)
{
    QString mode;
    mode = (arg1 == Qt::Checked ? "ON" : "OFF");
    printLogMsg("MultiInterval mode for generator changed to " + mode, this->logMode == "All");

    ui->pushButtonGeneratorAddInterval->setVisible(arg1 == Qt::Checked);
    ui->lcdNumberCurrentInterval->setVisible(arg1 == Qt::Checked);
    ui->textEditGeneratorScanningIntervals->setVisible(arg1 == Qt::Checked);

    if (arg1 == Qt::Unchecked)
        ui->textEditGeneratorScanningIntervals->clear();

    return;
}

void Amarok::on_pushButtonGeneratorAddInterval_clicked()
{
    ui->textEditGeneratorScanningIntervals->append(
                QString::number(ui->doubleSpinBoxFrequencyFromGenerator->value(), 'g', 10) + ":" +
                QString::number(ui->doubleSpinBoxFrequencyToGenerator->value(),   'g', 10) + ":" +
                QString::number(ui->doubleSpinBoxFrequencyStepGenerator->value(), 'g', 10));

    printLogMsg("Scanning interval for generator added", this->logMode == "All");

    return;
}

void Amarok::printCurrentReadingsFgen(const double &Fgen) const
{
    ui->lineEditCurrentReadingsFrequencyGenerator->setText(QString::number(Fgen, 'g', 6));

    return;
}

void Amarok::setFrequency(const double &frequency) const
{
    if (this->generatorActive) {
        try {
            this->generator->setFrequency(frequency);

            printCurrentReadingsFgen(frequency);
        } catch (...) {
            printLogMsg("Error in setting frequency on Generator!", this->logMode == "All");
        }
    }

    return;
}

// MAIN ACTIONS

void Amarok::on_comboBoxExperimentMode_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    printLogMsg("Experiment mode changed to " + arg1, this->logMode == "All");

    if (arg1 == "Kinetic") {
        if (!this->stop) {
            this->listLoad = true;
            ui->comboBoxExperimentMode->setCurrentText("Single");
            this->listLoad = false;
            return;
        }

        ui->spinBoxWait->setValue(this->waitAfterNewFrequencyKinetic);

        ui->pushButtonStart->setText("PreScan");
        ui->pushButtonPause->setText("Pause");
        ui->pushButtonStop->setText("Stop");

        ui->pushButtonExport->setDisabled(true);

        ui->lcdNumberCurrentRound->show();
    } else {
        this->listLoad = true;

        ui->comboBoxXAxisValue_1->clear();
        ui->comboBoxXAxisValue_1->addItems(this->xAxisNormalMode);
        ui->comboBoxXAxisValue_1->setCurrentText("Time");
        ui->comboBoxYAxisValue_1->clear();
        ui->comboBoxYAxisValue_1->addItems(this->yAxisNormalMode);
        ui->comboBoxYAxisValue_1->setCurrentText("R");
        ui->comboBoxXAxisValue_2->clear();
        ui->comboBoxXAxisValue_2->addItems(this->xAxisNormalMode);
        ui->comboBoxXAxisValue_2->setCurrentText("Time");
        ui->comboBoxYAxisValue_2->clear();
        ui->comboBoxYAxisValue_2->addItems(this->yAxisNormalMode);
        ui->comboBoxYAxisValue_2->setCurrentText("Theta");

        this->listLoad = false;

        ui->spinBoxWait->setValue(this->waitAfterNewFrequency);

        ui->pushButtonStart->setText("Start");
        ui->pushButtonPause->setText("Pause");
        ui->pushButtonStop->setText("Stop");

        ui->pushButtonExport->setEnabled(true);

        if (arg1 == "Single")
            ui->lcdNumberCurrentRound->hide();
        if (arg1 == "Continuous")
            ui->lcdNumberCurrentRound->show();

        ui->groupBoxAxis_1->show();
        ui->groupBoxAxis_2->show();

        this->currentKineticTask.clear();
    }

    ui->doubleSpinBoxResonanceFrequency->setDisabled(true);
    ui->groupBoxExperimentKinetics->hide();
    ui->checkBoxKineticsSecondPlotEnableRTheta->hide();

    return;
}

void Amarok::on_spinBoxAverageOfPoints_valueChanged()
{
    if (this->listLoad)
        return;

    experimentSettingsChange();

    return;
}

void Amarok::on_spinBoxWait_valueChanged()
{
    if (this->listLoad)
        return;

    experimentSettingsChange();

    return;
}

void Amarok::on_checkBoxKineticsSecondPlotEnableRTheta_stateChanged(int arg1)
{
    this->listLoad = true;

    if (arg1 == Qt::Checked) {
        ui->comboBoxXAxisValue_1->clear();
        ui->comboBoxXAxisValue_1->addItems(this->xAxisKineticsModeRTheta);
        ui->comboBoxXAxisValue_1->setCurrentText("Time");
        ui->comboBoxYAxisValue_1->clear();
        ui->comboBoxYAxisValue_1->addItems(this->yAxisKineticsModeRTheta);
        ui->comboBoxYAxisValue_1->setCurrentText("Theta");
        ui->comboBoxXAxisValue_2->clear();
        ui->comboBoxXAxisValue_2->addItems(this->xAxisKineticsModeRTheta);
        ui->comboBoxXAxisValue_2->setCurrentText("Time");
        ui->comboBoxYAxisValue_2->clear();
        ui->comboBoxYAxisValue_2->addItems(this->yAxisKineticsModeRTheta);
        ui->comboBoxYAxisValue_2->setCurrentText("R");

        updateAxisesLabels();
    } else {
        ui->comboBoxXAxisValue_1->clear();
        ui->comboBoxXAxisValue_1->addItems(this->xAxisKineticsModeNormalLeft);
        ui->comboBoxXAxisValue_1->setCurrentText("Time");
        ui->comboBoxYAxisValue_1->clear();
        ui->comboBoxYAxisValue_1->addItems(this->yAxisKineticsModeNormalLeft);
        ui->comboBoxYAxisValue_1->setCurrentText("Theta");
        ui->comboBoxXAxisValue_2->clear();
        ui->comboBoxXAxisValue_2->addItems(this->xAxisKineticsModeNormalRight);
        ui->comboBoxXAxisValue_2->setCurrentText("Time");
        ui->comboBoxYAxisValue_2->clear();
        ui->comboBoxYAxisValue_2->addItems(this->yAxisKineticsModeNormalRight);
        ui->comboBoxYAxisValue_2->setCurrentText("FGen");

        updateAxisesLabels();
    }

    this->listLoad = false;
}

void Amarok::experimentSettingsChange()
{
    printLogMsg("Experiment settings changed", this->logMode == "All");

    if (this->generatorActive) {
        this->frequencyFrom = ui->doubleSpinBoxFrequencyFromGenerator->value();
        this->frequencyTo = ui->doubleSpinBoxFrequencyToGenerator->value();
        this->frequencyStep = ui->doubleSpinBoxFrequencyStepGenerator->value();
        if (this->frequencyFrom > this->frequencyTo)
            this->frequencyStep *= -1;
    } else {
        this->frequencyFrom = 0;
        this->frequencyTo = 0;
        this->frequencyStep = 0;
    }

    if (this->generatorActive) {
        ui->progressBarExperiment->setMinimum(_min(this->frequencyFrom, this->frequencyTo));
        ui->progressBarExperiment->setMaximum(_max(this->frequencyFrom, this->frequencyTo));
    } else {
        ui->progressBarExperiment->setMinimum(0);
        ui->progressBarExperiment->setMaximum(0);
    }

    this->averageOfPoints = ui->spinBoxAverageOfPoints->value();

    if (ui->comboBoxExperimentMode->currentText() == "Kinetic")
        this->waitAfterNewFrequencyKinetic = ui->spinBoxWait->value();
    else
        this->waitAfterNewFrequency = ui->spinBoxWait->value();

    return;
}

bool Amarok::inRange(const double &min, const double &max, const double &value) const
{
    return ((_min(min, max) <= (value + EPS)) && ((value - EPS) <= _max(min, max)));
}

void Amarok::on_pushButtonStart_clicked()
{
    if (!ui->pushButtonStart->isEnabled())
        return;

    if (ui->pushButtonStart->text() != "Next")
        printLogMsg("Experiment starting", this->logMode == "All");

    this->setAttribute(Qt::WA_DeleteOnClose, true);

    if (ui->comboBoxExperimentMode->currentText() == "Single" ||
        ui->comboBoxExperimentMode->currentText() == "Continuous") {
        experimentSettingsChange();

        ui->doubleSpinBoxFrequencyFromGenerator->setEnabled(!ui->checkBoxMultiIntervalMode->isChecked());
        ui->doubleSpinBoxFrequencyToGenerator->setEnabled(!ui->checkBoxMultiIntervalMode->isChecked());
        ui->doubleSpinBoxFrequencyStepGenerator->setEnabled(!ui->checkBoxMultiIntervalMode->isChecked());

        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonPause->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);

        ui->lineEditTimeToRun->setText("UNKNOWN");

        Start();

        this->timeStart = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

        this->experimentData.clear();
        clearPlot();

        QString Time = getFileNameTimePostfix();
        exportSettings(this->reservePath + "Settings_" + this->reserveFileName + "_" + Time + ".set");
        QString File = this->reservePath + this->reserveFileName + "_" + Time + ".dat";

        if (freopen(File.toStdString().c_str(), "w+", stderr) == nullptr) {
            printLogMsg("Unable to open file for exporting!", this->logMode == "All");

            return;
        }

        fprintf(stderr, "Fext\tFextSD\tFgen\tR\tRSD\tTheta\tThetaSD\tX\tXSD\tY\tYSD\tTime\tInterval\tRound\n");

        if (ui->checkBoxMultiIntervalMode->checkState() == Qt::Checked)
            multiIntervalExperimentRun();
        if (ui->checkBoxMultiIntervalMode->checkState() == Qt::Unchecked)
            simpleExperimentRun();

        on_pushButtonStop_clicked();

        return;
    }

    if (ui->comboBoxExperimentMode->currentText() == "Kinetic") {
        ui->lineEditTimeToRun->setText("Inavalible in this mode");

        if (this->currentKineticTask.isEmpty()) {
            this->currentKineticTask = "PreScan";

            ui->pushButtonStart->setEnabled(false);
            ui->pushButtonPause->setEnabled(true);
            ui->pushButtonStop->setEnabled(true);

            QString timeString = getFileNameTimePostfix();
            this->currentKineticPath = this->kineticPath + ui->lineEditFileHeader->text() + "_" + timeString + "\\";

            QDir kineticDataFolder = QDir(this->currentKineticPath);
            if (!kineticDataFolder.exists())
                kineticDataFolder.mkpath(".");

            ui->comboBoxXAxisValue_1->setCurrentText("FGen");
            ui->comboBoxYAxisValue_1->setCurrentText("R");
            ui->comboBoxXAxisValue_2->setCurrentText("FGen");
            ui->comboBoxYAxisValue_2->setCurrentText("Theta");

            Start();

            this->timeStart = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

            this->experimentData.clear();
            clearPlot();

            kineticPrescanRun();

            Stop();

            exportData(this->currentKineticPath + "Prescan.dat");
            exportSettings(this->currentKineticPath + "Settings_Prescan.set");

            this->currentKineticTask = "Kinetic";

            ui->doubleSpinBoxResonanceFrequency->setEnabled(true);

            ui->groupBoxExperimentKinetics->show();
            ui->checkBoxKineticsSecondPlotEnableRTheta->show();

            ui->checkBoxKineticsSecondPlotEnableRTheta->setChecked(true);
            ui->checkBoxKineticsSecondPlotEnableRTheta->setChecked(false);

            ui->pushButtonStart->setText("Start Kin");
            ui->pushButtonStop->setText("Halt");

            ui->pushButtonStart->setEnabled(true);
            ui->pushButtonPause->setEnabled(false);
            ui->pushButtonStop->setEnabled(false);

            return;
        }

        if (this->currentKineticTask == "Kinetic") {
            ui->pushButtonStop->setEnabled(true);

            Start();

            if (ui->pushButtonStart->text() == "Start Kin") {
                ui->pushButtonStart->setText("Next");

                kineticExperimentRun();

                return;
            }

            if (ui->pushButtonStart->text() == "Next")
                this->nextKineticRound = true;

            return;
        }
    }

    return;
}

void Amarok::Start()
{
    this->pause = false;
    this->stop = false;

    return;
}

void Amarok::on_pushButtonPause_clicked()
{
    if (   ui->comboBoxExperimentMode->currentText() == "Single"
        || ui->comboBoxExperimentMode->currentText() == "Continuous"
        || (   ui->comboBoxExperimentMode->currentText() == "Kinetic"
            && this->currentKineticTask == "PreScan")) {

        if (!ui->pushButtonPause->isEnabled())
            return;

        if (ui->pushButtonPause->text() == "Pause") {
            ui->pushButtonPause->setText("Continue");

            printLogMsg("Experiment paused", this->logMode == "All");

        } else {
            ui->pushButtonPause->setText("Pause");

            printLogMsg("Experiment continued", this->logMode == "All");
        }

        Pause();

        return;
    }

    return;
}

void Amarok::Pause()
{
    this->pause = !this->pause;
    this->stop = false;

    return;
}

void Amarok::on_pushButtonStop_clicked()
{
    if (!ui->pushButtonStop->isEnabled())
        return;

    this->stop = true;

    printLogMsg("Experiment stopped", this->logMode == "All");

    this->setAttribute(Qt::WA_DeleteOnClose, false);

    fclose(stderr);

    ui->doubleSpinBoxFrequencyFromGenerator->setEnabled(true);
    ui->doubleSpinBoxFrequencyToGenerator->setEnabled(true);
    ui->doubleSpinBoxFrequencyStepGenerator->setEnabled(true);

    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);

    if (ui->comboBoxExperimentMode->currentText() == "Kinetic") {
        if (this->currentKineticTask == "PreScan") {
            ui->pushButtonStart->setText("PreScan");
            ui->pushButtonPause->setText("Pause");
            ui->pushButtonStop->setText("Stop");
        }

        this->nextKineticRound = true;
    }

    Stop();

    return;
}

void Amarok::Stop()
{
    this->pause = true;
    this->stop = true;

    return;
}

QString Amarok::getFileNamePathPrefix(const QString &str) const
{
    return this->dataPath + str;
}

QString Amarok::getFileNameTimePostfix(const QString &str) const
{
    QString ans = str;
    if (!ans.isEmpty())
        ans += "_";

    return ans + QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
}

void Amarok::on_pushButtonExport_clicked() const
{
    QString timeString = getFileNameTimePostfix();

    exportSettings(this->dataPath + "Settings_" + ui->lineEditFileHeader->text() + "_" + timeString + ".set");
    exportData(this->dataPath + ui->lineEditFileHeader->text() + "_" + timeString + ".dat");

    return;
}

void Amarok::exportData(const QString &fileName) const
{
    QString file = fileName.trimmed();

    if (file.isEmpty())
        file = getFileNameTimePostfix(this->reservePath + this->reserveFileName) + ".dat";

    fclose(stdout);
    if (freopen(file.toStdString().c_str(), "w+", stdout) == nullptr) {
        printLogMsg("Unable to open file " + file + " for exporting!", this->logMode != "Off");

		return;
	}

    printf("Fext\tFextSD\tFgen\tR\tRSD\tTheta\tThetaSD\tX\tXSD\tY\tYSD\tTime\tInterval\tRound\n");
    for (int i = 0; i < this->experimentData.getSize(); i++) {
        printf("%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%d\t%d\n",
               this->experimentData.getFextat(i),
               this->experimentData.getFextSDat(i),
               this->experimentData.getFgenat(i),
               this->experimentData.getRat(i),
               this->experimentData.getRSDat(i),
               this->experimentData.getThetaat(i),
               this->experimentData.getThetaSDat(i),
               this->experimentData.getXat(i),
               this->experimentData.getXSDat(i),
               this->experimentData.getYat(i),
               this->experimentData.getYSDat(i),
               this->experimentData.getTimeat(i),
               this->experimentData.getIntervalNumberat(i),
               this->experimentData.getRoundNumberat(i)
              );
    }

    fclose(stdout);

    printLogMsg("Experiment data exported to: " + file, this->logMode != "Off");

    return;
}

void Amarok::exportSettings(const QString &fileName) const
{
    QString file = fileName.trimmed();

    if (file.isEmpty())
        file = getFileNameTimePostfix(this->reservePath + "Settings_" + this->reserveFileName) + ".set";

    fclose(stdout);
    if (freopen(file.toStdString().c_str(), "w+", stdout) == nullptr) {
        printLogMsg("Unable to open file " + file + " for exporting!", this->logMode != "Off");

        return;
	}
	
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Current experiment settings:" << std::endl;
    std::cout << std::endl;

    if (!this->generatorActive) {
        std::cout << "Generator - not active" << std::endl;
    } else {
        std::cout << "Generator model: " << ui->comboBoxModelGenerator->currentText().toStdString() << std::endl;
        std::cout << "   Offset: " << ui->doubleSpinBoxOffsetGenerator->value() << " V" << std::endl;
        std::cout << "   Amplitude: " << ui->doubleSpinBoxAmplitudeGenerator->value() << " Vrms" << std::endl;
        if (ui->checkBoxMultiIntervalMode->checkState() == Qt::Unchecked) {
            std::cout << "   Intervals: " << 1 << std::endl;
            std::cout << "      Interval: " << 1 << std::endl;
            std::cout << "         Frequency from: " << ui->doubleSpinBoxFrequencyFromGenerator->value() << " Hz" << std::endl;
            std::cout << "         Frequency to:   " << ui->doubleSpinBoxFrequencyToGenerator->value()   << " Hz" << std::endl;
            std::cout << "         Frequency step: " << ui->doubleSpinBoxFrequencyStepGenerator->value() << " Hz" << std::endl;
        } else {
            std::cout << "   Intervals: " << this->intervalsAmmount << std::endl;
            for (int i = 0; i < this->intervalsAmmount; i++) {
                std::cout << "      Interval: " << i << std::endl;
                std::cout << "         Frequency from: " << this->intervalFrom[i] << " Hz" << std::endl;
                std::cout << "         Frequency to:   " << this->intervalTo[i]   << " Hz" << std::endl;
                std::cout << "         Frequency step: " << this->intervalStep[i] << " Hz" << std::endl;
            }
        }
        if (this->generator->workWithOutputImpedance()) {
            std::cout << "   Output impedance: " << this->generator->getOutputImpedance().toStdString() << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "Lock-in amplifier model: " << ui->comboBoxModelLockInAmplifier->currentText().toStdString() << std::endl;
    std::cout << "   Time constant: " << ui->comboBoxTimeConstantLockInAmplifier->currentText().toStdString() << std::endl;
    if (this->lockInAmplifier->workWithVoltageInputRange())
        std::cout << "   Input range:   " << ui->comboBoxInputRangeLockInAmplifier->currentText().toStdString() <<  std::endl;
    std::cout << "   Sensivity:     " << ui->comboBoxSensivityLockInAmplifier->currentText().toStdString() <<  std::endl;

    std::cout << std::endl;
    std::cout << "Experiment:" << std::endl;
    std::cout << "   Mode: " << ui->comboBoxExperimentMode->currentText().toStdString() << std::endl;
    if (   ui->comboBoxExperimentMode->currentText() == "Single"
        || ui->comboBoxExperimentMode->currentText() == "Continuous") {
        std::cout << "      Average of " << ui->spinBoxAverageOfPoints->value() << " points" << std::endl;
        std::cout << "      For new point wait for " << ui->spinBoxWait->value() << " ms" << std::endl;
    }
    if (ui->comboBoxExperimentMode->currentText() == "Continuous")
        std::cout << "       Last round was " << ui->lcdNumberCurrentRound->value() << std::endl;
    if (ui->comboBoxExperimentMode->currentText() == "Kinetic") {
        std::cout << "      Fres: " << ui->doubleSpinBoxResonanceFrequency->value() << " Hz" << std::endl;
        std::cout << "      dFres/dT " << ui->comboBoxDFresDT->currentText().toStdString() << std::endl;
        std::cout << "      Jump: " << ui->doubleSpinBoxJump->value() << " Hz" << std::endl;
        std::cout << "      Jump divider: " << ui->doubleSpinBoxJumpDivider->value() << std::endl;
    }

    fclose(stdout);

    printLogMsg("Experiment settings exported to: " + file, this->logMode != "Off");

    return;
}

SimpleExperimentPoint Amarok::getSimpleExperimentPoint(const double &currentFgen, const int &currentInterval, const int &currentRound)
{
    SimpleExperimentPoint ans;

    ans.Time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - this->timeStart;
    ans.IntervalNumber = currentInterval;
    ans.RoundNumber = currentRound;
    ans.Fgen = currentFgen;

    ans.Fext = ans.FextSD = 0;
    ans.R = ans.RSD = 0;
    ans.Theta = ans.ThetaSD = 0;
    ans.X = ans.XSD = 0;
    ans.Y = ans.YSD = 0;

    getSDAll(ans.R, ans.RSD, ans.Theta, ans.ThetaSD, ans.X, ans.XSD, ans.Y, ans.YSD, ans.Fext, ans.FextSD, this->averageOfPoints);

    return ans;
}

void Amarok::experimentRun(const int &currentInterval, const int &currentRound)
{
    if (this->calculateDebugTime)
        debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    int plotNumber = (currentRound - 1) * this->intervalsAmmount + currentInterval - 1;

    while (ui->customPlot_1->graphCount() < plotNumber + 1)
        ui->customPlot_1->addGraph();

    while (ui->customPlot_2->graphCount() < plotNumber + 1)
        ui->customPlot_2->addGraph();

    ui->lcdNumberCurrentInterval->display(currentInterval);
    ui->lcdNumberCurrentRound->display(currentRound);

    if (this->generatorActive) {
        ui->progressBarExperiment->setMinimum(_min(this->frequencyFrom, this->frequencyTo));
        ui->progressBarExperiment->setMaximum(_max(this->frequencyFrom, this->frequencyTo));
    } else {
        ui->progressBarExperiment->setMinimum(0);
        ui->progressBarExperiment->setMaximum(0);
    }

    QTest::qWait(this->waitBeforeExperimentStart);
    for (int point = 0; point < this->firstInvalidPoints; point++) {
        getR();
        QTest::qWait(this->waitAfterNewFrequency);
    }

    double Fres = -1;
    double minThetaData = 1000;
    double maxThetaData = -1000;
    double previousTheta = -1000;
    double maxDTheta = 0;
    double SNR = 1E9;

    double FGen = this->frequencyFrom;

    if (this->calculateDebugTime) {
        debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        printLogMsg("Preparing settings for experiment : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");
    }

    while (inRange(this->frequencyFrom, this->frequencyTo, FGen)) {
        if (this->calculateDebugTime)
            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

        if (this->generatorActive) {
            setFrequency(FGen);
            QTest::qWait(this->waitAfterNewFrequency);
        } else {
            QTest::qWait(this->minWait);
        }

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            if (debugTime2 - debugTime1 > 1)
                printLogMsg("Waiting before new point : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");

            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        }

        SimpleExperimentPoint point = getSimpleExperimentPoint(FGen, currentInterval, currentRound);

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            if (debugTime2 - debugTime1 > 1)
                printLogMsg("Getting new point : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");

            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        }

        this->experimentData.push_back(point);
        pushPointOnPlot(point);
        updatePlot();

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            if (debugTime2 - debugTime1 > 1)
                printLogMsg("Inserting new point : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");

            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        }


        fprintf(stderr,"%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%d\t%d\n",
               point.Fext,
               point.FextSD,
               point.Fgen,
               point.R,
               point.RSD,
               point.Theta,
               point.ThetaSD,
               point.X,
               point.XSD,
               point.Y,
               point.YSD,
               point.Time,
               point.IntervalNumber,
               point.RoundNumber
        );

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            if (debugTime2 - debugTime1 > 1)
                printLogMsg("Printing new point : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");

            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        }

        if (Fres < 0 || point.Theta < minThetaData) {
            Fres = point.Fext;
            ui->doubleSpinBoxResonanceFrequency->setValue(Fres);
            minThetaData = point.Theta;
        }

        if (Fres < 0 || point.Theta > maxThetaData) {
            maxThetaData = point.Theta;
        }
        ui->labelResonanceDepthValue->setText(QString::number(maxThetaData - minThetaData));

        if (previousTheta > -200) {
            maxDTheta = _max(maxDTheta, _abs(point.Theta-previousTheta));

            if (point.ThetaSD > 0) {
                SNR = _min(SNR, _abs(point.Theta/point.ThetaSD));
            } else {
                SNR = _min(SNR, _abs(point.Theta/maxDTheta));
            }

            ui->labelSNRValue->setText(QString::number(SNR));
        }
        previousTheta = point.Theta;

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            if (debugTime2 - debugTime1 > 1)
                printLogMsg("Analysing new point : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");

            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        }

//        ui->lineEditCurrentReadingsFrequencyGenerator->setText(QString::number(FGen));
//        ui->lineEditCurrentReadingsExternalFrequencyLockInAmplifier->setText(QString::number(point.Fext, 'g', 6) + " +- " +
//                                                                             QString::number(100*point.FextSD/point.Fext, 'g', 2) + "%");
//        ui->lineEditCurrentReadingsRLockInAmplifier->setText(QString::number(point.R, 'g', 6) + " +- " +
//                                                             QString::number(100*point.RSD/point.R, 'g', 2) + "%");
//        ui->lineEditCurrentReadingsThetaLockInAmplifier->setText(QString::number(point.Theta, 'g', 6) + " +- " +
//                                                                 QString::number(100*point.ThetaSD/point.Theta, 'g', 2) + "%");
        QTime run = QTime(0, 0, 0, 0).addMSecs(point.Time*1000);
        ui->lineEditTimePassed->setText(run.toString("hh:mm:ss.z"));

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            if (debugTime2 - debugTime1 > 1)
                printLogMsg("Outputting current readings : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");

            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        }

        if (this->generatorActive) {
            if (this->frequencyFrom < this->frequencyTo)
                ui->progressBarExperiment->setValue(FGen);
            else
                ui->progressBarExperiment->setValue(this->frequencyTo - FGen + this->frequencyFrom);
        }

        while (this->pause) {
            QTest::qWait(this->minWait);
            if (this->stop) {
                fclose(stderr);

                return;
            }
        }

        FGen += this->frequencyStep;

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            if (debugTime2 - debugTime1 > 1)
                printLogMsg("Some afterjob : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");
        }

    }

    return;
}

void Amarok::prepareIntervalList()
{
    this->intervalsAmmount = 0;
    this->intervalFrom.clear();
    this->intervalTo.clear();
    this->intervalStep.clear();

    QStringList intervals = ui->textEditGeneratorScanningIntervals->toPlainText().split('\n');

    for (int i = 0; i < intervals.size(); i++) {
        QStringList interval = intervals[i].split(':');
        if (interval.size() != 3) {
            printLogMsg("Interval " + QString::number(i) + " is invalid!", this->logMode != "Off");
        } else {
            this->intervalsAmmount++;
            this->intervalFrom.push_back(interval[0].toDouble());
            this->intervalTo.push_back(  interval[1].toDouble());
            this->intervalStep.push_back(interval[2].toDouble());
        }
    }

    return;
}

void Amarok::simpleExperimentRun()
{
    this->intervalsAmmount = 1;
    int round = 0;
    do {
        round++;

        experimentRun(1, round);

        while (this->pause) {
            QTest::qWait(this->minWait);
            if (this->stop) {
                fclose(stdout);

                return;
            }
        }
    } while (ui->comboBoxExperimentMode->currentText() == "Continuous");

    return;
}

void Amarok::multiIntervalExperimentRun()
{
    prepareIntervalList();

    int round = 0;
    do {
        round++;

        for (int interval = 1; interval <= this->intervalsAmmount; interval++) {
            this->frequencyFrom = this->intervalFrom[interval - 1];
            this->frequencyTo   = this->intervalTo[interval - 1];
            this->frequencyStep = this->intervalStep[interval - 1];

            setFrequency(this->frequencyFrom);
            if (interval != 1)
                QTest::qWait(_round(this->waitBetweenRoundsMultiplier*this->waitAfterNewFrequency));

            experimentRun(interval, round);

            while (this->pause) {
                QTest::qWait(this->minWait);
                if (this->stop) {
                    fclose(stderr);

                    return;
                }
            }
        }
    } while (ui->comboBoxExperimentMode->currentText() == "Continuous");

    return;
}

void Amarok::kineticPrescanRun()
{
    experimentSettingsChange();

    experimentRun(1, 1);

    return;
}

KineticFrequencyPoint Amarok::kineticRoundRun()
{
    if (this->calculateDebugTime) {
        debugTime3 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    }

    KineticFrequencyPoint ans;
    KineticThetaPoint point;

    this->kineticRoundData.clear();

    ans.Time = -1;

    if (this->calculateDebugTime) {
        debugTime4 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        printLogMsg("Preparing data for round " + QString::number(this->kineticRoundNumber) + " : " + QString::number(debugTime4 - debugTime3), this->logMode == "All");

        debugTime3 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    }

    ui->customPlot_1->clearGraphs();
    ui->customPlot_1->addGraph();

    if (ui->checkBoxKineticsSecondPlotEnableRTheta->checkState() == Qt::Checked) {
        ui->customPlot_2->clearGraphs();
        ui->customPlot_2->addGraph();
    }

    if (this->calculateDebugTime) {
        debugTime4 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        printLogMsg("Preparing graphs for round " + QString::number(this->kineticRoundNumber) + " : " + QString::number(debugTime4 - debugTime3), this->logMode == "All");

        debugTime3 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    }

    QString fileName = this->currentKineticPath + "Kinetic_Round_" + QString::number(this->kineticRoundNumber) + ".dat";
    if (freopen(fileName.toStdString().c_str(), "w+", stderr) == nullptr)
        printLogMsg("Unable to open file " + fileName + " for exporting!", this->logMode != "Off");


    fprintf(stderr,"TimeAll\tTimeExperiment\tR\tTheta\n");

    if (this->calculateDebugTime) {
        debugTime4 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        printLogMsg("Preparing files for round " + QString::number(this->kineticRoundNumber) + " : " + QString::number(debugTime4 - debugTime3), this->logMode == "All");
    }

    if (this->calculateDebugTime) {
        debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        printLogMsg("Entering round " + QString::number(this->kineticRoundNumber) + " : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");
    }

    for (int invalidPoint = 0; invalidPoint < this->firstInvalidPoints; invalidPoint++)
        getR();

    this->timeNewRound = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    double currentTime = -1;

    while (!this->nextKineticRound) {
        getRTheta(point.R, point.Theta);

        currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        point.TimeRound = currentTime - this->timeNewRound;
        point.TimeExperiment = currentTime - this->timeStart;

        this->kineticRoundData.push_back(point);

        pushPointOnPlot(point);

        if (ui->checkBoxKineticsSecondPlotEnableRTheta->checkState() == Qt::Checked)
            updatePlot(0);
        else
            updatePlot(1);

//        ui->lineEditCurrentReadingsThetaLockInAmplifier->setText(QString::number(point.Theta));
//        ui->lineEditCurrentReadingsRLockInAmplifier->setText(QString::number(point.R));
        ui->lineEditTimePassed->setText(QString::number(point.TimeExperiment));

        fprintf(stderr, "%7e\t%7e\t%7e\t%7e\n", point.TimeExperiment, point.TimeRound, point.R, point.Theta);

        if (ui->comboBoxExtremumMode->currentText() == "Min") {
            if (point.Theta < ans.ThetaPeak || ans.Time < 0) {
                ans.ThetaPeak = point.Theta;
                ans.Time = point.TimeExperiment;
            }
        } else {
            if (point.Theta > ans.ThetaPeak) {
                ans.ThetaPeak = point.Theta;
                ans.Time = point.TimeExperiment;
            }
        }

        QTest::qWait(this->minWait);
    }

    fclose(stderr);

    return ans;
}

void Amarok::kineticExperimentRun()
{
//  Just to be sure that some dudes will not be able to ruin that by changing folder name
    QDir kineticDataFolder = QDir(this->currentKineticPath);
    if (!kineticDataFolder.exists())
        kineticDataFolder.mkpath(".");

    KineticFrequencyPoint point;

    clearPlot();
    ui->customPlot_2->addGraph();

    ui->lcdNumberCurrentRound->display(0);
    this->kineticRoundNumber = 0;

    QString fileName = this->currentKineticPath + "Kinetic.kin";

    fclose(stdout);
    if (freopen(fileName.toStdString().c_str(), "w+", stdout) == nullptr)
        printLogMsg("Unable to open file " + fileName + " for exporting!", this->logMode != "Off");

    printf("Time\tFext\tFgen\tThetaPeak\n");

    on_comboBoxDFresDT_currentTextChanged(ui->comboBoxDFresDT->currentText());

    this->jumpValue = ui->doubleSpinBoxJump->value();
    this->jumpDivider = ui->doubleSpinBoxJumpDivider->value();

    double FGen = ui->doubleSpinBoxResonanceFrequency->value() + jumpSign*jumpValue;
    setFrequency(FGen);

    debugTime1 = 0;
    debugTime2 = 0;

    this->timeStart = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    while (!this->stop) {

        if (this->calculateDebugTime)
            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

        ui->lcdNumberCurrentRound->display(++this->kineticRoundNumber);

        this->nextKineticRound = false;

        if (this->kineticRoundNumber > 1) {
            FGen += jumpSign * jumpValue;
            setFrequency(FGen);
        }
        ui->doubleSpinBoxJump->setValue(jumpValue / jumpDivider);

//        qDebug() << "Round = " << this->kineticRoundNumber;
//        qDebug() << "FGen = " << FGen;
//        qDebug() << "Divider = " << jumpDivider;
//        qDebug() << "Jump = " << jumpSign * (jumpValue / jumpDivider) << endl;

        QTest::qWait(this->waitAfterNewFrequencyKinetic);
//        ui->lineEditCurrentReadingsFrequencyGenerator->setText(QString::number(FGen));

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            printLogMsg("Prepare for round " + QString::number(this->kineticRoundNumber) + " : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");
        }

        if (this->calculateDebugTime)
            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

        getFext();

        point = kineticRoundRun();

        point.Fgen = FGen;
        point.Fext = getFext();

        if (this->calculateDebugTime)
            debugTime1 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

//        qDebug() << point.Time << ", " << point.Fgen << ", " << point.Fext << ", " << point.ThetaPeak;

        this->kineticData.push_back(point);

        if (ui->checkBoxKineticsSecondPlotEnableRTheta->checkState() != Qt::Checked) {
            pushPointOnPlot(point);

            updatePlot(2);
        }

        printf("%0.20e\t%0.20e\t%0.20e\t%0.20e\n", point.Time, point.Fext, point.Fgen, point.ThetaPeak);

        if (this->calculateDebugTime) {
            debugTime2 = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
            printLogMsg("Print and out for round " + QString::number(this->kineticRoundNumber) + " : " + QString::number(debugTime2 - debugTime1), this->logMode == "All");
        }        
    }

    fclose(stdout);

    return;
}

void Amarok::on_comboBoxDFresDT_currentTextChanged(const QString &arg1)
{

    arg1.contains(">") ? jumpSign = 1 : jumpSign = -1;

    return;
}

void Amarok::on_doubleSpinBoxJump_valueChanged(double arg1)
{
    this->jumpValue = arg1;

    return;
}

void Amarok::on_doubleSpinBoxJumpDivider_valueChanged(double arg1)
{
    this->jumpDivider = arg1;

    return;
}
