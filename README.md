# VecMath
VecMath REPL shell
## Features
The VecMath REPL shell is a tool developed specifically for students Game Development at Digital Arts and Entertainment. 
The goal is to make it easier to execute the typical vector operations such as cross and dot products, linear combinations 
and trying out 3D rotations with quaternions.

## Documentation

Documentation on how to use the VecMath REPL shell can be found at: [VecMath Documentation](https://samynk.github.io/VecMath/)

## Developing

The C++ part of this project (VecMathREPLCpp) is built with CMake.

### Prerequisites
You will need an installation of CMake and a working C++ compiler. On Microsoft Windows, these are included with Microsoft Visual Studio. On Linux, you can get them with your package manager.
Debian and Ubuntu-based:
```
sudo apt install build-essential cmake
```
Arch-based:
```
sudo pacman -S base-devel cmake
```

### Setting up
To get started, you will need to clone the project to your machine:
```
git clone https://github.com/samynk/VecMath.git
cd VecMath
```

### Building
To build the project, you will need to use CMake to generate project files, and then build it:
```
cd VecMathREPLCpp
mkdir build
cmake -B build
cmake --build build
```

The executable will be located in `VecMathREPLCpp/build/vecmathmain/VecMathMain`, and the required resources are located in `VecMathREPLCpp/build/vecmathmain/resources`.
