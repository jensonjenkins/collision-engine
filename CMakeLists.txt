cmake_minimum_required(VERSION 3.20)
project(collision-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # executable output directory

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(SFML_VERSION 2.6.1)
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG ${SFML_VERSION}
)
FetchContent_MakeAvailable(SFML)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE "src")
target_link_libraries(main PRIVATE sfml-graphics)

add_executable(renderer_test tests/renderer_test.cpp)
target_include_directories(renderer_test PRIVATE "src")
set_target_properties(renderer_test PROPERTIES COMPILE_FLAGS "-g")
target_link_libraries(renderer_test PRIVATE sfml-graphics)

add_executable(solver_test tests/solver_test.cpp)
target_include_directories(solver_test PRIVATE "src")
set_target_properties(solver_test PROPERTIES COMPILE_FLAGS "-g")
target_link_libraries(solver_test PRIVATE sfml-graphics)

add_executable(allocator_test tests/allocator_test.cpp)
target_include_directories(allocator_test PRIVATE "src")

add_executable(grid_test tests/grid_test.cpp)
target_include_directories(grid_test PRIVATE "src")
