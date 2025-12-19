// Следующий блок ifdef — стандартный способ определения макроса, 
// упрощающего экспорт функций из DLL. Все файлы этой DLL компилируются
// с определенным символом в командной строке AVIATIONSPATIALTRAINING_EXPORTS.
// Этот символ не должен быть определён в проектах, использующих данную DLL. 
// Благодаря этому в других проектах, включающих этот заголовочный файл,
// функции с макросом AVIATIONSPATIALTRAINING_API воспринимаются как импортируемые из DLL,
// тогда как в самой DLL они считаются экспортируемыми.
#ifdef AVIATIONSPATIALTRAINING_EXPORTS
#define AVIATIONSPATIALTRAINING_API __declspec(dllexport)
#else
#define AVIATIONSPATIALTRAINING_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

// Этот класс экспортируется из AviationSpatialTraining.dll

class AVIATIONSPATIALTRAINING_API CAviationSpatialTraining {
public:
	CAviationSpatialTraining(void);
	// TODO: добавьте здесь свои методы.
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
	
	// Методы, специфичные для авиационного тренажера
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
	int nCurrentStage; // Этап обучения (от 1 до 5)
	
};

extern AVIATIONSPATIALTRAINING_API int nAviationSpatialTraining;

AVIATIONSPATIALTRAINING_API int fnAviationSpatialTraining(void);
