#ifndef __OgreFlightRender_H__
#define __OgreFlightRender_H__

#include <OGRE/Ogre.h>
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreException.h>
#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreViewport.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

using namespace std;

class OgreFlightRender : public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:
    OgreFlightRender();
    ~OgreFlightRender();

    void Init(long hWnd);
    void StartRendering();
    void StopRendering();
    void ReStartRendering();
    void Resize(int width, int height);
    void Update();

    // Обработчики событий мыши (OIS)
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Обработчики событий клавиатуры (OIS)
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );

    // Обработчики событий кадрового цикла
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

    void createFrameListener();

    // Методы управления и получения данных о полете
    void GetAircraftPosition(double &x, double &y, double &z);
    void GetAircraftAttitude(double &pitch, double &roll, double &yaw);
    void GetFlightInstruments(double &altitude, double &airspeed, double &heading, double &verticalSpeed);
    void SetAircraftControl(double pitchInput, double rollInput, double yawInput, double throttle);
    void SetTrainingStage(int stage);
    void SetTrainingScenario(int scenarioId);
    void ResetFlight();
    void ToggleInstrumentDisplay(bool showInstruments);
    void SetWeatherCondition(int condition);
    void SetTimeOfDay(int hour);
    void mouseMoved(int x, int y, int d);

private:
    void setupResources(void);
    void createCamera(void);
    void createViewports(void);
    void createResourceListener(void);
    void loadResources(void);
    void createScene(void);
    void destroyScene(void);
    void createInputSystem(Ogre::RenderWindow* win);
    void updateAircraft(float deltaTime);
    void updateFlightInstruments();
    void createFlightInstruments();
    void updateWeather();
    void updateEnvironment();

    // Объекты OGRE
    Ogre::Root *mRoot;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;
    Ogre::Entity* mAircraftEntity;
    Ogre::SceneNode* mAircraftNode;
    Ogre::Light* mLight;

    // Устройства ввода
    OIS::InputManager* mInputManager;
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;

    // Объекты графического интерфейса
    CEGUI::OgreRenderer* mGUIRenderer;
    CEGUI::Window* mSheet;

    // Параметры состояния воздушного судна
    double mPositionX, mPositionY, mPositionZ;
    double mPitch, mRoll, mYaw;           // Aircraft attitude
    double mVelocityX, mVelocityY, mVelocityZ;
    double mAltitude, mAirspeed, mHeading, mVerticalSpeed;
    double mPitchInput, mRollInput, mYawInput, mThrottle;
    
    // Параметры учебного режима
    int mCurrentStage;                    // текущий этап полета (1-5)
    int mWeatherCondition;                // Погодные условия (ясно, облачно, туман)
    int mTimeOfDay;                       // Время суток (0–23)
    bool mShowInstruments;                // Отображать ли приборную панель
    
    // Состояние симуляции
    bool m_bShutDown;
    bool m_bRender;
    bool m_bPause;
    Ogre::Real m_fFPS;
    Ogre::Real m_fSecsPerFrame;
    int m_nWidth, m_nHeight;
    Ogre::String m_sTitle;
};

#endif
