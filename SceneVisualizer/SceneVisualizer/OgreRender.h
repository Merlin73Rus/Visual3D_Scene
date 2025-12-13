#pragma once

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "BaseRender.h"
using namespace Ogre;


struct Route
{
	int X0;
	int Y0;
	int H0;

	int X;
	int Y;
	int H;
	int Speed;
	bool bExist;
	Ogre::ManualObject* _track;
	Ogre::SceneManager* mSceneMgr;
	

	Route()
	{
		bExist = false;
	}

	~Route()
	{	
		_track->clear();
		mSceneMgr->destroyManualObject(_track);
	}

	void SetStartWay(int x, int y, int h, Ogre::SceneManager* mSMgr, int nColor)
	{
		mSceneMgr = mSMgr;
		X0 = x;
		Y0 = y;
		H0 = h;
		Ogre::ColourValue color;
		color.setAsARGB(nColor);
		_track = mSceneMgr->createManualObject();
		_track->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
		_track->position(0, -5105, 0);
		_track->colour(color);
		_track->position(1, -5105, 1);
		_track->colour(color);
		_track->index(0);
		_track->index(1);
		_track->index(0);
		_track->end();
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(_track);
	}

	void UpdateWay(int x, int y, int h, int speed)
	{
		X = x;
		Y = y;
		H = h;
		Speed = speed;
		bExist = true;	

		Ogre::Vector3 pos = Ogre::Vector3::ZERO;
		
		_track->beginUpdate(0);

		pos.x=X0;
		pos.z=Y0;
		pos.y=H0+50;		
		_track->position(pos);

		pos.x=X;
		pos.z=Y;
		pos.y=H+50;
		_track->position(pos);

		_track->end();
	}
};


struct Routes
{
	int nCount;
	int nNumberWay;
	int Color;
	Route* _route;
	bool bCtreated;
	Ogre::SceneManager* mSceneMgr;

	int X0;
	int Y0;
	int H0;

	Routes(Ogre::SceneManager* mng, int nColor)
	{
		mSceneMgr = mng;
		bCtreated = false;
		nNumberWay = 0;
		Color = nColor;
	}
	~Routes()
	{
		DeleteAll();
	}

	void SetStartWay(int x, int y, int h)
	{
		X0 = x;
		Y0 = y;
		H0 = h;
	}

	void Init(int n)
	{
		if(bCtreated== true)
		{
			DeleteAll();
		}

		nNumberWay = n;
		_route = new Route [nNumberWay];
		
		bCtreated = true;
	}
	void DeleteAll()
	{
		nNumberWay = 0;
		delete[] _route;
		bCtreated = false;
	}

	void Message(int Data)
	{
		char a[255];
		itoa(Data, a , 10);
		MessageBox(NULL, a, "Data", 0);
	}

	void AddWay(int nWay, int x, int y, int h, int speed)
	{
		if(nWay<nNumberWay)
		{			
			if(nWay==0)
			{
				_route[nWay].SetStartWay(X0, Y0, H0, mSceneMgr, Color);
			}
			else
			{
				_route[nWay].SetStartWay(_route[nWay-1].X, _route[nWay-1].Y, _route[nWay-1].H, mSceneMgr, Color);
			}

			_route[nWay].UpdateWay(x, y, h, speed);
		}
	}

	bool GetWay(int &nWay, int &x, int &y, int &h, int &speed)
	{
		if(nNumberWay!=0){
			if (nWay >= nNumberWay)
			{
				nWay = (nWay+0)%nNumberWay;
			}
			if(_route[nWay].bExist == true)
			{
				x = _route[nWay].X;
				y = _route[nWay].Y;
				h = _route[nWay].H;
				speed = _route[nWay].Speed;
				return true;
			}
		}
		return false;
	}
};

class ShipListener : public Ogre::FrameListener
{
	Ogre::SceneNode* _node;
	Ogre::ManualObject* _track;
	Ogre::Vector3 destination;
	Ogre::Vector3 direction;
	Ogre::Vector3 src;
	Ogre::Real mDistance;
	Ogre::Quaternion quat;
	Ogre::Quaternion qSrc;
	Ogre::Quaternion qDest;
	Ogre::Degree RotateAngle;
	Ogre::Degree RotateAnglePositive;
	Ogre::Degree MaxRotateAngle;
	Ogre::Degree MaxRotateAngleNegative;
	Ogre::Degree CurrentRotateAngle;
	Ogre::Radian RadAng;
	Ogre::Degree LastRotation;
	Ogre::SceneManager* mSceneMgr;

	int nSpeed;
	int nDestSpeed;
	int nRotationSpeed;
	int nPositiveStatus;
	float nCount;
	float fangle;
	double vx;
	double vy;
	double vh;
	bool one;
	int nDestination;
	Real mRotateDistance;
	int StepH;
	int nType;
	bool bPitch;

	Routes* ObjectRoute;

public:
	ShipListener(Ogre::SceneNode* node, int nRSpeed, Ogre::SceneManager* mng, int nColor, int Type)
	{
		nType = Type;
		nPositiveStatus = 1;
		_node=node;
		mSceneMgr = mng;
		nSpeed = 0;
		nDestSpeed = 0;
		MaxRotateAngle = Ogre::Degree(6);
		MaxRotateAngleNegative = Ogre::Degree(-6);
		LastRotation = Ogre::Degree(0);
		CurrentRotateAngle = Ogre::Degree(6);
		destination = _node->getPosition();
		ObjectRoute = new Routes(mSceneMgr, nColor);
		ObjectRoute->SetStartWay(destination.x, destination.z, destination.y);
		nCount = 0;
		nRotationSpeed = nRSpeed;
		one = false;
		nDestination = 0;
		mRotateDistance = 1500;
		mDistance = 10000;
		vx = 0;
		vy = 0;
		vh = 0;
		StepH = 0;
		nType = Type;
		bPitch = false;
	}

	~ShipListener()
	{
		delete ObjectRoute;
	}
	void SetSpeed(int speed) {  nDestSpeed = speed;}
	int GetSpeed() { return nSpeed;}
	void GetSpeedVector(double &vX, double &vY, double &vH)
	{
		vX = vx;
		vY = vy;
		vH = vh;
	}

	void InitRoute(int nRouts)	{ ObjectRoute->Init(nRouts); }
	void AddDestination(int nWay, int x, int y, int h, int speed)
	{
		ObjectRoute->AddWay(nWay, x, y, h, speed);
		if(nWay==0)
		{
			SetDestination(0);
			SetToDirection();		
			RotateAngle = Ogre::Degree(0.1);
		}
	}
	void SetDestination(int nWay)
	{
		int x, y, h, speed;
		nDestination = nWay;
		if (ObjectRoute->GetWay(nDestination, x, y, h, speed) == true)
		{
			SetSpeed(speed);
			Ogre::Vector3 pos = Ogre::Vector3::ZERO;
			pos.x=x;
			pos.z=y;
			pos.y=h;
			SetDestination(pos);
		}
		else
		{
			SetSpeed(0);
		}
	}

	void SetNextDestination()
	{ 
		nDestination ++;
		SetDestination(nDestination);
	}

	void SetDestination(Ogre::Vector3 dest)
	{
		destination = dest;
		nCount = 0;
		one = true;
		nPositiveStatus = 1;
		CalculateRotation();	
		//mDistance = _node->getPosition().dotProduct(destination);
	}

	void CalculateDirection()
	{
		qSrc = _node->getOrientation();
		direction=destination - _node->getPosition(); //destination - _node->getPosition()
		src = _node->getOrientation() * Vector3::UNIT_Z; //Vector3::UNIT_X;
		mDistance = direction.normalise();		
		qDest = src.getRotationTo(destination)*qSrc;
		quat = Quaternion::Slerp(1, qSrc, qDest, true);
		RadAng = 2*Math::ACos(quat.w);
		RotateAngle = Ogre::Degree(RadAng);
		Ogre::Quaternion r = Ogre::Quaternion(RotateAngle, Vector3::UNIT_Y);
		RadAng = 2*Math::ACos(r.w);
	}

	void CalculateRotation()
	{
		if(bPitch)
		{
			_node->rotate(Vector3::UNIT_X, LastRotation, Ogre::Node::TS_LOCAL);
			bPitch = false;
		}

		direction = destination - _node->getPosition(); //destination - _node->getPosition()
		src = _node->getOrientation() * Vector3::UNIT_Z;
		src.y = 0;
		direction.y = 0;
		src.normalise();
		mDistance = direction.normalise();
		qDest = src.getRotationTo(direction, Vector3::UNIT_Y);
		//quat = Quaternion::Slerp(1, qSrc, qDest, false);
		RadAng = 2*Math::ACos(qDest.w);
		
		//Message(RadAng.valueAngleUnits());
		//Message(qDest.getYaw().valueAngleUnits());
		
		RotateAngle = Ogre::Degree(RadAng);
		//Message(RotateAngle.valueDegrees());
		if(qDest.getYaw().valueAngleUnits()>0)
		{
			//RotateAngle = Ogre::Degree(360)-RotateAngle;
			CurrentRotateAngle = MaxRotateAngle;
		}
		else
		{
			CurrentRotateAngle = MaxRotateAngleNegative;
		}
		//if (RotateAngle.valueDegrees() < 0.0) {nPositiveStatus=-1;}
		//RotateAnglePositive = RotateAngle*nPositiveStatus;

	}

	Real CalculateDistance()
	{
		qSrc = _node->getOrientation();
		direction=destination - _node->getPosition(); //destination - _node->getPosition()
		src = _node->getOrientation() * Vector3::UNIT_Z;
		Real Distance = direction.normalise();
		return Distance;
	}
	void Message(int Data)
	{
		char a[255];
		itoa(Data, a , 10);
		MessageBox(NULL, a, "Data", 0);
	}

	void RotateToDirection(const Ogre::FrameEvent &evt) //Развернуться в направлении движения
	{
		Ogre::Degree CurRotateAngle;
		CurRotateAngle = CurrentRotateAngle*evt.timeSinceLastFrame;
		
		if((RotateAngle.valueDegrees()-abs(CurRotateAngle.valueDegrees()))>MaxRotateAngle.valueDegrees())
		{
			_node->rotate(Vector3::UNIT_Y, CurRotateAngle, Ogre::Node::TS_LOCAL);
			nSpeed = nRotationSpeed;
			RotateAngle = Ogre::Degree(RotateAngle.valueDegrees()-abs(CurRotateAngle.valueDegrees()));
		}
		else
		{
			SetToDirection();
			if(!bPitch)
			{
				//SetToDirectionXY();
			}
			//if(nType!=1) { CalculateStepH(evt); }
			nSpeed = nDestSpeed;
			//LastRotation
		}
	}
	
	void SetToDirection()  // Направиться в направлении движения
	{
		direction = destination - _node->getPosition(); //destination - _node->getPosition()
		src = _node->getOrientation() * Vector3::UNIT_Z;
		src.y = 0;
		direction.y = 0;
		src.normalise();
		mDistance = direction.normalise();
		qDest = src.getRotationTo(direction, Vector3::UNIT_Y);
		
		_node->rotate(qDest, Ogre::Node::TS_LOCAL);
		RotateAngle = 0;

	
	}

	void SetToDirectionXY()  //
	{
		direction = destination - _node->getPosition(); //destination - _node->getPosition()
		src = _node->getOrientation() * Vector3::UNIT_Z;
		//src.x = 0;
		//direction.x = 0;
		src.normalise();
		mDistance = direction.normalise();
		qDest = src.getRotationTo(direction, Vector3::UNIT_X);
		LastRotation = qDest.getPitch();
		Message(LastRotation.valueAngleUnits());
		_node->rotate(Vector3::UNIT_X, Ogre::Degree(0) - LastRotation, Ogre::Node::TS_LOCAL);
		//_node->rotate(qDest, Ogre::Node::TS_LOCAL);
		bPitch = true;	
	}



	void CalculateStepH(const Ogre::FrameEvent &evt)
	{
		int HSpeed = 50;
		StepH = 0.0;

		if(abs(destination.y - _node->getPosition().y)>=HSpeed*evt.timeSinceLastFrame)
		{
			StepH = HSpeed*evt.timeSinceLastFrame*(destination.y - _node->getPosition().y)/abs(destination.y - _node->getPosition().y);
		}
		else
		{
			StepH = destination.y - _node->getPosition().y;
		}

	}

	virtual bool frameStarted(const Ogre::FrameEvent &evt)
	{
		//Ogre::Vector3 StartPos, EndPos;
		//StartPos = _node->getPosition();

		RotateToDirection(evt);
		_node->translate(0, 0, (nSpeed*evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);

		Ogre::Vector3 CurVec = _node->getOrientation()* Vector3::UNIT_Z;
		CurVec.normalise();

		//EndPos = _node->getPosition();

		vx = CurVec.x*nSpeed;
		vy = CurVec.z*nSpeed;
		vh = CurVec.y*nSpeed;
		
		mDistance = CalculateDistance();
		//if(mDistance <= mRotateDistance) { SetNextDestination();}
		if(mDistance/nSpeed<=1.5) { SetNextDestination();}

		

		return true;
	}

};

class Objects
{
	Ogre::SceneNode* ObjectNode;
	Ogre::SceneNode* EntityNode;
	Ogre::Entity * ObjectEntity;
	ShipListener* ObjectNodeListener;
	
	int Type;
	int nRSpeed;
	int nWayCount;


	Ogre::SceneManager* mSceneMgr;

public:
	float baseScale;
	float Scale2;
	float CurrentScale;
	bool bRadarCreated;
	Ogre::Root *CurRoot;

	Objects()
	{
		baseScale = 1;
		Scale2 = 1;
		CurrentScale = 1;
		nRSpeed = 0;
		Type = 0;
		nWayCount = 0;	
		bRadarCreated = false;
		ObjectNodeListener = NULL;

	}

	~Objects()
	{
		
		CurRoot->removeFrameListener(ObjectNodeListener);
		if (!ObjectNodeListener){ delete ObjectNodeListener;}
		
		mSceneMgr->destroySceneNode(ObjectNode);
	}
    
	void InitObject(Ogre::SceneManager* mng, Ogre::Root *mRoot, int nType, bool bRadar, int x, int y, int h, int nColor)
	{
		Type = nType;
		mSceneMgr = mng;
		CurRoot = mRoot;
		switch(nType)
		{
		case 0 : //Радар
			AddRadar(x, y, h);
			break;
		case 1 :		 //корабль
			AddShip(x, y, h);
			InitListener(mRoot, nColor, nType);
			break;
		case 2: //самолет
			AddPlane(x, y, h);
			InitListener(mRoot, nColor, nType);
			break;
		case 3: //ракета
			 AddMissile(x, y, h);
			 InitListener(mRoot, nColor, nType);
			break;
		default :
			break;
		}
	}

	void InitObject(Ogre::SceneManager* mng, Ogre::Root *mRoot, int nType, int x, int y, int h, double vx, double vy, double vh)
	{
		Type = nType;
		mSceneMgr = mng;
		CurRoot = mRoot;
		switch(nType)
		{
		case 0 : //Радар
			AddRadar(x, y, h);
			break;
		case 1 :		 //корабль
			AddShip(x, y, h);
			break;
		case 2: //самолет
			AddPlane(x, y, h);
			break;
		case 3: //ракета
			 AddMissile(x, y, h);
			break;
		default :
			break;
		}


		Ogre::Vector3 DesVec;
		DesVec.x = vx;
		DesVec.z = vy;
		DesVec.y = vh;
		
		DesVec.y = 0;
		DesVec.normalise();
		
		Ogre::Vector3 src = ObjectNode->getOrientation() * Vector3::UNIT_Z;
		src.y = 0;
		src.normalise();
		
		Ogre::Quaternion qDest = src.getRotationTo(DesVec, Vector3::UNIT_Y);
		ObjectNode->rotate(qDest, Ogre::Node::TS_LOCAL);


		/*Vector3::UNIT_Z;
		
			
		ObjectNode->rotate(qDest, Ogre::Node::TS_LOCAL);

		DesVec.y = 0;
		DesVec.normalise();
		


		_node->_setDerivedOrientation(const Quaternion& q);

		direction = destination - _node->getPosition(); //destination - _node->getPosition()
		src = _node->getOrientation() * Vector3::UNIT_Z;
		src.y = 0;
		direction.y = 0;
		src.normalise();
		mDistance = direction.normalise();
		qDest = src.getRotationTo(direction, Vector3::UNIT_Y);

		
		Ogre::Vector3 position = Ogre::Vector3::ZERO;
		position = ObjectNode->getPosition();
		Ogre::Vector3 destination = Ogre::Vector3::ZERO;
		destination.x = position.x + vx*100;
		destination.z = position.z + vy*100;
		Ogre::Vector3 direction = Ogre::Vector3::ZERO;

		direction = destination - position;
		Ogre::Vector3 src = ObjectNode->getOrientation() * Vector3::UNIT_Z;
		src.y = 0;
		direction.y = 0;
		src.normalise();
		Ogre::Quaternion qDest = src.getRotationTo(direction, Vector3::UNIT_Y);

		ObjectNode->rotate(qDest, Ogre::Node::TS_LOCAL);*/
	}
	
	void SetNRoute (int nRout)
	{
		ObjectNodeListener->InitRoute(nRout);
	}	

	void AddRoute(int x, int y, int h, int speed)
	{
	    ObjectNodeListener->AddDestination(nWayCount, x, y, h, speed);	
		nWayCount++;
	}
		
	void SetScale()
	{
		ObjectNode->setScale(CurrentScale,CurrentScale,CurrentScale);
	}

	Ogre::Vector3 GetPosition() {return ObjectNode->getPosition();}

	void GetData(int &nType, double &x, double &y, double& h, double &vx, double &vy, double &vh)
	{
		nType = Type;
		Ogre::Vector3 pos = Ogre::Vector3::ZERO;
		pos = ObjectNode->getPosition();
		x = pos.x;
		y = pos.z;
		h = pos.y;

		int nSpeed = ObjectNodeListener->GetSpeed();

		ObjectNodeListener->GetSpeedVector(vx, vy, vh);
		
	}

	void Start()
	{
		ObjectNodeListener->SetDestination(0);
	}

private:

	void InitListener(Ogre::Root *mRoot, int nColor, int nType)
	{
		ObjectNodeListener = new ShipListener(ObjectNode, nRSpeed, mSceneMgr, nColor, nType);
		mRoot->addFrameListener(ObjectNodeListener);
	}
	
	void AddRadar(int x, int y, int h)
	{
		bRadarCreated = true;
		nRSpeed = 0;
		baseScale = 5;
		Scale2 = baseScale*0.1;

		ObjectEntity = mSceneMgr->createEntity("sphere.mesh");
		ObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		ObjectNode->scale(baseScale, baseScale, baseScale);
		ObjectNode->translate(x, h, y);
		ObjectNode->attachObject(ObjectEntity);
		
	}

	void AddShip(int x, int y, int h)
	{
		nRSpeed = 5;
		baseScale = 0.25;
		Scale2 = baseScale*2;
		ObjectEntity = mSceneMgr->createEntity("Ship.mesh");
		ObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		EntityNode = ObjectNode->createChildSceneNode();
		ObjectNode->scale(baseScale, baseScale, baseScale);
		ObjectNode->translate(x, h, y);
		EntityNode->yaw(Ogre::Degree(180));
		EntityNode->attachObject(ObjectEntity);
	}

	void AddPlane(int x, int y, int h)
	{
		nRSpeed = 50;
		baseScale = 2;
		Scale2 = baseScale*12;
		ObjectEntity = mSceneMgr->createEntity("plane.mesh");
		ObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		EntityNode = ObjectNode->createChildSceneNode();
		ObjectNode->scale(baseScale, baseScale, baseScale);
		ObjectNode->translate(x, h, y);
		EntityNode->yaw(Ogre::Degree(180));
		EntityNode->attachObject(ObjectEntity);
	}

	void AddMissile(int x, int y, int h)
	{
		nRSpeed = 50;
		baseScale = 0.015;
		Scale2 = baseScale*40;
		ObjectEntity = mSceneMgr->createEntity("AIM-54 C Phoenix.mesh");
		ObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		EntityNode = ObjectNode->createChildSceneNode();
		ObjectNode->scale(baseScale, baseScale, baseScale);
		ObjectNode->translate(x, h, y);
		EntityNode->yaw(Ogre::Degree(180));
		EntityNode->attachObject(ObjectEntity);
	}

	
	
};

class RadarObjects
{
	Ogre::SceneNode* ObjectNode;
	Ogre::Entity * ObjectEntity;
	Ogre::SceneManager* mSceneMgr;

public:
	float baseScale;
	float Scale2;
	float CurrentScale;

	RadarObjects()
	{
		baseScale = 1;
		Scale2 = 1;
		CurrentScale = 1;
	}

	~RadarObjects()
	{
		mSceneMgr->destroySceneNode(ObjectNode);
	}
    
	void InitObject(Ogre::SceneManager* mMgr, int x, int y, int h)
	{
		mSceneMgr = mMgr;;
		AddRadar(x, y, h);
	}	
		
	void SetScale()
	{
		ObjectNode->setScale(CurrentScale,CurrentScale,CurrentScale);
	}

	Ogre::Vector3 GetPosition() {return ObjectNode->getPosition();}

	void GetData(double &x, double &y, double& h)
	{
		Ogre::Vector3 pos = Ogre::Vector3::ZERO;
		pos = ObjectNode->getPosition();
		x = pos.x;
		y = pos.z;
		h = pos.y;
	}

private:
	
	void AddRadar(int x, int y, int h)
	{
		baseScale = 5;
		Scale2 = baseScale*0.1;

		ObjectEntity = mSceneMgr->createEntity("sphere.mesh");
		ObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		ObjectNode->scale(baseScale, baseScale, baseScale);
		ObjectNode->translate(x, h, y);
		ObjectNode->attachObject(ObjectEntity);	

		
		
	}	
	
};


class OgreRender : public BaseRender
{
	private:
	Objects* _objects;
	RadarObjects* _radars;
	int nObjects;
	int nRadars;

public:
	
	void Test(int x, int y);

void SetNumberObjects(int number);
void SetNumberRadars(int number);
	
	void InitObjects();
	void InitRadars();
	void DeleteObjects() { delete [] _objects; delete [] _radars; }
	void AddRadar(int nObject, int x, int y, int h);
	void AddObject(int nObject, int nType, bool bRadar, int x, int y, int h, int nColor);
	void AddObject(int nObject, int nType, int x, int y, int h, double vx, double vy, double vh);
	void AddObjectRoute(int nObject, int speed, int x, int y, int h);
	void SetNRoute(int nObject, int nRout);
	void LookAll();
	void LookObject(int nObject, bool bGoing);

	ManualObject* ShipTrack;
	Ogre::SceneNode* ShipTrackNode;

	ManualObject* MissileTrack;
	Ogre::SceneNode* MissileTrackNode;

	ManualObject* PlaneTrack;
	Ogre::SceneNode* PlaneTrackNode;

	Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    bool mTerrainsImported;
	//OgreBites::Label* mInfoLabel;
	Ogre::Plane plane;
	Ogre::SceneNode* OceanNode;


	void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
	
	float baseRadarScale;
	float baseShipRadarScale;

	Ogre::Entity * RadarEntity1;
	Ogre::SceneNode* RadarNode1;

	Ogre::Entity * RadarEntity2;
	Ogre::SceneNode* RadarNode2;

	Ogre::Entity * RadarEntity3;
	Ogre::SceneNode* RadarNode3;
	
	Ogre::Entity * ShipEntity;
	Ogre::SceneNode* ShipNode;

	ShipListener* ShipNodeListener;
	ShipListener* MissileNodeListener;
	ShipListener* PlaneNodeListener; 
    int nShipSpeed;
	float baseShipScale;
	float baseMissileScale;
	float basePlaneScale;

	Ogre::Entity * MissileEntity;
	Ogre::SceneNode* MissileNode;

	Ogre::Entity * PlaneEntity;
	Ogre::SceneNode* PlaneNode;

	void GetObjectDataByPtr(Ogre::SceneNode* _node,ShipListener* _listener, int &x, int &y, int& h, int &speed, int &vx, int &vy, int &vh);
	void SetObjectDestinationByPtr(Ogre::SceneNode* _node, ShipListener* _listener, ManualObject* _track, int x, int y, int h, int speed);

public:
	OgreRender(void);
	virtual ~OgreRender(void);
	virtual void createFrameListener(void);

	void Init(long handle); //запуск рендера
	void Update(); //Обновление
    void Resize(int width, int height); //будет вызываться при событии OnRenderPanelResize
	void StopRendering();

	void GetEntitySize(Ogre::Entity *ent);
	bool shipStarted(const Ogre::FrameEvent &evt);
	
	void GetShipData(int &x, int &y, int &speed);
	void SetShipDestination(int x, int y, int speed);
	void GetObjectData(int nObjectID, int &x, int &y, int& h, int &speed, int &vx, int &vy, int &vh);
	void GetObjectData(int nObjectID, int &nType, double &x, double &y, double& h, double &vx, double &vy, double &vh);
	void SetObjectDestination(int nObjectID,int x, int y, int h, int speed);
	void GetRadarData(int nRadar, int &x, int &y, int& h, int &speed);
	void GetRadarData(int nRadar, double &x, double &y, double& h);
	bool keyPressed(int stepX, int stepY, int stepZ);
	void DeleteAllObjects();
	void StartAll();

//	void AddRadar(int nObject,int x, int y, int h);

protected:
    virtual void createScene(void);
	
    virtual void destroyScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);   

};

