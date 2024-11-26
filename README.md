# collision-engine
<img src="https://github.com/user-attachments/assets/1df50e6b-fc1e-4710-bf0e-44ecd0a3137d" alt="image description" width="700" height="700">

## Description
A multithreaded SIMD discrete collision engine, specific to arm64 machines.

This project is inspired by [this Pezzza's Work video.](https://www.youtube.com/watch?v=9IULfQH7E90&t=380s)

The AoS implementation relies on a linear allocator and multithreading to support its collision detection, along with vectors that support SIMD operations.

The SoA implementation relies on purely SIMD operations.

## Build
Create build directory
```bash
mkdir build
cd build
```
Build the project
```bash
cmake ..
cd ..
cmake --build ./build
```
## Run Demo
To run AoS (array of structs) implementation:
```bash
~/collision-engine$ ./build/bin/renderer_test
```
To run SoA (struct of arrays) implementation:
```bash
~/collision-engine$ ./build/bin/simd_renderer_test
```


