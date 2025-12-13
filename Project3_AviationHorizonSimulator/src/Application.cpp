#include "../include/Application.h"
#include <GLFW/glfw3.h>
#include <iostream>

Application::Application() {
    m_aircraft = nullptr;
    m_horizon = nullptr;
    m_instruments = nullptr;
    m_joystick = nullptr;
    m_window = nullptr;
    m_running = false;
}

Application::~Application() {
    shutdown();
}

bool Application::initialize() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Не удалось инициализировать GLFW" << std::endl;
        return false;
    }

    // Настройка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    m_window = glfwCreateWindow(800, 600, "Программа для формирования навыков пилотирования по приборам", NULL, NULL);
    if (m_window == NULL) {
        std::cerr << "Не удалось создать окно GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Включение VSync

    // Инициализация компонентов
    m_aircraft = new Aircraft();
    m_horizon = new ArtificialHorizon();
    m_instruments = new InstrumentsPanel();
    m_joystick = new JoystickHandler();

    // Инициализация джойстика
    if (!m_joystick->initialize()) {
        std::cout << "Предупреждение: джойстик не найден, управление будет доступно с клавиатуры" << std::endl;
    }

    // Настройка OpenGL
    setupOpenGL();
    
    // Настройка callback'ов
    setupCallbacks();

    m_running = true;
    return true;
}

void Application::setupOpenGL() {
    // Установка параметров OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Черный фон
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
}

void Application::setupCallbacks() {
    // Установка callback'а для изменения размера окна
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    
    // Установка callback'а для клавиатуры
    glfwSetKeyCallback(m_window, keyCallback);
    
    // Привязка контекста к окну для обратных вызовов
    glfwSetWindowUserPointer(m_window, this);
}

void Application::run() {
    if (!m_running) {
        std::cerr << "Приложение не инициализировано!" << std::endl;
        return;
    }

    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(m_window) && m_running) {
        double currTime = glfwGetTime();
        float deltaTime = static_cast<float>(currTime - prevTime);
        prevTime = currTime;

        // Обработка ввода
        handleInput();

        // Обновление симуляции
        updateSimulation(deltaTime);

        // Отрисовка кадра
        renderFrame();

        // Обмен буферов
        glfwSwapBuffers(m_window);

        // Обработка событий
        glfwPollEvents();
    }
}

void Application::handleInput() {
    // Обновляем состояние джойстика
    m_joystick->update();

    // Получаем значения с джойстика
    float stickX = m_joystick->getStickX();
    float stickY = m_joystick->getStickY();
    float rudder = m_joystick->getRudder();
    float throttle = m_joystick->getThrottle();

    // Альтернативное управление с клавиатуры на случай отсутствия джойстика
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) stickX = -1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) stickX = 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) stickY = -1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) stickY = 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) rudder = -1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) rudder = 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) throttle = 1.0f;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) throttle = 0.0f;

    // Применение управления к самолету
    m_aircraft->applyControls(stickX, stickY, rudder, throttle);
}

void Application::updateSimulation(float deltaTime) {
    // Обновление состояния самолета
    m_aircraft->update(deltaTime);

    // Обновление данных для приборов
    m_instruments->setAltitude(m_aircraft->altitude);
    m_instruments->setAirspeed(m_aircraft->airspeed);
    m_instruments->setHeading(m_aircraft->heading);

    // Обновление данных для искусственного горизонта
    m_horizon->setAttitude(m_aircraft->pitch, m_aircraft->roll);
}

void Application::renderFrame() {
    // Очистка буфера цвета
    glClear(GL_COLOR_BUFFER_BIT);

    // Установка режима 2D для отрисовки приборов
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Отрисовка искусственного горизонта (в центре экрана)
    glViewport(200, 100, 400, 400); // Устанавливаем область отображения для горизонта
    glPushMatrix();
    glLoadIdentity();
    m_horizon->setSize(400, 400);
    m_horizon->draw();
    glPopMatrix();

    // Восстанавливаем полный viewport
    glViewport(0, 0, 800, 600);

    // Отрисовка панели приборов
    m_instruments->setSize(800, 600);
    m_instruments->draw();
}

void Application::shutdown() {
    m_running = false;

    // Освобождение ресурсов
    if (m_aircraft) {
        delete m_aircraft;
        m_aircraft = nullptr;
    }

    if (m_horizon) {
        delete m_horizon;
        m_horizon = nullptr;
    }

    if (m_instruments) {
        delete m_instruments;
        m_instruments = nullptr;
    }

    if (m_joystick) {
        delete m_joystick;
        m_joystick = nullptr;
    }

    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // Обновление области отображения при изменении размера окна
    glViewport(0, 0, width, height);
    
    // Обновление проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}