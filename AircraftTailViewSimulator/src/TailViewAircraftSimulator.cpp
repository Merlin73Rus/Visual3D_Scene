#include "../include/TailViewAircraftSimulator.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Include libraries for joystick support
#include <GLFW/glfw3.h>

// Define constants
const float PI = 3.14159265359f;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

// Implementation of TailViewAircraftSimulator class
TailViewAircraftSimulator::TailViewAircraftSimulator() 
    : m_running(false), m_simulationTime(0.0f) {
    std::cout << "Initializing Tail View Aircraft Simulator..." << std::endl;
}

TailViewAircraftSimulator::~TailViewAircraftSimulator() {
    std::cout << "Cleaning up Tail View Aircraft Simulator..." << std::endl;
}

bool TailViewAircraftSimulator::initialize() {
    std::cout << "Initializing simulator components..." << std::endl;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Aircraft Tail View Simulator", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Enable joystick support
    if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_FALSE) {
        std::cout << "Warning: Joystick not detected. Using keyboard controls." << std::endl;
    } else {
        std::cout << "Logitech Extreme 3D Pro joystick detected." << std::endl;
    }

    // Initialize components
    m_aircraft = std::make_unique<AircraftModel>();
    m_camera = std::make_unique<CameraController>();
    m_inputHandler = std::make_unique<InputHandler>();
    m_flightPhysics = std::make_unique<FlightPhysics>();

    if (!m_inputHandler->initializeJoystick()) {
        std::cout << "Using keyboard controls as fallback." << std::endl;
    }

    m_running = true;
    std::cout << "Initialization complete." << std::endl;
    return true;
}

void TailViewAircraftSimulator::run() {
    std::cout << "Starting simulation loop..." << std::endl;
    
    GLFWwindow* window = glfwGetCurrentContext();
    float lastFrameTime = static_cast<float>(glfwGetTime());
    
    while (!glfwWindowShouldClose(window) && m_running) {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Handle events
        glfwPollEvents();

        // Update simulation
        update(deltaTime);

        // Render scene
        render();

        // Swap buffers
        glfwSwapBuffers(window);
    }
    
    std::cout << "Simulation ended." << std::endl;
}

void TailViewAircraftSimulator::update(float deltaTime) {
    // Poll input
    m_inputHandler->pollInputs();

    // Apply flight physics
    m_flightPhysics->update(*m_aircraft, 
                            m_inputHandler->getThrottle(),
                            m_inputHandler->getElevator(),
                            m_inputHandler->getAileron(),
                            m_inputHandler->getRudder(),
                            deltaTime);

    // Update camera to follow aircraft from tail view
    m_camera->setupTailView(*m_aircraft);

    m_simulationTime += deltaTime;
}

void TailViewAircraftSimulator::render() {
    // Clear the screen
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);  // Sky blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render ground/skybox
    renderEnvironment();

    // Render aircraft
    renderAircraft();

    // Render HUD
    renderHUD();
}

void TailViewAircraftSimulator::cleanup() {
    m_running = false;
    
    m_aircraft.reset();
    m_camera.reset();
    m_inputHandler.reset();
    m_flightPhysics.reset();
    
    glfwTerminate();
}

void TailViewAircraftSimulator::renderEnvironment() {
    // Simple ground rendering
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.2f);  // Green ground
    
    // Ground plane
    glVertex3f(-50.0f, -1.0f, -50.0f);
    glVertex3f(50.0f, -1.0f, -50.0f);
    glVertex3f(50.0f, -1.0f, 50.0f);
    glVertex3f(-50.0f, -1.0f, 50.0f);
    
    glEnd();
}

void TailViewAircraftSimulator::renderAircraft() {
    // Save current matrix
    glPushMatrix();
    
    // Position and orient the aircraft
    glTranslatef(m_aircraft->getX(), m_aircraft->getY(), m_aircraft->getZ());
    
    // Apply rotations in proper order: Z (roll), Y (yaw), X (pitch)
    glRotatef(m_aircraft->getRoll() * RAD_TO_DEG, 0.0f, 0.0f, 1.0f);
    glRotatef(m_aircraft->getYaw() * RAD_TO_DEG, 0.0f, 1.0f, 0.0f);
    glRotatef(m_aircraft->getPitch() * RAD_TO_DEG, 1.0f, 0.0f, 0.0f);
    
    // Draw simple aircraft shape (like a basic airplane)
    drawAircraftShape();
    
    // Restore matrix
    glPopMatrix();
}

void TailViewAircraftSimulator::drawAircraftShape() {
    // Draw fuselage
    glColor3f(0.8f, 0.8f, 0.8f);  // Light gray
    glBegin(GL_LINES);
    
    // Fuselage - from nose to tail
    glVertex3f(2.0f, 0.0f, 0.0f);   // Nose
    glVertex3f(-2.0f, 0.0f, 0.0f);  // Tail
    
    // Wings
    glVertex3f(-0.5f, 0.0f, -1.5f);  // Left wing tip
    glVertex3f(-0.5f, 0.0f, 1.5f);   // Right wing tip
    
    // Horizontal stabilizer
    glVertex3f(-1.8f, 0.1f, -0.5f);  // Left hstab tip
    glVertex3f(-1.8f, 0.1f, 0.5f);   // Right hstab tip
    
    // Vertical stabilizer
    glVertex3f(-1.8f, 0.1f, 0.0f);   // Base
    glVertex3f(-1.8f, 0.8f, 0.0f);   // Top
    
    glEnd();
}

void TailViewAircraftSimulator::renderHUD() {
    // Simple HUD showing flight parameters
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // This would normally render text, but for now we'll just note what should be displayed
    // - Altitude
    // - Airspeed
    // - Attitude indicators
    // - Control inputs
}

// Implementation of AircraftModel class
AircraftModel::AircraftModel() 
    : m_positionX(0.0f), m_positionY(0.0f), m_positionZ(0.0f),
      m_pitch(0.0f), m_yaw(0.0f), m_roll(0.0f),
      m_velocityX(0.0f), m_velocityY(0.0f), m_velocityZ(0.0f),
      m_angularVelocityX(0.0f), m_angularVelocityY(0.0f), m_angularVelocityZ(0.0f) {
}

AircraftModel::~AircraftModel() {
}

void AircraftModel::setPosition(float x, float y, float z) {
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
}

void AircraftModel::setOrientation(float pitch, float yaw, float roll) {
    m_pitch = pitch;
    m_yaw = yaw;
    m_roll = roll;
}

void AircraftModel::applyControlInputs(float throttle, float elevator, float aileron, float rudder) {
    // These values will be used by the flight physics system
    // Actual implementation depends on the physics calculations
}

// Implementation of CameraController class
CameraController::CameraController() 
    : m_distance(10.0f), m_heightOffset(3.0f), m_angleOffset(0.0f) {
}

CameraController::~CameraController() {
}

void CameraController::setupTailView(const AircraftModel& aircraft) {
    float aircraftX = aircraft.getX();
    float aircraftY = aircraft.getY();
    float aircraftZ = aircraft.getZ();
    float aircraftYaw = aircraft.getYaw();
    float aircraftPitch = aircraft.getPitch();
    
    // Calculate camera position behind and slightly above the aircraft
    float camX = aircraftX - cos(aircraftYaw) * m_distance;
    float camZ = aircraftZ - sin(aircraftYaw) * m_distance;
    float camY = aircraftY + m_heightOffset;
    
    // Calculate look-at point (the aircraft position)
    float targetX = aircraftX;
    float targetY = aircraftY;
    float targetZ = aircraftZ;
    
    // Calculate up vector (affected by aircraft attitude)
    float upX = -sin(aircraftRoll);
    float upY = cos(aircraftPitch);
    float upZ = sin(aircraftPitch) * sin(aircraftRoll);
    
    // Normalize up vector
    float upLength = sqrt(upX * upX + upY * upY + upZ * upZ);
    if (upLength > 0.0f) {
        upX /= upLength;
        upY /= upLength;
        upZ /= upLength;
    } else {
        upY = 1.0f;  // Default up vector if attitude is neutral
    }
    
    // Set the camera view (in a real implementation, this would set the view matrix)
    gluLookAt(camX, camY, camZ, targetX, targetY, targetZ, upX, upY, upZ);
}

void CameraController::updateViewMatrix() {
    // This would update the OpenGL view matrix in a real implementation
}

// Implementation of InputHandler class
InputHandler::InputHandler() 
    : m_throttle(0.0f), m_elevator(0.0f), m_aileron(0.0f), m_rudder(0.0f),
      m_joystickId(GLFW_JOYSTICK_1), m_joystickConnected(false) {
}

InputHandler::~InputHandler() {
}

bool InputHandler::initializeJoystick() {
    if (glfwJoystickPresent(m_joystickId)) {
        const char* name = glfwGetJoystickName(m_joystickId);
        std::cout << "Connected joystick: " << name << std::endl;
        
        int axesCount;
        const float* axes = glfwGetJoystickAxes(m_joystickId, &axesCount);
        
        if (axesCount >= 4) {
            m_joystickConnected = true;
            std::cout << "Logitech Extreme 3D Pro joystick initialized successfully." << std::endl;
            return true;
        }
    }
    
    std::cout << "Failed to initialize joystick." << std::endl;
    return false;
}

void InputHandler::pollInputs() {
    if (m_joystickConnected && glfwJoystickPresent(m_joystickId)) {
        int axesCount;
        const float* axes = glfwGetJoystickAxes(m_joystickId, &axesCount);
        
        if (axesCount >= 4) {
            // Assuming typical joystick mapping for Logitech Extreme 3D Pro:
            // Axis 0: Aileron (roll control) - left/right stick movement
            // Axis 1: Elevator (pitch control) - forward/back stick movement
            // Axis 2: Throttle - slider on the left
            // Axis 3: Rudder (yaw control) - rotation of stick
            
            // Note: Some axes might need inversion depending on the specific joystick
            m_aileron = (axesCount > 0) ? -axes[0] : 0.0f;      // Invert aileron for proper control
            m_elevator = (axesCount > 1) ? -axes[1] : 0.0f;     // Invert elevator for proper control
            m_throttle = (axesCount > 2) ? axes[2] : 0.0f;      // Throttle axis
            m_rudder = (axesCount > 3) ? axes[3] : 0.0f;        // Rudder axis
            
            // Additional axes may include hat switches, etc.
        }
    } else {
        // Fallback to keyboard controls if joystick is not available
        GLFWwindow* window = glfwGetCurrentContext();
        if (window) {
            // Map keyboard keys to aircraft controls
            m_elevator = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) m_elevator = -0.5f;
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) m_elevator = 0.5f;
            
            m_aileron = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) m_aileron = -0.5f;
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) m_aileron = 0.5f;
            
            m_throttle = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) m_throttle = 1.0f;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) m_throttle = -1.0f;
            
            m_rudder = 0.0f;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) m_rudder = -0.5f;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) m_rudder = 0.5f;
        }
    }
}

// Implementation of FlightPhysics class
FlightPhysics::FlightPhysics() 
    : m_mass(1000.0f),                    // 1000 kg aircraft
      m_momentOfInertiaX(2000.0f),       // Roll moment of inertia
      m_momentOfInertiaY(5000.0f),       // Pitch moment of inertia
      m_momentOfInertiaZ(4000.0f) {      // Yaw moment of inertia
}

FlightPhysics::~FlightPhysics() {
}

void FlightPhysics::update(AircraftModel& aircraft, float throttle, float elevator, 
                          float aileron, float rudder, float deltaTime) {
    // Calculate forces and moments based on control inputs
    calculateForcesAndMoments(throttle, elevator, aileron, rudder);
    
    // Integrate motion equations
    integrateMotion(aircraft, deltaTime);
}

void FlightPhysics::calculateForcesAndMoments(float throttle, float elevator, 
                                             float aileron, float rudder) {
    // Simplified force and moment calculations
    // In a real implementation, these would be more complex aerodynamic models
    
    // Thrust force along the longitudinal axis of the aircraft
    float thrust = throttle * 10000.0f;  // Max thrust of 10000N
    
    // Control surface effectiveness
    float pitchMoment = elevator * 5000.0f;   // Elevator effectiveness
    float rollMoment = aileron * 3000.0f;     // Aileron effectiveness
    float yawMoment = rudder * 2000.0f;       // Rudder effectiveness
    
    // These values would affect the aircraft's motion in the integration step
}

void FlightPhysics::integrateMotion(AircraftModel& aircraft, float deltaTime) {
    // Simple Euler integration for demonstration purposes
    // In a real flight simulator, more sophisticated integration methods would be used
    
    // Update angular velocities based on moments (simplified)
    float pitchRate = 0.0f;  // Would be calculated from pitch moment and moment of inertia
    float rollRate = 0.0f;   // Would be calculated from roll moment and moment of inertia
    float yawRate = 0.0f;    // Would be calculated from yaw moment and moment of inertia
    
    // Update orientations
    float newPitch = aircraft.getPitch() + pitchRate * deltaTime;
    float newYaw = aircraft.getYaw() + yawRate * deltaTime;
    float newRoll = aircraft.getRoll() + rollRate * deltaTime;
    
    // Keep angles within reasonable bounds
    newPitch = fmaxf(-PI/2, fminf(PI/2, newPitch));  // Limit pitch to ±90 degrees
    
    aircraft.setOrientation(newPitch, newYaw, newRoll);
    
    // Update position based on velocity and orientation
    // This is a very simplified approach
    float speed = 20.0f;  // Constant speed for demo purposes
    
    float velX = speed * cos(newPitch) * cos(newYaw);
    float velY = speed * sin(newPitch);
    float velZ = speed * cos(newPitch) * sin(newYaw);
    
    float newX = aircraft.getX() + velX * deltaTime;
    float newY = aircraft.getY() + velY * deltaTime;
    float newZ = aircraft.getZ() + velZ * deltaTime;
    
    aircraft.setPosition(newX, newY, newZ);
}