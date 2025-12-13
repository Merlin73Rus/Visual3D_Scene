// AviationSpatialTraining.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AviationSpatialTraining.h"
#include "OgreFlightRender.h"

OgreFlightRender * m_Render;

// This is an example of an exported variable
AVIATIONSPATIALTRAINING_API int nAviationSpatialTraining=0;

// This is an example of an exported function.
AVIATIONSPATIALTRAINING_API int fnAviationSpatialTraining(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see AviationSpatialTraining.h for the class definition
CAviationSpatialTraining::CAviationSpatialTraining()
{
	bRendering = false;
	bCreated = false;
	nCurrentStage = 1; // Start with stage 1: Aircraft familiarization
	return;
}

std::string CAviationSpatialTraining::GetProgramName(void)
{
	return "Программа для формирования навыков пространственной ориентации у студентов гражданской авиации";
}

int CAviationSpatialTraining::GetCurrentStage(void)
{
	return nCurrentStage;
}

void CAviationSpatialTraining::SetCurrentStage(int stage)
{
	if(stage >= 1 && stage <= 5) {
		nCurrentStage = stage;
		if(m_Render != nullptr) {
			m_Render->SetTrainingStage(stage);
		}
	}
}

void CAviationSpatialTraining::MouseMove( int x, int y, int d)
{
	if(bRendering==true) { m_Render->mouseMoved(x, y, d);}
}

void CAviationSpatialTraining::StartListening()
{
	m_Render->createFrameListener();
}

int CAviationSpatialTraining::GetInt(void)
{
	return 15;
}

void CAviationSpatialTraining::SetHandle(long handle)
{
	bRendering=false;
	if(bCreated==false)
	{
		bCreated = true;
		bRendering = false;
		m_Render = new OgreFlightRender();
		m_Render->Init(handle);		
	}
	
	/*if(bRendering==false)
	{
		bRendering = true;
		m_Render->StartRendering();
	}*/

}

void CAviationSpatialTraining::ReStartRendering()
{
	m_Render->ReStartRendering();
}

void CAviationSpatialTraining::StartRendering()
{
	if(bRendering==false)
	{
		bRendering = true;
		m_Render->StartRendering();
		m_Render->keyPressed(0, 0, 1);

	}
}

void CAviationSpatialTraining::Resize(int width, int height)
{
	if(bRendering==true) { m_Render->Resize(width, height);}
}

void CAviationSpatialTraining::Close()
{
	if(bRendering==true)
	{
		bRendering = false;
		m_Render->StopRendering();		
	}
}

void CAviationSpatialTraining::Update()
{
	m_Render->Update();
}

void CAviationSpatialTraining::KeyPressed(int stepX, int stepY, int stepZ)
{
	if(bRendering==true){ m_Render->keyPressed(stepX, stepY, stepZ);}
}

void CAviationSpatialTraining::StopRendering()
{
	if(bRendering==true)
	{
		bRendering = false;
		m_Render->StopRendering();

	}
}

void CAviationSpatialTraining::GetAircraftPosition(double &x, double &y, double &z)
{
	if(m_Render != nullptr) {
		m_Render->GetAircraftPosition(x, y, z);
	}
}

void CAviationSpatialTraining::GetAircraftAttitude(double &pitch, double &roll, double &yaw)
{
	if(m_Render != nullptr) {
		m_Render->GetAircraftAttitude(pitch, roll, yaw);
	}
}

void CAviationSpatialTraining::GetFlightInstruments(double &altitude, double &airspeed, double &heading, double &verticalSpeed)
{
	if(m_Render != nullptr) {
		m_Render->GetFlightInstruments(altitude, airspeed, heading, verticalSpeed);
	}
}

void CAviationSpatialTraining::SetAircraftControl(double pitchInput, double rollInput, double yawInput, double throttle)
{
	if(m_Render != nullptr) {
		m_Render->SetAircraftControl(pitchInput, rollInput, yawInput, throttle);
	}
}

void CAviationSpatialTraining::SetTrainingScenario(int scenarioId)
{
	if(m_Render != nullptr) {
		m_Render->SetTrainingScenario(scenarioId);
	}
}

void CAviationSpatialTraining::ResetFlight()
{
	if(m_Render != nullptr) {
		m_Render->ResetFlight();
	}
}

void CAviationSpatialTraining::ToggleInstrumentDisplay(bool showInstruments)
{
	if(m_Render != nullptr) {
		m_Render->ToggleInstrumentDisplay(showInstruments);
	}
}

void CAviationSpatialTraining::SetWeatherCondition(int condition)
{
	if(m_Render != nullptr) {
		m_Render->SetWeatherCondition(condition);
	}
}

void CAviationSpatialTraining::SetTimeOfDay(int hour)
{
	if(m_Render != nullptr) {
		m_Render->SetTimeOfDay(hour);
	}
}