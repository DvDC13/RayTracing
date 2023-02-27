# Simple RayTracer in C++

This is a simple RayTracer program implemented in C++. It uses the basic principles of RayTracing to create a simple image with spheres, triangles and  lighting, and shadows.

# Usage

To use the program, simply run the executable file and provide a file path for the output image. The command should be:

    ./SimpleRayTracer fileToSave.ppm

For example, if your executable file is named raytracer and you want to save the output image as output.ppm in the same directory, you can run:
    
    ./SimpleRayTracer output.ppm

# Dependencies

This program has the following dependencies:

    C++20

To build the program, simply run:

    mkdir build
    cd build
    cmake ..
    make

This will compile the main.cpp file and generate an executable named SimpleRayTracer

# Implementation Details
