/**
 * @file main.cpp
 * @brief Entry point for the Aircraft Tail View Simulator
 * 
 * This application provides a training environment for developing 
 * spatial orientation skills using a fixed external view of an aircraft
 * from the tail perspective, controlled via Logitech Extreme 3D Pro joystick.
 */

#include "../include/TailViewAircraftSimulator.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "=========================================" << std::endl;
    std::cout << "Aircraft Tail View Simulator" << std::endl;
    std::cout << "Developing spatial orientation skills for aviation students" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    // Create the simulator instance
    TailViewAircraftSimulator simulator;
    
    // Initialize the simulator
    if (!simulator.initialize()) {
        std::cerr << "Failed to initialize the simulator!" << std::endl;
        return -1;
    }
    
    std::cout << "Simulator initialized successfully." << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "- Logitech Extreme 3D Pro Joystick:" << std::endl;
    std::cout << "  * Stick: Pitch (forward/back) and Roll (left/right)" << std::endl;
    std::cout << "  * Rudder pedals or twist: Yaw control" << std::endl;
    std::cout << "  * Throttle lever: Engine power/thrust" << std::endl;
    std::cout << std::endl;
    std::cout << "Alternative Keyboard Controls:" << std::endl;
    std::cout << "  * Arrow Keys: Pitch and Roll" << std::endl;
    std::cout << "  * A/D Keys: Yaw control" << std::endl;
    std::cout << "  * W/S Keys: Throttle control" << std::endl;
    std::cout << std::endl;
    std::cout << "Starting simulation... Press ESC to exit." << std::endl;
    
    // Run the simulation
    simulator.run();
    
    // Cleanup
    simulator.cleanup();
    
    std::cout << "Simulation ended." << std::endl;
    return 0;
}