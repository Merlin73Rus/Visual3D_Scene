#include "StdAfx.h"

//#include "OgreException.h"
//#include "Ogre.h"

#include "OgreRender.h"


OgreRender::OgreRender(void)
{
	ShipNodeListener= NULL;
	MissileNodeListener = NULL;
	PlaneNodeListener = NULL;
	//mSceneMgr
	nShipSpeed = 0;
	nRadars = 0;
	nObjects = 0;

}
OgreRender::~OgreRender(void)
{
	if(ShipNodeListener)
	{
		delete ShipNodeListener;
	}
	if(MissileNodeListener)
	{
		delete MissileNodeListener;
	}
	if(PlaneNodeListener)
	{
		delete PlaneNodeListener;
	}

	destroyScene();   
}

void OgreRender::Init(long handle)
{
	Handle = handle;
	go();
   
}

void OgreRender::Update()
{
    mRoot->renderOneFrame();
    mWindow->update();
}
 
void OgreRender::Resize(int width, int height)
{
    mCamera->setAspectRatio((static_cast<float>(width)/static_cast<float>(height)));
    mWindow->resize(width, height);
    mWindow->windowMovedOrResized();
    Update();
}


//-------------------------------------------------------------------------------------
void OgreRender::destroyScene(void)
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;	
	delete mRoot;
}
void OgreRender::StopRendering()
{
	mRoot->queueEndRendering();
}
//-------------------------------------------------------------------------------------
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
 img.load("terrain513.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); //terrain.png
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
}
//-------------------------------------------------------------------------------------
void OgreRender::defineTerrain(long x, long y)
{
 Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
    {
        mTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}
//-------------------------------------------------------------------------------------
void OgreRender::initBlendMaps(Ogre::Terrain* terrain)
{
 Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}
//-------------------------------------------------------------------------------------
void OgreRender::configureTerrainDefaults(Ogre::Light* light)
{
 // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(300000);
 
    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
 
    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize =257;
    defaultimp.worldSize = 300000.0f; //30000 delta=10
    defaultimp.inputScale = 2000;  //3600
    defaultimp.minBatchSize = 33; //33
    defaultimp.maxBatchSize = 65; //65
    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 200;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 500;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 15000;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
	/*defaultimp.layerList[3].worldSize = 2000000;
    defaultimp.layerList[3].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[3].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");*/
}
//-------------------------------------------------------------------------------------
void OgreRender::LookAll()
{
	mCamera->setPosition(Ogre::Vector3(1, 400000, 1));
    mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	Ogre::Radian rad = Ogre::Radian(Ogre::Degree(-45));
	mCamera->yaw(rad);
	keyPressed(0, 0, 0);
}
void  OgreRender::LookObject(int nObject, bool bGoing)
{
	Ogre::Vector3 pos = _objects[nObject].GetPosition();
	pos.x -=200;
	pos.y += 200;
	//pos.z =100;
	mCamera->setPosition(pos);
    mCamera->lookAt(_objects[nObject].GetPosition().x,_objects[nObject].GetPosition().y, _objects[nObject].GetPosition().z);
	keyPressed(0, 0, 0);
}
void OgreRender::createScene(void)
{

    mCamera->setPosition(Ogre::Vector3(1500, 1500, 15000));
    mCamera->lookAt(Ogre::Vector3(100, 100, 100));
    mCamera->setNearClipDistance(100); //0.1
    mCamera->setFarClipDistance(300000);
 
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }

    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
 
    Ogre::Light* light = mSceneMgr->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

	plane.d = 1000;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	mSceneMgr->setSkyPlane(true, plane, "Examples/CloudySky", 1500, 40, true, 1.5f, 150, 150);
		
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
 
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 257,300000.0f);

    mTerrainGroup->setFilenameConvention(Ogre::String("SceneVisualizerTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
 
    configureTerrainDefaults(light);
 
    for (long x = 0; x <= 0; ++x)
        for (long y = 0; y <= 0; ++y)
            defineTerrain(x, y);

 int err = 0;	Message(err);
    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);
 err = 1; Message(err);
    if (mTerrainsImported)
    {
		 err = 2; Message(err);
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		 err = 3;  Message(err);
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
		err = 4;  Message(err);
    }
 err = 5; Message(err);
    mTerrainGroup->freeTemporaryResources();
err = 6;	Message(err);
	//mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

	Ogre::Entity* OceanEntity;
err = 7; Message(err);
	Ogre::Plane OceanPlane;
	OceanPlane.normal = Vector3::UNIT_Y;
	MeshManager::getSingleton().createPlane(
		"OceanPlane",
      ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      OceanPlane,
      300000, 300000,
      100, 100,
      true, 1,
      500, 500,
      Vector3::UNIT_Z); //
 err = 8;  Message(err);	
	OceanEntity = mSceneMgr->createEntity("water", "OceanPlane");
	OceanEntity->setMaterialName("OceanCg");   //OceanCg Examples/Water4  OceanHLSL_GLSL
 err = 9;  Message(err);
	OceanNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("OceanNode");
	OceanNode->attachObject(OceanEntity);
err = 10;
Message(err);
	OceanNode->translate(0, 1000, 0);
	keyPressed(0, 0, 0);


}

void OgreRender::SetNumberObjects(int number)
	{
		nObjects = number;
		InitObjects();
	}

void OgreRender::SetNumberRadars(int number)
{
	nRadars = number;
	InitRadars();
}

void OgreRender::DeleteAllObjects()
{
	if(nObjects!=0) { delete [] _objects; }
	nObjects = 0;
	if (nRadars!=0) { delete [] _radars; }
	nRadars = 0;
}

void OgreRender::InitObjects()
{ 
	_objects = new Objects [nObjects];
}

void OgreRender::InitRadars()
{
	_radars = new RadarObjects[nRadars];
}

void OgreRender::AddRadar(int nObject, int x, int y, int h)
{
	_radars[nObject].InitObject(mSceneMgr,x, y, h);
}

void OgreRender::GetRadarData(int nRadar, double &x, double &y, double& h)
{
	_radars[nRadar].GetData(x, y, h);
}

void OgreRender::GetObjectData(int nObjectID, int &nType, double &x, double &y, double& h, double &vx, double &vy, double &vh)
{
	_objects[nObjectID].GetData(nType, x, y, h, vx, vy, vh);
}

void OgreRender::AddObject(int nObject, int nType, bool bRadar, int x, int y, int h, int nColor)
{
	_objects[nObject].InitObject(mSceneMgr, mRoot, nType, bRadar, x, y, h, nColor);	
}

void OgreRender::AddObject(int nObject, int nType, int x, int y, int h, double vx, double vy, double vh)
{
	_objects[nObject].InitObject(mSceneMgr, mRoot, nType, x, y, h, vx, vy, vh);	
}

void OgreRender::SetNRoute(int nObject, int nRout)
{
	_objects[nObject].SetNRoute(nRout);
}

void OgreRender::AddObjectRoute(int nObject, int speed, int x, int y, int h)
{
	_objects[nObject].AddRoute(x, y, h, speed);
}

void OgreRender::GetEntitySize(Ogre::Entity *ent)
{
	Ogre::Vector3 size = Ogre::Vector3::ZERO;
	Ogre::AxisAlignedBox bb = ent->getBoundingBox();
	size = bb.getSize();
	
	char a[255];
	itoa(size.x, a, 10);
    MessageBox(NULL,a , "x", 0);
	itoa(size.y, a, 10);
    MessageBox(NULL,a , "y", 0);
	itoa(size.z, a, 10);
    MessageBox(NULL,a , "z", 0);
}

bool OgreRender::shipStarted(const Ogre::FrameEvent &evt)
{
	ShipNode->translate(Ogre::Vector3(0.1,0,0));
	return true;
}

void OgreRender::GetShipData(int &x, int &y, int &speed)
{
	Ogre::Vector3 pos = Ogre::Vector3::ZERO;
	pos = ShipNode->getPosition();
	speed = ShipNodeListener->GetSpeed();
	x = pos.x;
	y = pos.z;
}

void OgreRender::GetObjectData(int nObjectID, int &x, int &y, int& h, int &speed, int &vx, int &vy, int &vh)
{
	switch(nObjectID)
	{
	case 0 :
		break;
	case 1 :		 //корабль
		GetObjectDataByPtr(ShipNode, ShipNodeListener, x, y, h, speed, vx, vy, vh);
		break;
	case 2: //самолет
		GetObjectDataByPtr(PlaneNode, PlaneNodeListener, x, y, h, speed, vx, vy, vh);
		break;
	case 3: //ракета
		GetObjectDataByPtr(MissileNode, MissileNodeListener, x, y, h, speed, vx, vy, vh);
		break;
	default :
		break;
	}	
}

void OgreRender::GetObjectDataByPtr(Ogre::SceneNode* _node, ShipListener* _listener, int &x, int &y, int& h, int &speed, int &vx, int &vy, int &vh)
{
	Ogre::Vector3 pos = Ogre::Vector3::ZERO;
	pos = _node->getPosition();
	speed = _listener->GetSpeed();
	x = pos.x;
	y = pos.z;
	h = pos.y;
	vx = 1;
	vy = 1;
	vh = 1;
}

void OgreRender::GetRadarData(int nRadar, int &x, int &y, int& h, int &speed)
{
	Ogre::SceneNode* radar;
	switch(nRadar)
	{
	case 1 :		 
		radar = RadarNode1;
		speed = 0;
		break;
	case 2: 
		radar = RadarNode2;
		speed = 0;
		break;
	case 3: 
		radar = ShipNode;
		speed = ShipNodeListener->GetSpeed();
		break;
	default :
		break;
	}	
	Ogre::Vector3 pos = Ogre::Vector3::ZERO;
	pos = radar->getPosition();
	x = pos.x;
	y = pos.z;
	h = pos.y;	
}

void OgreRender::SetShipDestination(int x, int y, int speed)
{
	SetObjectDestinationByPtr(ShipNode, ShipNodeListener, ShipTrack, x, y, 1020, speed);

	/*ShipNodeListener->SetSpeed(speed);
	Ogre::Vector3 pos = Ogre::Vector3::ZERO;
	pos = ShipNode->getPosition();
	pos.x=x;
	pos.z=y;
	pos.y=1005;
	ShipNodeListener->SetDestination(pos);

	ShipTrack->beginUpdate(0);
	ShipTrack->position(ShipNode->getPosition());
	ShipTrack->position(pos);
	ShipTrack->end();*/

}
	
void OgreRender::SetObjectDestination(int nObjectID,int x, int y, int h, int speed)
{
	switch(nObjectID)
	{
	case 0 :
		break;
	case 1 :		 //корабль
		SetObjectDestinationByPtr(ShipNode, ShipNodeListener, ShipTrack, x, y, h, speed);
		break;
	case 2: //самолет
		SetObjectDestinationByPtr(PlaneNode, PlaneNodeListener, PlaneTrack, x, y, h, speed);
		break;
	case 3: //ракета
		SetObjectDestinationByPtr(MissileNode, MissileNodeListener, MissileTrack, x, y, h, speed);
		break;
	default :
		break;
	}
}

void OgreRender::SetObjectDestinationByPtr(Ogre::SceneNode* _node, ShipListener* _listener,
	                                        ManualObject* _track, int x, int y, int h, int speed)
{
	_listener->SetSpeed(speed);
	Ogre::Vector3 pos = Ogre::Vector3::ZERO;
	pos.x=x;
	pos.z=y;
	pos.y=h;
	_listener->SetDestination(pos);

	_track->beginUpdate(0);
	_track->position(_node->getPosition());
	_track->position(pos);
	_track->end();
}
//-------------------------------------------------------------------------------------
void OgreRender::createFrameListener(void)
{
	// MessageBox(NULL,"createFrameListener" , "x", 0);
	//BaseRender::createFrameListener();
    //BaseRender::createFrameListener();
    //mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
	/*ShipNodeListener = new ShipListener(ShipNode, ShipTrack, 5);
	mRoot->addFrameListener(ShipNodeListener);

	MissileNodeListener = new ShipListener(MissileNode, MissileTrack, 50);
	mRoot->addFrameListener(MissileNodeListener);

	PlaneNodeListener = new ShipListener(PlaneNode, PlaneTrack, 50);
	mRoot->addFrameListener(PlaneNodeListener);*/


}
//-------------------------------------------------------------------------------------
bool OgreRender::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseRender::frameRenderingQueued(evt);
	/*if (mTerrainGroup->isDerivedDataUpdateInProgress())
    {
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported)
        {
            mInfoLabel->setCaption("Building terrain, please wait...");
        }
        else
        {
            mInfoLabel->setCaption("Updating textures, patience...");
        }
    }
    else
    {
        mTrayMgr->removeWidgetFromTray(mInfoLabel);
        mInfoLabel->hide();
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }*/
    return ret;
}

void OgreRender::StartAll()
{
	if(nObjects>0){
	for(int i=0; i!=nObjects; i++)
	{
	    //_objects[i].Start();	
	}}
}

bool OgreRender::keyPressed(int stepX, int stepY, int stepZ)
{
	int z;

	if(nObjects>0){
	for(int i=0; i!=nObjects; i++)
	{
		z = (mCamera->getPosition() - _objects[i].GetPosition()).normalise();

		if(z<=5000)
		{ 
			_objects[i].CurrentScale = _objects[i].baseScale;
		}
		else if(z<=10000)
		{ 
			_objects[i].CurrentScale = _objects[i].Scale2;	
		}
		else
		{
			k=z/5000;
			_objects[i].CurrentScale = _objects[i].Scale2*k;
		}
		_objects[i].SetScale();
	}}

	if( nRadars>0){
	for(int i=0; i!=nRadars; i++)
	{
		z = (mCamera->getPosition() - _radars[i].GetPosition()).normalise();

		if(z<5000)
		{ 
			_radars[i].CurrentScale = _radars[i].baseScale;
		}
		else if(z<10000)
		{ 
			_radars[i].CurrentScale = _radars[i].Scale2;	
		}
		else
		{
			k=z/5000;
			_radars[i].CurrentScale = _radars[i].Scale2*k;
		}
		_radars[i].SetScale();
	}}


	z = mCamera->getPosition().length();
	k = 1;
	
	
	if(z<=5000)
	{ 
		mCamera->setNearClipDistance(0.1);
	}
	else if(z<=10000)
	{ 
		mCamera->setNearClipDistance(10);
	}
	else
	{
		k=z/5000;
		mCamera->setNearClipDistance(150);
	}	

	return BaseRender::keyPressed(stepX, stepY, stepZ);
}
