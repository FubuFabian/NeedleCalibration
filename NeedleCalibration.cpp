#include <QtTest/QTest>

#include "NeedleCalibration.h"



void NeedleCalibration::calibrate()
{
	igstk::RealTimeClock::Initialize();
	initTracker();

	pivotCalibration = igstk::PivotCalibration::New();
	
	//add observer for the events generated during pivot calibration initialization
	this->m_InitializationObserver = CalibrationObserverType::New();
	this->m_InitializationObserver->SetCallbackFunction(this, &NeedleCalibration::OnInitializationEvent );
	pivotCalibration->AddObserver(igstk::PivotCalibration::InitializationFailureEvent(),this->m_InitializationObserver );
	pivotCalibration->AddObserver(igstk::PivotCalibration::InitializationSuccessEvent(),this->m_InitializationObserver );

	//add observer for the events generated during pivot calibration
	this->m_CalibrationObserver = CalibrationObserverType::New();
	this->m_CalibrationObserver->SetCallbackFunction(this, &NeedleCalibration::OnCalibrationEvent );
	pivotCalibration->AddObserver(igstk::PivotCalibration::DataAcquisitionStartEvent(), this->m_CalibrationObserver );
	pivotCalibration->AddObserver(igstk::PivotCalibration::DataAcquisitionEvent(), this->m_CalibrationObserver );
	pivotCalibration->AddObserver(igstk::PivotCalibration::DataAcquisitionEndEvent(),this->m_CalibrationObserver );
	pivotCalibration->AddObserver(igstk::PivotCalibration::CalibrationSuccessEvent(),this->m_CalibrationObserver );
	pivotCalibration->AddObserver(igstk::PivotCalibration::CalibrationFailureEvent(),this->m_CalibrationObserver );
            
	//this->m_TransformToObserver = TransformToObserver::New();
	//this->m_PivotPointObserver = PivotPointObserver::New();
	//this->m_RMSEObserver = RMSEObserver::New();

	needleTool = polarisTracker->getTools().at(0).GetPointer();
	needleTracker = polarisTracker->getTracker().GetPointer();
	pivotCalibration->RequestInitialize(nframes, needleTool);

	std::cout<<std::endl<<"Calibration Start in "<<delay<<" seconds"<<std::endl;

	for(unsigned int i=this->delay; i>0; i-=1 ){
		std::cout<<".";
		QTest::qWait(1000);
    }

	std::cout<<std::endl<<"Calibration Started"<<std::endl<<std::endl;

	//pivotCalibration->RequestComputeCalibration();

	polarisTracker->stopTracking();

	std::cout<<"acabo"<<std::endl;

}

void NeedleCalibration::initTracker()
{
	
	polarisTracker = PolarisTracker::New();
	polarisTracker->setLoggerOn(true);
	std::cout<<"-Initializing Logger"<<std::endl;
    polarisTracker->initializeLogger();
	std::cout<<"-Initializing SerialCommunication"<<std::endl;
	polarisTracker->initializeSerialCommunication(3);
	std::cout<<"-Initializing Tracker"<<std::endl;
	polarisTracker->initializeTracker();
	std::cout<<"-Initializing Needle Tool"<<std::endl;
	polarisTracker->initializeTrackerTool(romFile);
	std::cout<<"-Attaching all tools"<<std::endl;
	polarisTracker->attachAllTools();
	//std::cout<<"-Creating observers for all tools"<<std::endl;
	//polarisTracker->createToolsObervers();
	std::cout<<"-Start Tracking"<<std::endl;
	polarisTracker->startTracking();
	//std::cout<<"-Transformation observer"<<std::endl;
	//polarisTracker->observeAllToolsTransformations();

}


void NeedleCalibration::setNumberOfFrames(int nframes)
{
	this->nframes = nframes;
}

void NeedleCalibration::setDelay(int delay)
{
	this->delay = delay;
}

void NeedleCalibration::setRomFile(std::string romFile)
{
	this->romFile = romFile;
}

void NeedleCalibration::OnInitializationEvent(itk::Object * itkNotUsed(caller),
                                             const itk::EventObject & event )
{


	if( dynamic_cast< const 
              igstk::PivotCalibration::InitializationSuccessEvent * > (&event) )
    {
		std::cout<<std::endl<<"Calibration Inizialization Succeeded"<<std::endl;
		std::cout<<"ROM File: "<<needleTool->GetTrackerToolIdentifier()<<std::endl;
		std::cout<<"Number of Frames: "<<nframes<<std::endl;
    }
	else if( dynamic_cast< 
        const igstk::PivotCalibration::InitializationFailureEvent * > (&event) )
    {
		std::cout<<"Failed to initialize pivot calibration.\n\
                                                    Check that tool is valid.";
    }
}

void NeedleCalibration::OnCalibrationEvent( itk::Object * caller, 
                                                const itk::EventObject & event )
{

	std::cout<<"OnCalibrationEvent"<<std::endl;

	if(dynamic_cast< const 
		    igstk::PivotCalibration::DataAcquisitionStartEvent * > (&event) )
	{
		std::cout<<"Data Acquisition Started"<<std::endl;
	}
	else if( const igstk::PivotCalibration::DataAcquisitionEvent *evt =
     dynamic_cast< const igstk::PivotCalibration::DataAcquisitionEvent * > (&event) )
	{
		// set progress bar
		std::cout<<"Data Acquisition"<<std::endl;
	}
	else if( dynamic_cast<
          const igstk::PivotCalibration::DataAcquisitionEndEvent * > (&event) )
	{
		//	do nothing
		std::cout<<"Data Acquisition Ended"<<std::endl;
	}
	else if( const igstk::PivotCalibration::CalibrationFailureEvent *evt =
       dynamic_cast<
          const igstk::PivotCalibration::CalibrationFailureEvent * > (&event) )
	{
		// handle error
		std::cout<<"Calibration failed"<<std::endl;
	} 
	else if( dynamic_cast<
          const igstk::PivotCalibration::CalibrationSuccessEvent * > (&event) )
	{
		// process results
		std::cout<<"Calibration Succed"<<std::endl;
	}

	/*if(dynamic_cast< const 
		    igstk::PivotCalibration::DataAcquisitionStartEvent * > (&event) )
	{
		std::cout<<"Data Acquisition Started"<<std::endl;
	}
	else if(dynamic_cast< const 
		    igstk::PivotCalibration::DataAcquisitionEvent * > (&event) )
    {
		std::cout<<"Data Acquisition"<<std::endl;
    }
	else if( dynamic_cast< 
	  const igstk::PivotCalibration::DataAcquisitionEndEvent * > (&event) )
    {
		std::cout<<"Data Acquisition Ended"<<std::endl;
    }
	else if(dynamic_cast< 
				const igstk::PivotCalibration::CalibrationFailureEvent * > (&event) )
    {
		std::cout<<"Calibration failed"<<std::endl;
	}
	else if( dynamic_cast< 
	  const igstk::PivotCalibration::CalibrationSuccessEvent * > (&event) )
    {
		std::cout<<"Calibration Succed"<<std::endl;
    }*/
}

unsigned long NeedleCalibration::AddObserver( const itk::EventObject & event, 
                                         itk::Command *command )
{
  return pivotCalibration->AddObserver( event, command );
}


unsigned long NeedleCalibration::AddObserver( const itk::EventObject & event, 
                                         itk::Command *command ) const
{
  return pivotCalibration->AddObserver( event, command );
}

void NeedleCalibration::RemoveObserver( unsigned long observerID )
{
	pivotCalibration->RemoveObserver( observerID );
}
