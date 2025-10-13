@echo off
REM ============================================================================
REM MAOS - Windows Build Script
REM ============================================================================
REM This script builds the LLVM Obfuscator with MAOS support on Windows
REM Requirements: CMake 3.20+, LLVM 18.1.3, Visual Studio 2019+
REM ============================================================================

echo ========================================
echo   MAOS Windows Build Script
echo ========================================
echo.

REM Check if running from project root
if not exist "CMakeLists.txt" (
    echo ERROR: CMakeLists.txt not found!
    echo Please run this script from the project root directory.
    pause
    exit /b 1
)

REM Check for CMake
where cmake >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake not found in PATH!
    echo Please install CMake 3.20 or higher.
    echo Download from: https://cmake.org/download/
    pause
    exit /b 1
)

REM Check for Visual Studio
where cl >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo ERROR: Visual Studio C++ compiler not found!
    echo Please run this script from a Visual Studio Developer Command Prompt.
    echo Or run: "C:\Program Files\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    pause
    exit /b 1
)

REM Create build directory
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

echo.
echo ========================================
echo   Configuring CMake...
echo ========================================
echo.

REM Configure with CMake (using Ninja if available, otherwise MSBuild)
where ninja >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo Using Ninja generator...
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17 ..
) else (
    echo Using Visual Studio generator...
    cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_CXX_STANDARD=17 ..
)

if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Building...
echo ========================================
echo.

REM Build the project
cmake --build . --config Release --parallel

if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Build Complete!
echo ========================================
echo.

REM Check if executable was created
if exist "Release\llvm-obfuscator.exe" (
    echo Executable: build\Release\llvm-obfuscator.exe
    echo Size: 
    dir /s "Release\llvm-obfuscator.exe" | findstr "llvm-obfuscator.exe"
) else if exist "llvm-obfuscator.exe" (
    echo Executable: build\llvm-obfuscator.exe
    echo Size:
    dir /s "llvm-obfuscator.exe" | findstr "llvm-obfuscator.exe"
) else (
    echo WARNING: Executable not found at expected location!
    echo Searching for llvm-obfuscator.exe...
    dir /s /b llvm-obfuscator.exe
)

cd ..

echo.
echo ========================================
echo   Testing Installation...
echo ========================================
echo.

REM Test the executable
if exist "build\Release\llvm-obfuscator.exe" (
    build\Release\llvm-obfuscator.exe --version
    if %ERRORLEVEL% equ 0 (
        echo.
        echo SUCCESS! MAOS is ready to use.
        echo.
        echo Quick Start:
        echo   build\Release\llvm-obfuscator.exe --help
        echo   build\Release\llvm-obfuscator.exe -i tests\test_easy.c -o output
        echo.
    )
) else if exist "build\llvm-obfuscator.exe" (
    build\llvm-obfuscator.exe --version
    if %ERRORLEVEL% equ 0 (
        echo.
        echo SUCCESS! MAOS is ready to use.
        echo.
        echo Quick Start:
        echo   build\llvm-obfuscator.exe --help
        echo   build\llvm-obfuscator.exe -i tests\test_easy.c -o output
        echo.
    )
)

echo Build log saved to: build\build_log.txt
echo.

pause
