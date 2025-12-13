#include "TailViewAircraftSimulator.h"
#include <iostream>
#include <sstream>

TailViewAircraftSimulator::TailViewAircraftSimulator() :
    mRoot(nullptr),
    mSceneMgr(nullptr),
    mCamera(nullptr),
    mWindow(nullptr),
    mAircraftEntity(nullptr),
    mAircraftNode(nullptr),
    mCameraNode(nullptr),
    mInputManager(nullptr),
    mKeyboard(nullptr),
    mMouse(nullptr),
    mJoyStick(nullptr),
    mPitch(0.0),
    mRoll(0.0),
    mYaw(0.0),
    mVelocity(50.0),
    mAltitude(1000.0),
    mThrottle(0.5),
    mStickX(0.0),
    mStickY(0.0),
    mRudder(0.0),
    mExitApp(false)
{
}

TailViewAircraftSimulator::~TailViewAircraftSimulator()
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

void TailViewAircraftSimulator::setup()
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
    params["title"] = "Aircraft Tail View Simulator";
    params["vsync"] = "false";
    mWindow = mRoot->createRenderWindow("Aircraft Tail View Simulator", 1200, 800, false, &params);
    
    // Set up scene manager
    mSceneMgr = mRoot->createSceneManager();
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    // Set this class as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    
    createScene();
    setupCamera();
    setupLights();
    setupInput();
    
    // Register as frame listener
    mRoot->addFrameListener(this);
}

void TailViewAircraftSimulator::shutdown()
{
    if (mRoot) {
        mRoot->removeFrameListener(this);
        mRoot->shutdown();
    }
}

void TailViewAircraftSimulator::createScene()
{
    // Create ground plane
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "ground",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane,
        20000, 20000, 20, 20,
        true,
        1, 500, 500,
        Ogre::Vector3::UNIT_Z
    );
    
    Ogre::Entity* groundEntity = mSceneMgr->createEntity("GroundEntity", "ground");
    groundEntity->setMaterialName("BaseWhite");
    groundEntity->setCastShadows(false);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    
    // Create sky
    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
    
    // Create a basic aircraft model using primitives
    createAircraftModel();
}

void TailViewAircraftSimulator::createAircraftModel()
{
    // For now, we'll create a simple box to represent the aircraft
    // In a real implementation, we would load a 3D aircraft model
    Ogre::Entity* fuselage = mSceneMgr->createEntity("fuselage.mesh"); // Placeholder
    
    // If the mesh doesn't exist, create a simple representation
    try {
        mAircraftEntity = mSceneMgr->createEntity("fuselage.mesh");
    } catch (...) {
        // Create a simple aircraft representation using basic geometry
        mAircraftEntity = mSceneMgr->createEntity("AircraftBody", Ogre::SceneManager::PT_CUBE);
        mAircraftEntity->setScale(3.0, 0.8, 1.0); // Make it look like a fuselage
    }
    
    mAircraftNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mAircraftNode->attachObject(mAircraftEntity);
    mAircraftNode->setPosition(0, mAltitude, 0);
    
    // Add wings and tail as separate entities for better visualization
    try {
        Ogre::Entity* wingEntity = mSceneMgr->createEntity("wing.mesh");
        Ogre::SceneNode* wingNode = mAircraftNode->createChildSceneNode("Wings");
        wingNode->attachObject(wingEntity);
    } catch (...) {
        // Create simple wing representation
        Ogre::Entity* wingEntity = mSceneMgr->createEntity("AircraftWings", Ogre::SceneManager::PT_CUBE);
        wingEntity->setScale(0.2, 6.0, 1.0);
        Ogre::SceneNode* wingNode = mAircraftNode->createChildSceneNode("Wings");
        wingNode->attachObject(wingEntity);
        wingNode->setPosition(0, 0.2, 0);
    }
}

void TailViewAircraftSimulator::setupCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setNearClipDistance(5);
    mCamera->setFarClipDistance(50000);
    
    // Set up camera to follow aircraft from tail view
    mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CameraNode");
    mCameraNode->attachObject(mCamera);
    
    // Position camera behind and above the aircraft
    mCameraNode->setPosition(0, mAltitude + 10, 20);
    mCameraNode->lookAt(mAircraftNode->_getDerivedPosition(), Ogre::Node::TS_WORLD);
}

void TailViewAircraftSimulator::setupLights()
{
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);
    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(1.0, 1.0, 1.0);
}

void TailViewAircraftSimulator::setupInput()
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

bool TailViewAircraftSimulator::frameRenderingQueued(const Ogre::FrameEvent& fe)
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
    
    // Update camera position to maintain tail view
    Ogre::Vector3 aircraftPos = mAircraftNode->_getDerivedPosition();
    Ogre::Quaternion aircraftOrientation = mAircraftNode->_getDerivedOrientation();
    
    // Calculate camera position behind the aircraft
    Ogre::Vector3 offset(-20, 10, 0); // Start with offset in aircraft local space
    Ogre::Vector3 worldOffset = aircraftOrientation * offset;
    mCameraNode->setPosition(aircraftPos + worldOffset);
    
    // Make camera look at aircraft
    mCameraNode->setFixedYawAxis(true);
    mCameraNode->lookAt(aircraftPos, Ogre::Node::TS_WORLD);
    
    return !mExitApp;
}

void TailViewAircraftSimulator::updateJoystickInput()
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

void TailViewAircraftSimulator::updateFlightParameters(const Ogre::FrameEvent& fe)
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

void TailViewAircraftSimulator::updateAircraft(const Ogre::FrameEvent& fe)
{
    // Update aircraft orientation
    Ogre::Quaternion pitchQuat = Ogre::Quaternion(Ogre::Radian(mPitch), Ogre::Vector3::UNIT_X);
    Ogre::Quaternion rollQuat = Ogre::Quaternion(Ogre::Radian(mRoll), Ogre::Vector3::UNIT_Y);
    Ogre::Quaternion yawQuat = Ogre::Quaternion(Ogre::Radian(mYaw), Ogre::Vector3::UNIT_Z);
    
    // Combine rotations: apply in the order yaw, pitch, roll
    Ogre::Quaternion orientation = yawQuat * pitchQuat * rollQuat;
    mAircraftNode->setOrientation(orientation);
    
    // Calculate movement vector based on current orientation and velocity
    Ogre::Vector3 forward = orientation * Ogre::Vector3::NEGATIVE_UNIT_Z;
    Ogre::Vector3 movement = forward * mVelocity * fe.timeSinceLastFrame;
    
    // Update position
    Ogre::Vector3 newPos = mAircraftNode->getPosition();
    newPos += movement;
    newPos.y = mAltitude; // Keep altitude consistent
    mAircraftNode->setPosition(newPos);
}

void TailViewAircraftSimulator::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
    
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void TailViewAircraftSimulator::windowClosed(Ogre::RenderWindow* rw)
{
    if (rw == mWindow) {
        mExitApp = true;
    }
}

bool TailViewAircraftSimulator::keyPressed(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_ESCAPE) {
        mExitApp = true;
    }
    return true;
}

bool TailViewAircraftSimulator::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}

bool TailViewAircraftSimulator::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}

bool TailViewAircraftSimulator::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool TailViewAircraftSimulator::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool TailViewAircraftSimulator::buttonPressed(const OIS::JoyStickEvent &arg, int button)
{
    if (button == 0) {  // Typically trigger button
        mExitApp = true;
    }
    return true;
}

bool TailViewAircraftSimulator::buttonReleased(const OIS::JoyStickEvent &arg, int button)
{
    return true;
}

bool TailViewAircraftSimulator::axisMoved(const OIS::JoyStickEvent &arg, int axis)
{
    return true;
}

bool TailViewAircraftSimulator::povMoved(const OIS::JoyStickEvent &arg, int pov)
{
    return true;
}