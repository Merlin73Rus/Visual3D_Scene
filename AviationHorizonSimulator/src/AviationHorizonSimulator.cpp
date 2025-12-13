#include "AviationHorizonSimulator.h"
#include <iostream>
#include <sstream>

AviationHorizonSimulator::AviationHorizonSimulator() :
    mRoot(nullptr),
    mSceneMgr(nullptr),
    mCamera(nullptr),
    mWindow(nullptr),
    mInstrumentPanelNode(nullptr),
    mHorizonManualObj(nullptr),
    mAltimeterManualObj(nullptr),
    mAirspeedManualObj(nullptr),
    mInputManager(nullptr),
    mKeyboard(nullptr),
    mMouse(nullptr),
    mJoyStick(nullptr),
    mPitch(0.0),
    mRoll(0.0),
    mYaw(0.0),
    mVelocity(100.0),
    mAltitude(1000.0),
    mThrottle(0.5),
    mStickX(0.0),
    mStickY(0.0),
    mRudder(0.0),
    mExitApp(false)
{
}

AviationHorizonSimulator::~AviationHorizonSimulator()
{
    if (mJoyStick) {
        mInputManager->destroyInputObject(mJoyStick);
        mJoyStick = nullptr;
    }
    if (mMouse) {
        mInputManager->destroyInputObject(mMouse);
        mMouse = nullptr;
    }
    if (mKeyboard) {
        mInputManager->destroyInputObject(mKeyboard);
        mKeyboard = nullptr;
    }
    if (mInputManager) {
        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = nullptr;
    }

    delete mRoot;
}

void AviationHorizonSimulator::setup()
{
    mRoot = new Ogre::Root("", "", "");
    
    // Configure rendering system
    Ogre::RenderSystem *renderSys = mRoot->getRenderSystemByName("OpenGL 3+");
    if (!renderSys) {
        renderSys = mRoot->getAvailableRenderers().begin()->second;
    }
    mRoot->setRenderSystem(renderSys);
    
    // Initialize root without creating window yet
    mRoot->initialise(false);
    
    // Create window
    Ogre::NameValuePairList params;
    params["title"] = "Aviation Horizon Simulator";
    params["vsync"] = "false";
    mWindow = mRoot->createRenderWindow("Aviation Horizon Simulator", 1200, 800, false, &params);
    
    // Set up scene manager
    mSceneMgr = mRoot->createSceneManager();
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
    
    // Set this class as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    
    createScene();
    setupCamera();
    setupLights();
    setupInstruments();
    setupInput();
    
    // Register as frame listener
    mRoot->addFrameListener(this);
}

void AviationHorizonSimulator::shutdown()
{
    if (mRoot) {
        mRoot->removeFrameListener(this);
        mRoot->shutdown();
    }
}

void AviationHorizonSimulator::createScene()
{
    // Create a simple sky background
    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
    
    // Create cockpit background
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "cockpit_back",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane,
        10, 10, 1, 1,
        true,
        1, 1, 1,
        Ogre::Vector3::UNIT_Z
    );
    
    Ogre::Entity* cockpitBack = mSceneMgr->createEntity("CockpitBack", "cockpit_back");
    cockpitBack->setMaterialName("BaseWhite");
    Ogre::SceneNode* backNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    backNode->attachObject(cockpitBack);
    backNode->setPosition(0, 0, -5); // Position behind the instruments
}

void AviationHorizonSimulator::setupCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);
    mCamera->setAspectRatio(1200.0f / 800.0f);
    
    // Position camera at the pilot's eye level
    mCamera->setPosition(0, 0, 0);
    mCamera->lookAt(0, 0, 10);
}

void AviationHorizonSimulator::setupLights()
{
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(0, 5, 0);
    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(1.0, 1.0, 1.0);
}

void AviationHorizonSimulator::setupInstruments()
{
    // Create instrument panel node
    mInstrumentPanelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mInstrumentPanelNode->setPosition(0, 0, 2); // Position in front of camera
    
    createArtificialHorizon();
    createAltimeter();
    createAirspeedIndicator();
}

void AviationHorizonSimulator::createArtificialHorizon()
{
    // Create manual object for artificial horizon
    mHorizonManualObj = mSceneMgr->createManualObject("ArtificialHorizon");
    mHorizonManualObj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    
    // Draw a circular artificial horizon with pitch and roll indicators
    float radius = 1.5f;
    int segments = 64;
    
    // Center of the horizon
    Ogre::Vector3 center(0, 0, 0);
    
    // Draw the circular outline
    mHorizonManualObj->position(center.x, center.y, center.z);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2 * Ogre::Math::PI * i / segments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        mHorizonManualObj->position(x, y, center.z);
        
        if (i > 0) {
            mHorizonManualObj->triangle(0, i, i == segments ? 1 : i + 1);
        }
    }
    
    mHorizonManualObj->end();
    
    // Attach to instrument panel
    mInstrumentPanelNode->attachObject(mHorizonManualObj);
}

void AviationHorizonSimulator::createAltimeter()
{
    // Create manual object for altimeter
    mAltimeterManualObj = mSceneMgr->createManualObject("Altimeter");
    mAltimeterManualObj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    
    // Draw a circular altimeter
    float radius = 0.5f;
    int segments = 32;
    Ogre::Vector3 center(-1.0f, -1.0f, 0); // Position to the lower left
    
    // Center of the altimeter
    mAltimeterManualObj->position(center.x, center.y, center.z);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2 * Ogre::Math::PI * i / segments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        mAltimeterManualObj->position(x, y, center.z);
        
        if (i > 0) {
            mAltimeterManualObj->triangle(0, i, i == segments ? 1 : i + 1);
        }
    }
    
    mAltimeterManualObj->end();
    
    // Attach to instrument panel
    mInstrumentPanelNode->attachObject(mAltimeterManualObj);
}

void AviationHorizonSimulator::createAirspeedIndicator()
{
    // Create manual object for airspeed indicator
    mAirspeedManualObj = mSceneMgr->createManualObject("AirspeedIndicator");
    mAirspeedManualObj->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    
    // Draw a circular airspeed indicator
    float radius = 0.5f;
    int segments = 32;
    Ogre::Vector3 center(1.0f, -1.0f, 0); // Position to the lower right
    
    // Center of the airspeed indicator
    mAirspeedManualObj->position(center.x, center.y, center.z);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2 * Ogre::Math::PI * i / segments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        mAirspeedManualObj->position(x, y, center.z);
        
        if (i > 0) {
            mAirspeedManualObj->triangle(0, i, i == segments ? 1 : i + 1);
        }
    }
    
    mAirspeedManualObj->end();
    
    // Attach to instrument panel
    mInstrumentPanelNode->attachObject(mAirspeedManualObj);
}

void AviationHorizonSimulator::setupInput()
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    
    mInputManager = OIS::InputManager::createInputSystem(pl);
    
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mJoyStick = nullptr;
    
    // Look for connected joysticks
    if (mInputManager->numJoySticks() > 0) {
        mJoyStick = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, true));
        
        // Set this as the joystick listener
        mJoyStick->setEventCallback(this);
    } else {
        std::cout << "No joystick found!" << std::endl;
    }
    
    mKeyboard->setEventCallback(this);
    mMouse->setEventCallback(this);
}

bool AviationHorizonSimulator::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    if (mWindow->isClosed()) {
        return false;
    }
    
    mKeyboard->capture();
    mMouse->capture();
    if (mJoyStick) {
        mJoyStick->capture();
    }
    
    updateJoystickInput();
    updateFlightParameters(fe);
    updateAircraft(fe);
    updateInstruments();
    
    return !mExitApp;
}

void AviationHorizonSimulator::updateJoystickInput()
{
    if (!mJoyStick) return;
    
    const OIS::JoyStickState &state = mJoyStick->getJoyStickState();
    
    // Map joystick axes to aircraft controls
    // Axis 0: X-axis (typically aileron/roll control)
    mStickX = static_cast<float>(state.mAxes[0].abs) / 32768.0f - 1.0f;
    
    // Axis 1: Y-axis (typically elevator/pitch control) 
    mStickY = static_cast<float>(state.mAxes[1].abs) / 32768.0f - 1.0f;
    
    // Axis 2: Rudder (typically Z-axis or slider)
    if (state.mAxes.size() > 2) {
        mRudder = static_cast<float>(state.mAxes[2].abs) / 32768.0f - 1.0f;
    }
    
    // Throttle might be on a slider or another axis
    if (state.mAxes.size() > 3) {
        mThrottle = static_cast<float>(state.mAxes[3].abs) / 65536.0f; // 0 to 1 range
    }
}

void AviationHorizonSimulator::updateFlightParameters(const Ogre::FrameEvent& fe)
{
    const float dt = fe.timeSinceLastFrame;
    const float pitchRate = 0.5f;  // rad/s
    const float rollRate = 1.0f;   // rad/s
    const float yawRate = 0.3f;    // rad/s
    
    // Update angular rates based on stick positions
    float pitchChange = -mStickY * pitchRate * dt;  // Negative because y-axis is inverted
    float rollChange = mStickX * rollRate * dt;
    float yawChange = mRudder * yawRate * dt;
    
    // Apply limits to prevent excessive attitude changes
    mPitch += pitchChange;
    mRoll += rollChange;
    mYaw += yawChange;
    
    // Apply limits
    mPitch = std::max(-Ogre::Math::PI/2, std::min(Ogre::Math::PI/2, mPitch));
    mRoll = std::max(-Ogre::Math::PI/2, std::min(Ogre::Math::PI/2, mRoll));
    
    // Update velocity based on throttle and pitch
    float thrust = (mThrottle - 0.5f) * 20.0f;  // -10 to +10 m/s^2
    float drag = -0.1f * mVelocity;             // Simple drag model
    float netForce = thrust + drag;
    mVelocity += netForce * dt;
    mVelocity = std::max(10.0f, mVelocity);  // Minimum speed to keep flying
    
    // Calculate altitude change based on pitch
    float altitudeChange = mVelocity * sin(mPitch) * dt;
    mAltitude += altitudeChange;
    mAltitude = std::max(10.0f, mAltitude);  // Don't go underground
}

void AviationHorizonSimulator::updateAircraft(const Ogre::FrameEvent& fe)
{
    // In this simulator, we're not moving the aircraft in 3D space
    // We're just updating the flight parameters to reflect on instruments
}

void AviationHorizonSimulator::updateInstruments()
{
    // Update artificial horizon based on pitch and roll
    if (mHorizonManualObj) {
        // The artificial horizon display would be updated here
        // This would involve creating a texture or updating the manual object
        // to show the horizon line at the correct pitch and roll angle
    }
    
    // Update altimeter based on altitude
    if (mAltimeterManualObj) {
        // The altimeter display would be updated here
    }
    
    // Update airspeed indicator based on velocity
    if (mAirspeedManualObj) {
        // The airspeed indicator would be updated here
    }
}

void AviationHorizonSimulator::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
    
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void AviationHorizonSimulator::windowClosed(Ogre::RenderWindow* rw)
{
    if (rw == mWindow) {
        mExitApp = true;
    }
}

bool AviationHorizonSimulator::keyPressed(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_ESCAPE) {
        mExitApp = true;
    }
    return true;
}

bool AviationHorizonSimulator::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}

bool AviationHorizonSimulator::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}

bool AviationHorizonSimulator::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool AviationHorizonSimulator::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool AviationHorizonSimulator::buttonPressed(const OIS::JoyStickEvent &arg, int button)
{
    if (button == 0) {  // Typically trigger button
        mExitApp = true;
    }
    return true;
}

bool AviationHorizonSimulator::buttonReleased(const OIS::JoyStickEvent &arg, int button)
{
    return true;
}

bool AviationHorizonSimulator::axisMoved(const OIS::JoyStickEvent &arg, int axis)
{
    return true;
}

bool AviationHorizonSimulator::povMoved(const OIS::JoyStickEvent &arg, int pov)
{
    return true;
}