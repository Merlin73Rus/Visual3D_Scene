// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCENEVISUALIZER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCENEVISUALIZER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SCENEVISUALIZER_EXPORTS
#define SCENEVISUALIZER_API __declspec(dllexport)
#else
#define SCENEVISUALIZER_API __declspec(dllimport)
#endif

#include <string>
using namespace std;

// This class is exported from the SceneVisualizer.dll

class SCENEVISUALIZER_API CSceneVisualizer {
public:
	CSceneVisualizer(void);
	// TODO: add your methods here.
	int GetInt(void);
	string SetText(void);
	void SetHandle(long handle);
	void ReStartRendering();
	void Resize(int width, int height);
	void Close();
	void Update();
	void StartListening();
	void MouseMove( int x, int y, int d);
	void KeyPressed(int stepX, int stepY, int stepZ);
	void StopRendering();
	void GetShipSize();
	void GetShipData(int &x, int &y, int &speed);
	void SetShipDestination(int x, int y, int speed);

	void GetObjectData(int nObjectID, int &x, int &y, int& h, int &speed, int &vx, int &vy, int &vh);
	void GetObjectData(int nObjectID, int &nType, double &x, double &y, double& h, double &vx, double &vy, double &vh);
	void GetRadarData(int nRadar, int &x, int &y, int& h, int &speed);
	void GetRadarData(int nRadar, double &x, double &y, double& h);
	void SetObjectDestination(int nObjectID,int x, int y, int h, int speed);

	void DeleteAllObjectes();
	void AddRadar(int nObject, int x, int y, int h);
	void AddObject(int nObject, int nType, bool bRadar, int x, int y, int h, int nColor);
	void AddObject(int nObject, int nType, int x, int y, int h, double vx, double vy, double vh);
	void AddObjectRoute(int nObject, int speed, int x, int y, int h);
	void SetNumberObject(int number);
	void SetNumberRadar(int number);
	void StartRendering();
	void SetNRoute(int nObject, int nRout);
	void LookAll();
	void LookObject(int nObject, bool bGoing);
	void StartAll();

private:
	bool bRendering;
	bool bCreated;
	
};


//extern SCENEVISUALIZER_API string CSceneVisualizer::SetText(void);
extern SCENEVISUALIZER_API int nSceneVisualizer;

SCENEVISUALIZER_API int fnSceneVisualizer(void);
