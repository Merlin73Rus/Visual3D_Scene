#include "../include/HorizonDiagnosticApp.h"
#include <iostream>

int main(int argc, char* argv[]) {
    HorizonDiagnosticApp app;
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize the application!" << std::endl;
        return -1;
    }
    
    std::cout << "Aviation Horizon Diagnostic Program started." << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "- Joystick trigger (Button 0): Indicate LEFT bank" << std::endl;
    std::cout << "- Joystick Button 1: Indicate RIGHT bank" << std::endl;
    std::cout << "- Keyboard LEFT arrow: Indicate LEFT bank" << std::endl;
    std::cout << "- Keyboard RIGHT arrow: Indicate RIGHT bank" << std::endl;
    std::cout << "- ESC key: Exit program" << std::endl;
    std::cout << std::endl;
    std::cout << "Program will automatically advance slides every 3 seconds." << std::endl;
    std::cout << "Starting diagnostic test..." << std::endl;
    
    app.run();
    
    std::cout << "Diagnostic test completed. Results saved to file." << std::endl;
    
    return 0;
}