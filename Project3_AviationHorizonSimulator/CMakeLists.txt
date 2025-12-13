cmake_minimum_required(VERSION 3.10)
project(AviationHorizonSimulator)

set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Application.cpp
    src/Aircraft.cpp
    src/ArtificialHorizon.cpp
    src/InstrumentsPanel.cpp
    src/JoystickHandler.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL 
    glfw 
    GLEW::GLEW
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()