cmake_minimum_required(VERSION 3.10)

# Project name
project(AircraftTailViewSimulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)

# On Windows, we might need to find GLFW separately or specify paths
# For now, assuming package managers handle this
if(WIN32)
    find_package(glfw3 REQUIRED CONFIG)
    find_package(GLEW REQUIRED)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    find_package(GLEW REQUIRED)
endif()

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/TailViewAircraftSimulator.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIRS}
)

# Link libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        OpenGL::GL
        glfw
        GLEW::GLEW
        ${CMAKE_DL_LIBS}
    )
else()
    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        GLEW::GLEW
        ${CMAKE_DL_LIBS}
    )
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)