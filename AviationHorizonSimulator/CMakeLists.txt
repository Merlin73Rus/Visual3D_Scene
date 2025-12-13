cmake_minimum_required(VERSION 3.10)

project(AviationHorizonSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OGRE
find_package(OGRE REQUIRED)

# Find OIS
find_package(Threads REQUIRED)
find_package(OGRE COMPONENTS Overlay)

# Include OGRE directories
include_directories(${OGRE_INCLUDE_DIRS})
include_directories(/usr/include/OGRE)
include_directories(/usr/include/OIS)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/AviationHorizonSimulator.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${OGRE_LIBRARIES} 
    OIS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

# Set C++ flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -std=c++17)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)