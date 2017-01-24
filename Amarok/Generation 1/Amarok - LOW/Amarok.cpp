#include "Amarok.h"
#include "ui_Amarok.h"

Amarok::Amarok(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Amarok)
{
    this->constructor = true;
    this->listLoad = true;

    ui->setupUi(this);

    hideAll();

    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);

    QDir currentFolder = QDir();
    this->absolutePath = currentFolder.absolutePath() + "\\";
    this->dataPath = this->absolutePath + "Data\\";
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

    this->generator = new Generator();
    this->lockInAmplifier = new LockInAmplifier();

    this->experimentMode.clear();
    this->experimentMode.push_back("Single");
    this->experimentMode.push_back("Continuous");
    this->experimentMode.push_back("Kinetic");
    ui->comboBoxExperimentMode->addItems(this->experimentMode);
    ui->comboBoxExperimentMode->setCurrentText("Single");

    this->replotMode.clear();
    this->replotMode.push_back("Auto");
    this->replotMode.push_back("Manual");
    ui->comboBoxReplot_1->addItems(this->replotMode);
    ui->comboBoxReplot_1->setCurrentText("Auto");
    ui->comboBoxReplot_2->addItems(this->replotMode);
    ui->comboBoxReplot_2->setCurrentText("Auto");

    this->autoKineticsJumpMode.clear();
    this->autoKineticsJumpMode.push_back("Min");
    this->autoKineticsJumpMode.push_back("Max");
    ui->comboBoxJumpAfterExtremum->addItems(this->autoKineticsJumpMode);
    ui->comboBoxJumpAfterExtremum->setCurrentText("Min");

    this->signMode.clear();
    this->signMode.push_back(" > 0");
    this->signMode.push_back(" < 0");
    ui->comboBoxDFresDT->addItems(this->signMode);
    ui->comboBoxDFresDT->setCurrentText(" > 0");

    this->xAxisNormalMode.clear();
    this->xAxisNormalMode.push_back("Fgen");
    this->xAxisNormalMode.push_back("Fext");
    this->xAxisNormalMode.push_back("Time");
    ui->comboBoxXAxisValue_1->addItems(this->xAxisNormalMode);
    ui->comboBoxXAxisValue_1->setCurrentText("Time");
    ui->comboBoxXAxisValue_2->addItems(this->xAxisNormalMode);
    ui->comboBoxXAxisValue_2->setCurrentText("Time");

    this->yAxisNormalMode.clear();
    this->yAxisNormalMode.push_back("R");
    this->yAxisNormalMode.push_back("Theta");
    ui->comboBoxYAxisValue_1->addItems(this->yAxisNormalMode);
    ui->comboBoxYAxisValue_1->setCurrentText("R");
    ui->comboBoxYAxisValue_2->addItems(this->yAxisNormalMode);
    ui->comboBoxYAxisValue_2->setCurrentText("Theta");

    this->lockInAmplifierModel = this->lockInAmplifier->getSupportedTypes();
    this->lockInAmplifierModel.push_back("SET IT");
    ui->comboBoxModelLockInAmplifier->addItems(this->lockInAmplifierModel);
    ui->comboBoxModelLockInAmplifier->setCurrentText("SET IT");

    this->generatorModel = this->generator->getSupportedTypes();
    this->generatorModel.push_back("SET IT");
    ui->comboBoxModelGenerator->addItems(this->generatorModel);
    ui->comboBoxModelGenerator->setCurrentText("SET IT");

    this->allCom.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        this->allCom.push_back(info.portName());
    this->allCom.push_back("AUTO");
    this->allCom.push_back("SET IT");

    this->generatorCom.clear();
    ui->comboBoxSerialPortGenerator->clear();
    ui->comboBoxSerialPortGenerator->addItems(this->allCom);
    ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");

    this->lockInAmplifierCom.clear();
    ui->comboBoxSerialPortLockInAmplifier->clear();
    ui->comboBoxSerialPortLockInAmplifier->addItems(this->allCom);
    ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");

    ui->lcdNumber->display(0);
    ui->progressBarExperiment->setValue(0);

    connect(ui->plot_1, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(on_plot_1_Clicked(QMouseEvent*)));
    connect(ui->plot_2, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(on_plot_2_Clicked(QMouseEvent*)));

    connect(ui->doubleSpinBoxFrequencyFromGenerator, SIGNAL(valueChanged(QString)),
            this, SLOT(changeCurrentExperimentSettings(QString)));
    connect(ui->doubleSpinBoxFrequencyToGenerator,   SIGNAL(valueChanged(QString)),
            this, SLOT(changeCurrentExperimentSettings(QString)));
    connect(ui->doubleSpinBoxFrequencyStepGenerator, SIGNAL(valueChanged(QString)),
            this, SLOT(changeCurrentExperimentSettings(QString)));
    connect(ui->spinBoxAverageOfPoints,              SIGNAL(valueChanged(QString)),
            this, SLOT(changeCurrentExperimentSettings(QString)));
    connect(ui->spinBoxWait,                         SIGNAL(valueChanged(QString)),
            this, SLOT(changeCurrentExperimentSettings(QString)));
    connect(ui->comboBoxExperimentMode,              SIGNAL(currentTextChanged(QString)),
            this, SLOT(changeCurrentExperimentSettings(QString)));
    connect(ui->checkBoxGeneratorPoll,               SIGNAL(clicked(bool)),
            this, SLOT(changeCurrentExperimentSettings(bool)));
    connect(ui->checkBoxAutosettingsLockInAmplifier, SIGNAL(clicked(bool)),
            this, SLOT(changeCurrentExperimentSettings(bool)));

    connect(ui->doubleSpinBoxAmplitudeGenerator,     SIGNAL(valueChanged(QString)),
            this, SLOT(changeGeneratorSettings(QString)));
    connect(ui->doubleSpinBoxOffsetGenerator,        SIGNAL(valueChanged(QString)),
            this, SLOT(changeGeneratorSettings(QString)));

    connect(&(this->replotTimer), SIGNAL(timeout()), this, SLOT(updatePlot()));

    initPlot();
    changeCurrentExperimentSettings();

    this->listLoad = false;
    this->constructor = false;

    ui->comboBoxSerialPortGenerator->setCurrentText("COM6");
    ui->comboBoxSerialPortLockInAmplifier->setCurrentText("COM5");
//    ui->comboBoxExperimentMode->setCurrentText("Kinetic");

}

Amarok::~Amarok()
{
    delete ui;
}

//  Hiding and Showing

void Amarok::hideGraph(const int &i) const
{
    if (i == 0 || i == 1) {
        ui->groupBoxPlot_1->hide();
        ui->plot_1->hide();
    }
    if (i == 0 || i == 2) {
        ui->groupBoxPlot_2->hide();
        ui->plot_2->hide();
    }

    return;
}

void Amarok::hideAxises(const int &i) const
{
    if (i == 0 || i == 1)
        ui->groupBoxAxis_1->hide();
    if (i == 0 || i == 2)
        ui->groupBoxAxis_2->hide();

    return;
}

void Amarok::hideCurrentReadingsAndTiming() const
{
    ui->groupBoxCurrentReadingsAndTiming->hide();

    return;
}

void Amarok::hideExperimentMainSettings() const
{
    ui->groupBoxExprimentMainSettings->hide();

    return;
}

void Amarok::hideExperimentKinetics() const
{
    ui->groupBoxExperimentKinetics->hide();

    return;
}

void Amarok::hideGroupBoxes() const
{
    hideGraph();
    hideCurrentReadingsAndTiming();
    hideExperimentMainSettings();
    hideExperimentKinetics();

    return;
}

void Amarok::hideLockInAmplifier() const
{
    ui->labelTimeConstantLockInAmplifier->hide();
    ui->comboBoxTimeConstantLockInAmplifier->hide();

    ui->checkBoxAutosettingsLockInAmplifier->hide();

    ui->labelInputRangeLockInAmplifier->hide();
    ui->comboBoxInputRangeLockInAmplifier->hide();

    ui->labelSensivityLockInAmplifier->hide();
    ui->comboBoxSensivityLockInAmplifier->hide();

    return;
}

void Amarok::hideGenerator() const
{
    ui->labelAmplitudeGenerator->hide();
    ui->doubleSpinBoxAmplitudeGenerator->hide();

    ui->labelOffsetGenerator->hide();
    ui->doubleSpinBoxOffsetGenerator->hide();

    ui->labelFrequencyFromGenerator->hide();
    ui->doubleSpinBoxFrequencyFromGenerator->hide();

    ui->labelFrequencyToGenerator->hide();
    ui->doubleSpinBoxFrequencyToGenerator->hide();

    ui->labelFrequencyStepGenerator->hide();
    ui->doubleSpinBoxFrequencyStepGenerator->hide();

    return;
}

void Amarok::hideTest() const
{
    ui->pushButtonTestEquipment->hide();

    return;
}

void Amarok::hideGeneratorPolling() const
{
    ui->checkBoxGeneratorPoll->hide();

    return;
}

void Amarok::hideExperimentMode() const
{
    ui->labelExperimentMode->hide();
    ui->comboBoxExperimentMode->hide();

    return;
}

void Amarok::hideAll() const
{
    hideGroupBoxes();
    hideLockInAmplifier();
    hideGeneratorPolling();
    hideTest();
    hideGenerator();
    hideExperimentMode();

    return;
}

void Amarok::showGraph(const int &i) const
{
    if (i == 0 || i == 1) {
        ui->groupBoxPlot_1->show();
        ui->plot_1->show();
    }
    if (i == 0 || i == 2) {
        ui->groupBoxPlot_2->show();
        ui->plot_2->show();
    }

    return;
}

void Amarok::showAxises(const int &i) const
{
    if (i == 0 || i == 1)
        ui->groupBoxAxis_1->show();
    if (i == 0 || i == 2)
        ui->groupBoxAxis_2->show();

    return;
}

void Amarok::showCurrentReadingsAndTiming() const
{
    ui->groupBoxCurrentReadingsAndTiming->show();

    return;
}

void Amarok::showExperimentMainSettings() const
{
    ui->groupBoxExprimentMainSettings->show();

    return;
}

void Amarok::showExperimentKinetics() const
{
    ui->groupBoxExperimentKinetics->show();

    return;
}

void Amarok::showGroupBoxes() const
{
    showGraph();
    showCurrentReadingsAndTiming();
    showExperimentMainSettings();
    showExperimentKinetics();

    return;
}

void Amarok::showLockInAmplifier() const
{
    if (this->lockInAmplifier->workWithTimeConstant()) {
        ui->labelTimeConstantLockInAmplifier->show();
        ui->comboBoxTimeConstantLockInAmplifier->show();
    }

    ui->checkBoxAutosettingsLockInAmplifier->show();

    if (this->lockInAmplifier->workWithVoltageInputRange()) {
        ui->labelInputRangeLockInAmplifier->show();
        ui->comboBoxInputRangeLockInAmplifier->show();
    }

    if (this->lockInAmplifier->workWithSensivity()) {
        ui->labelSensivityLockInAmplifier->show();
        ui->comboBoxSensivityLockInAmplifier->show();
    }

    return;
}

void Amarok::showGenerator() const
{
    if (this->generator->workWithAmplitude()) {
        ui->labelAmplitudeGenerator->show();
        ui->doubleSpinBoxAmplitudeGenerator->show();
    }

    if (this->generator->workWithOffset()) {
        ui->labelOffsetGenerator->show();
        ui->doubleSpinBoxOffsetGenerator->show();
    }

    if (this->generator->workWithFrequency()) {
        ui->labelFrequencyFromGenerator->show();
        ui->doubleSpinBoxFrequencyFromGenerator->show();

        ui->labelFrequencyToGenerator->show();
        ui->doubleSpinBoxFrequencyToGenerator->show();

        ui->labelFrequencyStepGenerator->show();
        ui->doubleSpinBoxFrequencyStepGenerator->show();
    }

    return;
}

void Amarok::showTest() const
{
    ui->pushButtonTestEquipment->show();

    return;
}

void Amarok::showGeneratorPolling() const
{
    ui->checkBoxGeneratorPoll->show();

    return;
}

void Amarok::showExperimentMode() const
{
    ui->labelExperimentMode->show();
    ui->comboBoxExperimentMode->show();

    return;
}

void Amarok::showAll() const
{
    showGroupBoxes();
    showLockInAmplifier();
    showGenerator();
    showTest();
    showGeneratorPolling();
    showExperimentMode();

    return;
}

// Plotting

void Amarok::initPlot(const int &i) const
{
    if (i == 1 || i == 0) {
        //* OPTIMIZE FOR SPEED
        ui->plot_1->setNotAntialiasedElements(QCP::aeAll);
        QFont font;
        font.setStyleStrategy(QFont::NoAntialias);
        ui->plot_1->xAxis->setTickLabelFont(font);
        ui->plot_1->yAxis->setTickLabelFont(font);
        ui->plot_1->legend->setFont(font);
        //*/

        ui->plot_1->clearGraphs();
        ui->plot_1->addGraph();

        updateAxisesLabels(1);

        ui->plot_1->xAxis->setTickLabelType(QCPAxis::ltNumber);
        ui->plot_1->axisRect()->setupFullAxesBox();
    }
    if (i == 2 || i == 0) {
        //* OPTIMIZE FOR SPEED
        ui->plot_2->setNotAntialiasedElements(QCP::aeAll);
        QFont font;
        font.setStyleStrategy(QFont::NoAntialias);
        ui->plot_2->xAxis->setTickLabelFont(font);
        ui->plot_2->yAxis->setTickLabelFont(font);
        ui->plot_2->legend->setFont(font);
        //*/

        ui->plot_2->clearGraphs();
        ui->plot_2->addGraph();

        updateAxisesLabels(2);

        ui->plot_2->xAxis->setTickLabelType(QCPAxis::ltNumber);
        ui->plot_2->axisRect()->setupFullAxesBox();
    }

    return;
}

void Amarok::setBoundaries(const int &i) const
{
    if (i == 1 || i == 0) {
        ui->lineEditRangeXmax_1->setText("-1E50");
        ui->lineEditRangeXmin_1->setText("+1E50");
        ui->lineEditRangeYmax_1->setText("-1E50");
        ui->lineEditRangeYmin_1->setText("+1E50");
    }
    if (i == 1 || i == 0) {
        ui->lineEditRangeXmax_2->setText("-1E50");
        ui->lineEditRangeXmin_2->setText("+1E50");
        ui->lineEditRangeYmax_2->setText("-1E50");
        ui->lineEditRangeYmin_2->setText("+1E50");
    }

    return;
}

void Amarok::fillAxisesLabels(const QString &experimentMode) const
{
    if (experimentMode == "Normal") {
        updateAxisesLabels();
    }

    if (experimentMode == "PreScan") {
        ui->comboBoxXAxisValue_1->setCurrentText("Fext");
        ui->comboBoxYAxisValue_1->setCurrentText("R");
        ui->comboBoxXAxisValue_2->setCurrentText("Fext");
        ui->comboBoxYAxisValue_2->setCurrentText("Theta");

        updateAxisesLabels();
    }

    if (experimentMode == "Kinetic") {

        ui->plot_1->xAxis->setLabel("Time");
        ui->plot_1->yAxis->setLabel("Theta");
        ui->plot_1->replot();

        ui->plot_2->xAxis->setLabel("Time");
        ui->plot_2->yAxis->setLabel("Frequency");
        ui->plot_2->replot();
    }

    return;
}

void Amarok::updateAxisesLabels(const int &i) const
{
    if (i == 1 || i == 0) {
        ui->plot_1->xAxis->setLabel(ui->comboBoxXAxisValue_1->currentText());
        ui->plot_1->yAxis->setLabel(ui->comboBoxYAxisValue_1->currentText());

        ui->plot_1->replot();
    }
    if (i == 2 || i == 0) {
        ui->plot_2->xAxis->setLabel(ui->comboBoxXAxisValue_2->currentText());
        ui->plot_2->yAxis->setLabel(ui->comboBoxYAxisValue_2->currentText());

        ui->plot_2->replot();
    }

    return;
}

void Amarok::on_plot_1_Clicked(QMouseEvent *event) const
{
    ui->lineEditPointX_1->setText(QString::number(ui->plot_1->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditPointY_1->setText(QString::number(ui->plot_1->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Amarok::on_plot_2_Clicked(QMouseEvent *event) const
{
    ui->lineEditPointX_2->setText(QString::number(ui->plot_2->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditPointY_2->setText(QString::number(ui->plot_2->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Amarok::updatePlot(const int &i) const
{
    if (i == 1 || i == 0) {
        ui->plot_1->xAxis->setRange(ui->lineEditRangeXmin_1->text().toDouble()*(1 - this->subDown),
                                    ui->lineEditRangeXmax_1->text().toDouble()*(1 + this->addUp));
        ui->plot_1->yAxis->setRange(ui->lineEditRangeYmin_1->text().toDouble()*(1 - this->subDown),
                                    ui->lineEditRangeYmax_1->text().toDouble()*(1 + this->addUp));
        ui->plot_1->replot();
    }

    if (i == 2 || i == 0) {
        ui->plot_2->xAxis->setRange(ui->lineEditRangeXmin_2->text().toDouble(),
                                    ui->lineEditRangeXmax_2->text().toDouble());
        ui->plot_2->yAxis->setRange(ui->lineEditRangeYmin_2->text().toDouble(),
                                    ui->lineEditRangeYmax_2->text().toDouble());
        ui->plot_2->replot();
    }

    return;
}

void Amarok::updatePlot() const
{
    return updatePlot(0);
}

void Amarok::replotPlot(const int &i)
{
    if (this->constructor)
        return;

    if (this->simpleExperimentData.isEmpty())
        return;

    timerPause();

    if (i == 1 || i == 0) {
        ui->plot_1->graph(0)->clearData();

        if (ui->comboBoxXAxisValue_1->currentText() == "Fgen") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(this->simpleExperimentData.getFgenat(point),
                                                  this->simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(this->simpleExperimentData.getFgenat(point),
                                                  this->simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_1->currentText() == "Fext") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(this->simpleExperimentData.getFextat(point),
                                                  this->simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(this->simpleExperimentData.getFextat(point),
                                                  this->simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_1->currentText() == "Time") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(this->simpleExperimentData.getTimeat(point),
                                                  this->simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(this->simpleExperimentData.getTimeat(point),
                                                  this->simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }

    if (i == 2 || i == 0) {
        ui->plot_2->graph(0)->clearData();

        if (ui->comboBoxXAxisValue_2->currentText() == "Fgen") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(this->simpleExperimentData.getFgenat(point),
                                                  this->simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(this->simpleExperimentData.getFgenat(point),
                                                  this->simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_2->currentText() == "Fext") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(this->simpleExperimentData.getFextat(point),
                                                  this->simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(this->simpleExperimentData.getFextat(point),
                                                  this->simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_2->currentText() == "Time") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(this->simpleExperimentData.getTimeat(point),
                                                  this->simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                for (int point = 0; point < this->simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(this->simpleExperimentData.getTimeat(point),
                                                  this->simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }

    updatePlot(i);

    timerPause();

    return;
}

void Amarok::replotPlot()
{
    return replotPlot(0);
}

void Amarok::pushPointOnPlot(const SimpleExperimentPoint &point) const
{
    if (ui->comboBoxXAxisValue_1->currentText() == "Fgen") {
        if (ui->comboBoxYAxisValue_1->currentText() == "R") {
            ui->plot_1->graph(0)->addData(point.Fgen, point.R);

            if (ui->comboBoxReplot_1->currentText() == "Auto") {
                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
        }
        if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
            ui->plot_1->graph(0)->addData(point.Fgen, point.Theta);

            if (ui->comboBoxReplot_1->currentText() == "Auto") {
                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }
    if (ui->comboBoxXAxisValue_1->currentText() == "Fext") {
        if (ui->comboBoxYAxisValue_1->currentText() == "R") {
            ui->plot_1->graph(0)->addData(point.Fext, point.R);

            if (ui->comboBoxReplot_1->currentText() == "Auto") {
                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
        }
        if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
            ui->plot_1->graph(0)->addData(point.Fext, point.Theta);

            if (ui->comboBoxReplot_1->currentText() == "Auto") {
                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }
    if (ui->comboBoxXAxisValue_1->currentText() == "Time") {
        if (ui->comboBoxYAxisValue_1->currentText() == "R") {
            ui->plot_1->graph(0)->addData(point.Time, point.R);

            if (ui->comboBoxReplot_1->currentText() == "Auto") {
                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
        }
        if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
            ui->plot_1->graph(0)->addData(point.Time, point.Theta);

            if (ui->comboBoxReplot_1->currentText() == "Auto") {
                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }

    if (ui->comboBoxXAxisValue_2->currentText() == "Fgen") {
        if (ui->comboBoxYAxisValue_2->currentText() == "R") {
            ui->plot_2->graph(0)->addData(point.Fgen, point.R);

            if (ui->comboBoxReplot_2->currentText() == "Auto") {
                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
        }
        if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
            ui->plot_2->graph(0)->addData(point.Fgen, point.Theta);

            if (ui->comboBoxReplot_2->currentText() == "Auto") {
                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }
    if (ui->comboBoxXAxisValue_2->currentText() == "Fext") {
        if (ui->comboBoxYAxisValue_2->currentText() == "R") {
            ui->plot_2->graph(0)->addData(point.Fext, point.R);

            if (ui->comboBoxReplot_2->currentText() == "Auto") {
                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
        }
        if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
            ui->plot_2->graph(0)->addData(point.Fext, point.Theta);

            if (ui->comboBoxReplot_2->currentText() == "Auto") {
                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }
    if (ui->comboBoxXAxisValue_2->currentText() == "Time") {
        if (ui->comboBoxYAxisValue_2->currentText() == "R") {
            ui->plot_2->graph(0)->addData(point.Time, point.R);

            if (ui->comboBoxReplot_2->currentText() == "Auto") {
                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
        }
        if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
            ui->plot_2->graph(0)->addData(point.Time, point.Theta);

            if (ui->comboBoxReplot_2->currentText() == "Auto") {
                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }

    return;
}

void Amarok::pushPointOnPlot(const KineticThetaPoint &point) const
{
    ui->plot_1->graph(0)->addData(point.Time, point.Theta);

    ui->lineEditRangeXmin_1->setText(QString::number(this->kineticThetaData.getTimeMin()));
    ui->lineEditRangeXmax_1->setText(QString::number(this->kineticThetaData.getTimeMax()));

    ui->lineEditRangeYmin_1->setText(QString::number(this->kineticThetaData.getThetaMin()));
    ui->lineEditRangeYmax_1->setText(QString::number(this->kineticThetaData.getThetaMax()));

    return;
}

void Amarok::pushPointOnPlot(const KineticFrequencyPoint &point) const
{
    ui->plot_2->graph(0)->addData(point.Time, point.Fext);

    ui->lineEditRangeXmin_2->setText(QString::number(this->kineticFrequencyData.getTimeMin()));
    ui->lineEditRangeXmax_2->setText(QString::number(this->kineticFrequencyData.getTimeMax()));

    ui->lineEditRangeYmin_2->setText(QString::number(this->kineticFrequencyData.getFextMin()));
    ui->lineEditRangeYmax_2->setText(QString::number(this->kineticFrequencyData.getFextMax()));

    return;
}

// Lock-in Amplifier

double Amarok::getMaxValueInRange(const double &min, const double &max) const
{
    double signal = -1E10;

    if (!this->generatorActive)
        return this->lockInAmplifier->getR();

    this->lockInAmplifier->setDefaultSensivity();
    this->lockInAmplifier->setDefaultVoltageInputRange();

    for (int i = 0; i < this->analizingPointsForLockInAmplifierAutosettings; i++) {
        double freq = _min (max, min) + i*(_max(max, min) - _min(max, min)) / (this->analizingPointsForLockInAmplifierAutosettings - 1);

        this->generator->setFrequency(freq);

        QTest::qWait(this->waitAfterNewFrequency);

        signal = _max(signal, this->lockInAmplifier->getR());
    }

    return signal;

}

void Amarok::fillLockInAmplifierButtons()
{
    this->listLoad = true;

//    this->lockInAmplifier->setDefaultSettings();

    if (this->lockInAmplifier->workWithTimeConstant()) {
        ui->labelTimeConstantLockInAmplifier->show();
        ui->comboBoxTimeConstantLockInAmplifier->show();
        ui->comboBoxTimeConstantLockInAmplifier->clear();
        ui->comboBoxTimeConstantLockInAmplifier->addItems(this->lockInAmplifier->getTimeConstantList());
        ui->comboBoxTimeConstantLockInAmplifier->setCurrentText(this->lockInAmplifier->getTimeConstant());
    } else {
        ui->labelTimeConstantLockInAmplifier->hide();
        ui->comboBoxTimeConstantLockInAmplifier->hide();
    }

    if (this->lockInAmplifier->workWithVoltageInputRange()) {
        ui->labelInputRangeLockInAmplifier->show();
        ui->comboBoxInputRangeLockInAmplifier->show();
        ui->comboBoxInputRangeLockInAmplifier->clear();
        ui->comboBoxInputRangeLockInAmplifier->addItems(this->lockInAmplifier->getVoltageInputRangeList());
        ui->comboBoxInputRangeLockInAmplifier->setCurrentText(this->lockInAmplifier->getVoltageInputRange());
    } else {
        ui->labelInputRangeLockInAmplifier->hide();
        ui->comboBoxInputRangeLockInAmplifier->hide();
    }

    if (this->lockInAmplifier->workWithSensivity()) {
        ui->labelSensivityLockInAmplifier->show();
        ui->comboBoxSensivityLockInAmplifier->show();
        ui->comboBoxSensivityLockInAmplifier->clear();
        ui->comboBoxSensivityLockInAmplifier->addItems(this->lockInAmplifier->getSensivityList());
        ui->comboBoxSensivityLockInAmplifier->setCurrentText(this->lockInAmplifier->getSensivity());
    } else {
        ui->labelSensivityLockInAmplifier->hide();
        ui->comboBoxSensivityLockInAmplifier->hide();
    }

    this->listLoad = false;

    return;
}

double Amarok::getR() const
{
    return this->lockInAmplifier->getR();
}

void Amarok::getR(double &R) const
{
    R = getR();

    return;
}

double Amarok::getTheta() const
{
    return this->lockInAmplifier->getTheta();
}

void Amarok::getTheta(double &Theta) const
{
    Theta = getTheta();

    return;
}

double Amarok::getFext() const
{
    return this->lockInAmplifier->getExternalFrequency();
}

void Amarok::getFext(double &Fext) const
{
    Fext = getFext();

    return;
}

void Amarok::getRThetaF(double &R, double &Theta, double &Fext) const
{
    this->lockInAmplifier->getRThetaFext(R, Theta, Fext);

    return;
}

void Amarok::getSDAll(double &R, double &RSD, double &Theta, double &ThetaSD, double &Fext, double &FextSD, const int &average)
{
    double temp_R = 0;
    double temp_Theta = 0;
    double temp_Fext = 0;

    R = 0;
    RSD = 0;
    Theta = 0;
    ThetaSD = 0;
    Fext = 0;
    FextSD = 0;

    this->RSDvector.resize(average);
    this->ThetaSDvector.resize(average);
    this->FSDvector.resize(average);

    for (int point = 0; point < this->averageOfPoints; point++) {
        getRThetaF(temp_R, temp_Theta, temp_Fext);

        RSDvector[point] = temp_R;
        ThetaSDvector[point] = temp_Theta;
        FSDvector[point] = temp_Fext;

        R += temp_R;
        Theta += temp_Theta;
        Fext += temp_Fext;
    }

    R /= average;
    Theta /= average;
    Fext /= average;

    for (int point = 0; point < average; point++) {
        RSD += _sqr(RSDvector[point] - R);
        ThetaSD += _sqr(ThetaSDvector[point] - Theta);
        FextSD += _sqr(FSDvector[point] - Fext);
    }

    RSD = _sqrt(RSD/average);
    ThetaSD = _sqrt(ThetaSD/average);
    FextSD = _sqrt(FextSD/average);

    return;
}

// Generator

void Amarok::changeGeneratorSettings(const QString &str) const
{
    if (!this->generatorActive)
        return;

    if (this->listLoad)
        return;

    this->generator->setAmplitude(ui->doubleSpinBoxAmplitudeGenerator->value(), "VR");
    this->generator->setOffset(ui->doubleSpinBoxOffsetGenerator->value());

    return;
}

void Amarok::fillGeneratorButtons()
{
    this->listLoad = true;

//    this->generator->setDefaultSettings();

    if (this->generator->workWithAmplitude()) {
        ui->labelAmplitudeGenerator->show();
        ui->doubleSpinBoxAmplitudeGenerator->show();
        if (this->generator->workWithOutputImpedance()) {
            ui->doubleSpinBoxAmplitudeGenerator->setMinimum(this->generator->getMinAmplitude(this->generator->getFunction(),
                                                                                             this->generator->getOutputImpedance(),
                                                                                             true));
            ui->doubleSpinBoxAmplitudeGenerator->setMaximum(this->generator->getMaxAmplitude(this->generator->getFunction(),
                                                                                             this->generator->getOutputImpedance(),
                                                                                             true));
            ui->doubleSpinBoxAmplitudeGenerator->setSingleStep(this->generator->getStepAmplitude(this->generator->getFunction(),
                                                                                                 this->generator->getOutputImpedance(),
                                                                                                 true));
            ui->doubleSpinBoxAmplitudeGenerator->setDecimals(this->generator->getDecimalsAmplitude(this->generator->getFunction(),
                                                                                                   this->generator->getOutputImpedance(),
                                                                                                   true));
        } else {
            ui->doubleSpinBoxAmplitudeGenerator->setMinimum(this->generator->getMinAmplitude(this->generator->getFunction(),
                                                                                             "VR"));
            ui->doubleSpinBoxAmplitudeGenerator->setMaximum(this->generator->getMaxAmplitude(this->generator->getFunction(),
                                                                                             "VR"));
            ui->doubleSpinBoxAmplitudeGenerator->setSingleStep(this->generator->getStepAmplitude(this->generator->getFunction(),
                                                                                                 "VR"));
            ui->doubleSpinBoxAmplitudeGenerator->setDecimals(this->generator->getDecimalsAmplitude(this->generator->getFunction(),
                                                                                                   "VR"));
        }
        ui->doubleSpinBoxAmplitudeGenerator->setValue(this->generator->getAmplitude("VR"));
    } else {
        ui->labelAmplitudeGenerator->hide();
        ui->doubleSpinBoxAmplitudeGenerator->hide();
    }

    if (this->generator->workWithOffset()) {
        ui->labelOffsetGenerator->show();
        ui->doubleSpinBoxOffsetGenerator->show();
        ui->doubleSpinBoxOffsetGenerator->setMinimum(this->generator->getMinOffset());
        ui->doubleSpinBoxOffsetGenerator->setMaximum(this->generator->getMaxOffset());
        ui->doubleSpinBoxOffsetGenerator->setSingleStep(this->generator->getStepOffset());
        ui->doubleSpinBoxOffsetGenerator->setDecimals(this->generator->getDecimalsOffset());
        ui->doubleSpinBoxOffsetGenerator->setValue(this->generator->getOffset());
    } else {
        ui->labelOffsetGenerator->hide();
        ui->doubleSpinBoxOffsetGenerator->hide();
    }

    if (this->generator->workWithFrequency()) {
        ui->labelFrequencyFromGenerator->show();
        ui->doubleSpinBoxFrequencyFromGenerator->show();
        ui->doubleSpinBoxFrequencyFromGenerator->setMaximum(this->generator->getMaxFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyFromGenerator->setMinimum(this->generator->getMinFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyFromGenerator->setSingleStep(this->generator->getStepFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyFromGenerator->setDecimals(this->generator->getDecimalsFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyFromGenerator->setValue(this->generator->getFrequency());

        ui->labelFrequencyToGenerator->show();
        ui->doubleSpinBoxFrequencyToGenerator->show();
        ui->doubleSpinBoxFrequencyToGenerator->setMaximum(this->generator->getMaxFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyToGenerator->setMinimum(this->generator->getMinFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyToGenerator->setSingleStep(this->generator->getStepFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyToGenerator->setDecimals(this->generator->getDecimalsFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyToGenerator->setValue(this->generator->getFrequency());

        ui->labelFrequencyFromGenerator->show();
        ui->doubleSpinBoxFrequencyStepGenerator->show();
        ui->doubleSpinBoxFrequencyStepGenerator->setMaximum(this->generator->getMaxFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyStepGenerator->setMinimum(-this->generator->getMaxFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyStepGenerator->setSingleStep(this->generator->getStepFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyStepGenerator->setDecimals(this->generator->getDecimalsFrequency(this->generator->getFunction()));
        ui->doubleSpinBoxFrequencyStepGenerator->setValue(100);
    } else {
        ui->labelFrequencyFromGenerator->hide();
        ui->doubleSpinBoxFrequencyFromGenerator->hide();

        ui->labelFrequencyToGenerator->hide();
        ui->doubleSpinBoxFrequencyToGenerator->hide();

        ui->labelFrequencyFromGenerator->hide();
        ui->doubleSpinBoxFrequencyStepGenerator->hide();
    }

    this->listLoad = false;

    return;
}

void Amarok::setFrequency(const double &new_frequency, bool confirm) const
{
    this->generator->setFrequency(new_frequency);

    if (!confirm)
        return;

    for (int i = 0; i < this->generatorSendTimes; i ++) {
        if (_abs(new_frequency - this->generator->getFrequency()) < this->eps)
            return;

        this->generator->setFrequency(new_frequency);
        QTest::qWait(100);
    }
}

void Amarok::setFrequency(const double &new_frequency) const
{
    return setFrequency(new_frequency, this->generatorPoll);
}

// Experiment

bool Amarok::inRange(const double &min, const double &max, const double &value) const
{
    return (_min(min, max) <= value &&
            value <= _max(min, max));
}

void Amarok::changeCurrentExperimentSettings()
{
    if (this->generatorActive) {
        this->from = ui->doubleSpinBoxFrequencyFromGenerator->value();
        this->to   = ui->doubleSpinBoxFrequencyToGenerator->value();
        if (((this->from < this->to) && (ui->doubleSpinBoxFrequencyStepGenerator->value() > 0)) ||
            ((this->from > this->to) && (ui->doubleSpinBoxFrequencyStepGenerator->value() < 0)))
            this->step = ui->doubleSpinBoxFrequencyStepGenerator->value();
        else
            this->step = -ui->doubleSpinBoxFrequencyStepGenerator->value();

        this->generatorPoll = ui->checkBoxGeneratorPoll->isChecked() ? true : false;
    } else {
        this->from = 0;
        this->to   = 0;
        this->step = 0;

        this->generatorPoll = false;
    }

    this->lockInAmplifierAutosettings = ui->checkBoxAutosettingsLockInAmplifier->isChecked() ? true : false;

    this->averageOfPoints = ui->spinBoxAverageOfPoints->value();
    this->waitAfterNewFrequency = ui->spinBoxWait->value();

    this->continuousMode = ui->comboBoxExperimentMode->currentText() == "Continuous";

    return;
}

void Amarok::changeCurrentExperimentSettings(const QString &str)
{
    changeCurrentExperimentSettings();

    return;
}

void Amarok::changeCurrentExperimentSettings(const bool &flag)
{
    changeCurrentExperimentSettings();

    return;
}

void Amarok::exportSettings(const QString &fileName) const
{
    QString File = fileName.trimmed();

    if (File.isEmpty()) {
        File = getFileNameExtension(this->reservePath + "Settings_" + reserveFileName + getFileNameTimePostfix(), "Settings");
    }

    fclose(stdout);
    freopen(File.toStdString().c_str(), "w+", stdout);

    std::cout << "Current experiment settings:" << std::endl;
    std::cout << std::endl;

    if (!this->generatorActive) {
        std::cout << "Generator - not active" << std::endl;
    } else {
        std::cout << "Generator model: " << ui->comboBoxModelGenerator->currentText().toStdString() << std::endl;
        std::cout << "   Offset: " << ui->doubleSpinBoxOffsetGenerator->value() << " V" << std::endl;
        std::cout << "   Amplitude: " << ui->doubleSpinBoxAmplitudeGenerator->value() << " Vrms" << std::endl;
        std::cout << "   Frequency from: " << ui->doubleSpinBoxFrequencyFromGenerator->value() << " Hz" << std::endl;
        std::cout << "   Frequency to:   " << ui->doubleSpinBoxFrequencyToGenerator->value() << " Hz" << std::endl;
        std::cout << "   Frequency step: " << ui->doubleSpinBoxFrequencyStepGenerator->value() << " Hz" << std::endl;
        if (this->generator->workWithOutputImpedance()) {
            std::cout << "  Output impedance: " << this->generator->getOutputImpedance().toStdString() << std::endl;
        }
        if (ui->checkBoxGeneratorPoll->isChecked())
            std::cout << "GENERATOR POLLING IS ON" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Lock-in amplifier model: " << ui->comboBoxModelLockInAmplifier->currentText().toStdString() << std::endl;
    std::cout << "   Time constant: " << ui->comboBoxTimeConstantLockInAmplifier->currentText().toStdString() << std::endl;
    if (this->lockInAmplifier->workWithVoltageInputRange())
        std::cout << "   Input range:   " << ui->comboBoxInputRangeLockInAmplifier->currentText().toStdString() <<  std::endl;
    std::cout << "   Sensivity:     " << ui->comboBoxSensivityLockInAmplifier->currentText().toStdString() <<  std::endl;
    if (ui->checkBoxAutosettingsLockInAmplifier->isChecked())
        std::cout << "   LOCK-IN AMPLIFIER AUTOSETTINGS CHECKED" << std::endl;

    std::cout << std::endl;
    std::cout << "Experiment:" << std::endl;
    std::cout << "   Mode: " << ui->comboBoxExperimentMode->currentText().toStdString() << std::endl;
    if (ui->comboBoxExperimentMode->currentText() == "Continuous")
        std::cout << "       Last round was " << ui->lcdNumber->value() << std::endl;
    std::cout << "   Average of " << ui->spinBoxAverageOfPoints->value() << " points" << std::endl;
    std::cout << "   For new point wait for " << ui->spinBoxWait->value() << " ms" << std::endl;

    fclose(stdout);

    return;
}

QString Amarok::getFileNamePathPrefix(const QString &str) const
{
    return this->absolutePath + str;
}

QString Amarok::getFileNameTimePostfix(const QString &str) const
{
    QString ans = str;
    if (!ans.isEmpty())
        ans += "_";
    return ans + QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
}

QString Amarok::getFileNameExtension(const QString &str, const QString &mode) const
{
    QString ans = str;

    if (mode == "SimpleExperiment")
        ans += ".dat";
    if (mode == "Settings")
        ans += ".set";
    if (mode == "KineticForm")
        ans += ".dat";
    if (mode == "KineticTheta")
        ans += ".dat";
    if (mode == "KineticFrequency")
        ans += ".kin";

    return ans;
}

void Amarok::timerStart()
{
    this->pause = false;
    this->stop = false;

    this->replotTimer.start(this->plotUpdateTime);

    return;
}

void Amarok::timerPause()
{
    if (this->pause)
        this->replotTimer.start(this->plotUpdateTime);
    else
        this->replotTimer.stop();

    this->pause = !this->pause;
    this->stop = false;

    return;
}

void Amarok::timerStop()
{
    this->pause = true;
    this->stop = true;

    this->replotTimer.stop();

    return;
}

void Amarok::experimentInit() const
{
    if (!this->generatorActive)
        return;

    this->generator->setOffset(ui->doubleSpinBoxOffsetGenerator->value());
    this->generator->setAmplitude(ui->doubleSpinBoxAmplitudeGenerator->value(), "VR");
//    this->generator->setFrequency(ui->doubleSpinBoxFrequencyFromGenerator->value());

    setFrequency(this->from);

    return;
}

void Amarok::experimentExport(const QString &fileName) const
{
    QString File = fileName.trimmed();

    if (File.isEmpty()) {
        File = getFileNameExtension(this->reservePath + "Settings_" + reserveFileName + getFileNameTimePostfix(), "Settings");
    }

    fclose(stdout);
    freopen(File.toStdString().c_str(), "w+", stdout);

    printf("Fext\tFextSD\tFgen\tR\tRSD\tTheta\tThetaSD\tTime\n");
    for (int i = 0; i < this->simpleExperimentData.getSize(); i++) {
        printf("%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\n",
               this->simpleExperimentData.getFextat(i),
               this->simpleExperimentData.getFextSDat(i),
               this->simpleExperimentData.getFgenat(i),
               this->simpleExperimentData.getRat(i),
               this->simpleExperimentData.getRSDat(i),
               this->simpleExperimentData.getThetaat(i),
               this->simpleExperimentData.getThetaSDat(i),
               this->simpleExperimentData.getTimeat(i));
    }
    fclose(stdout);

    return;
}

SimpleExperimentPoint Amarok::fillPoint(const double &Fgen, const int &average)
{
    SimpleExperimentPoint ans;

    ans.Time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - this->startTime;
    ans.Fext = 0;
    ans.FextSD = 0;
    ans.Fgen = Fgen;
    ans.R = 0;
    ans.RSD = 0;
    ans.Theta = 0;
    ans.ThetaSD = 0;

    getSDAll(ans.R, ans.RSD, ans.Theta, ans.ThetaSD, ans.Fext, ans.FextSD, average);

    return ans;
}

void Amarok::addNewPoint(const double &Fgen)
{
    SimpleExperimentPoint point = fillPoint(Fgen, this->averageOfPoints);

    ui->lineEditCurrentReadingsFrequencyGenerator->setText(QString::number(Fgen));
    ui->lineEditCurrentReadingsRLockInAmplifier->setText(QString::number(point.R, 'g', 6) + " +- " +
                                                         QString::number(100*point.RSD/point.R, 'g', 2) + "%");
    ui->lineEditCurrentReadingsThetaLockInAmplifier->setText(QString::number(point.Theta, 'g', 6) + " +- " +
                                                             QString::number(100*point.ThetaSD/point.Theta, 'g', 2) + "%");
    ui->lineEditCurrentReadingsExternalFrequencyLockInAmplifier->setText(QString::number(point.Fext, 'g', 6) + " +- " +
                                                                         QString::number(100*point.FextSD/point.Fext, 'g', 2) + "%");

    QTime run = QTime(0, 0, 0, 0).addMSecs(point.Time*1000);
    ui->lineEditTimePassed->setText(run.toString("hh:mm:ss.z"));

    this->simpleExperimentData.push_back(point);

    printf("%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\t%0.20e\n",
           point.Fext,
           point.FextSD,
           point.Fgen,
           point.R,
           point.RSD,
           point.Theta,
           point.ThetaSD,
           point.Time);

    pushPointOnPlot(point);

    return;
}

void Amarok::simpleExperimentRun(const QString &runtimeExportDir, const QString &runtimeExportFile)
{
    ui->plot_1->graph(0)->clearData();
    ui->plot_2->graph(0)->clearData();

    simpleExperimentData.clear();

    setBoundaries();
    updatePlot();

    changeCurrentExperimentSettings();

    QString Dir = "";
    QString File = "";

    if (runtimeExportDir.trimmed().isEmpty()) {
        Dir = this->reservePath;
    } else {
        Dir = runtimeExportDir;
    }

    if (runtimeExportFile.trimmed().isEmpty()) {
        File = this->reserveFileName;
    } else {
        File = runtimeExportFile;
    }

    QString Time = getFileNameTimePostfix();
    exportSettings(Dir + "Settings_" + getFileNameExtension(File + "_" + Time, "Settings"));

    QString fullFile = Dir + getFileNameExtension(File + "_" + Time, "SimpleExperiment");
    freopen(fullFile.toStdString().c_str(), "w", stdout);
    printf("Fext\tFextSD\tFgen\tR\tRSD\tTheta\tThetaSD\tTime\n");

    setbuf(stdout, NULL); // DISABLE BUFERING

    ui->lineEditRangeXmin_1->setText(QString::number(0));
    ui->lineEditRangeXmax_1->setText(QString::number(0));
    ui->lineEditRangeYmin_1->setText(QString::number(0));
    ui->lineEditRangeYmax_1->setText(QString::number(0));

    ui->lineEditRangeXmin_2->setText(QString::number(0));
    ui->lineEditRangeXmax_2->setText(QString::number(0));
    ui->lineEditRangeYmin_2->setText(QString::number(0));
    ui->lineEditRangeYmax_2->setText(QString::number(0));

    if (this->generatorActive) {
        ui->progressBarExperiment->setMinimum(_min(this->from, this->to));
        ui->progressBarExperiment->setMaximum(_max(this->from, this->to));
    } else {
        ui->progressBarExperiment->setMinimum(0);
        ui->progressBarExperiment->setMaximum(0);
    }
    ui->lcdNumber->display(0);

    double Fgen = 0;

    do {
        ui->lcdNumber->display(ui->lcdNumber->value() + 1);
        ui->progressBarExperiment->setValue(0);

        experimentInit();
        QTest::qWait(this->waitBeforeExperiment);

        for (int point = 0; point < this->invalidPoints; point++) {
            getR();
            QTest::qWait(this->waitAfterNewFrequency);
        }

        Fgen = this->from;
        while (inRange(this->from, this->to, Fgen)) {
            if (this->generatorActive) {
                setFrequency(Fgen);
                QTest::qWait(this->waitAfterNewFrequency);
            } else {
                QTest::qWait(this->minWait);
            }

            addNewPoint(Fgen);

            if (this->generatorActive) {
                if (this->from < this->to) {
                    ui->progressBarExperiment->setValue(Fgen);
                } else {
                    ui->progressBarExperiment->setValue(this->to - Fgen + this->from);
                }
            }

            while (this->pause) {
                QTest::qWait(this->minWait);
                if (this->stop) {
                    fclose(stdout);

                    return;
                }
            }

            Fgen += this->step;
        }
    } while (this->continuousMode);

    fclose(stdout);

    return;
}

void Amarok::kineticPreScanRun()
{
    timerStart();

    ui->doubleSpinBoxResonanceFrequency->setValue(0);

    simpleExperimentRun(this->currentKineticPath, "PreScan");

    timerStop();

    ui->comboBoxJumpAfterExtremum->setCurrentIndex(1 - ui->comboBoxJumpAfterExtremum->currentIndex());
    ui->comboBoxJumpAfterExtremum->setCurrentIndex(1 - ui->comboBoxJumpAfterExtremum->currentIndex());

    return;
}

KineticRoundReturn Amarok::kineticRoundRun(const QString &fileName)
{
    KineticRoundReturn ans;
    KineticThetaPoint point;

    kineticThetaData.clear();

    ans.Time = -1;

    ui->plot_1->graph(0)->clearData();
    ui->plot_1->replot();

    fclose(stdout);
    freopen(fileName.toStdString().c_str(), "w+", stdout);

    printf("Time\tTheta\n");

    this->roundRun = true;

    int pointsAfterExtremum = 0;

    int pointNumber = 0;

    this->newRoundTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    for (int point = 0; point < this->invalidPoints; point++)
        getTheta();

    while (this->roundRun) {

        point.Theta = getTheta();
        point.Time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - this->newRoundTime;

        kineticThetaData.push_back(point);
        pushPointOnPlot(point);
        updatePlot(1);

        if (ans.Time == -1) {
            ans.Time = point.Time;
            ans.ThetaPeak = point.Theta;
        }

        ui->lineEditCurrentReadingsThetaLockInAmplifier->setText(QString::number(point.Theta));
        ui->lineEditTimePassed->setText(QString::number(this->allTime.elapsed()/1000));

        printf("%20e\t%20e\n", point.Time, point.Theta);
//        qDebug() << "Time = " << point.Time;

//*
//        if (pointNumber > this->minNormalPointNumber &&
//            _max(this->thetaExtremumDepth, _abs(this->thetaLine-point.Theta)) /
//            _min(this->thetaExtremumDepth, _abs(this->thetaLine-point.Theta)) >
//            this->thetaMaxDeviation) {
            if (ui->comboBoxJumpAfterExtremum->currentText() == "Min") {
                if (point.Theta < ans.ThetaPeak) {
                    ans.ThetaPeak = point.Theta;
                    ans.Time = point.Time;
                    pointsAfterExtremum = -1;
                }
            } else {
                if (point.Theta > ans.ThetaPeak) {
                    ans.ThetaPeak = point.Theta;
                    ans.Time = point.Time;
                    pointsAfterExtremum = -1;
                }
            }
//        }
//*/

        pointsAfterExtremum++;

        if (ui->checkBoxAutoKinetics->isChecked())
            if (this->kineticRound != 1)
                if (pointsAfterExtremum > ui->spinBoxJumpAfterExtremum->value())
                    on_pushButtonStart_clicked();

        pointNumber++;
        if (pointNumber % this->maxPointForReplot == 0) {
            QTest::qWait(this->minWait);
        }

        if (!this->kineticRun) {
            fclose(stdout);

            qDebug() << "Round " << this->kineticRound << ", Theta peak = " << ans.ThetaPeak << ", Time = " << ans.Time;

            return ans;
        }
    }

    return ans;
}

void Amarok::kineticExperimentRun()
{
    if (this->kineticTask == "PreScan") {

        QString timeString = getFileNameTimePostfix();
        this->currentKineticPath = this->kineticPath + ui->lineEditFileHeader->text() + "_" + timeString + "\\";

        QDir kineticDataFolder = QDir(this->currentKineticPath);
        if (!kineticDataFolder.exists())
            kineticDataFolder.mkpath(".");

        fillAxisesLabels("PreScan");

        kineticPreScanRun();

        return;
    }

    if (this->kineticTask == "Kinetic") {

        setBoundaries(0);

        kineticFrequencyData.clear();

        fillAxisesLabels("Kinetic");

        QString File = this->currentKineticPath + "Kinetics.kin";

        fclose(stderr);
        freopen(File.toStdString().c_str(), "w+", stderr);

        std::ios_base::sync_with_stdio(0); // ACCELERATING OUTPUT
        std::cerr << std::fixed << std::setprecision(20);
        std::cerr << "Time\tFrequencyExternal\tFrequencyGenerator\tThetaPeak\n";

        ui->plot_1->graph(0)->clearData();
        ui->plot_1->replot();
        ui->plot_2->graph(0)->clearData();
        ui->plot_2->replot();

        this->kineticRound = 0;

        setFrequency(ui->doubleSpinBoxResonanceFrequency->value());
        QTest::qWait(ui->spinBoxWait->value());
        this->thetaExtremumDepth = _abs(getTheta()-this->thetaLine);
        QTest::qWait(ui->spinBoxWait->value());

        int sign;
        ui->comboBoxDFresDT->currentText() == " > 0" ? sign = 1 : sign = -1;

        double jump = ui->doubleSpinBoxJump->value();
        double jumpDevider = 1;

        double Fgen = ui->doubleSpinBoxResonanceFrequency->value() + sign*jump;
        setFrequency(Fgen);

        KineticRoundReturn ans;
        KineticFrequencyPoint point;

        this->startTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

        this->allTime.start();

        while (this->kineticRun) {
            this->kineticRound++;

            ui->lcdNumber->display(this->kineticRound);

            if (this->kineticRound != 1) {
                jumpDevider *= ui->doubleSpinBoxJumpDevider->value();
                Fgen += sign*(jump / (jumpDevider));
                setFrequency(Fgen);
            }

//            qDebug() << "Fgen = " << Fgen;
//            qDebug() << "jumpDevider = " << jumpDevider;
//            qDebug() << "jump = " << sign*(jump / (jumpDevider));
//            qDebug() << "Round = " << this->kineticRound;

            QTest::qWait(this->waitAfterNewFrequencyKinetic);
            ui->lineEditCurrentReadingsFrequencyGenerator->setText(QString::number(Fgen));

            ans = kineticRoundRun(this->currentKineticPath + "Kinetic_Round_" + QString::number(this->kineticRound) + ".dat");
            ans.Fext = getFext();

            point.Fext = ans.Fext;
            point.Time = ans.Time + this->newRoundTime - this->startTime;

//            if (this->kineticRound == 1) {
//                fixTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 - this->startTime;
//            } else {
//                point.Time += fixTime;
//            }

//            qDebug() << "Ans.Time = " << ans.Time << ", Point.Time = " << point.Time << ", Round - Start = " << this->newRoundTime - this->startTime;

//            if (this->kineticRound == 1) // MAGIC!!!
//                point.Time -= 10;

            kineticFrequencyData.push_back(point);
            pushPointOnPlot(point);
            updatePlot(2);

            ui->lineEditCurrentReadingsExternalFrequencyLockInAmplifier->setText(QString::number(ans.Fext));

            std::cerr << point.Time << "\t" << ans.Fext << "\t" << Fgen << "\t" << ans.ThetaPeak << "\n";
        }
    }

    return;
}

// Buttons react

void Amarok::on_comboBoxExperimentMode_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Single") {
        this->kineticTask = "";

        ui->progressBarExperiment->show();
        ui->lcdNumber->hide();

        ui->labelExperimentAverageOfPoints->show();
        ui->spinBoxAverageOfPoints->show();

        ui->pushButtonExport->setEnabled(true);

        fillAxisesLabels("Normal");

        showAxises();
        hideExperimentKinetics();

        if (this->generator->isActive())
            showGenerator();

        ui->pushButtonStart->setText("Start");
        ui->pushButtonPause->setText("Pause");
        ui->pushButtonStop->setText("Stop");

        return;
    }

    if (arg1 == "Continuous") {
        this->kineticTask = "";

        ui->progressBarExperiment->show();
        ui->lcdNumber->show();

        ui->labelExperimentAverageOfPoints->show();
        ui->spinBoxAverageOfPoints->show();

        ui->pushButtonExport->setEnabled(true);

        fillAxisesLabels("Normal");

        showAxises();
        hideExperimentKinetics();

        if (this->generator->isActive())
            showGenerator();

        ui->pushButtonStart->setText("Start");
        ui->pushButtonPause->setText("Pause");
        ui->pushButtonStop->setText("Stop");

        return;
    }

    if (arg1 == "Kinetic") {
        if (!this->generator->isActive()) {
            ui->comboBoxExperimentMode->setCurrentText("Single");

            return;
        }

//        ui->progressBarExperiment->hide();
        ui->lcdNumber->show();

        ui->labelExperimentAverageOfPoints->hide();
        ui->spinBoxAverageOfPoints->hide();

        ui->pushButtonExport->setEnabled(false);

        hideAxises();
        showExperimentKinetics();

        if (this->generator->isActive())
            showGenerator();

        ui->pushButtonStart->setText("PreScan");
        ui->pushButtonPause->setText("Pause");
        ui->pushButtonStop->setText("Stop");

        ui->plot_1->graph(0)->clearData();
        ui->plot_1->replot();
        ui->plot_2->graph(0)->clearData();
        ui->plot_2->replot();

        return;
    }

    return;
}

void Amarok::on_comboBoxReplot_1_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Auto") {
        ui->pushButtonRangeManualReplot_1->hide();

        ui->labelRangeYmax_1->hide();
        ui->lineEditRangeYmax_1->hide();

        ui->labelRangeYmin_1->hide();
        ui->lineEditRangeYmin_1->hide();

        ui->labelRangeXmin_1->hide();
        ui->lineEditRangeXmin_1->hide();

        ui->labelRangeXmax_1->hide();
        ui->lineEditRangeXmax_1->hide();
    } else {
        ui->pushButtonRangeManualReplot_1->show();

        ui->labelRangeYmax_1->show();
        ui->lineEditRangeYmax_1->show();

        ui->labelRangeYmin_1->show();
        ui->lineEditRangeYmin_1->show();

        ui->labelRangeXmin_1->show();
        ui->lineEditRangeXmin_1->show();

        ui->labelRangeXmax_1->show();
        ui->lineEditRangeXmax_1->show();
    }

    return;
}

void Amarok::on_comboBoxReplot_2_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Auto") {
        ui->pushButtonRangeManualReplot_2->hide();

        ui->labelRangeYmax_2->hide();
        ui->lineEditRangeYmax_2->hide();

        ui->labelRangeYmin_2->hide();
        ui->lineEditRangeYmin_2->hide();

        ui->labelRangeXmin_2->hide();
        ui->lineEditRangeXmin_2->hide();

        ui->labelRangeXmax_2->hide();
        ui->lineEditRangeXmax_2->hide();
    } else {
        ui->pushButtonRangeManualReplot_2->show();

        ui->labelRangeYmax_2->show();
        ui->lineEditRangeYmax_2->show();

        ui->labelRangeYmin_2->show();
        ui->lineEditRangeYmin_2->show();

        ui->labelRangeXmin_2->show();
        ui->lineEditRangeXmin_2->show();

        ui->labelRangeXmax_2->show();
        ui->lineEditRangeXmax_2->show();
    }

    return;
}

void Amarok::on_comboBoxXAxisValue_1_currentTextChanged(const QString &arg1)
{
    updateAxisesLabels(1);
    replotPlot(1);

    return;
}

void Amarok::on_comboBoxYAxisValue_1_currentTextChanged(const QString &arg1)
{
    updateAxisesLabels(1);
    replotPlot(1);

    return;
}

void Amarok::on_comboBoxXAxisValue_2_currentTextChanged(const QString &arg1)
{
    updateAxisesLabels(2);
    replotPlot(2);

    return;
}

void Amarok::on_comboBoxYAxisValue_2_currentTextChanged(const QString &arg1)
{
    updateAxisesLabels(2);
    replotPlot(2);

    return;
}

void Amarok::on_pushButtonRangeManualReplot_1_clicked()
{
    updatePlot(1);

    return;
}

void Amarok::on_pushButtonRangeManualReplot_2_clicked()
{
    updatePlot(2);

    return;
}

void Amarok::on_comboBoxTimeConstantLockInAmplifier_currentTextChanged(const QString &arg1)
{
    if (!this->listLoad)
        this->lockInAmplifier->setTimeConstant(arg1);

    return;
}

void Amarok::on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1)
{
    if (!this->listLoad)
        this->lockInAmplifier->setVoltageInputRange(arg1);

    return;
}

void Amarok::on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1)
{
    if (!this->listLoad)
        this->lockInAmplifier->setSensivity(arg1);

    return;
}

void Amarok::on_pushButtonExport_clicked()
{
    QString timeString = getFileNameTimePostfix();
    exportSettings(this->dataPath + "Settings_" + ui->lineEditFileHeader->text() + "_" + timeString + ".set");
    experimentExport(this->dataPath + ui->lineEditFileHeader->text() + "_" + timeString + ".dat");

    return;
}

void Amarok::on_pushButtonStart_clicked()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    if (ui->comboBoxExperimentMode->currentText() == "Single" ||
        ui->comboBoxExperimentMode->currentText() == "Continuous") {

        changeCurrentExperimentSettings();

        double timeToFinish = this->waitBeforeExperiment +
                (_round(_abs(this->to - this->from)/this->step) +
                 1 + this->invalidPoints) *
                (this->generator->getAverageInputTime() +
                 this->waitAfterNewFrequency +
                 this->averageOfPoints *
                 (this->lockInAmplifier->getAverageInputTime() +
                  this->lockInAmplifier->getAverageOutputTime()));

        QTime run = QTime(0, 0, 0, 0).addMSecs(timeToFinish);

        ui->lineEditTimeToRun->setText(run.toString("hh:mm:ss.z"));

        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonPause->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);

        this->simpleExperimentData.clear();

        this->allTime = QTime::currentTime();
        this->allTime.start();

        this->startTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

        timerStart();

        simpleExperimentRun();

        on_pushButtonStop_clicked();

        return;
    }

    if (ui->comboBoxExperimentMode->currentText() == "Kinetic") {

        ui->lineEditTimeToRun->setText("Inavalible for this mode");

        if (this->kineticTask.isEmpty()) {
            ui->plot_1->graph(0)->clearData();
            ui->plot_1->replot();
            ui->plot_2->graph(0)->clearData();
            ui->plot_2->replot();

            this->kineticTask = "PreScan";

            ui->pushButtonStart->setEnabled(false);
            ui->pushButtonPause->setEnabled(false);
            ui->pushButtonStop->setEnabled(true);

            kineticExperimentRun();

            ui->pushButtonStart->setEnabled(true);

            ui->pushButtonStart->setText("Start Kin");

            return;
        }

        if (this->kineticTask == "PreScan") {
            ui->progressBarExperiment->hide();

            this->kineticTask = "Kinetic";

            this->kineticRun = true;

            ui->pushButtonStart->setEnabled(true);
            ui->pushButtonPause->setEnabled(false);
            ui->pushButtonStop->setEnabled(true);

            ui->pushButtonStart->setText("Next");
            ui->pushButtonStop->setText("Halt");

            kineticExperimentRun();

            return;
        }

        if (this->kineticTask == "Kinetic") {
            this->roundRun = false;

            return;
        }

        return;
    }

    return;
}

void Amarok::on_pushButtonPause_clicked()
{
    if (ui->comboBoxExperimentMode->currentText() == "Single" ||
        ui->comboBoxExperimentMode->currentText() == "Continuous") {

        if (!ui->pushButtonPause->isEnabled())
            return;

        if (ui->pushButtonPause->text() == "Pause") {
            ui->pushButtonPause->setText("Continue");
        } else {
            ui->pushButtonPause->setText("Pause");
        }

        timerPause();

        return;
    }

    if (ui->comboBoxExperimentMode->currentText() == "Kinetic") {
        if (this->kineticTask == "PreScan") {
            if (ui->pushButtonPause->text() == "Pause") {
                ui->pushButtonPause->setText("Continue");
            } else {
                ui->pushButtonPause->setText("Pause");
            }

            timerPause();

            return;
        }

        return;
    }

    return;
}

void Amarok::on_pushButtonStop_clicked()
{
    this->setAttribute(Qt::WA_DeleteOnClose, false);

    if (ui->comboBoxExperimentMode->currentText() == "Single" ||
        ui->comboBoxExperimentMode->currentText() == "Continuous") {

        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonPause->setEnabled(false);
        ui->pushButtonStop->setEnabled(false);

        timerStop();

        return;
    }

    if (ui->comboBoxExperimentMode->currentText() == "Kinetic") {
        if (this->kineticTask == "PreScan") {
            timerStop();

            this->kineticTask = "";

            ui->pushButtonStart->setEnabled(true);
            ui->pushButtonPause->setEnabled(false);
            ui->pushButtonStop->setEnabled(false);

            ui->pushButtonStart->setText("PreScan");
            ui->pushButtonPause->setEnabled("Pause");
            ui->pushButtonStop->setEnabled("Stop");

            ui->comboBoxExperimentMode->setCurrentText("Single");
            ui->comboBoxExperimentMode->setCurrentText("Kinetic");

            return;
        }
        if (this->kineticTask == "Kinetic") {
            this->kineticRun = false;

            this->kineticTask = "";

            ui->pushButtonStart->setEnabled(true);
            ui->pushButtonPause->setEnabled(false);
            ui->pushButtonStop->setEnabled(false);

            ui->pushButtonStart->setText("PreScan");
            ui->pushButtonPause->setEnabled("Pause");
            ui->pushButtonStop->setEnabled("Stop");

//            ui->comboBoxExperimentMode->setCurrentText("Single");
//            ui->comboBoxExperimentMode->setCurrentText("Kinetic");

            return;
        }

        return;
    }

    return;
}

void Amarok::on_comboBoxModelLockInAmplifier_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    ui->groupBoxLockInAmplifier->setTitle("Lock-In Amplifier");

    if (this->lockInAmplifier->isActive())
        this->lockInAmplifier->disconnect();

    hideLockInAmplifier();
    hideGraph();
    hideAxises();
    hideCurrentReadingsAndTiming();
    hideExperimentMainSettings();
    hideExperimentMode();
    if (!this->generator->isActive())
        hideTest();

    if (arg1 == "SET IT") {
        this->listLoad = true;

        if (this->lockInAmplifier->isActive())
            this->lockInAmplifier->disconnect();

        ui->comboBoxSerialPortLockInAmplifier->clear();
        ui->comboBoxSerialPortLockInAmplifier->addItems(this->allCom);
        ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");

        this->listLoad = false;

        return;
    }

    if (arg1 == "AUTO") {
        if (ui->comboBoxSerialPortLockInAmplifier->currentText() == "SET IT")
            return;

        if (ui->comboBoxSerialPortLockInAmplifier->currentText() == "AUTO") {
            this->listLoad = true;

            ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");

            this->listLoad = false;

            return;
        }

        this->listLoad = true;

        this->lockInAmplifier->autoSetLockInAmplifierType(ui->comboBoxSerialPortLockInAmplifier->currentText());
        ui->comboBoxModelLockInAmplifier->setCurrentText(this->lockInAmplifier->getLockInAmplifierType());

        this->listLoad = false;

        if (this->lockInAmplifier->isActive()) {
            ui->groupBoxLockInAmplifier->setTitle("Connected");
            fillLockInAmplifierButtons();

            showLockInAmplifier();
            showGraph();
            showAxises();
            showCurrentReadingsAndTiming();
            showExperimentMainSettings();
            showTest();
            showExperimentMode();
        }

    } else {
        this->listLoad = true;

        ui->comboBoxSerialPortLockInAmplifier->clear();

        foreach (const QString &com, this->allCom) {
            if (com != "AUTO" && com != "SET IT") {
                this->lockInAmplifier->autoSetLockInAmplifierType(com);
                if (this->lockInAmplifier->getLockInAmplifierType() == arg1) {
                    ui->comboBoxSerialPortLockInAmplifier->addItem(com);
                    this->lockInAmplifier->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortLockInAmplifier->addItem("SET IT");
        ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");

        this->listLoad = false;
    }

    return;
}

void Amarok::on_comboBoxSerialPortLockInAmplifier_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    ui->groupBoxLockInAmplifier->setTitle("Lock-In Amplifier");

    if (this->lockInAmplifier->isActive())
        this->lockInAmplifier->disconnect();

    hideLockInAmplifier();
    hideGraph();
    hideAxises();
    hideCurrentReadingsAndTiming();
    hideExperimentMainSettings();
    hideExperimentMode();
    if (!this->generator->isActive())
        hideTest();

    if (arg1 == "SET IT") {
        if (this->lockInAmplifier->isActive())
            this->lockInAmplifier->disconnect();
        return;
    }

    if (arg1 == "AUTO") {
        if (ui->comboBoxModelLockInAmplifier->currentText() == "SET IT")
            return;

        if (ui->comboBoxModelLockInAmplifier->currentText() == "AUTO") {
            this->listLoad = true;

            ui->comboBoxModelLockInAmplifier->setCurrentText("SET IT");

            this->listLoad = false;

            return;
        }

        this->listLoad = true;

        ui->comboBoxSerialPortLockInAmplifier->clear();

        foreach (const QString &com, this->allCom) {
            if (com != "AUTO" && com != "SET IT") {
                this->lockInAmplifier->autoSetLockInAmplifierType(com);
                if (this->lockInAmplifier->getLockInAmplifierType() == arg1) {
                    ui->comboBoxSerialPortLockInAmplifier->addItem(com);
                    this->lockInAmplifier->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortLockInAmplifier->addItem("SET IT");
        ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");

        this->listLoad = false;
    } else {
        this->listLoad = true;

        this->lockInAmplifier->autoSetLockInAmplifierType(ui->comboBoxSerialPortLockInAmplifier->currentText());
        ui->comboBoxModelLockInAmplifier->setCurrentText(this->lockInAmplifier->getLockInAmplifierType());

        if (!this->lockInAmplifier->isActive())
            this->lockInAmplifier->setConnection(ui->comboBoxSerialPortLockInAmplifier->currentText(),
                                                 19200,
                                                 this->lockInAmplifier->getLockInAmplifierType());

        if (this->lockInAmplifier->isActive()) {
            ui->groupBoxLockInAmplifier->setTitle("Connected");
            fillLockInAmplifierButtons();

            showLockInAmplifier();
            showGraph();
            showAxises();
            showCurrentReadingsAndTiming();
            showExperimentMainSettings();
            showTest();
            showExperimentMode();
        } else {
            ui->comboBoxModelLockInAmplifier->setCurrentText("SET IT");
            ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");
        }

        this->listLoad = false;
    }

    return;
}

void Amarok::on_comboBoxModelGenerator_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    ui->groupBoxGenerator->setTitle("Generator");

    if (this->generator->isActive())
        this->generator->disconnect();

    hideGenerator();
    hideGeneratorPolling();
    if (!this->lockInAmplifier->isActive())
        hideTest();

    if (arg1 == "SET IT") {
        this->listLoad = true;

        if (this->generator->isActive())
            this->generator->disconnect();

        ui->comboBoxSerialPortGenerator->clear();
        ui->comboBoxSerialPortGenerator->addItems(this->allCom);
        ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");

        this->listLoad = false;

        return;
    }

    if (arg1 == "AUTO") {
        if (ui->comboBoxSerialPortGenerator->currentText() == "SET IT")
            return;

        if (ui->comboBoxSerialPortGenerator->currentText() == "AUTO") {
            this->listLoad = true;

            ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");

            this->listLoad = false;

            return;
        }

        this->listLoad = true;

        this->generator->autoSetGeneratorType(ui->comboBoxSerialPortGenerator->currentText());
        ui->comboBoxModelGenerator->setCurrentText(this->generator->getGeneratorType());

        this->listLoad = false;

        if (this->generator->isActive()) {
            ui->groupBoxGenerator->setTitle("Connected");

            this->generatorActive = true;

            fillGeneratorButtons();

            showGenerator();
            showGeneratorPolling();
            showTest();
        }

    } else {
        this->listLoad = true;

        ui->comboBoxSerialPortGenerator->clear();

        foreach (const QString &com, this->allCom) {
            if (com != "AUTO" && com != "SET IT") {
                this->generator->autoSetGeneratorType(com);
                if (this->generator->getGeneratorType() == arg1) {
                    ui->comboBoxSerialPortGenerator->addItem(com);
                    this->generator->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortGenerator->addItem("SET IT");
        ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");

        this->listLoad = false;
    }

    return;
}

void Amarok::on_comboBoxSerialPortGenerator_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    ui->groupBoxGenerator->setTitle("Generator");

    if (this->generator->isActive())
        this->generator->disconnect();

    hideGenerator();
    hideGeneratorPolling();
    if (!this->lockInAmplifier->isActive())
        hideTest();

    if (arg1 == "SET IT") {
        if (this->generator->isActive())
            this->generator->disconnect();
        return;
    }

    if (arg1 == "AUTO") {
        if (ui->comboBoxModelGenerator->currentText() == "SET IT")
            return;

        if (ui->comboBoxModelGenerator->currentText() == "AUTO") {
            this->listLoad = true;

            ui->comboBoxModelGenerator->setCurrentText("SET IT");

            this->listLoad = false;

            return;
        }

        this->listLoad = true;

        ui->comboBoxSerialPortGenerator->clear();

        foreach (const QString &com, this->allCom) {
            if (com != "AUTO" && com != "SET IT") {
                this->generator->autoSetGeneratorType(com);
                if (this->generator->getGeneratorType() == arg1) {
                    ui->comboBoxSerialPortGenerator->addItem(com);
                    this->generator->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortGenerator->addItem("SET IT");
        ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");

        this->listLoad = false;
    } else {
        this->listLoad = true;

        this->generator->autoSetGeneratorType(ui->comboBoxSerialPortGenerator->currentText());
        ui->comboBoxModelGenerator->setCurrentText(this->generator->getGeneratorType());

        if (!this->generator->isActive())
            this->generator->setConnection(ui->comboBoxSerialPortGenerator->currentText(),
                                           9600,
                                           this->generator->getGeneratorType());

        this->listLoad = false;

        if (this->generator->isActive()) {
            ui->groupBoxGenerator->setTitle("Connected");

            this->generatorActive = true;

            fillGeneratorButtons();
            showGenerator();
            showGeneratorPolling();
            showTest();
        } else {
            ui->comboBoxModelGenerator->setCurrentText("SET IT");
            ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");
        }
    }

    return;
}

void Amarok::on_pushButtonTestEquipment_clicked()
{
    ui->pushButtonTestEquipment->setEnabled(false);
    ui->pushButtonTestEquipment->setText("Gen...");

    if (this->generator->isActive()) {
        if (this->generator->test()) {
            ui->groupBoxGenerator->setTitle("Connected; Test passed");
        } else {
            ui->groupBoxGenerator->setTitle("Connected; Test failed");
        }
        this->generator->setDefaultSettings();
    }

    ui->pushButtonTestEquipment->setText("L-I A...");

    if (this->lockInAmplifier->isActive()) {
        if (this->lockInAmplifier->test()) {
            ui->groupBoxLockInAmplifier->setTitle("Connected; Test passed");
        } else {
            ui->groupBoxLockInAmplifier->setTitle("Connected; Test failed");
        }
        this->lockInAmplifier->setDefaultSettings();
    }

    ui->pushButtonTestEquipment->setText("Test");
    ui->pushButtonTestEquipment->setEnabled(true);

    return;
}

void Amarok::on_comboBoxJumpAfterExtremum_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    double minExtremumThetaPoint = 0;
    double maxExtremumThetaPoint = 0;

    for (int point = 0; point < this->simpleExperimentData.getSize(); point++) {
        if (this->simpleExperimentData.getThetaat(point) < this->simpleExperimentData.getThetaat(minExtremumThetaPoint)) {
            minExtremumThetaPoint = point;
        }
        if (this->simpleExperimentData.getThetaat(point) > this->simpleExperimentData.getThetaat(maxExtremumThetaPoint)) {
            maxExtremumThetaPoint = point;
        }
    }


    if (arg1 == "Min") {
        ui->doubleSpinBoxResonanceFrequency->setValue(this->simpleExperimentData.getFextat(minExtremumThetaPoint));
        this->thetaLine = this->simpleExperimentData.getFextat(maxExtremumThetaPoint);
    } else {
        ui->doubleSpinBoxResonanceFrequency->setValue(this->simpleExperimentData.getFextat(maxExtremumThetaPoint));
        this->thetaLine = this->simpleExperimentData.getFextat(minExtremumThetaPoint);
    }
    this->thetaExtremumDepth = this->simpleExperimentData.getFextat(maxExtremumThetaPoint) - this->simpleExperimentData.getFextat(minExtremumThetaPoint);

    return;
}
