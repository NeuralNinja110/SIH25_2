# ============================================================================
# MAOS - Windows Build Script (PowerShell)
# ============================================================================
# Modern PowerShell build script with better error handling
# ============================================================================

[CmdletBinding()]
param(
    [Parameter(Mandatory=$false)]
    [ValidateSet("Release", "Debug")]
    [string]$BuildType = "Release",
    
    [Parameter(Mandatory=$false)]
    [ValidateSet("Ninja", "VS2019", "VS2022")]
    [string]$Generator = "Auto",
    
    [Parameter(Mandatory=$false)]
    [int]$Jobs = 0
)

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  MAOS Windows Build Script (PowerShell)" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if running from project root
if (-not (Test-Path "CMakeLists.txt")) {
    Write-Host "ERROR: CMakeLists.txt not found!" -ForegroundColor Red
    Write-Host "Please run this script from the project root directory." -ForegroundColor Yellow
    exit 1
}

# Check for CMake
try {
    $cmakeVersion = cmake --version
    Write-Host "✓ CMake found: $($cmakeVersion[0])" -ForegroundColor Green
} catch {
    Write-Host "ERROR: CMake not found in PATH!" -ForegroundColor Red
    Write-Host "Please install CMake 3.20 or higher." -ForegroundColor Yellow
    Write-Host "Download from: https://cmake.org/download/" -ForegroundColor Yellow
    exit 1
}

# Check for Visual Studio
try {
    $vsVersion = (Get-Command cl.exe -ErrorAction SilentlyContinue).Version
    if ($vsVersion) {
        Write-Host "✓ Visual Studio C++ compiler found: $vsVersion" -ForegroundColor Green
    } else {
        throw "Not found"
    }
} catch {
    Write-Host "ERROR: Visual Studio C++ compiler not found!" -ForegroundColor Red
    Write-Host "Please run this script from a Visual Studio Developer PowerShell." -ForegroundColor Yellow
    Write-Host "Or initialize environment:" -ForegroundColor Yellow
    Write-Host '  & "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"' -ForegroundColor Cyan
    exit 1
}

# Determine optimal job count
if ($Jobs -eq 0) {
    $Jobs = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors
    Write-Host "Using $Jobs parallel jobs" -ForegroundColor Green
}

# Create build directory
if (-not (Test-Path "build")) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Set-Location "build"

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Configuring CMake..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Determine generator
$cmakeGenerator = ""
if ($Generator -eq "Auto") {
    # Check for Ninja
    try {
        ninja --version | Out-Null
        $cmakeGenerator = "Ninja"
        Write-Host "✓ Using Ninja generator (fast builds)" -ForegroundColor Green
    } catch {
        $cmakeGenerator = "Visual Studio 17 2022"
        Write-Host "Using Visual Studio 2022 generator" -ForegroundColor Yellow
    }
} elseif ($Generator -eq "Ninja") {
    $cmakeGenerator = "Ninja"
} elseif ($Generator -eq "VS2022") {
    $cmakeGenerator = "Visual Studio 17 2022"
} elseif ($Generator -eq "VS2019") {
    $cmakeGenerator = "Visual Studio 16 2019"
}

# Configure
Write-Host "Generator: $cmakeGenerator" -ForegroundColor Cyan
Write-Host "Build Type: $BuildType" -ForegroundColor Cyan

try {
    if ($cmakeGenerator -eq "Ninja") {
        cmake -G "Ninja" `
              -DCMAKE_BUILD_TYPE=$BuildType `
              -DCMAKE_CXX_STANDARD=17 `
              ..
    } else {
        cmake -G $cmakeGenerator `
              -A x64 `
              -DCMAKE_CXX_STANDARD=17 `
              ..
    }
} catch {
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Building..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Build
try {
    cmake --build . --config $BuildType --parallel $Jobs
} catch {
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Build Complete!" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Find executable
$exePaths = @(
    "Release\llvm-obfuscator.exe",
    "Debug\llvm-obfuscator.exe",
    "llvm-obfuscator.exe"
)

$foundExe = $null
foreach ($path in $exePaths) {
    if (Test-Path $path) {
        $foundExe = $path
        break
    }
}

if ($foundExe) {
    $exeInfo = Get-Item $foundExe
    Write-Host "✓ Executable: build\$foundExe" -ForegroundColor Green
    Write-Host "  Size: $([math]::Round($exeInfo.Length / 1MB, 2)) MB" -ForegroundColor Green
} else {
    Write-Host "WARNING: Executable not found at expected location!" -ForegroundColor Yellow
    $allExes = Get-ChildItem -Recurse -Filter "llvm-obfuscator.exe"
    if ($allExes) {
        Write-Host "Found at:" -ForegroundColor Yellow
        $allExes | ForEach-Object { Write-Host "  $_" -ForegroundColor Yellow }
    }
}

Set-Location ..

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Testing Installation..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Test executable
if ($foundExe) {
    $fullPath = Join-Path "build" $foundExe
    
    try {
        $versionOutput = & $fullPath --version 2>&1
        Write-Host $versionOutput
        Write-Host ""
        Write-Host "SUCCESS! MAOS is ready to use." -ForegroundColor Green
        Write-Host ""
        Write-Host "Quick Start:" -ForegroundColor Cyan
        Write-Host "  $fullPath --help" -ForegroundColor White
        Write-Host "  $fullPath -i tests\test_easy.c -o output.exe" -ForegroundColor White
        Write-Host ""
    } catch {
        Write-Host "WARNING: Could not execute binary" -ForegroundColor Yellow
    }
}

Write-Host "Build complete!" -ForegroundColor Green
Write-Host "Build log saved to: build\CMakeFiles\" -ForegroundColor Gray
Write-Host ""

# Summary
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Build Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Generator: $cmakeGenerator" -ForegroundColor White
Write-Host "Build Type: $BuildType" -ForegroundColor White
Write-Host "Parallel Jobs: $Jobs" -ForegroundColor White
Write-Host "Status: SUCCESS" -ForegroundColor Green
Write-Host ""
