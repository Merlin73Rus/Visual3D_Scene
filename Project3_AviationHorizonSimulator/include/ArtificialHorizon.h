#pragma once
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

// Класс, реализующий искусственный горизонт (вид с воздушного судна)
class ArtificialHorizon {
public:
    // Конструктор
    ArtificialHorizon();
    
    // Установка углов тангажа и крена для отображения
    void setAttitude(float pitch, float roll);
    
    // Отрисовка искусственного горизонта
    void draw();
    
    // Установка размеров области отображения
    void setSize(int width, int height);

private:
    float m_pitch;  // Угол тангажа (градусы)
    float m_roll;   // Угол крена (градусы)
    int m_width;    // Ширина области отображения
    int m_height;   // Высота области отображения
    
    // Рисование горизонтальной линии горизонта
    void drawHorizonLine();
    
    // Рисование шкалы крена
    void drawRollScale();
    
    // Рисование шкалы тангажа
    void drawPitchScale();
};