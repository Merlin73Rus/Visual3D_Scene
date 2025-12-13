// AviationTrainingUI.cpp : main project file.

#include "stdafx.h"
#include "MainForm.h"
#include "TrainingStagesForm.h"
#include "FlightInstrumentsForm.h"

using namespace AviationSpatialTraining;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew MainForm());
	return 0;
}