cmake_minimum_required(VERSION 3.10)
project(AviationHorizonDiagnostic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
if(PkgConfig_FOUND)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/HorizonDiagnosticApp.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES} 
    ${SDL2IMAGE_LIBRARIES}
    -lSDL2 -lSDL2_image
)

# Set include directories for target
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SDL2_INCLUDE_DIRS}
    ${SDL2IMAGE_INCLUDE_DIRS}
)

# Compiler-specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()