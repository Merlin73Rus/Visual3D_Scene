#include "stdafx.h"
#include "OgreFlightRender.h"

OgreFlightRender::OgreFlightRender() :
    mRoot(0),
    mSceneMgr(0),
    mWindow(0),
    mCamera(0),
    mViewport(0),
    mAircraftEntity(0),
    mAircraftNode(0),
    mLight(0),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mGUIRenderer(0),
    mSheet(0),
    mPositionX(0.0), mPositionY(1000.0), mPositionZ(0.0),  // Start at 1000m altitude
    mPitch(0.0), mRoll(0.0), mYaw(0.0),                    // Level flight
    mVelocityX(0.0), mVelocityY(0.0), mVelocityZ(0.0),
    mAltitude(1000.0), mAirspeed(150.0), mHeading(0.0), mVerticalSpeed(0.0),
    mPitchInput(0.0), mRollInput(0.0), mYawInput(0.0), mThrottle(0.5),  // Half throttle
    mCurrentStage(1),                                       // Stage 1: Familiarization
    mWeatherCondition(0),                                   // Clear weather
    mTimeOfDay(12),                                         // Noon
    mShowInstruments(true),                                 // Show instruments initially
    m_bShutDown(false),
    m_bRender(false),
    m_bPause(false),
    m_fFPS(60.0),
    m_fSecsPerFrame(1.0/60.0),
    m_nWidth(800),
    m_nHeight(600),
    m_sTitle("Aviation Spatial Orientation Training")
{
}

OgreFlightRender::~OgreFlightRender()
{
    if (mWindow)
        mWindow->removeAllViewports();

    if (mSceneMgr)
        mRoot->destroySceneManager(mSceneMgr);

    if (mWindow)
        mRoot->detachRenderTarget(mWindow);

    if (mInputManager)
    {
        mInputManager->destroyInputObject(mMouse);
        mInputManager->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = 0;
    }

    if(mGUIRenderer)
    {
        CEGUI::System::getSingleton().setRenderer(0);
        delete mGUIRenderer;
    }

    if (mRoot)
        delete mRoot;
}

void OgreFlightRender::Init(long hWnd)
{
    mRoot = new Ogre::Root();
    
    // Configure OGRE
    setupResources();
    bool carryOn = mRoot->restoreConfig();
    if (!carryOn)
    {
        mRoot->showConfigDialog();
    }
    
    mWindow = mRoot->initialise(true, m_sTitle);
    mWindow->resize(m_nWidth, m_nHeight);
    mWindow->windowMovedOrResized();
    
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    
    // Create camera
    createCamera();
    
    // Create viewport
    createViewports();
    
    // Set default scene manager
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    // Create light
    mLight = mSceneMgr->createLight("MainLight");
    mLight->setPosition(20, 80, 50);
    
    // Initialize input system
    createInputSystem(mWindow);
    
    // Create the scene
    createScene();
    
    // Initialize GUI
    mGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
        (CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("schemas", "../../../Samples/media/schemas/");
    rp->setResourceGroupDirectory("imagesets", "../../../Samples/media/imagesets/");
    rp->setResourceGroupDirectory("fonts", "../../../Samples/media/fonts/");
    rp->setResourceGroupDirectory("layouts", "../../../Samples/media/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "../../../Samples/media/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "../../../Samples/media/lua_scripts/");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    mSheet = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("training_screen.layout");
    CEGUI::System::getSingleton().setGUISheet(mSheet);
    
    // Enable flight instruments based on training stage
    createFlightInstruments();
}

void OgreFlightRender::StartRendering()
{
    m_bRender = true;
    mRoot->addFrameListener(this);
}

void OgreFlightRender::StopRendering()
{
    m_bRender = false;
    mRoot->removeFrameListener(this);
}

void OgreFlightRender::ReStartRendering()
{
    StopRendering();
    StartRendering();
}

void OgreFlightRender::Resize(int width, int height)
{
    if(mWindow)
    {
        m_nWidth = width;
        m_nHeight = height;
        mWindow->resize(width, height);
        mWindow->windowMovedOrResized();
        
        // Update viewport aspect ratio
        if(mViewport)
        {
            mViewport->setDimensions(0, 0, 1, 1);
        }
    }
}

void OgreFlightRender::Update()
{
    if(m_bRender && !m_bShutDown)
    {
        mRoot->renderOneFrame();
    }
}

bool OgreFlightRender::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mWindow->isClosed())
        return false;

    // Capture/update input devices
    mKeyboard->capture();
    mMouse->capture();

    // Update aircraft physics
    updateAircraft(evt.timeSinceLastFrame);
    
    // Update flight instruments display
    updateFlightInstruments();
    
    // Update environment based on settings
    updateEnvironment();
    
    return true;
}

bool OgreFlightRender::frameEnded(const Ogre::FrameEvent& evt)
{
    return true;
}

void OgreFlightRender::createFrameListener()
{
    // Already handled in StartRendering
}

void OgreFlightRender::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

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

void OgreFlightRender::createCamera(void)
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Ogre::Vector3(0, 1000, 0));  // Start at 1000m altitude
    mCamera->lookAt(Ogre::Vector3(0, 1000, -100));    // Look forward
    mCamera->setNearClipDistance(5);
}

void OgreFlightRender::createViewports(void)
{
    mViewport = mWindow->addViewport(mCamera);
    mViewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.3, 0.6));
    mCamera->setAspectRatio(
        Ogre::Real(mViewport->getActualWidth()) / 
        Ogre::Real(mViewport->getActualHeight()));
}

void OgreFlightRender::createInputSystem(Ogre::RenderWindow* win)
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    win->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));

    unsigned int width, height, depth;
    int left, top;
    win->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
}

void OgreFlightRender::createScene(void)
{
    // Create sky
    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    
    // Create ground plane
    Ogre::Plane plane;
    plane.d = 0; // Ground level at Y=0
    plane.normal = Ogre::Vector3::UNIT_Y; // Normal pointing up
    
    Ogre::MeshManager::getSingleton().createPlane("ground", 
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
        plane, 20000, 20000, 20, 20, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);
        
    Ogre::Entity* groundEntity = mSceneMgr->createEntity("GroundEntity", "ground");
    groundEntity->setMaterialName("Examples/Rockwall");
    mSceneMgr->getRootSceneNode()->attachObject(groundEntity);
    
    // Create aircraft model (placeholder - would use real aircraft model in practice)
    mAircraftEntity = mSceneMgr->createEntity("Aircraft", "ogrehead.mesh");  // Placeholder
    mAircraftNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("AircraftNode");
    mAircraftNode->attachObject(mAircraftEntity);
    mAircraftNode->setScale(0.5, 0.5, 0.5);  // Scale down the placeholder model
    
    // Position aircraft at starting position
    mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
    
    // Adjust camera based on training stage
    updateCameraForStage();
}

void OgreFlightRender::updateAircraft(float deltaTime)
{
    // Calculate forces and moments based on control inputs
    double pitchRate = mPitchInput * 0.5;  // Simplified model
    double rollRate = mRollInput * 0.8;
    double yawRate = mYawInput * 0.3;
    
    // Update attitude
    mPitch += pitchRate * deltaTime;
    mRoll += rollRate * deltaTime;
    mYaw += yawRate * deltaTime;
    
    // Normalize angles
    if (mPitch > Ogre::Math::PI) mPitch -= 2 * Ogre::Math::PI;
    if (mPitch < -Ogre::Math::PI) mPitch += 2 * Ogre::Math::PI;
    if (mRoll > Ogre::Math::PI) mRoll -= 2 * Ogre::Math::PI;
    if (mRoll < -Ogre::Math::PI) mRoll += 2 * Ogre::Math::PI;
    if (mYaw > Ogre::Math::PI) mYaw -= 2 * Ogre::Math::PI;
    if (mYaw < -Ogre::Math::PI) mYaw += 2 * Ogre::Math::PI;
    
    // Calculate velocity vector in world coordinates
    double speed = mAirspeed / 3.6; // Convert km/h to m/s
    double velX = speed * cos(mPitch) * sin(mYaw);
    double velZ = speed * cos(mPitch) * cos(mYaw);
    double velY = speed * sin(mPitch);
    
    // Update position
    mPositionX += velX * deltaTime;
    mPositionY += velY * deltaTime;
    mPositionZ += velZ * deltaTime;
    
    // Update altitude
    mAltitude = mPositionY;
    
    // Simple altitude-dependent airspeed adjustment
    if (mAltitude < 100) {  // Prevent crashing into ground
        mPositionY = 100;
        mAltitude = 100;
        mPitch = abs(mPitch) > 0.1 ? mPitch * 0.9 : 0;  // Correct pitch if near ground
    }
    
    // Update aircraft node position and orientation
    if (mAircraftNode) {
        mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
        mAircraftNode->resetOrientation();
        mAircraftNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(mYaw));
        mAircraftNode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(mPitch));
        mAircraftNode->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(mRoll));
    }
    
    // Update camera to follow aircraft appropriately based on stage
    updateCameraForStage();
}

void OgreFlightRender::updateCameraForStage()
{
    if (!mAircraftNode || !mCamera) return;
    
    Ogre::Vector3 aircraftPos = mAircraftNode->getPosition();
    
    switch(mCurrentStage) {
        case 1:  // Familiarization: View from behind aircraft
            mCamera->setPosition(aircraftPos + Ogre::Vector3(-20, 5, 20));
            mCamera->lookAt(aircraftPos);
            break;
        case 2:  // Basic maneuvers: Closer view from behind
            mCamera->setPosition(aircraftPos + Ogre::Vector3(-10, 2, 10));
            mCamera->lookAt(aircraftPos);
            break;
        case 3:  // Visual flight rules: From cockpit perspective
            mCamera->setPosition(aircraftPos);
            mCamera->setOrientation(mAircraftNode->getOrientation());
            break;
        case 4:  // Transition to instruments: Mixed view
            mCamera->setPosition(aircraftPos + Ogre::Vector3(0, 2, 10));
            mCamera->lookAt(aircraftPos);
            break;
        case 5:  // Instrument flight: Cockpit view with instruments
            mCamera->setPosition(aircraftPos);
            mCamera->setOrientation(mAircraftNode->getOrientation());
            break;
    }
}

void OgreFlightRender::updateFlightInstruments()
{
    // Update instrument readings based on current flight state
    // These would be displayed on the GUI
    mAltitude = mPositionY;  // Altitude above ground level
    mHeading = mYaw * 180.0 / Ogre::Math::PI;  // Convert to degrees
    if (mHeading < 0) mHeading += 360;  // Normalize to 0-360 range
    
    // Vertical speed in meters per second
    mVerticalSpeed = mVelocityY;
    
    // Update CEGUI elements with current values
    if (mShowInstruments) {
        std::stringstream ss;
        ss << "Altitude: " << (int)mAltitude << " m";
        // In a real implementation, we would update the CEGUI elements
        // CEGUI::Window* altWin = CEGUI::WindowManager::getSingleton().getWindow("AltitudeDisplay");
        // altWin->setText(ss.str());
    }
}

void OgreFlightRender::createFlightInstruments()
{
    // Create flight instruments based on the current training stage
    // In a real implementation, this would create CEGUI elements for each instrument
    // depending on the training requirements of each stage
    switch(mCurrentStage) {
        case 1:  // Familiarization: Basic position visualization
            mShowInstruments = false;  // Focus on visual scene
            break;
        case 2:  // Basic maneuvers: Simple instruments
            mShowInstruments = true;
            break;
        case 3:  // Visual flight rules: Standard VFR instruments
            mShowInstruments = true;
            break;
        case 4:  // Transition: Mixed display
            mShowInstruments = true;
            break;
        case 5:  // Instrument flight: Full instrument panel
            mShowInstruments = true;
            break;
    }
}

void OgreFlightRender::updateEnvironment()
{
    // Update environment based on weather and time of day settings
    updateWeather();
}

void OgreFlightRender::updateWeather()
{
    // Apply weather effects based on mWeatherCondition
    switch(mWeatherCondition) {
        case 0:  // Clear
            mSceneMgr->setFog(Ogre::FOG_NONE);
            break;
        case 1:  // Light clouds
            mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::White, 0.0001, 5000, 8000);
            break;
        case 2:  // Heavy clouds/fog
            mSceneMgr->setFog(Ogre::FOG_EXP, Ogre::ColourValue(0.8, 0.8, 0.8), 0.001);
            break;
    }
    
    // Adjust lighting based on time of day
    float lightIntensity = 0.3f;  // Base ambient
    if (mTimeOfDay >= 6 && mTimeOfDay <= 18) {
        // Daytime - brighter
        lightIntensity = 0.5f + 0.4f * (abs(12 - mTimeOfDay) / 12.0f);  // Brightest at noon
    } else {
        // Nighttime - darker
        lightIntensity = 0.1f;
    }
    
    mSceneMgr->setAmbientLight(Ogre::ColourValue(lightIntensity, lightIntensity, lightIntensity));
}

bool OgreFlightRender::mouseMoved( const OIS::MouseEvent &arg )
{
    // Handle mouse movement for camera control during certain stages
    if (mCurrentStage <= 3) {  // Allow external camera control in early stages
        if (arg.state.buttonDown(OIS::MB_Left)) {
            // Rotate camera around aircraft
            if (mAircraftNode && mCamera) {
                Ogre::Vector3 relPos = mCamera->getPosition() - mAircraftNode->getPosition();
                Ogre::Quaternion rotX, rotY;
                
                rotX.FromAngleAxis(Ogre::Degree(-arg.state.X.rel * 0.1), Ogre::Vector3::UNIT_Y);
                rotY.FromAngleAxis(Ogre::Degree(-arg.state.Y.rel * 0.1), relPos.perpendicular().normalisedCopy());
                
                relPos = rotX * rotY * relPos;
                mCamera->setPosition(mAircraftNode->getPosition() + relPos);
                mCamera->lookAt(mAircraftNode->getPosition());
            }
        }
    }
    
    return true;
}

bool OgreFlightRender::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}

bool OgreFlightRender::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}

bool OgreFlightRender::keyPressed( const OIS::KeyEvent &arg )
{
    // Handle keyboard input for aircraft control
    switch(arg.key) {
        case OIS::KC_W:  // Pitch up
            mPitchInput = 1.0;
            break;
        case OIS::KC_S:  // Pitch down
            mPitchInput = -1.0;
            break;
        case OIS::KC_A:  // Roll left
            mRollInput = -1.0;
            break;
        case OIS::KC_D:  // Roll right
            mRollInput = 1.0;
            break;
        case OIS::KC_Q:  // Yaw left
            mYawInput = -1.0;
            break;
        case OIS::KC_E:  // Yaw right
            mYawInput = 1.0;
            break;
        case OIS::KC_R:  // Increase throttle
            mThrottle = std::min(1.0, mThrottle + 0.1);
            mAirspeed = 100 + mThrottle * 200;  // Map throttle to airspeed
            break;
        case OIS::KC_F:  // Decrease throttle
            mThrottle = std::max(0.0, mThrottle - 0.1);
            mAirspeed = 100 + mThrottle * 200;  // Map throttle to airspeed
            break;
        case OIS::KC_SPACE:  // Reset aircraft position
            ResetFlight();
            break;
        case OIS::KC_1:  // Switch to stage 1
            SetTrainingStage(1);
            break;
        case OIS::KC_2:  // Switch to stage 2
            SetTrainingStage(2);
            break;
        case OIS::KC_3:  // Switch to stage 3
            SetTrainingStage(3);
            break;
        case OIS::KC_4:  // Switch to stage 4
            SetTrainingStage(4);
            break;
        case OIS::KC_5:  // Switch to stage 5
            SetTrainingStage(5);
            break;
        case OIS::KC_I:  // Toggle instruments
            mShowInstruments = !mShowInstruments;
            break;
    }
    
    return true;
}

bool OgreFlightRender::keyReleased( const OIS::KeyEvent &arg )
{
    // Release controls when keys are released
    switch(arg.key) {
        case OIS::KC_W:
        case OIS::KC_S:
            if (!(mKeyboard->isKeyDown(OIS::KC_W) || mKeyboard->isKeyDown(OIS::KC_S))) {
                mPitchInput = 0.0;
            }
            break;
        case OIS::KC_A:
        case OIS::KC_D:
            if (!(mKeyboard->isKeyDown(OIS::KC_A) || mKeyboard->isKeyDown(OIS::KC_D))) {
                mRollInput = 0.0;
            }
            break;
        case OIS::KC_Q:
        case OIS::KC_E:
            if (!(mKeyboard->isKeyDown(OIS::KC_Q) || mKeyboard->isKeyDown(OIS::KC_E))) {
                mYawInput = 0.0;
            }
            break;
    }
    
    return true;
}

void OgreFlightRender::GetAircraftPosition(double &x, double &y, double &z)
{
    x = mPositionX;
    y = mPositionY;
    z = mPositionZ;
}

void OgreFlightRender::GetAircraftAttitude(double &pitch, double &roll, double &yaw)
{
    pitch = mPitch;
    roll = mRoll;
    yaw = mYaw;
}

void OgreFlightRender::GetFlightInstruments(double &altitude, double &airspeed, double &heading, double &verticalSpeed)
{
    altitude = mAltitude;
    airspeed = mAirspeed;
    heading = mHeading;
    verticalSpeed = mVerticalSpeed;
}

void OgreFlightRender::SetAircraftControl(double pitchInput, double rollInput, double yawInput, double throttle)
{
    mPitchInput = pitchInput;
    mRollInput = rollInput;
    mYawInput = yawInput;
    mThrottle = throttle;
    
    // Map throttle to airspeed (simplified)
    mAirspeed = 100 + throttle * 200;
}

void OgreFlightRender::SetTrainingStage(int stage)
{
    if(stage >= 1 && stage <= 5) {
        mCurrentStage = stage;
        
        // Adjust simulation parameters based on stage
        switch(stage) {
            case 1:  // Aircraft familiarization
                mShowInstruments = false;
                break;
            case 2:  // Basic maneuvers
                mShowInstruments = true;
                break;
            case 3:  // Visual flight
                mShowInstruments = true;
                break;
            case 4:  // Transition to instruments
                mShowInstruments = true;
                mWeatherCondition = 1;  // Light clouds
                break;
            case 5:  // Instrument flight
                mShowInstruments = true;
                mWeatherCondition = 2;  // Heavy clouds/fog
                break;
        }
        
        // Update camera position for the new stage
        updateCameraForStage();
    }
}

void OgreFlightRender::SetTrainingScenario(int scenarioId)
{
    // Reset flight and set up a specific training scenario
    ResetFlight();
    
    switch(scenarioId) {
        case 1:  // Takeoff scenario
            mPositionY = 100;  // On runway
            mAirspeed = 0;
            mAltitude = 100;
            break;
        case 2:  // Cruise scenario
            mPositionY = 1000;  // At cruise altitude
            mAirspeed = 200;    // At cruise speed
            mAltitude = 1000;
            break;
        case 3:  // Landing approach
            mPositionY = 300;   // On approach
            mAirspeed = 150;    // Approach speed
            mAltitude = 300;
            mPitch = -0.1;      // Slight descent
            break;
    }
    
    // Update aircraft position in the scene
    if (mAircraftNode) {
        mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
    }
    
    // Update camera for the current stage
    updateCameraForStage();
}

void OgreFlightRender::ResetFlight()
{
    // Reset aircraft to initial state
    mPositionX = 0.0;
    mPositionY = 1000.0;  // 1000m altitude
    mPositionZ = 0.0;
    mPitch = 0.0;
    mRoll = 0.0;
    mYaw = 0.0;  // North heading
    mVelocityX = 0.0;
    mVelocityY = 0.0;
    mVelocityZ = 0.0;
    mAltitude = 1000.0;
    mAirspeed = 150.0;  // 150 km/h
    mHeading = 0.0;
    mVerticalSpeed = 0.0;
    mPitchInput = 0.0;
    mRollInput = 0.0;
    mYawInput = 0.0;
    mThrottle = 0.5;  // Half throttle
    
    // Update aircraft node position
    if (mAircraftNode) {
        mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
        mAircraftNode->resetOrientation();
    }
    
    // Update camera for current stage
    updateCameraForStage();
}

void OgreFlightRender::ToggleInstrumentDisplay(bool showInstruments)
{
    mShowInstruments = showInstruments;
}

void OgreFlightRender::SetWeatherCondition(int condition)
{
    if(condition >= 0 && condition <= 2) {
        mWeatherCondition = condition;
        updateWeather();
    }
}

void OgreFlightRender::SetTimeOfDay(int hour)
{
    if(hour >= 0 && hour <= 23) {
        mTimeOfDay = hour;
        updateWeather();  // Time affects lighting
    }
}

void OgreFlightRender::mouseMoved(int x, int y, int d)
{
    // Wrapper method for compatibility with interface
    // Actual implementation is in the OIS version
}