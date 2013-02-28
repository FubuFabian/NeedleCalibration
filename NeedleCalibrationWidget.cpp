#include "NeedleCalibrationWidget.h"
#include "ui_NeedleCalibrationWidget.h"

NeedleCalibrationWidget::NeedleCalibrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeedleCalibrationWidget)
{
    ui->setupUi(this);
	
	frames = 300;
	delaySeconds = 1;
	ui->framesSlider->setTickInterval(20);
    ui->framesSlider->setRange(300, 1800);

	ui->delaySlider->setTickInterval(1);
    ui->delaySlider->setRange(1, 30);

	needleCalibration = new NeedleCalibration();
}

NeedleCalibrationWidget::~NeedleCalibrationWidget()
{
    delete ui;
}

void NeedleCalibrationWidget::numberOfFrames(int n)
{
	frames = n;
	QString str = QString::number(n);
	ui->frameDisplay->setText(str);
}

void  NeedleCalibrationWidget::delay(int n)
{
	delaySeconds = n;
	QString str = QString::number(n);
	ui->delayDisplay->setText(str);
}

void  NeedleCalibrationWidget::calibrate()
{
	needleCalibration->setDelay(delaySeconds);
	needleCalibration->setNumberOfFrames(frames);
	needleCalibration->setRomFile(romFile);
	needleCalibration->calibrate();

}

void  NeedleCalibrationWidget::save()
{

}

void NeedleCalibrationWidget::setRomFile(std::string romFile)
{
	this->romFile = romFile;
}