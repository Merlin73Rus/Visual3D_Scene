// SceneVisualizer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SceneVisualizer.h"
#include "OgreRender.h"


OgreRender * m_Render;

// This is an example of an exported variable
SCENEVISUALIZER_API int nSceneVisualizer=0;

// This is an example of an exported function.
SCENEVISUALIZER_API int fnSceneVisualizer(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see SceneVisualizer.h for the class definition
CSceneVisualizer::CSceneVisualizer()
{
	bRendering = false;
	bCreated = false;
	return;
}

string CSceneVisualizer::SetText(void)
{
	return "Мой текст для проверки";
}

void CSceneVisualizer::MouseMove( int x, int y, int d)
{
	if(bRendering==true) { m_Render->mouseMoved(x, y, d);}
}

void CSceneVisualizer::StartListening()
{
	m_Render->createFrameListener();
}

int CSceneVisualizer::GetInt(void)
{
	return 15;
}

void CSceneVisualizer::SetHandle(long handle)
{
	bRendering=false;
	if(bCreated==false)
	{
		bCreated = true;
		bRendering = false;
		m_Render = new OgreRender();
		m_Render->Init(handle);		
	}
	
	/*if(bRendering==false)
	{
		bRendering = true;
		m_Render->StartRendering();
	}*/

}

void CSceneVisualizer::ReStartRendering()
{
	m_Render->ReStartRendering();
}

void CSceneVisualizer::StartRendering()
{
	if(bRendering==false)
	{
		bRendering = true;
		m_Render->StartRendering();
		m_Render->keyPressed(0, 0, 1);

	}

}

void CSceneVisualizer::Resize(int width, int height)
{
	if(bRendering==true) { m_Render->Resize(width, height);}
}

void CSceneVisualizer::Close()
{
	if(bRendering==true)
	{
		bRendering = false;
		m_Render->StopRendering();		
	}
}

void CSceneVisualizer::Update()
{
	m_Render->Update();
}

void CSceneVisualizer::KeyPressed(int stepX, int stepY, int stepZ)
{
	if(bRendering==true){ m_Render->keyPressed(stepX, stepY, stepZ);}
}

void CSceneVisualizer::StopRendering()
{
	if(bRendering==true)
	{
		bRendering = false;
		m_Render->StopRendering();		

	}
}

void CSceneVisualizer::GetShipSize()
{
	//m_Render->GetShipSize();
}

void CSceneVisualizer::GetShipData(int &x, int &y, int &speed)
{
	m_Render->GetShipData(x, y, speed);

}

void CSceneVisualizer::SetShipDestination(int x, int y, int speed)
{
	m_Render->SetShipDestination(x, y, speed);
}

void CSceneVisualizer::GetObjectData(int nObjectID, int &x, int &y, int& h, int &speed, int &vx, int &vy, int &vh)
{
	m_Render->GetObjectData(nObjectID, x, y, h, speed, vx, vy, vh);
}

void CSceneVisualizer::GetObjectData(int nObjectID, int &nType, double &x, double &y, double &h, double &vx, double &vy, double &vh)
{
	m_Render->GetObjectData(nObjectID, nType, x, y, h, vx, vy, vh);
	h = h - 1005.0;
}

void CSceneVisualizer::SetObjectDestination(int nObjectID,int x, int y, int h, int speed)
{
	m_Render->SetObjectDestination(nObjectID, x, y, h, speed);
}

void CSceneVisualizer::GetRadarData(int nRadar, int &x, int &y, int& h, int &speed)
{
	m_Render->GetRadarData(nRadar, x, y, h, speed);
}

void CSceneVisualizer::GetRadarData(int nRadar, double &x, double &y, double& h)
{
	m_Render->GetRadarData(nRadar, x, y, h);
	h = h - 1005.0;
}

void CSceneVisualizer::SetNumberObject(int number)
{
	m_Render->SetNumberObjects(number);
}

void  CSceneVisualizer::SetNumberRadar(int number)
{
	m_Render->SetNumberRadars(number);
}

void CSceneVisualizer::DeleteAllObjectes()
{
	m_Render->DeleteAllObjects();
}
void  CSceneVisualizer::AddRadar(int nObject, int x, int y, int h)
{
	m_Render->AddRadar(nObject,x, y, h+1005);
}
void CSceneVisualizer::AddObject(int nObject, int nType, bool bRadar, int x, int y, int h, int nColor)
{
	m_Render->AddObject(nObject, nType, bRadar, x, y, h+1005, nColor);
}

void CSceneVisualizer::AddObject(int nObject, int nType, int x, int y, int h, double vx, double vy, double vh)
{
	m_Render->AddObject(nObject, nType, x, y, h+1005, vx, vy, vh);
}

void CSceneVisualizer::SetNRoute(int nObject, int nRout)
{
	m_Render->SetNRoute(nObject, nRout);
}

void CSceneVisualizer::AddObjectRoute(int nObject, int speed, int x, int y, int h)
{
	m_Render->AddObjectRoute(nObject, speed, x, y, h+1005);
}

void CSceneVisualizer::LookAll()
{
	m_Render->LookAll();
}

void CSceneVisualizer::LookObject(int nObject, bool bGoing)
{
	m_Render->LookObject(nObject, bGoing);
}

void CSceneVisualizer::StartAll()
{
	m_Render->StartAll();
}