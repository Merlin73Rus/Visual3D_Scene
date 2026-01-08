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
    mPositionX(0.0), mPositionY(1000.0), mPositionZ(0.0),  // Начальная высота - 1000 м
    mPitch(0.0), mRoll(0.0), mYaw(0.0),                    // Горизонтальный полет
    mVelocityX(0.0), mVelocityY(0.0), mVelocityZ(0.0),
    mAltitude(1000.0), mAirspeed(150.0), mHeading(0.0), mVerticalSpeed(0.0),
    mPitchInput(0.0), mRollInput(0.0), mYawInput(0.0), mThrottle(0.5),  // Тяга - 50%
    mCurrentStage(1),                                       // Этап 1: Знакомство с ВС
    mWeatherCondition(0),                                   // Ясная погода
    mTimeOfDay(12),                                         // Полдень
    mShowInstruments(true),                                 // Инструменты отображаются по умолчанию
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
    
    // Настройка OGRE
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
    
    // Создание камеры
    createCamera();
    
    // Создание области отображения
    createViewports();
    
    // Установка базового уровня освещения сцены
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    // Создание источника света
    mLight = mSceneMgr->createLight("MainLight");
    mLight->setPosition(20, 80, 50);
    
    // Инициализация системы ввода
    createInputSystem(mWindow);
    
    // Построение сцены
    createScene();
    
    // Инициализация графического интерфейса
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
    
    // Формирование панели приборов в зависимости от этапа обучения
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
        
        // Обновление соотношения сторон области отображения
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

    // Захват и обновление данных устройств ввода
    mKeyboard->capture();
    mMouse->capture();

    // Обновление физики воздушного судна
    updateAircraft(evt.timeSinceLastFrame);
    
    // Обновление показаний приборов
    updateFlightInstruments();
    
    // Обновление окружения в соответствии с настройками
    updateEnvironment();
    
    return true;
}

bool OgreFlightRender::frameEnded(const Ogre::FrameEvent& evt)
{
    return true;
}

void OgreFlightRender::createFrameListener()
{
    // Уже обрабатывается в StartRendering
}

void OgreFlightRender::setupResources(void)
{
    // Загрузка путей к ресурсам из конфигурационного файла
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    // Проход по всем секциям и параметрам файла
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
    mCamera->setPosition(Ogre::Vector3(0, 1000, 0));  // Начальная позиция на высоте 1000 м
    mCamera->lookAt(Ogre::Vector3(0, 1000, -100));    // Взгляд вперед
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
    // Создание неба
    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    
    // Создание поверхности земли
    Ogre::Plane plane;
    plane.d = 0; // Уровень земли по Y = 0
    plane.normal = Ogre::Vector3::UNIT_Y; // Нормаль направлена вверх
    
    Ogre::MeshManager::getSingleton().createPlane("ground", 
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
        plane, 20000, 20000, 20, 20, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);
        
    Ogre::Entity* groundEntity = mSceneMgr->createEntity("GroundEntity", "ground");
    groundEntity->setMaterialName("Examples/Rockwall");
    mSceneMgr->getRootSceneNode()->attachObject(groundEntity);
    
    // Загрузка модели ВС
    mAircraftEntity = mSceneMgr->createEntity("Aircraft", "ogrehead.mesh");  // Placeholder
    mAircraftNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("AircraftNode");
    mAircraftNode->attachObject(mAircraftEntity);
    mAircraftNode->setScale(0.5, 0.5, 0.5);  // Scale down the placeholder model
    
    // Установка начального положения ВС
    mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
    
    // Настройка камеры в зависимости от этапа обучения
    updateCameraForStage();
}

void OgreFlightRender::updateAircraft(float deltaTime)
{
    // Расчет угловых скоростей на основе управляющих воздействий
    double pitchRate = mPitchInput * 0.5;  // Упрощенная модель
    double rollRate = mRollInput * 0.8;
    double yawRate = mYawInput * 0.3;
    
    // Обновление углов тангажа, крена и рыскания
    mPitch += pitchRate * deltaTime;
    mRoll += rollRate * deltaTime;
    mYaw += yawRate * deltaTime;
    
    // Нормализация углов в диапазон [-π, π]
    if (mPitch > Ogre::Math::PI) mPitch -= 2 * Ogre::Math::PI;
    if (mPitch < -Ogre::Math::PI) mPitch += 2 * Ogre::Math::PI;
    if (mRoll > Ogre::Math::PI) mRoll -= 2 * Ogre::Math::PI;
    if (mRoll < -Ogre::Math::PI) mRoll += 2 * Ogre::Math::PI;
    if (mYaw > Ogre::Math::PI) mYaw -= 2 * Ogre::Math::PI;
    if (mYaw < -Ogre::Math::PI) mYaw += 2 * Ogre::Math::PI;
    
    // Расчет вектора скорости в мировых координатах
    double speed = mAirspeed / 3.6; // Перевод км/ч в м/с
    double velX = speed * cos(mPitch) * sin(mYaw);
    double velZ = speed * cos(mPitch) * cos(mYaw);
    double velY = speed * sin(mPitch);
    
    // Обновление положения
    mPositionX += velX * deltaTime;
    mPositionY += velY * deltaTime;
    mPositionZ += velZ * deltaTime;
    
    // Обновление высоты
    mAltitude = mPositionY;
    
    // Простейшая защита от столкновения с землей
    if (mAltitude < 100) {  // Prevent crashing into ground
        mPositionY = 100;
        mAltitude = 100;
        mPitch = abs(mPitch) > 0.1 ? mPitch * 0.9 : 0;  // Уменьшение угла тангажа у земли
    }
    
    // Обновление положения и ориентации ВС в сцене
    if (mAircraftNode) {
        mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
        mAircraftNode->resetOrientation();
        mAircraftNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(mYaw));
        mAircraftNode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(mPitch));
        mAircraftNode->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(mRoll));
    }
    
    // Обновление положения камеры в зависимости от этапа обучения
    updateCameraForStage();
}

void OgreFlightRender::updateCameraForStage()
{
    if (!mAircraftNode || !mCamera) return;
    
    Ogre::Vector3 aircraftPos = mAircraftNode->getPosition();
    
    switch(mCurrentStage) {
        case 1:  // Знакомство: вид сзади и сверху
            mCamera->setPosition(aircraftPos + Ogre::Vector3(-20, 5, 20));
            mCamera->lookAt(aircraftPos);
            break;
        case 2:  // Базовые маневры: более близкий вид сзади
            mCamera->setPosition(aircraftPos + Ogre::Vector3(-10, 2, 10));
            mCamera->lookAt(aircraftPos);
            break;
        case 3:  // Правила визуальных полетов: вид из кабины
            mCamera->setPosition(aircraftPos);
            mCamera->setOrientation(mAircraftNode->getOrientation());
            break;
        case 4:  // Переход к приборному полету: комбинированный вид
            mCamera->setPosition(aircraftPos + Ogre::Vector3(0, 2, 10));
            mCamera->lookAt(aircraftPos);
            break;
        case 5:  // Приборный полет: Вид из кабины с приборами
            mCamera->setPosition(aircraftPos);
            mCamera->setOrientation(mAircraftNode->getOrientation());
            break;
    }
}

void OgreFlightRender::updateFlightInstruments()
{
    // Обновление показаний приборов на основе текущего состояния ВС
    // Эти данные отображаются в графическом интерфейсе
    mAltitude = mPositionY;  // Высота над уровнем земли
    mHeading = mYaw * 180.0 / Ogre::Math::PI;  // Перевод в градусы
    if (mHeading < 0) mHeading += 360;  // Нормализация в диапазон 0–360
    
    // Вертикальная скорость (м/с)
    mVerticalSpeed = mVelocityY;
    
    // Обновление соответствующих элементов интерфейса
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
    // Формирование панели приборов в зависимости от этапа обучения
    switch(mCurrentStage) {
        case 1:  // Знакомство: упор на визуальное восприятие
            mShowInstruments = false;  // Полет без приборов
            break;
        case 2:  // Выполнение базовых маневров с отображением приборной панели
            mShowInstruments = true;
            break;
        case 3:  // Визуальные полеты с отображением приборной панели
            mShowInstruments = true;
            break;
        case 4:  // Переход к совмещенному отображению приборов
            mShowInstruments = true;
            break;
        case 5:  // Приборный полет
            mShowInstruments = true;
            break;
    }
}

void OgreFlightRender::updateEnvironment()
{
    // Обновление окружающей среды в зависимости от погоды и времени суток
    updateWeather();
}

void OgreFlightRender::updateWeather()
{
    // Применение погодных эффектов в зависимости от mWeatherCondition
    switch(mWeatherCondition) {
        case 0:  // Ясно
            mSceneMgr->setFog(Ogre::FOG_NONE);
            break;
        case 1:  // Легкая облачность
            mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue::White, 0.0001, 5000, 8000);
            break;
        case 2:  // Сплошная облачность / туман
            mSceneMgr->setFog(Ogre::FOG_EXP, Ogre::ColourValue(0.8, 0.8, 0.8), 0.001);
            break;
    }
    
    // Настройка освещения в зависимости от времени суток
    float lightIntensity = 0.3f;  // Базовый уровень
    if (mTimeOfDay >= 6 && mTimeOfDay <= 18) {
        // Днем - ярче
        lightIntensity = 0.5f + 0.4f * (abs(12 - mTimeOfDay) / 12.0f);  // Максимум в полдень
    } else {
        // Ночью - темнее
        lightIntensity = 0.1f;
    }
    
    mSceneMgr->setAmbientLight(Ogre::ColourValue(lightIntensity, lightIntensity, lightIntensity));
}

bool OgreFlightRender::mouseMoved( const OIS::MouseEvent &arg )
{
     // Обработка движения мыши для управления камерой на ранних этапах
    if (mCurrentStage <= 3) {  // Внешнее управление камерой разрешено только на этапах 1–3
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
    // Обработка клавиатуры для управления ВС
    switch(arg.key) {
        case OIS::KC_W:  // Тангаж вверх
            mPitchInput = 1.0;
            break;
        case OIS::KC_S:  // Тангаж вниз
            mPitchInput = -1.0;
            break;
        case OIS::KC_A:  // Крен влево
            mRollInput = -1.0;
            break;
        case OIS::KC_D:  // Крен вправо
            mRollInput = 1.0;
            break;
        case OIS::KC_Q:  // Рыскание влево
            mYawInput = -1.0;
            break;
        case OIS::KC_E:  // Рыскание вправо
            mYawInput = 1.0;
            break;
        case OIS::KC_R:  // Увеличить тягу
            mThrottle = std::min(1.0, mThrottle + 0.1);
            mAirspeed = 100 + mThrottle * 200;  // Map throttle to airspeed
            break;
        case OIS::KC_F:  // Уменьшить тягу
            mThrottle = std::max(0.0, mThrottle - 0.1);
            mAirspeed = 100 + mThrottle * 200;  // Map throttle to airspeed
            break;
        case OIS::KC_SPACE:  // Сброс положения ВС
            ResetFlight();
            break;
        case OIS::KC_1:  // Этап 1
            SetTrainingStage(1);
            break;
        case OIS::KC_2:  // Этап 2
            SetTrainingStage(2);
            break;
        case OIS::KC_3:  // Этап 3
            SetTrainingStage(3);
            break;
        case OIS::KC_4:  // Этап 4
            SetTrainingStage(4);
            break;
        case OIS::KC_5:  // Этап 5
            SetTrainingStage(5);
            break;
        case OIS::KC_I:  // Переключение видимости приборов
            mShowInstruments = !mShowInstruments;
            break;
    }
    
    return true;
}

bool OgreFlightRender::keyReleased( const OIS::KeyEvent &arg )
{
    // Сброс управляющих сигналов при отпускании клавиш
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
    
    // Зависимость скорости от тяги
    mAirspeed = 100 + throttle * 200;
}

void OgreFlightRender::SetTrainingStage(int stage)
{
    if(stage >= 1 && stage <= 5) {
        mCurrentStage = stage;
        
        // Настройка параметров в зависимости от этапа
        switch(stage) {
            case 1:  // Знакомство с ВС
                mShowInstruments = false;
                break;
            case 2:  // Выполнение базовых маневров
                mShowInstruments = true;
                break;
            case 3:  // Визуальный полет
                mShowInstruments = true;
                break;
            case 4:  // Переход к приборам
                mShowInstruments = true;
                mWeatherCondition = 1;  // Light clouds
                break;
            case 5:  // Приборный полет
                mShowInstruments = true;
                mWeatherCondition = 2;  // Heavy clouds/fog
                break;
        }
        
        // Обновление положения камеры в зависимости от этапа
        updateCameraForStage();
    }
}

void OgreFlightRender::SetTrainingScenario(int scenarioId)
{
    // Сброс состояния и настройка конкретного учебного сценария
    ResetFlight();
    
    switch(scenarioId) {
        case 1:  // Взлет
            mPositionY = 100;  // On runway
            mAirspeed = 0;
            mAltitude = 100;
            break;
        case 2:  // Крейсерский полет
            mPositionY = 1000;  // At cruise altitude
            mAirspeed = 200;    // At cruise speed
            mAltitude = 1000;
            break;
        case 3:  // Заход на посадку
            mPositionY = 300;   // На глиссаде
            mAirspeed = 150;    // Скорость захода
            mAltitude = 300;
            mPitch = -0.1;      // Легкое снижение
            break;
    }
    
    // Обновление положения ВС в сцене
    if (mAircraftNode) {
        mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
    }
    
    // Обновление камеры в зависимости от этапа
    updateCameraForStage();
}

void OgreFlightRender::ResetFlight()
{
    // Возврат ВС в исходное состояние
    mPositionX = 0.0;
    mPositionY = 1000.0;  // Высота 1000 м
    mPositionZ = 0.0;
    mPitch = 0.0;
    mRoll = 0.0;
    mYaw = 0.0;  // Курс на север
    mVelocityX = 0.0;
    mVelocityY = 0.0;
    mVelocityZ = 0.0;
    mAltitude = 1000.0;
    mAirspeed = 150.0;  // 150 км/ч
    mHeading = 0.0;
    mVerticalSpeed = 0.0;
    mPitchInput = 0.0;
    mRollInput = 0.0;
    mYawInput = 0.0;
    mThrottle = 0.5;  // 50% тяги
    
    // Обновление узла ВС
    if (mAircraftNode) {
        mAircraftNode->setPosition(mPositionX, mPositionY, mPositionZ);
        mAircraftNode->resetOrientation();
    }
    
    // Обновление камеры в зависимости от этапа
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
        updateWeather();  // Освещение зависит от времени суток
    }
}

void OgreFlightRender::mouseMoved(int x, int y, int d)
{
    // Адаптер для внешнего интерфейса: фактическая обработка ввода выполняется через OIS
}
