#ifndef NEEDLECALIBRATION_H
#define NEEDLECALIBRATION_H

#include <iostream>

#include "itkCommand.h"
#include "igstkStateMachine.h"
#include "igstkMacros.h"
#include "igstkEvents.h"
#include "igstkTracker.h"
#include "igstkPivotCalibration.h"
#include "PolarisTracker.h"

using namespace  std;

class NeedleCalibration
{

public:

	static NeedleCalibration *New()
	{
		return new NeedleCalibration;
	}

	void setNumberOfFrames(int);
	void setDelay(int);
	void setRomFile(std::string);
	void calibrate();

	unsigned long AddObserver( const itk::EventObject & event, 
                            itk::Command *command );
	unsigned long AddObserver( const itk::EventObject & event, 
                             itk::Command *command ) const;
	void RemoveObserver( unsigned long observerID );

private:

	void initTracker();

	typedef itk::MemberCommand<NeedleCalibration>   CalibrationObserverType;

	void OnInitializationEvent( itk::Object *caller, const itk::EventObject & event );
	CalibrationObserverType::Pointer m_InitializationObserver;  
	
	void OnCalibrationEvent( itk::Object *caller, const itk::EventObject & event );
	CalibrationObserverType::Pointer m_CalibrationObserver;

	igstkObserverMacro( TransformTo, 
                      igstk::CoordinateSystemTransformToEvent, 
                      igstk::CoordinateSystemTransformToResult )
	TransformToObserver::Pointer m_TransformToObserver;

	//pivot calibration get pivot point observer
	igstkObserverMacro( PivotPoint, 
                      igstk::PointEvent, 
                      igstk::EventHelperType::PointType )
	PivotPointObserver::Pointer m_PivotPointObserver;

	//pivot calibration get RMSE observer
	igstkObserverMacro( RMSE, 
                      igstk::DoubleTypeEvent, 
                      igstk::EventHelperType::DoubleType )
	RMSEObserver::Pointer m_RMSEObserver;

	PolarisTracker* polarisTracker;
	igstk::PivotCalibration::Pointer pivotCalibration;
	igstk::TrackerTool::Pointer needleTool;
	igstk::Tracker::Pointer needleTracker;
	int nframes;
	int delay;
	std::string romFile;

};

#endif // NEEDLECALIBRATION_H

