#ifndef NEEDLECALIBRATIONWIDGET_H
#define NEEDLECALIBRATIONWIDGET_H

#include <QWidget>
#include "NeedleCalibration.h"

namespace Ui {
class NeedleCalibrationWidget;
}

class NeedleCalibrationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit NeedleCalibrationWidget(QWidget *parent = 0);
    ~NeedleCalibrationWidget();
    
	void setRomFile(std::string);

private:
    Ui::NeedleCalibrationWidget *ui;

    int frames;
    int delaySeconds;

	std::string romFile;

	NeedleCalibration* needleCalibration;

private slots:

    void numberOfFrames(int);
    void delay(int);
    void calibrate();
    void save();

};

#endif // NEEDLECALIBRATIONWIDGET_H
