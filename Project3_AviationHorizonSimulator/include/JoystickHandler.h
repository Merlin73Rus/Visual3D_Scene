#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <GLFW/glfw3.h>
#include <iostream>

// Класс для обработки ввода с джойстика Logitech Extreme 3D Pro
class JoystickHandler {
public:
    // Конструктор
    JoystickHandler();
    
    // Инициализация джойстика
    bool initialize();
    
    // Обновление состояния джойстика
    void update();
    
    // Получение значений осей
    float getStickX();      // Отклонение по оси X (крен)
    float getStickY();      // Отклонение по оси Y (тангаж)
    float getRudder();      // Положение руля направления
    float getThrottle();    // Положение тяги
    
    // Получение состояния кнопок
    bool isButtonPressed(int button);

private:
    GLFWgamepadstate m_gamepadState;
    bool m_joystickPresent;
    
    // Нормализация значений осей (-1.0 до 1.0)
    float normalizeAxis(float value);
};