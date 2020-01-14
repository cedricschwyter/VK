[![Build Status](https://travis-ci.com/D3PSI/VK.svg?branch=master)](https://travis-ci.com/D3PSI/VK)
# VK
## Vulkan part of my Vulkan vs. OpenGL comparison

This project features the first part of my matura thesis which is about comparing the Vulkan API and OpenGL with one another. This repository contains the source code for the Vulkan application. The second part can be found here: [https://github.com/D3PSI/OGL](https://github.com/D3PSI/OGL)

The final document I turned in is also on GitHub: [D3PSI/matura-thesis-latex](https://github.com/D3PSI/matura-thesis-latex)

The project itself has been written in C++17 and the Vulkan API spec-version 1.1.114.0.

## Double Pendulum Simulation
The video for the comparison of the two double pendulums can be found here: 
https://www.dropbox.com/sh/ctnefnmycs6zo5f/AACb-YRdl-YjTGpkaIwxF0Gsa?dl=0

## Requirements

The only real requirement your system has to have is a GPU (preferrably a dedicated one) that supports Vulkan (obviously).

## Installing

### Windows

To build this application under Windows, simply open the `VK.sln` file in the projects root directory in Visual Studio 2019. 
Under Build-Configuration, select `Vulkan 1.1.114.0` and `64-bit`. Then, you should be able to build the project:
Run `Build` > `Rebuild All` and the build should run through smoothly.
If you want to make sure that the application is configured properly for Windows, open the `VK/Version.hpp` file in a text editor and make sure that the line containing `#define WIN_64` is uncommented and that the lines containing `#define WIN_32`, `#define MACOSX` and `#define LINUX` are commented out. Then, rebuild the solution.

### Linux

To build this application under Linux, simply execute the following commands:

    git clone https://github.com/D3PSI/VK.git
    cd VK/install/
    sudo ./INSTALL.sh
    cd ..
    make

This will give you the executable in `bin/Linux/x64/VK by D3PSI`. 
To run it execute the following (in the root directory of the project):

    ./RUN.sh

or

    make run

Either one should work. If it gives you errors about includes from Windows or whatever, make sure to open `VK/Version.hpp` in a text editor and check that the line containing `#define LINUX` is uncommented and that the lines containing `#define WIN_64 `, `#define WIN_32` and `#define MACOSX` are commented out. Then try to recompile the project by `cd`-ing into the projects root directory and running `make run`.

### Hint
If you are a little lazy like me, there are precompiled binaries for Windows x64 and Linux x64 hidden somewhere in this repository. I am sure that you will manage to find them.
To actually execute these binaries, copy them and (in the Windows binary's case) all of the dynamic libraries (`.dll`'s) over into the `VK` folder, which is the executables runtime directory.

##### Another hint

They are located in `bin/`. You are welcome.

## Uninstalling

Well, if you figured out how to install the project, you will manage to uninstall it on your own. It does not create any temporary files outside of the project folder, so, delete just that:

    rm -r VK/

Thank you for your interest in this project!
