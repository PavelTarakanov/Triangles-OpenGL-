# 3D Triangles Intersection & OpenGL Renderer

A cross-platform C++17 project for detecting intersections between 3D triangles (both coplanar and non-coplanar) utilizing Axis-Aligned Bounding Box (AABB) broad-phase optimization, modular Google Test unit suites, and 3D scene visualization via OpenGL/GLUT.

## Prerequisites

* A **C++23** compliant compiler (Clang / GCC)
* **CMake** (version 3.14 or higher)
* **OpenGL** and **GLUT** development libraries
* **Google Test** framework (for building and running unit tests)

## Executables

All compiled binaries are placed in the `build/` directory:

* **`build/out`** — The main visualization application. Accepts a path to an input scene file as an optional command-line argument (otherwise reads triangle data from `stdin`).
* **`build/triangles_tests`** — The Google Test unit test suite binary (requires no arguments).

## Controls (OpenGL Viewer)

* **`W` / `S` or `Up` / `Down` arrows** — Adjust camera pitch
* **`A` / `D` or `Left` / `Right` arrows** — Adjust camera yaw
* **`+` / `-`** — Zoom camera in / out
* **`Enter`** — Exit the application