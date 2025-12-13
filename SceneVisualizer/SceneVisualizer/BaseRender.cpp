#include "StdAfx.h"
#include "BaseRender.h"


//-------------------------------------------------------------------------------------
BaseRender::BaseRender(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    //mTrayMgr(0),
    //mCameraMan(0),
    //mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false)//,
    //mInputManager(0),
    //mMouse(0),
    //mKeyboard(0)
{
}

//-------------------------------------------------------------------------------------
BaseRender::~BaseRender(void)
{
    //if (mTrayMgr) delete mTrayMgr;
    //if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    //Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
bool BaseRender::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    /*if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        //mWindow = mRoot->initialise(true, "TutorialApplication Render Window");
		mWindow = mRoot->initialise(false, "TutorialApplication Render Window");
		
		Ogre::NameValuePairList Options;
    Options["externalWindowHandle"] = Ogre::StringConverter::toString(Handle);
 
    mWindow = mRoot->createRenderWindow("EditorWindow", 100, 100, false, &Options);
    mWindow->setActive(true);
        return true;
    }
    else
    {
        return false;
    }
	*/
	Ogre::RenderSystemList rs = mRoot->getAvailableRenderers(); 
	if(rs.size() && rs.at(0)->getName().compare("RenderSystem_Direct3D9")) //if(rs.size() && rs.at(0)->getName().compare("RenderSystem_Direct3D9"))
	{
		Ogre::RenderSystem * r=rs.at(0); 
		mRoot->setRenderSystem(r);
		r->setConfigOption("Full Screen","No"); 
		r->setConfigOption("Video Mode","1280 x 1024 @ 32-bit colour"); 
		r->validateConfigOptions();
	}
	else { return false;}

	mWindow = mRoot->initialise(false, "3D Scene Visualizer Render Window");
		
	Ogre::NameValuePairList Options;
    Options["externalWindowHandle"] = Ogre::StringConverter::toString(Handle);
 
    mWindow = mRoot->createRenderWindow("EditorWindow", 100, 100, false, &Options);
    mWindow->setActive(true);

	return true;

}
//-------------------------------------------------------------------------------------
void BaseRender::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "CEditorSceneManager");
}
//-------------------------------------------------------------------------------------
void BaseRender::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
	MoveVector = Ogre::Vector3(0,10,500);
    mCamera->setPosition(MoveVector);
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);

    //mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
/*void BaseRender::createFrameListener(void)
{

    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = Handle;
    std::ostringstream windowHndStr;

	

    mWindow->getCustomAttribute("WINDOW", &windowHnd); //WINDOW
	
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );
	
    long han = mInputManager->getNumberOfDevices(OIS::OISKeyboard);
	
	mInputManager->createInputObject( OIS::OISKeyboard, true );
	
	try
	{

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, false ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();
	

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
	}
	catch(...)
	{
	}
	size_t windowHnd = Handle;
	mWindow->getCustomAttribute("WINDOW", &windowHnd); //WINDOW
	//Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
   // mRoot->addFrameListener(this);


}*/
//-------------------------------------------------------------------------------------
void BaseRender::destroyScene(void)
{
	
}
//-------------------------------------------------------------------------------------
void BaseRender::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseRender::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void BaseRender::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseRender::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseRender::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    if (!setup())
        return;   

    // clean up
    //destroyScene();
}
void BaseRender::ReStartRendering(void)
{
	mRoot->startRendering();
}
void BaseRender::StartRendering()
{
	mRoot->startRendering();
}
//-------------------------------------------------------------------------------------
bool BaseRender::setup(void)
{

    mRoot = new Ogre::Root(mPluginsCfg);
    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();

    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    //createFrameListener();


    return true;
};
//-------------------------------------------------------------------------------------
bool BaseRender::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	MessageBox(NULL,"frameRenderingQueued" , "frameRenderingQueued", 0);
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    //Need to capture/update each device
   /* mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }*/

    return true;
}
//-------------------------------------------------------------------------------------
/*bool BaseRender::keyPressed( const OIS::KeyEvent &arg )
{
    return true;
}

bool BaseRender::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);
    return true;
}*/

bool BaseRender::keyPressed(int stepX, int stepY, int stepZ)
{
	MoveVector=Ogre::Vector3::ZERO;
	MoveVector.x = stepX;
	MoveVector.y = stepY;
	MoveVector.z = stepZ*k;
	mCamera->moveRelative(MoveVector);
	return true;
}

/*bool BaseRender::mouseMoved( const OIS::MouseEvent &arg )
{
    return true;
}*/

bool BaseRender::mouseMoved( int x, int y , int d)
{

	Ogre::Radian angle1(x*0.002);
	Ogre::Radian angle2 (y*0.002);
	
	mCamera->yaw(angle1);
	mCamera->pitch(angle2);
	
	if(d!=0)
	{
		Ogre::Vector3 vec = d*mCamera->getUp();
		mCamera->moveRelative(mCamera->getPosition() + mCamera->getOrientation()*vec);
	}
	
	
	//windowResized(mWindow);

    return true;
}

/*bool BaseRender::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}*/

/*bool BaseRender::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    //if (mTrayMgr->injectMouseUp(arg, id)) return true;
    //mCameraMan->injectMouseUp(arg, id);
    return true;
}*/

//Adjust mouse clipping area
void BaseRender::windowResized(Ogre::RenderWindow* rw)
{
    /*unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;*/
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseRender::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    /*if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }*/
}