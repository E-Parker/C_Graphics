@echo off
echo Rebuilding project for Windows.
echo Removing previous build ...
rmdir /s /q %~dp0\build
echo Generating new build ...
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Debug
cd ..
echo Done!
pause 
