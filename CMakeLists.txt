cmake_minimum_required(VERSION 3.20)
project(collision-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # executable output directory

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -O3                # Enable aggressive optimization
        -march=native      # Use instructions specific to the target CPU
        -ffast-math        # Optimize floating-point math (relaxes IEEE compliance)
    )
endif()

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

add_executable(renderer_test tests/renderer_test.cpp)
target_include_directories(renderer_test PRIVATE "src")
set_target_properties(renderer_test PROPERTIES COMPILE_FLAGS "-g")
target_compile_options(renderer_test PRIVATE -O3 -march=native -ffast-math)
target_link_libraries(renderer_test PRIVATE sfml-graphics)

add_executable(solver_test tests/solver_test.cpp)
target_include_directories(solver_test PRIVATE "src")
set_target_properties(solver_test PROPERTIES COMPILE_FLAGS "-g")
target_link_libraries(solver_test PRIVATE sfml-graphics)

add_executable(allocator_test tests/allocator_test.cpp)
target_include_directories(allocator_test PRIVATE "src")

add_executable(grid_test tests/grid_test.cpp)
target_include_directories(grid_test PRIVATE "src")

add_executable(simd_collection_test tests/simd_collection_test.cpp)
set_target_properties(simd_collection_test PROPERTIES COMPILE_FLAGS "-g")
target_include_directories(simd_collection_test PRIVATE "src")

add_executable(simd_solver_test tests/simd_solver_test.cpp)
set_target_properties(simd_solver_test PROPERTIES COMPILE_FLAGS "-g")
target_include_directories(simd_solver_test PRIVATE "src")

add_executable(simd_renderer_test tests/simd_renderer_test.cpp)
set_target_properties(simd_renderer_test PROPERTIES COMPILE_FLAGS "-g")
target_compile_options(simd_renderer_test PRIVATE -O3 -march=native -ffast-math)
target_include_directories(simd_renderer_test PRIVATE "src")
target_link_libraries(simd_renderer_test PRIVATE sfml-graphics)
