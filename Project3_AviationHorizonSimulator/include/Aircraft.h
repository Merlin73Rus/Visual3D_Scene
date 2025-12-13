#pragma once
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>

// Класс, представляющий воздушное судно
class Aircraft {
public:
    // Позиция и ориентация
    float x, y, z;           // Позиция
    float pitch, yaw, roll;  // Углы Эйлера (тангаж, рысканье, крен)
    
    // Динамика полета
    float velocity_x, velocity_y, velocity_z;
    float angular_velocity_pitch, angular_velocity_yaw, angular_velocity_roll;
    
    // Параметры полета
    float altitude;
    float airspeed;
    float heading;
    
    // Конструктор
    Aircraft();
    
    // Обновление состояния самолета
    void update(float deltaTime);
    
    // Применение управляющих воздействий
    void applyControls(float stickX, float stickY, float rudder, float throttle);
    
    // Отрисовка самолета (не используется напрямую в этом проекте, но может быть полезна)
    void draw();
    
private:
    // Физические параметры
    static constexpr float GRAVITY = 9.81f;
    static constexpr float MASS = 1000.0f; // условная масса
    static constexpr float DRAG_COEFFICIENT = 0.05f;
    static constexpr float LIFT_COEFFICIENT = 0.1f;
};