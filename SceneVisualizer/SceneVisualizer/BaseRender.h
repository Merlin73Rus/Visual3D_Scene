#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

//#include <OISEvents.h>
//#include <OISInputManager.h>
//#include <OISKeyboard.h>
//#include <OISMouse.h>

//#include <SdkTrays.h>
//#include <SdkCameraMan.h>

#include "OgreMeshManager.h"
#include "OgreManualObject.h"

class BaseRender : public Ogre::FrameListener/*, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener*/
{
public:
    BaseRender(void);
    virtual ~BaseRender(void);

    virtual void go(void);	
	virtual void createFrameListener(void){};
	bool mouseMoved( int x, int y, int d );
	bool keyPressed(int stepX, int stepY, int stepZ);
	void ReStartRendering(void);
	void StartRendering();
	int k;

	void Message(int Data)
	{
		char a[255];
		itoa(Data, a , 10);
		//MessageBox(NULL, a, "Data", 0);
	}

protected:
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
	
   // virtual bool keyPressed( const OIS::KeyEvent &arg );
   // virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    //virtual bool mouseMoved( const OIS::MouseEvent &arg );
	
    //virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    //virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

	long Handle;

    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
	Ogre::Vector3 MoveVector;

    // OgreBites
    //OgreBites::SdkTrayManager* mTrayMgr;
    //OgreBites::SdkCameraMan* mCameraMan;       // basic camera controller
    //OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;

    //OIS Input devices
    //OIS::InputManager* mInputManager;
    //OIS::Mouse*    mMouse;
    //OIS::Keyboard* mKeyboard;
};

