#include "../include/InstrumentsPanel.h"
#include <cstdio>
#include <cstring>

InstrumentsPanel::InstrumentsPanel() {
    m_altitude = 1000.0f;  // Начальная высота
    m_airspeed = 150.0f;   // Начальная скорость
    m_heading = 0.0f;      // Начальный курс
    m_width = 800;
    m_height = 600;
}

void InstrumentsPanel::setAltitude(float altitude) {
    m_altitude = altitude;
}

void InstrumentsPanel::setAirspeed(float airspeed) {
    m_airspeed = airspeed;
}

void InstrumentsPanel::setHeading(float heading) {
    m_heading = heading;
}

void InstrumentsPanel::setSize(int width, int height) {
    m_width = width;
    m_height = height;
}

void InstrumentsPanel::draw() {
    // Рисуем панель приборов в нижней части экрана
    int altimeter_x = 50;
    int altimeter_y = 50;
    int airspeed_x = m_width - 150;
    int airspeed_y = 50;
    int heading_x = m_width / 2 - 100;
    int heading_y = m_height - 100;
    
    drawAltimeter();
    drawAirspeedIndicator();
    drawHeadingIndicator();
}

void InstrumentsPanel::drawAltimeter() {
    // Рисуем высотомер
    int x = 50;
    int y = 50;
    int radius = 80;
    
    // Основной круг
    glColor3f(0.1f, 0.1f, 0.1f); // темно-серый
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius);
    for (int i = 0; i <= 360; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(x + radius + radius * cos(rad), y + radius + radius * sin(rad));
    }
    glEnd();
    
    // Внутренний круг
    glColor3f(0.8f, 0.8f, 0.8f); // светло-серый
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius);
    for (int i = 0; i <= 360; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(x + radius + (radius - 10) * cos(rad), y + radius + (radius - 10) * sin(rad));
    }
    glEnd();
    
    // Шкала высотомера
    glColor3f(0.0f, 0.0f, 0.0f); // черный
    glLineWidth(1.0f);
    
    for (int i = 0; i < 10; i++) {
        float angle = 180.0f + i * 30.0f; // от 180 до 480 (эквивалентно -180 до +60)
        float inner_radius = radius - 15;
        float outer_radius = (i % 2 == 0) ? radius - 5 : radius - 10; // длинные и короткие метки
        
        float rad = angle * M_PI / 180.0f;
        float x1 = x + radius + inner_radius * cos(rad);
        float y1 = y + radius + inner_radius * sin(rad);
        float x2 = x + radius + outer_radius * cos(rad);
        float y2 = y + radius + outer_radius * sin(rad);
        
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
    
    // Стрелка высотомера (вращается каждые 1000 футов)
    glColor3f(1.0f, 0.0f, 0.0f); // красный
    glLineWidth(2.0f);
    float altimeter_angle = fmod(m_altitude, 1000.0f) * 360.0f / 1000.0f; // угол стрелки
    float needle_rad = (altimeter_angle - 90.0f) * M_PI / 180.0f;
    float needle_x = x + radius + (radius - 20) * cos(needle_rad);
    float needle_y = y + radius + (radius - 20) * sin(needle_rad);
    
    glBegin(GL_LINES);
    glVertex2f(x + radius, y + radius);
    glVertex2f(needle_x, needle_y);
    glEnd();
    
    // Маленькая стрелка для тысяч
    glColor3f(0.0f, 0.0f, 1.0f); // синий
    float thousands = floor(m_altitude / 1000.0f);
    float thousands_angle = thousands * 360.0f / 10.0f; // 10,000 футов за полный оборот
    float thousands_needle_rad = (thousands_angle - 90.0f) * M_PI / 180.0f;
    float thousands_needle_x = x + radius + (radius - 35) * cos(thousands_needle_rad);
    float thousands_needle_y = y + radius + (radius - 35) * sin(thousands_needle_rad);
    
    glBegin(GL_LINES);
    glVertex2f(x + radius, y + radius);
    glVertex2f(thousands_needle_x, thousands_needle_y);
    glEnd();
    
    // Центральный кружок
    glColor3f(0.0f, 0.0f, 0.0f); // черный
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius);
    for (int i = 0; i <= 360; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(x + radius + 5 * cos(rad), y + radius + 5 * sin(rad));
    }
    glEnd();
    
    // Показания высоты текстом
    char text[20];
    sprintf(text, "%.0f", m_altitude);
    glColor3f(0.0f, 0.0f, 0.0f); // черный
    drawText(text, x + radius - 20, y + radius - 120);
}

void InstrumentsPanel::drawAirspeedIndicator() {
    // Рисуем указатель скорости
    int x = m_width - 150;
    int y = 50;
    int radius = 80;
    
    // Основной круг
    glColor3f(0.1f, 0.1f, 0.1f); // темно-серый
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius);
    for (int i = 0; i <= 360; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(x + radius + radius * cos(rad), y + radius + radius * sin(rad));
    }
    glEnd();
    
    // Внутренний круг
    glColor3f(0.8f, 0.8f, 0.8f); // светло-серый
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius);
    for (int i = 0; i <= 360; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(x + radius + (radius - 10) * cos(rad), y + radius + (radius - 10) * sin(rad));
    }
    glEnd();
    
    // Шкала указателя скорости
    glColor3f(0.0f, 0.0f, 0.0f); // черный
    glLineWidth(1.0f);
    
    for (int i = 0; i < 10; i++) {
        float angle = 180.0f + i * 30.0f;
        float inner_radius = radius - 15;
        float outer_radius = (i % 2 == 0) ? radius - 5 : radius - 10;
        
        float rad = angle * M_PI / 180.0f;
        float x1 = x + radius + inner_radius * cos(rad);
        float y1 = y + radius + inner_radius * sin(rad);
        float x2 = x + radius + outer_radius * cos(rad);
        float y2 = y + radius + outer_radius * sin(rad);
        
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
    
    // Стрелка указателя скорости
    glColor3f(1.0f, 0.0f, 0.0f); // красный
    glLineWidth(2.0f);
    float airspeed_angle = (m_airspeed / 300.0f) * 270.0f; // 0-300 км/ч -> 0-270 градусов
    float needle_rad = (airspeed_angle - 90.0f) * M_PI / 180.0f;
    float needle_x = x + radius + (radius - 20) * cos(needle_rad);
    float needle_y = y + radius + (radius - 20) * sin(needle_rad);
    
    glBegin(GL_LINES);
    glVertex2f(x + radius, y + radius);
    glVertex2f(needle_x, needle_y);
    glEnd();
    
    // Центральный кружок
    glColor3f(0.0f, 0.0f, 0.0f); // черный
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + radius, y + radius);
    for (int i = 0; i <= 360; i += 5) {
        float rad = i * M_PI / 180.0f;
        glVertex2f(x + radius + 5 * cos(rad), y + radius + 5 * sin(rad));
    }
    glEnd();
    
    // Показания скорости текстом
    char text[20];
    sprintf(text, "%.0f", m_airspeed);
    glColor3f(0.0f, 0.0f, 0.0f); // черный
    drawText(text, x + radius - 20, y + radius - 120);
}

void InstrumentsPanel::drawHeadingIndicator() {
    // Рисуем курсоуказатель
    int x = m_width / 2 - 100;
    int y = m_height - 80;
    int width = 200;
    int height = 40;
    
    // Фон индикатора
    glColor3f(0.1f, 0.1f, 0.1f); // темно-серый
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Шкала курса
    glColor3f(1.0f, 1.0f, 1.0f); // белый
    glLineWidth(1.0f);
    
    // Рисуем деления шкалы курса
    for (int i = 0; i < 36; i++) {
        int course = (i * 10) % 360;
        float pos_x = x + 100 + (course - m_heading) * (width / 60.0f); // Центрируем на текущем курсе
        
        // Обеспечиваем цикличность (360 <-> 0)
        if (pos_x < x) pos_x += width * 360.0f / width; // корректируем для видимости
        else if (pos_x > x + width) pos_x -= width * 360.0f / width;
        
        if (pos_x >= x && pos_x <= x + width) {
            if (i % 3 == 0) { // крупные деления на каждые 30 градусов
                glBegin(GL_LINES);
                glVertex2f(pos_x, y);
                glVertex2f(pos_x, y + 15);
                glEnd();
                
                // Метки N, E, S, W
                switch(course) {
                    case 0: drawText("N", pos_x - 5, y + 20); break;
                    case 90: drawText("E", pos_x - 5, y + 20); break;
                    case 180: drawText("S", pos_x - 5, y + 20); break;
                    case 270: drawText("W", pos_x - 5, y + 20); break;
                    default:
                        if (course % 30 == 0) {
                            char text[5];
                            sprintf(text, "%d", course / 10);
                            drawText(text, pos_x - 5, y + 20);
                        }
                        break;
                }
            } else { // мелкие деления
                glBegin(GL_LINES);
                glVertex2f(pos_x, y);
                glVertex2f(pos_x, y + 8);
                glEnd();
            }
        }
    }
    
    // Треугольник указателя курса
    glColor3f(1.0f, 1.0f, 1.0f); // белый
    glBegin(GL_TRIANGLES);
    glVertex2f(x + width/2.0f - 10, y + height);
    glVertex2f(x + width/2.0f + 10, y + height);
    glVertex2f(x + width/2.0f, y + height - 10);
    glEnd();
    
    // Центральная линия
    glColor3f(1.0f, 0.0f, 0.0f); // красный
    glBegin(GL_LINES);
    glVertex2f(x + width/2.0f, y);
    glVertex2f(x + width/2.0f, y + height);
    glEnd();
}

void InstrumentsPanel::drawText(const std::string& text, float x, float y) {
    // Упрощенная функция вывода текста
    // В реальной реализации здесь будет использовать библиотека для рендеринга текста
    // Для простоты в данной реализации просто игнорируем вывод текста
    // В полноценной системе можно использовать FreeType/GLUT для рендеринга текста
}