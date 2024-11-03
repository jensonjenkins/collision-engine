cmake_minimum_required(VERSION 3.20)
project(collision-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #executable output directory

# Optionally build SFML as a static library
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# FetchContent to pull SFML directly from GitHub
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x  
)
FetchContent_MakeAvailable(SFML)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add all .cpp files in src/ directory to the build
file(GLOB_RECURSE SOURCE_FILES "src/main.cpp")
file(GLOB_RECURSE TEST_FILES "src/tests/*.cpp")

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_executable(tests ${TEST_FILES})

# Include directories and link SFML libraries
target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
