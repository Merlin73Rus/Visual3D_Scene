#pragma once
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

// Класс, реализующий панель приборов (высотомер и указатель скорости)
class InstrumentsPanel {
public:
    // Конструктор
    InstrumentsPanel();
    
    // Установка значений приборов
    void setAltitude(float altitude);
    void setAirspeed(float airspeed);
    void setHeading(float heading);
    
    // Отрисовка панели приборов
    void draw();
    
    // Установка размеров области отображения
    void setSize(int width, int height);

private:
    float m_altitude;   // Высота
    float m_airspeed;   // Скорость
    float m_heading;    // Курс
    
    int m_width;        // Ширина области отображения
    int m_height;       // Высота области отображения
    
    // Рисование высотомера
    void drawAltimeter();
    
    // Рисование указателя скорости
    void drawAirspeedIndicator();
    
    // Рисование курсоуказателя
    void drawHeadingIndicator();
    
    // Рисование текста
    void drawText(const std::string& text, float x, float y);
};