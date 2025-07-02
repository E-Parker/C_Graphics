#!/bin/bash

echo "Rebuilding project for Linux!"

rm -rf build
mkdir build

cp -r assets build

cd build

echo "Executing cmake..."
cmake ..
echo "Building Project..."
make
echo "Executing program..."
./OpenGL_Build
