README
======

Daniel 'MaTachi' Jonsson

The code is based on the n00b tutorials found on
[opengl-tutorials.org](http://www.opengl-tutorial.org/). [Tutorial: OpenGL 3.1
The First Triangle
(C++/Win)](http://www.opengl.org/wiki/Tutorial:_OpenGL_3.1_The_First_Triangle_%28C%2B%2B/Win%29)
was also useful.

The development was done on Ubuntu 12.10. The [CMake config
file](CMakeLists.txt) hasn't been configured to work on Windows, or even tested
on Windows, so with 99.99 % probability it won't compile there.  
The shaders are written in GLSL 1.30, so it's runnable on Intel HD3000. It's
also confirmed to work with newer/better GPUs, more specifically Nvidia 660.

## Compile

Install dependencies for compiling: `sudo apt-get install cmake build-essential
mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev`  
(Not completely sure if mesa-common-dev actually is needed)

First time configuration: `mkdir build ; cd build ; cmake .. ; cd ..`

How to compile and run the game: `cd build/ ; make ; cd .. ; ./build/Main`
