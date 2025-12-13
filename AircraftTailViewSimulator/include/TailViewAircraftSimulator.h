#pragma once

#include <iostream>
#include <memory>
#include <vector>

// Forward declarations
class AircraftModel;
class CameraController;
class InputHandler;
class FlightPhysics;

/**
 * @brief Main class for the Tail View Aircraft Simulator
 * 
 * This class manages the entire simulation system including:
 * - 3D visualization of aircraft from tail view perspective
 * - Input handling from Logitech Extreme 3D Pro joystick
 * - Flight physics simulation
 * - Camera control from fixed tail view position
 */
class TailViewAircraftSimulator {
public:
    /**
     * @brief Constructor initializes the simulator components
     */
    TailViewAircraftSimulator();

    /**
     * @brief Destructor cleans up resources
     */
    ~TailViewAircraftSimulator();

    /**
     * @brief Initialize the simulator system
     * @return true if initialization successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Run the main simulation loop
     */
    void run();

    /**
     * @brief Update simulation state
     * @param deltaTime Time elapsed since last update
     */
    void update(float deltaTime);

    /**
     * @brief Render the current scene
     */
    void render();

    /**
     * @brief Cleanup resources before exit
     */
    void cleanup();

private:
    std::unique_ptr<AircraftModel> m_aircraft;
    std::unique_ptr<CameraController> m_camera;
    std::unique_ptr<InputHandler> m_inputHandler;
    std::unique_ptr<FlightPhysics> m_flightPhysics;

    bool m_running;
    float m_simulationTime;
};

/**
 * @brief Class representing the aircraft model in 3D space
 */
class AircraftModel {
public:
    AircraftModel();
    ~AircraftModel();

    void setPosition(float x, float y, float z);
    void setOrientation(float pitch, float yaw, float roll);
    
    void applyControlInputs(float throttle, float elevator, float aileron, float rudder);
    
    // Getters for position and orientation
    float getX() const { return m_positionX; }
    float getY() const { return m_positionY; }
    float getZ() const { return m_positionZ; }
    
    float getPitch() const { return m_pitch; }
    float getYaw() const { return m_yaw; }
    float getRoll() const { return m_roll; }

private:
    float m_positionX, m_positionY, m_positionZ;
    float m_pitch, m_yaw, m_roll;
    float m_velocityX, m_velocityY, m_velocityZ;
    float m_angularVelocityX, m_angularVelocityY, m_angularVelocityZ;
};

/**
 * @brief Class controlling the camera from tail view perspective
 */
class CameraController {
public:
    CameraController();
    ~CameraController();

    void setupTailView(const AircraftModel& aircraft);
    void updateViewMatrix();
    
    // Setters for camera parameters
    void setDistance(float distance) { m_distance = distance; }
    void setHeightOffset(float offset) { m_heightOffset = offset; }
    void setAngleOffset(float angle) { m_angleOffset = angle; }

private:
    float m_distance;      // Distance behind the aircraft
    float m_heightOffset;  // Height above the aircraft
    float m_angleOffset;   // Angular offset from direct tail view
};

/**
 * @brief Class handling input from Logitech Extreme 3D Pro joystick
 */
class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    bool initializeJoystick();
    void pollInputs();
    
    // Get normalized control inputs (-1.0 to 1.0)
    float getThrottle() const { return m_throttle; }
    float getElevator() const { return m_elevator; }  // Pitch control
    float getAileron() const { return m_aileron; }    // Roll control
    float getRudder() const { return m_rudder; }      // Yaw control

private:
    float m_throttle;
    float m_elevator;
    float m_aileron;
    float m_rudder;
    
    int m_joystickId;
    bool m_joystickConnected;
};

/**
 * @brief Class implementing basic flight physics
 */
class FlightPhysics {
public:
    FlightPhysics();
    ~FlightPhysics();

    void update(AircraftModel& aircraft, float throttle, float elevator, 
                float aileron, float rudder, float deltaTime);

private:
    void calculateForcesAndMoments(float throttle, float elevator, 
                                   float aileron, float rudder);
    void integrateMotion(AircraftModel& aircraft, float deltaTime);

    // Physical properties
    float m_mass;
    float m_momentOfInertiaX;
    float m_momentOfInertiaY;
    float m_momentOfInertiaZ;
};