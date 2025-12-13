#ifndef HORIZONDIAGNOSTICAPP_H
#define HORIZONDIAGNOSTICAPP_H

#include <OGRE/Ogre.h>
#include <OGRE/OgreApplicationContext.h>
#include <OGRE/OgreInput.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <vector>
#include <string>

struct HorizonImage {
    int rollDirection;  // -1 for left, 1 for right
    int rotationAngle;  // in degrees (0, 90, 180, 270)
    bool answered;
    bool correctAnswer;
};

class HorizonDiagnosticApp : public Ogre::FrameListener,
                             public Ogre::WindowEventListener,
                             public OIS::KeyListener,
                             public OIS::MouseListener,
                             public OIS::JoyStickListener
{
public:
    HorizonDiagnosticApp();
    virtual ~HorizonDiagnosticApp();

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
    void setupInput();
    void createArtificialHorizon();
    void generateHorizonImages();
    void displayCurrentHorizon();
    void nextHorizon();
    void startModule(int moduleNum);
    void saveResults();
    void displayResults();
    
    Ogre::Root* mRoot;
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
    Ogre::RenderWindow* mWindow;
    Ogre::ManualObject* mHorizonDisplay;
    Ogre::SceneNode* mHorizonNode;
    
    OIS::InputManager* mInputManager;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;
    OIS::JoyStick* mJoyStick;

    // Diagnostic state
    int mCurrentModule;        // 1 or 2
    int mCurrentImage;         // current image index
    int mTotalImages;          // 50 for each module
    std::vector<HorizonImage> mHorizonImages;
    Ogre::Timer mTimer;        // for timing 3-second intervals
    bool mImageDisplayed;      // track if image is currently shown
    int mUserAnswer;           // -1 for left, 1 for right, 0 for no answer
    int mCorrectAnswers;       // count for current module
    std::string mResultsFile;
    
    bool mExitApp;
    bool mTestRunning;
};

#endif // HORIZONDIAGNOSTICAPP_H