#ifndef AVIATIONHORIZONSIMULATOR_H
#define AVIATIONHORIZONSIMULATOR_H

#include <OGRE/Ogre.h>
#include <OGRE/OgreApplicationContext.h>
#include <OGRE/OgreInput.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <vector>

class AviationHorizonSimulator : public Ogre::FrameListener,
                                 public Ogre::WindowEventListener,
                                 public OIS::KeyListener,
                                 public OIS::MouseListener,
                                 public OIS::JoyStickListener
{
public:
    AviationHorizonSimulator();
    virtual ~AviationHorizonSimulator();

    void setup();
    void shutdown();

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

    // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // OIS::JoyStickListener
    virtual bool buttonPressed(const OIS::JoyStickEvent &arg, int button);
    virtual bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
    virtual bool axisMoved(const OIS::JoyStickEvent &arg, int axis);
    virtual bool povMoved(const OIS::JoyStickEvent &arg, int pov);

private:
    void createScene();
    void setupCamera();
    void setupLights();
    void setupInstruments();
    void setupInput();
    void updateAircraft(const Ogre::FrameEvent& fe);
    void updateFlightParameters(const Ogre::FrameEvent& fe);
    void updateJoystickInput();
    void updateInstruments();
    void createArtificialHorizon();
    void createAltimeter();
    void createAirspeedIndicator();
    
    Ogre::Root* mRoot;
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
    Ogre::RenderWindow* mWindow;
    Ogre::SceneNode* mInstrumentPanelNode;
    
    // Instrument displays
    Ogre::ManualObject* mHorizonManualObj;
    Ogre::ManualObject* mAltimeterManualObj;
    Ogre::ManualObject* mAirspeedManualObj;
    
    OIS::InputManager* mInputManager;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;
    OIS::JoyStick* mJoyStick;

    // Flight parameters
    Ogre::Real mPitch;      // in radians
    Ogre::Real mRoll;       // in radians
    Ogre::Real mYaw;        // in radians
    Ogre::Real mVelocity;   // in m/s
    Ogre::Real mAltitude;   // in meters
    
    // Control inputs from joystick
    float mThrottle;
    float mStickX;  // Aileron control (-1 to 1)
    float mStickY;  // Elevator control (-1 to 1)
    float mRudder;  // Rudder control (-1 to 1)
    
    bool mExitApp;
};

#endif // AVIATIONHORIZONSIMULATOR_H