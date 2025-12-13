#include "../include/JoystickHandler.h"

JoystickHandler::JoystickHandler() {
    m_joystickPresent = false;
    memset(&m_gamepadState, 0, sizeof(m_gamepadState));
}

bool JoystickHandler::initialize() {
    // Проверяем наличие джойстика
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        m_joystickPresent = true;
        std::cout << "Джойстик найден: " << glfwGetJoystickName(GLFW_JOYSTICK_1) << std::endl;
        return true;
    } else if (glfwJoystickPresent(GLFW_JOYSTICK_0)) {
        m_joystickPresent = true;
        std::cout << "Джойстик найден: " << glfwGetJoystickName(GLFW_JOYSTICK_0) << std::endl;
        return true;
    } else {
        std::cout << "Джойстик не найден!" << std::endl;
        m_joystickPresent = false;
        return false;
    }
}

void JoystickHandler::update() {
    if (!m_joystickPresent) return;

    // Проверяем наличие джойстика и получаем его состояние
    int present = glfwJoystickPresent(GLFW_JOYSTICK_0) ? GLFW_JOYSTICK_0 :
                  glfwJoystickPresent(GLFW_JOYSTICK_1) ? GLFW_JOYSTICK_1 : -1;

    if (present != -1) {
        int count;
        const float* axes = glfwGetJoystickAxes(present, &count);
        
        // Предполагаем, что используем Logitech Extreme 3D Pro
        // Оси: 0 - X (крен), 1 - Y (тангаж), 2 - Z (газ), 3 - Rudder (рысканье)
        if (count >= 4) {
            m_gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_X] = normalizeAxis(axes[0]);  // Крен
            m_gamepadState.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] = normalizeAxis(axes[1]);  // Тангаж (инвертирован)
            m_gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] = normalizeAxis(axes[3]); // Рысканье
            m_gamepadState.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] = normalizeAxis(axes[2]); // Газ (ось Z)
        }

        // Получаем состояние кнопок
        const unsigned char* buttons;
        int buttonCount;
        buttons = glfwGetJoystickButtons(present, &buttonCount);
        for (int i = 0; i < (buttonCount > GLFW_GAMEPAD_BUTTON_LAST ? GLFW_GAMEPAD_BUTTON_LAST : buttonCount); i++) {
            m_gamepadState.buttons[i] = buttons[i];
        }
    }
}

float JoystickHandler::getStickX() {
    // Ось X для крена (лево/право)
    // В Logitech Extreme 3D Pro ось X - это первая ось (индекс 0)
    int present = glfwJoystickPresent(GLFW_JOYSTICK_0) ? GLFW_JOYSTICK_0 :
                  glfwJoystickPresent(GLFW_JOYSTICK_1) ? GLFW_JOYSTICK_1 : -1;

    if (present != -1) {
        int count;
        const float* axes = glfwGetJoystickAxes(present, &count);
        if (count > 0) {
            return normalizeAxis(axes[0]);
        }
    }
    return 0.0f;
}

float JoystickHandler::getStickY() {
    // Ось Y для тангажа (вверх/вниз), инвертируем для правильного направления
    // В Logitech Extreme 3D Pro ось Y - это вторая ось (индекс 1)
    int present = glfwJoystickPresent(GLFW_JOYSTICK_0) ? GLFW_JOYSTICK_0 :
                  glfwJoystickPresent(GLFW_JOYSTICK_1) ? GLFW_JOYSTICK_1 : -1;

    if (present != -1) {
        int count;
        const float* axes = glfwGetJoystickAxes(present, &count);
        if (count > 1) {
            return -normalizeAxis(axes[1]); // Инвертируем, чтобы вперед было вниз
        }
    }
    return 0.0f;
}

float JoystickHandler::getRudder() {
    // Ось для руля направления (рысканье)
    // В Logitech Extreme 3D Pro руль направления - это четвертая ось (индекс 3)
    int present = glfwJoystickPresent(GLFW_JOYSTICK_0) ? GLFW_JOYSTICK_0 :
                  glfwJoystickPresent(GLFW_JOYSTICK_1) ? GLFW_JOYSTICK_1 : -1;

    if (present != -1) {
        int count;
        const float* axes = glfwGetJoystickAxes(present, &count);
        if (count > 3) {
            return normalizeAxis(axes[3]);
        }
    }
    return 0.0f;
}

float JoystickHandler::getThrottle() {
    // Положение газа (ось Z)
    // В Logitech Extreme 3D Pro ось Z - это третья ось (индекс 2)
    // Диапазон обычно от -1 (малый газ) до 1 (полный газ), преобразуем к 0-1
    int present = glfwJoystickPresent(GLFW_JOYSTICK_0) ? GLFW_JOYSTICK_0 :
                  glfwJoystickPresent(GLFW_JOYSTICK_1) ? GLFW_JOYSTICK_1 : -1;

    if (present != -1) {
        int count;
        const float* axes = glfwGetJoystickAxes(present, &count);
        if (count > 2) {
            // Преобразуем из диапазона [-1, 1] в [0, 1]
            float rawValue = axes[2];
            return (rawValue + 1.0f) / 2.0f;
        }
    }
    return 0.0f;
}

bool JoystickHandler::isButtonPressed(int button) {
    if (!m_joystickPresent) return false;

    int present = glfwJoystickPresent(GLFW_JOYSTICK_0) ? GLFW_JOYSTICK_0 :
                  glfwJoystickPresent(GLFW_JOYSTICK_1) ? GLFW_JOYSTICK_1 : -1;

    if (present != -1) {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(present, &count);
        if (button < count) {
            return buttons[button] == GLFW_PRESS;
        }
    }
    return false;
}

float JoystickHandler::normalizeAxis(float value) {
    // Ограничиваем значение в диапазоне [-1, 1]
    if (value > 1.0f) return 1.0f;
    if (value < -1.0f) return -1.0f;
    return value;
}