#include "Amarok.h"
#include "ui_Amarok.h"

Amarok::Amarok(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Amarok)
{
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

    initPlot();

    connect(ui->customPlot_1, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(on_plot_1_Clicked(QMouseEvent*)));
    connect(ui->customPlot_2, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(on_plot_2_Clicked(QMouseEvent*)));
    connect(ui->customPlot_3, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(on_plot_3_Clicked(QMouseEvent*)));

    ui->pushButtonActionLockInAmplifier->setText("Connect");
    ui->pushButtonActionGenerator->setText("Connect");

    this->listLoad = true;

    ui->comboBoxExperimentMode->addItem("Single");
    ui->comboBoxExperimentMode->addItem("Continuous");
    ui->comboBoxExperimentMode->addItem("Kinetic");

    ui->comboBoxDFresDT->clear();
    ui->comboBoxDFresDT->addItem(" > 0");
    ui->comboBoxDFresDT->addItem(" < 0");

    ui->comboBoxJumpAfterExtremum->clear();
    ui->comboBoxJumpAfterExtremum->addItem("Min");
    ui->comboBoxJumpAfterExtremum->addItem("Max");
    ui->comboBoxJumpAfterExtremum->setCurrentText("Min");

    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);

    ui->lcdNumber->display(0);
    ui->progressBarExperiment->setValue(0);

    lockInAmplifierShow(false);
    generatorShow(false);

    on_actionCOM_Upd_triggered();

    this->lockInAmplifier = new LockInAmplifier();
    ui->comboBoxModelLockInAmplifier->addItems(this->lockInAmplifier->getSupportedModels());

    this->generator = new Generator();
    ui->comboBoxModelGenerator->addItems(this->generator->getSupportedModels());

    this->listLoad = false;

    ui->comboBoxModelLockInAmplifier->setCurrentIndex(0);
    ui->comboBoxModelLockInAmplifier->setCurrentText("AUTO");
    ui->comboBoxModelGenerator->setCurrentIndex(0);
    ui->comboBoxModelGenerator->setCurrentText("AUTO");

    ui->comboBoxExperimentMode->setCurrentText("Kinetic");
    ui->comboBoxExperimentMode->setCurrentText("Single");

    // ------------------------------------------------ //
    QVector <double> x(101), y2(101), y3(101), y4(101);
    for (int i = 0; i < 101; ++i) {
        x[i] = i/50.0 - 1;
        y2[i] = x[i]*x[i];
        y3[i] = x[i]*x[i]*x[i];
        y4[i] = x[i]*x[i]*x[i]*x[i];
    }
    // create graph and assign data to it:
    ui->customPlot_1->graph(0)->setData(x, y2);
    ui->customPlot_2->graph(0)->setData(x, y3);
    ui->customPlot_3->graph(0)->setData(x, y4);
    // give the axes some labels:
    ui->customPlot_1->xAxis->setLabel("x");
    ui->customPlot_1->yAxis->setLabel("y = x ^ 2");
    ui->customPlot_2->xAxis->setLabel("x");
    ui->customPlot_2->yAxis->setLabel("y = x ^ 3");
    ui->customPlot_3->xAxis->setLabel("x");
    ui->customPlot_3->yAxis->setLabel("y = x ^ 4");
    // set axes ranges, so we see all data:
    ui->customPlot_1->xAxis->setRange(-1, 1);
    ui->customPlot_1->yAxis->setRange(0, 1);
    ui->customPlot_1->replot();
    ui->customPlot_2->xAxis->setRange(-1, 1);
    ui->customPlot_2->yAxis->setRange(-1, 1);
    ui->customPlot_2->replot();
    ui->customPlot_3->xAxis->setRange(-1, 1);
    ui->customPlot_3->yAxis->setRange(0, 1);
    ui->customPlot_3->replot();
    // ------------------------------------------------ //
}

Amarok::~Amarok()
{
    delete ui;
}

void Amarok::initPlot(int i)
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

        ui->customPlot_1->xAxis->setTickLabelType(QCPAxis::ltNumber);
        ui->customPlot_1->axisRect()->setupFullAxesBox();

        ui->customPlot_1->setInteraction(QCP::iRangeZoom, true);
    }
    if (i == 0 || i == 2) {
        ui->customPlot_2->setNotAntialiasedElements(QCP::aeAll);
        ui->customPlot_2->xAxis->setTickLabelFont(font);
        ui->customPlot_2->yAxis->setTickLabelFont(font);
        ui->customPlot_2->legend->setFont(font);

        ui->customPlot_2->clearGraphs();
        ui->customPlot_2->addGraph();

        ui->customPlot_2->xAxis->setTickLabelType(QCPAxis::ltNumber);
        ui->customPlot_2->axisRect()->setupFullAxesBox();

        ui->customPlot_2->setInteraction(QCP::iRangeZoom, true);
    }
    if (i == 0 || i == 3) {
        ui->customPlot_3->setNotAntialiasedElements(QCP::aeAll);
        ui->customPlot_3->xAxis->setTickLabelFont(font);
        ui->customPlot_3->yAxis->setTickLabelFont(font);
        ui->customPlot_3->legend->setFont(font);

        ui->customPlot_3->clearGraphs();
        ui->customPlot_3->addGraph();

        ui->customPlot_3->xAxis->setTickLabelType(QCPAxis::ltNumber);
        ui->customPlot_3->axisRect()->setupFullAxesBox();

        ui->customPlot_3->setInteraction(QCP::iRangeZoom, true);
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

void Amarok::on_plot_3_Clicked(QMouseEvent *event) const
{
    ui->lineEditPointX_3->setText(QString::number(ui->customPlot_3->xAxis->pixelToCoord(event->pos().x())));
    ui->lineEditPointY_3->setText(QString::number(ui->customPlot_3->yAxis->pixelToCoord(event->pos().y())));

    return;
}

void Amarok::on_actionCOM_Upd_triggered()
{
    QStringList allCom;

    allCom.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        allCom.push_back(info.portName());
    allCom.push_back("SET IT");

    this->listLoad = true;

    ui->comboBoxSerialPortLockInAmplifier->clear();
    ui->comboBoxSerialPortLockInAmplifier->addItems(allCom);
    ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");

    ui->comboBoxSerialPortGenerator->clear();
    ui->comboBoxSerialPortGenerator->addItems(allCom);
    ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");

    this->listLoad = false;

    return;
}

void Amarok::lockInAmplifierShow(bool show) const
{
    ui->labelTimeConstantLockInAmplifier->setVisible(show);
    ui->comboBoxTimeConstantLockInAmplifier->setVisible(show);
    ui->pushButtonAutoPresettingsLockInAmplifier->setVisible(show);
    ui->labelInputRangeLockInAmplifier->setVisible(show);
    ui->comboBoxInputRangeLockInAmplifier->setVisible(show);
    ui->labelSensivityLockInAmplifier->setVisible(show);
    ui->comboBoxSensivityLockInAmplifier->setVisible(show);

    return;
}

void Amarok::on_comboBoxModelLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedLockInAmplifier();
}

void Amarok::on_comboBoxSerialPortLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedLockInAmplifier();
}

void Amarok::currentConnectionTextChangedLockInAmplifier() const
{
    if (ui->comboBoxModelLockInAmplifier->currentText() == "AUTO") {
        if (ui->comboBoxSerialPortLockInAmplifier->currentText() == "SET IT") {
            ui->pushButtonActionLockInAmplifier->setText("Fill Smth");
            ui->pushButtonActionLockInAmplifier->setEnabled(false);
        } else {
            ui->pushButtonActionLockInAmplifier->setText("Connect");
            ui->pushButtonActionLockInAmplifier->setEnabled(true);
        }
    } else {
        ui->pushButtonActionLockInAmplifier->setEnabled(true);
        if (ui->comboBoxSerialPortLockInAmplifier->currentText() == "SET IT") {
            ui->pushButtonActionLockInAmplifier->setText("Scan");
        } else {
            ui->pushButtonActionLockInAmplifier->setText("Connect");
        }
    }

    return;
}

void Amarok::on_pushButtonActionLockInAmplifier_clicked()
{
    if (ui->pushButtonActionLockInAmplifier->text() == "Fill Smth") {
        ui->comboBoxModelLockInAmplifier->setEnabled(true);
        ui->comboBoxSerialPortLockInAmplifier->setEnabled(true);
        return;
    }
    if (ui->pushButtonActionLockInAmplifier->text() == "Scan") {
        this->listLoad = true;

        ui->comboBoxSerialPortLockInAmplifier->clear();

        QStringList allCom;
        for (int i = 0; i < ui->comboBoxSerialPortLockInAmplifier->count(); i++)
            allCom.push_back(ui->comboBoxSerialPortLockInAmplifier->itemText(i));

        foreach (const QString &com, allCom) {
            if (com != "SET IT") {
                this->lockInAmplifier->autoSetLockInAmplifierModel(com);
                if (this->lockInAmplifier->getLockInAmplifierModel() == ui->comboBoxModelLockInAmplifier->currentText()) {
                    ui->comboBoxSerialPortLockInAmplifier->addItem(com);
                    this->lockInAmplifier->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortLockInAmplifier->addItem("SET IT");
        ui->comboBoxSerialPortLockInAmplifier->setCurrentText("SET IT");

        this->listLoad = false;

        ui->comboBoxModelLockInAmplifier->setEnabled(true);
        ui->comboBoxSerialPortLockInAmplifier->setEnabled(true);

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

    return;
}

void Amarok::on_comboBoxTimeConstantLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (!this->listLoad)
        this->lockInAmplifier->setTimeConstant(arg1);

    return;
}

void Amarok::on_comboBoxInputRangeLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (!this->listLoad)
        this->lockInAmplifier->setVoltageInputRange(arg1);

    return;
}

void Amarok::on_comboBoxSensivityLockInAmplifier_currentTextChanged(const QString &arg1) const
{
    if (!this->listLoad)
        this->lockInAmplifier->setSensivity(arg1);

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

    return;
}

void Amarok::generatorShow(bool show) const
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

    return;
}

void Amarok::on_comboBoxModelGenerator_currentTextChanged(const QString &arg1) const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedGenerator();
}

void Amarok::on_comboBoxSerialPortGenerator_currentTextChanged(const QString &arg1) const
{
    if (this->listLoad)
        return;

    return currentConnectionTextChangedGenerator();
}

void Amarok::currentConnectionTextChangedGenerator() const
{
    if (ui->comboBoxModelGenerator->currentText() == "AUTO") {
        if (ui->comboBoxSerialPortGenerator->currentText() == "SET IT") {
            ui->pushButtonActionGenerator->setText("Fill Smth");
            ui->pushButtonActionGenerator->setEnabled(false);
        } else {
            ui->pushButtonActionGenerator->setText("Connect");
            ui->pushButtonActionGenerator->setEnabled(true);
        }
    } else {
        ui->pushButtonActionGenerator->setEnabled(true);
        if (ui->comboBoxSerialPortGenerator->currentText() == "SET IT") {
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
        return;
    }
    if (ui->pushButtonActionGenerator->text() == "Scan") {
        this->listLoad = true;

        ui->comboBoxSerialPortGenerator->clear();

        QStringList allCom;
        for (int i = 0; i < ui->comboBoxSerialPortGenerator->count(); i++)
            allCom.push_back(ui->comboBoxSerialPortGenerator->itemText(i));

        foreach (const QString &com, allCom) {
            if (com != "SET IT") {
                this->generator->autoSetGeneratorModel(com);
                if (this->generator->getGeneratorModel() == ui->comboBoxModelGenerator->currentText()) {
                    ui->comboBoxSerialPortGenerator->addItem(com);
                    this->generator->disconnect();
                }
            }
        }

        ui->comboBoxSerialPortGenerator->addItem("SET IT");
        ui->comboBoxSerialPortGenerator->setCurrentText("SET IT");

        this->listLoad = false;

        ui->comboBoxModelGenerator->setEnabled(true);
        ui->comboBoxSerialPortGenerator->setEnabled(true);

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

        ui->comboBoxAmplitudeUnitGenerator->clear();

        return;
    }

    return;
}

void Amarok::fillButtonsGenerator()
{
    this->listLoad = true;

    ui->comboBoxAmplitudeUnitGenerator->clear();
    ui->labelAmplitudeGenerator->setVisible(this->generator->workWithAmplitude());
    ui->comboBoxAmplitudeUnitGenerator->setVisible(this->generator->workWithAmplitude());
    ui->doubleSpinBoxAmplitudeGenerator->setVisible(this->generator->workWithAmplitude());
    if (this->generator->workWithAmplitude()) {
        ui->comboBoxAmplitudeUnitGenerator->addItems(this->generator->getAmplitudeTypeList());
        ui->comboBoxAmplitudeUnitGenerator->setCurrentText("VR");

        ui->doubleSpinBoxAmplitudeGenerator->setMinimum(this->generator->getMinAmplitudeCurrent(ui->comboBoxAmplitudeUnitGenerator->currentText()));
        ui->doubleSpinBoxAmplitudeGenerator->setMaximum(this->generator->getMaxAmplitudeCurrent(ui->comboBoxAmplitudeUnitGenerator->currentText()));
        ui->doubleSpinBoxAmplitudeGenerator->setSingleStep(this->generator->getStepAmplitudeCurrent(ui->comboBoxAmplitudeUnitGenerator->currentText()));
        ui->doubleSpinBoxAmplitudeGenerator->setDecimals(this->generator->getDecimalsAmplitudeCurrent(ui->comboBoxAmplitudeUnitGenerator->currentText()));
        ui->doubleSpinBoxAmplitudeGenerator->setValue(this->generator->getAmplitude(ui->comboBoxAmplitudeUnitGenerator->currentText()));
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

    this->listLoad = false;

    return;
}

void Amarok::on_comboBoxAmplitudeUnitGenerator_currentTextChanged(const QString &arg1)
{
    if (this->listLoad)
        return;

    fillButtonsGenerator();

    return;
}

void Amarok::on_doubleSpinBoxAmplitudeGenerator_valueChanged(double arg1)
{
    if (this->listLoad)
        return;

    this->generator->setAmplitude(arg1, ui->comboBoxAmplitudeUnitGenerator->currentText());

    return;
}

void Amarok::on_doubleSpinBoxOffsetGenerator_valueChanged(double arg1)
{
    if (this->listLoad)
        return;

    this->generator->setOffset(arg1);

    return;
}

void Amarok::on_doubleSpinBoxFrequencyFromGenerator_valueChanged(double arg1)
{
    if (this->listLoad)
        return;

    this->generator->setFrequency(arg1);

    return;
}

void Amarok::on_comboBoxExperimentMode_currentTextChanged(const QString &arg1) const
{
    if (this->listLoad)
        return;

    ui->doubleSpinBoxResonanceFrequency->setEnabled(arg1 == "Kinetic");
    ui->labelResonanceWidth->setVisible(arg1 == "Kinetic");
    ui->labelResonanceWidthValue->setVisible(arg1 == "Kinetic");
    ui->labelSNR->setVisible(arg1 == "Kinetic");
    ui->labelSNRValue->setVisible(arg1 == "Kinetic");
    ui->comboBoxJumpAfterExtremum->setEnabled(arg1 == "Kinetic");
    ui->comboBoxJumpAfterExtremum->setVisible(arg1 == "Kinetic");
    ui->labelDFresDT->setVisible(arg1 == "Kinetic");
    ui->comboBoxDFresDT->setEnabled(arg1 == "Kinetic");
    ui->comboBoxDFresDT->setVisible(arg1 == "Kinetic");
    ui->checkBoxAutoKinetics->setEnabled(arg1 == "Kinetic");
    ui->checkBoxAutoKinetics->setVisible(arg1 == "Kinetic");
    ui->labelJump->setVisible(arg1 == "Kinetic");
    ui->spinBoxJumpAfterExtremum->setEnabled(arg1 == "Kinetic");
    ui->spinBoxJumpAfterExtremum->setVisible(arg1 == "Kinetic");
    ui->doubleSpinBoxJump->setEnabled(arg1 == "Kinetic");
    ui->doubleSpinBoxJump->setVisible(arg1 == "Kinetic");
    ui->labelJumpDevider->setVisible(arg1 == "Kinetic");
    ui->doubleSpinBoxJumpDevider->setEnabled(arg1 == "Kinetic");
    ui->doubleSpinBoxJumpDevider->setVisible(arg1 == "Kinetic");

    if (arg1 == "Kinetics") {
        ui->lcdNumber->show();

    } else {
        if (arg1 == "Single") {
            ui->lcdNumber->hide();

        }
        if (arg1 == "Continuous") {
            ui->lcdNumber->show();

        }
    }

    return;
}
