#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>

// Структура для хранения данных о каждом слайде
struct SlideData {
    std::string imagePath;
    int rotationAngle; // Угол поворота изображения (0, 90, 180, 270)
    bool bankLeft;     // Истинное направление крена (true = левый, false = правый)
    int rotationMultiple; // Множитель для углов поворота (0, 1, 2, 3 для 0, 90, 180, 270 градусов)
};

class HorizonDiagnosticApp {
public:
    HorizonDiagnosticApp();
    ~HorizonDiagnosticApp();

    bool initialize();
    void run();
    void cleanup();

private:
    // SDL objects
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* currentSlideTexture;
    
    // Application state
    bool running;
    int currentSlideIndex;
    int currentModule; // 1 - первый модуль, 2 - второй модуль
    std::vector<SlideData> slides;
    std::vector<bool> userResponses; // Ответы пользователя
    std::vector<bool> correctness;   // Корректность ответов
    std::chrono::steady_clock::time_point slideStartTime;
    bool slideDisplayed;
    
    // Timing
    const int SLIDE_DISPLAY_TIME_MS = 3000; // 3 секунды на слайд
    
    // Input handling
    int joystickXAxis;
    int joystickYAxis;
    int joystickButtons;
    
    // Methods
    void handleEvents();
    void update();
    void render();
    void loadSlides();
    void generateTestSlides(int module);
    void displayCurrentSlide();
    void saveResults();
    void nextSlide();
    void switchToNextModule();
};