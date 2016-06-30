#include "Amarok.h"
#include "ui_Amarok.h"

Amarok::Amarok(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Amarok)
{
    this->constructor = true;

    ui->setupUi(this);

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

    this->experimentMode.clear();
    this->experimentMode.push_back("Single");
    this->experimentMode.push_back("Continuous");
    this->experimentMode.push_back("Kinetics");
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

    initPlot();
    changeCurrentExperimentSettings();

    this->constructor = false;
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
    ui->checkBoxAutosettingsLockInAmplifier->hide();

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
    hideGenerator();
    hideTest();
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
    ui->checkBoxAutosettingsLockInAmplifier->show();

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

void Amarok::replotPlot(const int &i) const
{
    if (this->constructor)
        return;

    if (simpleExperimentData.isEmpty())
        return;

    if (i == 1 || i == 0) {
        ui->plot_1->graph(0)->clearData();

        if (ui->comboBoxXAxisValue_1->currentText() == "Fgen") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(simpleExperimentData.getFgenat(point),
                                                  simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(simpleExperimentData.getFgenat(point),
                                                  simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_1->currentText() == "Fext") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(simpleExperimentData.getFextat(point),
                                                  simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(simpleExperimentData.getFextat(point),
                                                  simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_1->currentText() == "Time") {
            if (ui->comboBoxYAxisValue_1->currentText() == "R") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(simpleExperimentData.getTimeat(point),
                                                  simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_1->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_1->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_1->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_1->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_1->currentText() == "Theta") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_1->graph(0)->addData(simpleExperimentData.getTimeat(point),
                                                  simpleExperimentData.getThetaat(point));

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
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(simpleExperimentData.getFgenat(point),
                                                  simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(simpleExperimentData.getFgenat(point),
                                                  simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFgenMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFgenMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_2->currentText() == "Fext") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(simpleExperimentData.getFextat(point),
                                                  simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(simpleExperimentData.getFextat(point),
                                                  simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getFextMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getFextMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
        if (ui->comboBoxXAxisValue_2->currentText() == "Time") {
            if (ui->comboBoxYAxisValue_2->currentText() == "R") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(simpleExperimentData.getTimeat(point),
                                                  simpleExperimentData.getRat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getRMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getRMax()));
            }
            if (ui->comboBoxYAxisValue_2->currentText() == "Theta") {
                for (int point = 0; point < simpleExperimentData.getSize(); point++)
                    ui->plot_2->graph(0)->addData(simpleExperimentData.getTimeat(point),
                                                  simpleExperimentData.getThetaat(point));

                ui->lineEditRangeXmin_2->setText(QString::number(this->simpleExperimentData.getTimeMin()));
                ui->lineEditRangeXmax_2->setText(QString::number(this->simpleExperimentData.getTimeMax()));

                ui->lineEditRangeYmin_2->setText(QString::number(this->simpleExperimentData.getThetaMin()));
                ui->lineEditRangeYmax_2->setText(QString::number(this->simpleExperimentData.getThetaMax()));
            }
        }
    }

    updatePlot(i);

    return;
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

// Generator

void Amarok::changeGeneratorSettings(const QString &str) const
{
    if (!this->generatorActive)
        return;

    this->generator->setAmplitude(ui->doubleSpinBoxAmplitudeGenerator->value(), "VR");
    this->generator->setOffset(ui->doubleSpinBoxOffsetGenerator->value());

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
    }
}

void Amarok::setFrequency(const double &new_frequency) const
{
    return setFrequency(new_frequency, this->generatorPoll);
}

// Experiment

void Amarok::changeCurrentExperimentSettings()
{
    if (this->generatorActive) {
        this->from = ui->doubleSpinBoxFrequencyFromGenerator->value();
        this->to   = ui->doubleSpinBoxFrequencyToGenerator->value();
        this->step = ui->doubleSpinBoxFrequencyStepGenerator->value();

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


    return;
}

void Amarok::timerPause()
{


    return;
}

void Amarok::timerStop()
{


    return;
}

void Amarok::experimentInit()
{


    return;
}

void Amarok::experimentExport(const QString &fileName)
{


    return;
}

void Amarok::simpleExperimentRun()
{


    return;
}

void Amarok::kineticExperimentRun()
{


    return;
}

// Buttons react

void Amarok::on_comboBoxExperimentMode_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Single") {
        ui->progressBarExperiment->show();
        ui->lcdNumber->hide();

        ui->labelExperimentAverageOfPoints->show();
        ui->spinBoxAverageOfPoints->show();

        ui->pushButtonExport->setEnabled(true);

        updateAxisesLabels();

        showAxises();
        hideExperimentKinetics();
    }

    if (arg1 == "Continuous") {
        ui->progressBarExperiment->show();
        ui->lcdNumber->show();

        ui->labelExperimentAverageOfPoints->show();
        ui->spinBoxAverageOfPoints->show();

        ui->pushButtonExport->setEnabled(true);

        updateAxisesLabels();

        showAxises();
        hideExperimentKinetics();
    }

    if (arg1 == "Kinetics") {
        ui->progressBarExperiment->hide();
        ui->lcdNumber->show();

        ui->labelExperimentAverageOfPoints->hide();
        ui->spinBoxAverageOfPoints->hide();

        ui->pushButtonExport->setEnabled(false);

        ui->plot_1->xAxis->setLabel("Time");
        ui->plot_1->yAxis->setLabel("Theta");
        ui->plot_1->replot();

        ui->plot_2->xAxis->setLabel("Time");
        ui->plot_2->yAxis->setLabel("Frequency");
        ui->plot_2->replot();

        hideAxises();
        showExperimentKinetics();
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

    return;
}

void Amarok::on_comboBoxYAxisValue_1_currentTextChanged(const QString &arg1)
{
    updateAxisesLabels(1);

    return;
}

void Amarok::on_comboBoxXAxisValue_2_currentTextChanged(const QString &arg1)
{
    updateAxisesLabels(2);

    return;
}

void Amarok::on_comboBoxYAxisValue_2_currentTextChanged(const QString &arg1)
{
    updateAxisesLabels(2);

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
    this->lockInAmplifier->setTimeConstant(arg1);

    return;
}

void Amarok::on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1)
{
    this->lockInAmplifier->setVoltageInputRange(arg1);

    return;
}

void Amarok::on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1)
{
    this->lockInAmplifier->setSensivity(arg1);

    return;
}

void Amarok::on_pushButtonExport_clicked()
{


    return;
}

void Amarok::on_pushButtonStart_clicked()
{


    return;
}

void Amarok::on_pushButtonPause_clicked()
{


    return;
}

void Amarok::on_pushButtonStop_clicked()
{


    return;
}

void Amarok::on_comboBoxModelLockInAmplifier_currentTextChanged(const QString &arg1)
{


    return;
}

void Amarok::on_comboBoxSerialPortLockInAmplifier_currentTextChanged(const QString &arg1)
{


    return;
}

void Amarok::on_comboBoxModelGenerator_currentTextChanged(const QString &arg1)
{


    return;
}

void Amarok::on_comboBoxSerialPortGenerator_currentTextChanged(const QString &arg1)
{


    return;
}
