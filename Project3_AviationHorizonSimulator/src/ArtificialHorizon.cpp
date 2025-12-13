#include "../include/ArtificialHorizon.h"

ArtificialHorizon::ArtificialHorizon() {
    m_pitch = 0.0f;
    m_roll = 0.0f;
    m_width = 400;
    m_height = 400;
}

void ArtificialHorizon::setAttitude(float pitch, float roll) {
    m_pitch = pitch;
    m_roll = roll;
}

void ArtificialHorizon::setSize(int width, int height) {
    m_width = width;
    m_height = height;
}

void ArtificialHorizon::draw() {
    // Сохраняем текущую матрицу
    glPushMatrix();
    
    // Переводим начало координат в центр области отображения
    glTranslatef(m_width / 2.0f, m_height / 2.0f, 0.0f);
    
    // Применяем крен
    glRotatef(m_roll, 0.0f, 0.0f, 1.0f);
    
    // Рисуем небо (синий цвет)
    glColor3f(0.3f, 0.6f, 1.0f); // светло-синий
    glBegin(GL_QUADS);
    glVertex2f(-m_width/2.0f, 0.0f);
    glVertex2f(m_width/2.0f, 0.0f);
    glVertex2f(m_width/2.0f, m_height/2.0f);
    glVertex2f(-m_width/2.0f, m_height/2.0f);
    glEnd();
    
    // Рисуем землю (коричневый/зеленый цвет)
    glColor3f(0.6f, 0.4f, 0.2f); // коричневато-зеленоватый
    glBegin(GL_QUADS);
    glVertex2f(-m_width/2.0f, 0.0f);
    glVertex2f(m_width/2.0f, 0.0f);
    glVertex2f(m_width/2.0f, -m_height/2.0f);
    glVertex2f(-m_width/2.0f, -m_height/2.0f);
    glEnd();
    
    // Рисуем линию горизонта с учетом тангажа
    glPushMatrix();
    glTranslatef(0.0f, -m_pitch * (m_height / 90.0f) / 2.0f, 0.0f); // масштабирование тангажа
    
    glColor3f(1.0f, 1.0f, 1.0f); // белый цвет для горизонта
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-m_width/2.0f, 0.0f);
    glVertex2f(m_width/2.0f, 0.0f);
    glEnd();
    
    // Рисуем шкалу углов тангажа
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.0f);
    
    // Горизонтальные линии для шкалы тангажа (через 10 градусов)
    for (int angle = -90; angle <= 90; angle += 10) {
        if (angle == 0) continue; // пропускаем центральную линию
        
        float y_pos = -angle * (m_height / 90.0f) / 2.0f;
        
        // Проверяем, находится ли линия в пределах экрана
        if (y_pos >= -m_height/2.0f && y_pos <= m_height/2.0f) {
            glBegin(GL_LINES);
            glVertex2f(-20.0f, y_pos);
            glVertex2f(20.0f, y_pos);
            glEnd();
            
            // Метки углов
            char buffer[10];
            sprintf(buffer, "%d", abs(angle));
            // Здесь в идеале нужно добавить вывод текста, но для упрощения опустим
        }
    }
    
    glPopMatrix();
    
    // Рисуем самолет (центральный элемент)
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    
    // Центральный маркер (нос самолета)
    glBegin(GL_LINES);
    glVertex2f(-30.0f, 0.0f);
    glVertex2f(30.0f, 0.0f); // основная линия по горизонтали
    glEnd();
    
    // Вертикальная линия в центре
    glBegin(GL_LINES);
    glVertex2f(0.0f, -15.0f);
    glVertex2f(0.0f, 15.0f);
    glEnd();
    
    // Боковые указатели
    glBegin(GL_TRIANGLES);
    // Левый треугольник
    glVertex2f(-40.0f, 5.0f);
    glVertex2f(-50.0f, -5.0f);
    glVertex2f(-30.0f, -5.0f);
    
    // Правый треугольник
    glVertex2f(40.0f, 5.0f);
    glVertex2f(50.0f, -5.0f);
    glVertex2f(30.0f, -5.0f);
    glEnd();
    
    // Восстанавливаем матрицу
    glPopMatrix();
}

void ArtificialHorizon::drawHorizonLine() {
    // Уже реализовано в основном методе draw()
}

void ArtificialHorizon::drawRollScale() {
    // Рисуем шкалу крена вокруг центра
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Рисуем метки крена в верхней части индикатора
    float center_x = m_width / 2.0f;
    float center_y = m_height / 2.0f;
    
    // Сохраняем текущую матрицу
    glPushMatrix();
    glTranslatef(center_x, center_y, 0.0f);
    
    // Метки крена (в градусах): 10, 20, 30, 60, 90
    int rollMarks[] = {10, 20, 30, 60, 90};
    int numMarks = sizeof(rollMarks) / sizeof(rollMarks[0]);
    
    for (int i = 0; i < numMarks; i++) {
        // Положительное значение крена (правый поворот)
        glPushMatrix();
        glRotatef(rollMarks[i], 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, -m_height/2.0f + 10.0f);
        glVertex2f(0.0f, -m_height/2.0f + 20.0f);
        glEnd();
        glPopMatrix();
        
        // Отрицательное значение крена (левый поворот)
        glPushMatrix();
        glRotatef(-rollMarks[i], 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex2f(0.0f, -m_height/2.0f + 10.0f);
        glVertex2f(0.0f, -m_height/2.0f + 20.0f);
        glEnd();
        glPopMatrix();
    }
    
    // Метки 45 и 90 градусов помечаем цифрами
    for (int angle : {30, 45, 60, 90}) {
        glPushMatrix();
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        // Здесь в идеале нужно добавить вывод текста
        glPopMatrix();
        
        glPushMatrix();
        glRotatef(-angle, 0.0f, 0.0f, 1.0f);
        // Здесь в идеале нужно добавить вывод текста
        glPopMatrix();
    }
    
    glPopMatrix();
}

void ArtificialHorizon::drawPitchScale() {
    // Уже частично реализовано в основном методе draw()
}