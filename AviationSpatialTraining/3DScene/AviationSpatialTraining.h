// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AVIATIONSPATIALTRAINING_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AVIATIONSPATIALTRAINING_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef AVIATIONSPATIALTRAINING_EXPORTS
#define AVIATIONSPATIALTRAINING_API __declspec(dllexport)
#else
#define AVIATIONSPATIALTRAINING_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

// This class is exported from the AviationSpatialTraining.dll

class AVIATIONSPATIALTRAINING_API CAviationSpatialTraining {
public:
	CAviationSpatialTraining(void);
	// TODO: add your methods here.
	int GetInt(void);
	std::string GetProgramName(void);
	void SetHandle(long handle);
	void ReStartRendering();
	void Resize(int width, int height);
	void Close();
	void Update();
	void StartListening();
	void MouseMove( int x, int y, int d);
	void KeyPressed(int stepX, int stepY, int stepZ);
	void StopRendering();
	
	// Aviation-specific methods
	int GetCurrentStage(void);
	void SetCurrentStage(int stage);
	void GetAircraftPosition(double &x, double &y, double &z);
	void GetAircraftAttitude(double &pitch, double &roll, double &yaw);
	void GetFlightInstruments(double &altitude, double &airspeed, double &heading, double &verticalSpeed);
	void SetAircraftControl(double pitchInput, double rollInput, double yawInput, double throttle);
	void SetTrainingScenario(int scenarioId);
	void ResetFlight();
	void ToggleInstrumentDisplay(bool showInstruments);
	void SetWeatherCondition(int condition);
	void SetTimeOfDay(int hour);

private:
	bool bRendering;
	bool bCreated;
	int nCurrentStage; // Training stage (1-5)
	
};

extern AVIATIONSPATIALTRAINING_API int nAviationSpatialTraining;

AVIATIONSPATIALTRAINING_API int fnAviationSpatialTraining(void);