#!/usr/bin/env sh

mkdir build-proj.cmake-Desktop-Default
cd build-proj.cmake-Desktop-Default

#generate cmake project in the "build" folder
cmake ..

#build it
make -j 4

#move to working data folder with resources
cd ../../data

#run executable
./../proj.cmake/build/GamePart5
