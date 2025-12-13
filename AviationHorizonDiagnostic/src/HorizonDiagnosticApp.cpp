#include "../include/HorizonDiagnosticApp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

HorizonDiagnosticApp::HorizonDiagnosticApp() 
    : window(nullptr), renderer(nullptr), currentSlideTexture(nullptr),
      running(false), currentSlideIndex(0), currentModule(1), slideDisplayed(false),
      joystickXAxis(0), joystickYAxis(0), joystickButtons(0) {
}

HorizonDiagnosticApp::~HorizonDiagnosticApp() {
    cleanup();
}

bool HorizonDiagnosticApp::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create window
    window = SDL_CreateWindow("Aviation Horizon Diagnostic Program",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize joystick
    if (SDL_NumJoysticks() < 1) {
        std::cerr << "Warning: No joysticks connected!" << std::endl;
    } else {
        SDL_Joystick* joystick = SDL_JoystickOpen(0);
        if (joystick == nullptr) {
            std::cerr << "Warning: Unable to open joystick! SDL Error: " << SDL_GetError() << std::endl;
        }
    }

    // Generate test slides for both modules
    generateTestSlides(1); // First module
    generateTestSlides(2); // Second module

    running = true;
    slideStartTime = std::chrono::steady_clock::now();
    slideDisplayed = true;

    return true;
}

void HorizonDiagnosticApp::run() {
    while (running) {
        handleEvents();
        update();
        render();
        
        SDL_Delay(16); // ~60 FPS
    }
}

void HorizonDiagnosticApp::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
                
            case SDL_JOYAXISMOTION:
                if (event.jaxis.axis == 0) { // X-axis (roll control)
                    joystickXAxis = event.jaxis.value;
                } else if (event.jaxis.axis == 1) { // Y-axis (pitch control)
                    joystickYAxis = event.jaxis.value;
                }
                break;
                
            case SDL_JOYBUTTONDOWN:
                joystickButtons |= (1 << event.jbutton.button);
                // Handle button presses for user responses
                // Button 0 (trigger) - left bank
                // Button 1 - right bank
                if (event.jbutton.button == 0 && slideDisplayed) { // Trigger - Left bank
                    userResponses.push_back(true); // User thinks it's left bank
                    bool correct = (slides[currentSlideIndex].bankLeft == true);
                    correctness.push_back(correct);
                    nextSlide();
                } else if (event.jbutton.button == 1 && slideDisplayed) { // Button 1 - Right bank
                    userResponses.push_back(false); // User thinks it's right bank
                    bool correct = (slides[currentSlideIndex].bankLeft == false);
                    correctness.push_back(correct);
                    nextSlide();
                }
                break;
                
            case SDL_JOYBUTTONUP:
                joystickButtons &= ~(1 << event.jbutton.button);
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_LEFT:
                        if (slideDisplayed) {
                            userResponses.push_back(true); // Left bank
                            bool correct = (slides[currentSlideIndex].bankLeft == true);
                            correctness.push_back(correct);
                            nextSlide();
                        }
                        break;
                    case SDLK_RIGHT:
                        if (slideDisplayed) {
                            userResponses.push_back(false); // Right bank
                            bool correct = (slides[currentSlideIndex].bankLeft == false);
                            correctness.push_back(correct);
                            nextSlide();
                        }
                        break;
                }
                break;
        }
    }
}

void HorizonDiagnosticApp::update() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - slideStartTime).count();
    
    // Automatically advance to next slide after 3 seconds if no response
    if (elapsed >= SLIDE_DISPLAY_TIME_MS && slideDisplayed) {
        // Record no response or default response
        userResponses.push_back(false); // Default to right bank if no response
        correctness.push_back(false); // Mark as incorrect if no response
        nextSlide();
    }
}

void HorizonDiagnosticApp::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Display current slide if available
    if (currentSlideIndex < slides.size() && currentSlideTexture != nullptr) {
        SDL_Rect renderQuad = { 200, 100, 400, 400 }; // Centered on screen
        
        // Render rotated texture if needed
        SDL_Point center = {200, 200}; // Center of the image
        SDL_RenderCopyEx(renderer, currentSlideTexture, NULL, &renderQuad, 
                         slides[currentSlideIndex].rotationAngle, &center, SDL_FLIP_NONE);
    }

    // Display instructions
    SDL_Color textColor = {255, 255, 255, 255}; // White color
    // Note: We would normally render text here using SDL_ttf, but for simplicity we're skipping text rendering
    
    // Show current module and slide info
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect border = { 195, 95, 410, 410 };
    SDL_RenderDrawRect(renderer, &border);
    
    SDL_RenderPresent(renderer);
}

void HorizonDiagnosticApp::generateTestSlides(int module) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> bankDist(0, 1); // 0 = right bank, 1 = left bank
    std::uniform_int_distribution<> rotationDist(0, 3); // 0, 1, 2, 3 for 0, 90, 180, 270 degrees
    
    int startIndex = slides.size();
    int slideCount = 50; // 50 slides per module
    
    for (int i = 0; i < slideCount; ++i) {
        SlideData slide;
        
        // Generate random bank direction
        slide.bankLeft = (bankDist(gen) == 1);
        
        if (module == 1) {
            // First module: no rotation
            slide.rotationAngle = 0;
            slide.rotationMultiple = 0;
        } else {
            // Second module: random rotation (0, 90, 180, 270 degrees)
            int rotMult = rotationDist(gen);
            slide.rotationMultiple = rotMult;
            slide.rotationAngle = rotMult * 90;
        }
        
        // For now, we'll use placeholder images - in real implementation these would point to actual horizon images
        slide.imagePath = "assets/horizon_slide_" + std::to_string(i % 10) + ".png"; // Placeholder
        
        slides.push_back(slide);
    }
}

void HorizonDiagnosticApp::nextSlide() {
    currentSlideIndex++;
    
    // Check if we've completed the current module
    int moduleStartIndex = (currentModule == 1) ? 0 : 50;
    if (currentSlideIndex >= moduleStartIndex + 50) {
        if (currentModule == 1) {
            // Switch to second module
            switchToNextModule();
        } else {
            // Completed both modules - save results and exit
            saveResults();
            running = false;
        }
    } else {
        // Load next slide
        slideStartTime = std::chrono::steady_clock::now();
        slideDisplayed = true;
    }
}

void HorizonDiagnosticApp::switchToNextModule() {
    currentModule = 2;
    currentSlideIndex = 50; // Start from index 50 for second module
    
    // Generate slides for second module if not already done
    if (slides.size() < 100) {
        generateTestSlides(2);
    }
    
    slideStartTime = std::chrono::steady_clock::now();
    slideDisplayed = true;
}

void HorizonDiagnosticApp::saveResults() {
    std::ofstream outFile("diagnostic_results_" + std::to_string(std::time(0)) + ".txt");
    
    if (outFile.is_open()) {
        outFile << "Результаты диагностики способности к пространственному анализу авиагоризонта\n";
        outFile << "=======================================================================\n";
        outFile << "Дата и время: " << std::time(nullptr) << "\n\n";
        
        // Results for Module 1
        int correctModule1 = 0;
        for (int i = 0; i < 50 && i < correctness.size(); ++i) {
            if (correctness[i]) correctModule1++;
        }
        
        outFile << "Модуль 1 (базовый): 50 слайдов без вращения\n";
        outFile << "Правильных ответов: " << correctModule1 << "/50 (" 
                << (correctModule1 * 100 / 50) << "%)\n\n";
        
        // Results for Module 2
        int correctModule2 = 0;
        for (int i = 50; i < 100 && i < correctness.size(); ++i) {
            if (correctness[i]) correctModule2++;
        }
        
        outFile << "Модуль 2 (усложненный): 50 слайдов с вращением\n";
        outFile << "Правильных ответов: " << correctModule2 << "/50 (" 
                << (correctModule2 * 100 / 50) << "%)\n\n";
        
        outFile << "Общие результаты:\n";
        outFile << "Всего правильных ответов: " << (correctModule1 + correctModule2) << "/100 (" 
                << ((correctModule1 + correctModule2) * 100 / 100) << "%)\n\n";
        
        outFile << "Детализированные результаты:\n";
        for (size_t i = 0; i < userResponses.size(); ++i) {
            outFile << "Слайд " << (i+1) << ": ";
            outFile << "Истинное направление: " << (slides[i].bankLeft ? "левый" : "правый") 
                    << ", Ответ пользователя: " << (userResponses[i] ? "левый" : "правый")
                    << ", Правильно: " << (correctness[i] ? "да" : "нет");
            
            if (i >= 50) {
                outFile << ", Угол поворота: " << slides[i].rotationAngle << "°";
            }
            outFile << "\n";
        }
        
        outFile.close();
        std::cout << "Результаты сохранены в файл diagnostic_results_" << std::time(0) << ".txt\n";
    }
}

void HorizonDiagnosticApp::cleanup() {
    if (currentSlideTexture) {
        SDL_DestroyTexture(currentSlideTexture);
        currentSlideTexture = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    IMG_Quit();
    SDL_Quit();
}