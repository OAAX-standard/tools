@echo off
REM Exit on error
setlocal enabledelayedexpansion
cd /d %~dp0

set "BUILD_DIR=%cd%\build"

REM Remove previous build directory if it exists
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"

REM Create a fresh build directory
mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

REM Run CMake to configure the project
cmake ..

REM Build the project
cmake --build . --config Debug

REM Run the unit tests binary
.\Debug\oaax_utils_tests.exe
