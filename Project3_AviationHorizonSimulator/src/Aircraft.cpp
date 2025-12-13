#include "../include/Aircraft.h"

Aircraft::Aircraft() {
    // Инициализация начального состояния
    x = y = z = 0.0f;
    pitch = yaw = roll = 0.0f;
    
    velocity_x = velocity_y = velocity_z = 0.0f;
    angular_velocity_pitch = angular_velocity_yaw = angular_velocity_roll = 0.0f;
    
    altitude = 1000.0f; // Начальная высота 1000 метров
    airspeed = 150.0f;  // Начальная скорость 150 км/ч
    heading = 0.0f;     // Начальный курс 0 градусов (на север)
}

void Aircraft::update(float deltaTime) {
    // Обновление позиции на основе скорости
    x += velocity_x * deltaTime;
    y += velocity_y * deltaTime;
    z += velocity_z * deltaTime;
    
    // Обновление углов на основе угловых скоростей
    pitch += angular_velocity_pitch * deltaTime;
    yaw += angular_velocity_yaw * deltaTime;
    roll += angular_velocity_roll * deltaTime;
    
    // Ограничение углов
    while (pitch > 180.0f) pitch -= 360.0f;
    while (pitch <= -180.0f) pitch += 360.0f;
    while (roll > 180.0f) roll -= 360.0f;
    while (roll <= -180.0f) roll += 360.0f;
    while (yaw > 180.0f) yaw -= 360.0f;
    while (yaw <= -180.0f) yaw += 360.0f;
    
    // Обновление параметров полета
    altitude = -y + 1000.0f; // Высота над уровнем моря (условно)
    airspeed = sqrt(velocity_x * velocity_x + velocity_y * velocity_y + velocity_z * velocity_z) * 3.6f; // в км/ч
    heading = yaw;
    
    // Применение силы тяжести
    velocity_y -= GRAVITY * deltaTime;
    
    // Применение аэродинамических сил (упрощенно)
    float speed = sqrt(velocity_x * velocity_x + velocity_y * velocity_y + velocity_z * velocity_z);
    float drag_force = DRAG_COEFFICIENT * speed * speed;
    
    // Направление вектора скорости
    if (speed > 0.001f) {
        float inv_speed = 1.0f / speed;
        velocity_x -= drag_force * velocity_x * inv_speed * deltaTime;
        velocity_y -= drag_force * velocity_y * inv_speed * deltaTime;
        velocity_z -= drag_force * velocity_z * inv_speed * deltaTime;
    }
}

void Aircraft::applyControls(float stickX, float stickY, float rudder, float throttle) {
    // Управление креном (stickX)
    // При отклонении стика влево или вправо создается момент крена
    angular_velocity_roll += stickX * 2.0f; // коэффициент чувствительности
    
    // Управление тангажом (stickY)
    // При отклонении стика вперед или назад создается момент тангажа
    angular_velocity_pitch += stickY * 2.0f; // коэффициент чувствительности
    
    // Управление рысканьем (rudder)
    // Используется для изменения курса
    angular_velocity_yaw += rudder * 1.0f; // коэффициент чувствительности
    
    // Учет тяги двигателя (throttle)
    // Увеличение тяги увеличивает скорость вперед
    float thrust = throttle * 50.0f; // максимальная тяга
    
    // Применение тяги в направлении носа самолета
    float pitch_rad = pitch * M_PI / 180.0f;
    float yaw_rad = yaw * M_PI / 180.0f;
    
    velocity_x += thrust * cos(pitch_rad) * sin(yaw_rad);
    velocity_y += thrust * sin(pitch_rad);
    velocity_z += thrust * cos(pitch_rad) * cos(yaw_rad);
    
    // Ограничение угловых скоростей
    const float MAX_ANGULAR_VELOCITY = 10.0f;
    if (angular_velocity_pitch > MAX_ANGULAR_VELOCITY) angular_velocity_pitch = MAX_ANGULAR_VELOCITY;
    if (angular_velocity_pitch < -MAX_ANGULAR_VELOCITY) angular_velocity_pitch = -MAX_ANGULAR_VELOCITY;
    if (angular_velocity_yaw > MAX_ANGULAR_VELOCITY) angular_velocity_yaw = MAX_ANGULAR_VELOCITY;
    if (angular_velocity_yaw < -MAX_ANGULAR_VELOCITY) angular_velocity_yaw = -MAX_ANGULAR_VELOCITY;
    if (angular_velocity_roll > MAX_ANGULAR_VELOCITY) angular_velocity_roll = MAX_ANGULAR_VELOCITY;
    if (angular_velocity_roll < -MAX_ANGULAR_VELOCITY) angular_velocity_roll = -MAX_ANGULAR_VELOCITY;
}

void Aircraft::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);  // Тангаж
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);    // Рысканье
    glRotatef(roll, 0.0f, 0.0f, 1.0f);   // Крен
    
    // В этом проекте визуализация не требуется, так как основное внимание на приборы
    // Но можно добавить базовую модель для демонстрации
    
    glPopMatrix();
}