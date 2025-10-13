@echo off
REM Comprehensive Reverse Engineering Benchmark Runner for Windows
REM Automates the complete benchmarking process

setlocal enabledelayedexpansion

echo ========================================================================
echo LLVM OBFUSCATOR REVERSE ENGINEERING BENCHMARK
echo ========================================================================
echo.

set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%"
set "RE_DIR=%PROJECT_ROOT%reverse_engineering"

echo Project Root: %PROJECT_ROOT%
echo RE Directory: %RE_DIR%
echo.

REM Check if obfuscator is built
if not exist "%PROJECT_ROOT%build\Release\llvm-obfuscator.exe" (
    if not exist "%PROJECT_ROOT%build\llvm-obfuscator.exe" (
        echo X ERROR: Obfuscator not built!
        echo.
        echo Please build the project first:
        echo   build_windows.bat
        echo.
        pause
        exit /b 1
    )
)

echo [OK] Obfuscator found
echo.

REM Check Python
python --version >nul 2>&1
if errorlevel 1 (
    echo X ERROR: Python not found!
    echo Please install Python 3.7 or later
    pause
    exit /b 1
)

echo [OK] Python found
python --version
echo.

echo ========================================================================
echo STARTING BENCHMARK
echo ========================================================================
echo.
echo This will:
echo   1. Compile test cases in all modes (baseline, normal, size-conservative, maximum-security)
echo   2. Perform reverse engineering analysis on each binary
echo   3. Generate comprehensive reports with statistical analysis
echo.
echo Estimated time: 10-20 minutes depending on system performance
echo.
pause
echo.

REM Run the benchmark
cd /d "%PROJECT_ROOT%"
python "%RE_DIR%\scripts\run_benchmark.py" --project-root "%PROJECT_ROOT%" --output "%RE_DIR%\results"

if errorlevel 1 (
    echo.
    echo X Benchmark failed! Check error messages above.
    pause
    exit /b 1
)

echo.
echo ========================================================================
echo BENCHMARK COMPLETE!
echo ========================================================================
echo.
echo Results location: %RE_DIR%\results\
echo.
echo Generated files:
echo   [Reports] %RE_DIR%\results\reports\comprehensive_report.md
echo   [CSV]     %RE_DIR%\results\reports\comparison_table.csv
echo   [Stats]   %RE_DIR%\results\reports\statistical_analysis.json
echo   [Bins]    %RE_DIR%\results\binaries\
echo   [Metrics] %RE_DIR%\results\metrics\
echo.
echo To view the main report, open:
echo   %RE_DIR%\results\reports\comprehensive_report.md
echo.
pause
