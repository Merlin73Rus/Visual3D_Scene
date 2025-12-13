#pragma once
#include "Aircraft.h"
#include "ArtificialHorizon.h"
#include "InstrumentsPanel.h"
#include "JoystickHandler.h"

// Основной класс приложения
class Application {
public:
    // Конструктор
    Application();
    
    // Деструктор
    ~Application();
    
    // Инициализация приложения
    bool initialize();
    
    // Запуск главного цикла приложения
    void run();
    
    // Остановка приложения
    void shutdown();

private:
    // Указатели на компоненты
    Aircraft* m_aircraft;
    ArtificialHorizon* m_horizon;
    InstrumentsPanel* m_instruments;
    JoystickHandler* m_joystick;
    
    // GLFW окно
    GLFWwindow* m_window;
    
    // Состояние приложения
    bool m_running;
    
    // Методы для внутреннего использования
    void setupOpenGL();
    void setupCallbacks();
    void renderFrame();
    void handleInput();
    void updateSimulation(float deltaTime);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};