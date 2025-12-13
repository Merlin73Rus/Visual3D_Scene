#include "HorizonDiagnosticApp.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>

HorizonDiagnosticApp::HorizonDiagnosticApp() :
    mRoot(nullptr),
    mSceneMgr(nullptr),
    mCamera(nullptr),
    mWindow(nullptr),
    mHorizonDisplay(nullptr),
    mHorizonNode(nullptr),
    mInputManager(nullptr),
    mKeyboard(nullptr),
    mMouse(nullptr),
    mJoyStick(nullptr),
    mCurrentModule(0),
    mCurrentImage(0),
    mTotalImages(50),
    mImageDisplayed(false),
    mUserAnswer(0),
    mCorrectAnswers(0),
    mResultsFile("diagnostic_results.txt"),
    mExitApp(false),
    mTestRunning(false)
{
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(0)));
}

HorizonDiagnosticApp::~HorizonDiagnosticApp()
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

void HorizonDiagnosticApp::setup()
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
    params["title"] = "Aviation Horizon Diagnostic";
    params["vsync"] = "false";
    mWindow = mRoot->createRenderWindow("Aviation Horizon Diagnostic", 1200, 800, false, &params);
    
    // Set up scene manager
    mSceneMgr = mRoot->createSceneManager();
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
    
    // Set this class as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    
    createScene();
    setupCamera();
    setupLights();
    setupInput();
    createArtificialHorizon();
    
    // Generate horizon images for both modules
    generateHorizonImages();
    
    // Start with module 1
    startModule(1);
    
    // Register as frame listener
    mRoot->addFrameListener(this);
}

void HorizonDiagnosticApp::shutdown()
{
    if (mRoot) {
        mRoot->removeFrameListener(this);
        mRoot->shutdown();
    }
}

void HorizonDiagnosticApp::createScene()
{
    // Create a dark background to highlight the artificial horizon
    mSceneMgr->setSkyDome(true, "Examples/SpaceSkyPlane", 5, 8);
}

void HorizonDiagnosticApp::setupCamera()
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);
    mCamera->setAspectRatio(1200.0f / 800.0f);
    
    // Position camera to look at the horizon display
    mCamera->setPosition(0, 0, 5);
    mCamera->lookAt(0, 0, 0);
}

void HorizonDiagnosticApp::setupLights()
{
    // Add a subtle light to illuminate the scene
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(0, 5, 5);
    light->setDiffuseColour(0.8, 0.8, 0.8);
    light->setSpecularColour(1.0, 1.0, 1.0);
}

void HorizonDiagnosticApp::createArtificialHorizon()
{
    // Create a manual object to represent the artificial horizon display
    mHorizonDisplay = mSceneMgr->createManualObject("ArtificialHorizonDisplay");
    
    // Create a simple circular display
    float radius = 2.0f;
    int segments = 64;
    
    mHorizonDisplay->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
    
    // Draw a filled circle for the horizon display
    for (int i = 0; i <= segments; ++i) {
        float angle = 2 * Ogre::Math::PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        
        // Outer edge
        mHorizonDisplay->position(x, y, 0);
        
        // Inner edge (for a border effect)
        if (i == 0) {
            mHorizonDisplay->position(0, 0, 0);  // Center
        }
    }
    
    mHorizonDisplay->end();
    
    // Create a scene node for the horizon display
    mHorizonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mHorizonNode->attachObject(mHorizonDisplay);
    mHorizonNode->setPosition(0, 0, 0);
}

void HorizonDiagnosticApp::generateHorizonImages()
{
    mHorizonImages.clear();
    
    // Generate 50 horizon images for each module
    for (int module = 1; module <= 2; ++module) {
        for (int i = 0; i < mTotalImages; ++i) {
            HorizonImage img;
            
            // Random roll direction (-1 for left, 1 for right)
            img.rollDirection = (std::rand() % 2 == 0) ? -1 : 1;
            
            if (module == 1) {
                // Module 1: no rotation (0 degrees)
                img.rotationAngle = 0;
            } else {
                // Module 2: random rotation (0, 90, 180, or 270 degrees)
                int rotationIndex = std::rand() % 4;
                img.rotationAngle = rotationIndex * 90;
            }
            
            img.answered = false;
            img.correctAnswer = false;
            
            mHorizonImages.push_back(img);
        }
    }
}

void HorizonDiagnosticApp::startModule(int moduleNum)
{
    mCurrentModule = moduleNum;
    mCurrentImage = 0;
    mCorrectAnswers = 0;
    mTestRunning = true;
    
    // Shuffle the images for this module
    int startIndex = (moduleNum - 1) * mTotalImages;
    int endIndex = startIndex + mTotalImages;
    
    std::vector<HorizonImage> moduleImages;
    for (int i = startIndex; i < endIndex; ++i) {
        moduleImages.push_back(mHorizonImages[i]);
    }
    
    // Shuffle the module images
    std::random_shuffle(moduleImages.begin(), moduleImages.end());
    
    // Replace the module's images in the main array
    for (int i = 0; i < mTotalImages; ++i) {
        mHorizonImages[startIndex + i] = moduleImages[i];
    }
    
    mTimer.reset();
    displayCurrentHorizon();
    
    std::cout << "Starting Module " << moduleNum << std::endl;
}

void HorizonDiagnosticApp::displayCurrentHorizon()
{
    if (mCurrentImage >= mTotalImages) {
        // Module completed
        std::cout << "Module " << mCurrentModule << " completed. Correct answers: " 
                  << mCorrectAnswers << "/" << mTotalImages << std::endl;
        
        if (mCurrentModule == 1) {
            // Start module 2
            startModule(2);
        } else {
            // Both modules completed
            saveResults();
            mTestRunning = false;
            std::cout << "Diagnostic test completed. Results saved to " << mResultsFile << std::endl;
        }
        return;
    }
    
    // Update the artificial horizon display based on the current image
    int currentIndex = (mCurrentModule - 1) * mTotalImages + mCurrentImage;
    const HorizonImage& img = mHorizonImages[currentIndex];
    
    // Apply rotation based on the image's rotation angle
    Ogre::Quaternion rotation = Ogre::Quaternion(Ogre::Radian(Ogre::Degree(img.rotationAngle)), Ogre::Vector3::UNIT_Z);
    mHorizonNode->setOrientation(rotation);
    
    // Reset user answer for this image
    mUserAnswer = 0;
    mImageDisplayed = true;
    mTimer.reset();
}

void HorizonDiagnosticApp::nextHorizon()
{
    if (mCurrentImage >= mTotalImages) return;
    
    // Check if user answered correctly (if they answered at all)
    int currentIndex = (mCurrentModule - 1) * mTotalImages + mCurrentImage;
    HorizonImage& img = mHorizonImages[currentIndex];
    
    if (mUserAnswer != 0) {  // User provided an answer
        img.answered = true;
        if (mUserAnswer == img.rollDirection) {
            img.correctAnswer = true;
            mCorrectAnswers++;
            std::cout << "Correct answer for image " << mCurrentImage + 1 << std::endl;
        } else {
            std::cout << "Incorrect answer for image " << mCurrentImage + 1 
                      << ". Expected: " << (img.rollDirection == -1 ? "Left" : "Right") << std::endl;
        }
    } else {
        std::cout << "No answer for image " << mCurrentImage + 1 << std::endl;
    }
    
    mCurrentImage++;
    displayCurrentHorizon();
}

void HorizonDiagnosticApp::setupInput()
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

bool HorizonDiagnosticApp::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    if (mWindow->isClosed()) {
        return false;
    }
    
    mKeyboard->capture();
    mMouse->capture();
    if (mJoyStick) {
        mJoyStick->capture();
    }
    
    // Check if 3 seconds have passed for the current image
    if (mTestRunning && mImageDisplayed && mTimer.getMilliseconds() >= 3000) {
        nextHorizon();
    }
    
    return !mExitApp;
}

void HorizonDiagnosticApp::saveResults()
{
    std::ofstream file(mResultsFile);
    if (!file.is_open()) {
        std::cerr << "Could not open results file: " << mResultsFile << std::endl;
        return;
    }
    
    file << "Aviation Horizon Diagnostic Results\n";
    file << "==================================\n\n";
    
    file << "Module 1 Results (Standard Images):\n";
    file << "Total Images: " << mTotalImages << "\n";
    file << "Correct Answers: " << mHorizonImages[0].rollDirection;  // Placeholder - we need to count properly
    
    int module1Correct = 0;
    int module2Correct = 0;
    
    for (int i = 0; i < mTotalImages; ++i) {
        if (mHorizonImages[i].answered && mHorizonImages[i].correctAnswer) {
            module1Correct++;
        }
    }
    
    for (int i = mTotalImages; i < 2 * mTotalImages; ++i) {
        if (mHorizonImages[i].answered && mHorizonImages[i].correctAnswer) {
            module2Correct++;
        }
    }
    
    file << "Module 1 Score: " << module1Correct << "/" << mTotalImages << " (" 
         << (module1Correct * 100.0 / mTotalImages) << "%)\n";
    file << "Module 2 Score: " << module2Correct << "/" << mTotalImages << " (" 
         << (module2Correct * 100.0 / mTotalImages) << "%)\n";
    file << "Overall Score: " << (module1Correct + module2Correct) << "/" << (2 * mTotalImages) << " (" 
         << ((module1Correct + module2Correct) * 100.0 / (2 * mTotalImages)) << "%)\n\n";
    
    file << "Detailed Results:\n";
    file << "Module 1:\n";
    for (int i = 0; i < mTotalImages; ++i) {
        file << "  Image " << (i+1) << ": Roll=" 
             << (mHorizonImages[i].rollDirection == -1 ? "Left" : "Right")
             << ", Rotation=" << mHorizonImages[i].rotationAngle << "°, "
             << (mHorizonImages[i].answered ? 
                 (mHorizonImages[i].correctAnswer ? "Correct" : "Incorrect") : "No Answer")
             << "\n";
    }
    
    file << "\nModule 2:\n";
    for (int i = mTotalImages; i < 2 * mTotalImages; ++i) {
        int imageNum = i - mTotalImages + 1;
        file << "  Image " << imageNum << ": Roll=" 
             << (mHorizonImages[i].rollDirection == -1 ? "Left" : "Right")
             << ", Rotation=" << mHorizonImages[i].rotationAngle << "°, "
             << (mHorizonImages[i].answered ? 
                 (mHorizonImages[i].correctAnswer ? "Correct" : "Incorrect") : "No Answer")
             << "\n";
    }
    
    file.close();
}

void HorizonDiagnosticApp::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
    
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void HorizonDiagnosticApp::windowClosed(Ogre::RenderWindow* rw)
{
    if (rw == mWindow) {
        mExitApp = true;
    }
}

bool HorizonDiagnosticApp::keyPressed(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_ESCAPE) {
        mExitApp = true;
    }
    // For testing purposes, we could use keys to indicate left/right roll
    else if (arg.key == OIS::KC_LEFT || arg.key == OIS::KC_A) {
        mUserAnswer = -1;  // Left roll
    }
    else if (arg.key == OIS::KC_RIGHT || arg.key == OIS::KC_D) {
        mUserAnswer = 1;   // Right roll
    }
    
    return true;
}

bool HorizonDiagnosticApp::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}

bool HorizonDiagnosticApp::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}

bool HorizonDiagnosticApp::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool HorizonDiagnosticApp::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool HorizonDiagnosticApp::buttonPressed(const OIS::JoyStickEvent &arg, int button)
{
    // Use joystick buttons to indicate roll direction
    // Button 0: Left roll
    // Button 1: Right roll
    if (button == 0) {
        mUserAnswer = -1;  // Left roll
    }
    else if (button == 1) {
        mUserAnswer = 1;   // Right roll
    }
    else if (button == 2) {  // Exit button
        mExitApp = true;
    }
    
    return true;
}

bool HorizonDiagnosticApp::buttonReleased(const OIS::JoyStickEvent &arg, int button)
{
    return true;
}

bool HorizonDiagnosticApp::axisMoved(const OIS::JoyStickEvent &arg, int axis)
{
    return true;
}

bool HorizonDiagnosticApp::povMoved(const OIS::JoyStickEvent &arg, int pov)
{
    return true;
}