@echo off
REM ============================================================================
REM MAOS - Windows Launcher Menu
REM ============================================================================
REM Interactive menu for all MAOS operations
REM ============================================================================

:MENU
cls
echo.
echo ========================================
echo   MAOS - Windows Launcher
echo ========================================
echo.
echo   1. Build MAOS
echo   2. Quick Test
echo   3. Run Full Test Suite
echo   4. Run Scientific Tests
echo   5. Obfuscate Single File
echo   6. Batch Obfuscate Directory
echo   7. View Documentation
echo   8. Clean Build Files
echo   9. Exit
echo.
echo ========================================
echo.

set /p choice="Enter your choice (1-9): "

if "%choice%"=="1" goto BUILD
if "%choice%"=="2" goto QUICK_TEST
if "%choice%"=="3" goto FULL_TEST
if "%choice%"=="4" goto SCIENTIFIC_TEST
if "%choice%"=="5" goto SINGLE_FILE
if "%choice%"=="6" goto BATCH_OBFUSCATE
if "%choice%"=="7" goto DOCUMENTATION
if "%choice%"=="8" goto CLEAN
if "%choice%"=="9" goto EXIT

echo Invalid choice. Please try again.
pause
goto MENU

:BUILD
cls
echo.
echo ========================================
echo   Building MAOS...
echo ========================================
echo.
call build_windows.bat
pause
goto MENU

:QUICK_TEST
cls
echo.
echo ========================================
echo   Running Quick Test...
echo ========================================
echo.
call quick_test_windows.bat
pause
goto MENU

:FULL_TEST
cls
echo.
echo ========================================
echo   Running Full Test Suite...
echo ========================================
echo.
call run_tests_windows.bat
pause
goto MENU

:SCIENTIFIC_TEST
cls
echo.
echo ========================================
echo   Running Scientific Tests...
echo ========================================
echo.
call run_scientific_tests_windows.bat
pause
goto MENU

:SINGLE_FILE
cls
echo.
echo ========================================
echo   Obfuscate Single File
echo ========================================
echo.

set OBFUSCATOR=build\Release\llvm-obfuscator.exe
if not exist "%OBFUSCATOR%" (
    set OBFUSCATOR=build\llvm-obfuscator.exe
)

if not exist "%OBFUSCATOR%" (
    echo ERROR: Obfuscator not built yet!
    echo Please run option 1 (Build MAOS) first.
    pause
    goto MENU
)

set /p INPUT_FILE="Enter input file path: "
if not exist "%INPUT_FILE%" (
    echo ERROR: File not found: %INPUT_FILE%
    pause
    goto MENU
)

set /p OUTPUT_FILE="Enter output file path: "

echo.
echo Select mode:
echo   1. Normal (fast, balanced)
echo   2. Size-Conservative (minimal size increase)
echo   3. Maximum-Security (maximum protection)
echo.
set /p MODE_CHOICE="Enter mode (1-3): "

if "%MODE_CHOICE%"=="1" (
    set MODE_CONFIG=-l 2 -C 2
    set MODE_NAME=Normal
)
if "%MODE_CHOICE%"=="2" (
    set MODE_CONFIG=-c config\size_conservative.yaml
    set MODE_NAME=Size-Conservative
)
if "%MODE_CHOICE%"=="3" (
    set MODE_CONFIG=-c config\maximum_security.yaml
    set MODE_NAME=Maximum-Security
)

echo.
echo Obfuscating with %MODE_NAME% mode...
echo Command: %OBFUSCATOR% -i "%INPUT_FILE%" -o "%OUTPUT_FILE%" %MODE_CONFIG%
echo.

%OBFUSCATOR% -i "%INPUT_FILE%" -o "%OUTPUT_FILE%" %MODE_CONFIG%

if %ERRORLEVEL% equ 0 (
    echo.
    echo SUCCESS! Obfuscated file created: %OUTPUT_FILE%
    if exist "%OUTPUT_FILE%" (
        for %%F in ("%OUTPUT_FILE%") do echo Size: %%~zF bytes
    )
) else (
    echo.
    echo ERROR: Obfuscation failed!
)

pause
goto MENU

:BATCH_OBFUSCATE
cls
echo.
echo ========================================
echo   Batch Obfuscate Directory
echo ========================================
echo.

set OBFUSCATOR=build\Release\llvm-obfuscator.exe
if not exist "%OBFUSCATOR%" (
    set OBFUSCATOR=build\llvm-obfuscator.exe
)

if not exist "%OBFUSCATOR%" (
    echo ERROR: Obfuscator not built yet!
    pause
    goto MENU
)

set /p INPUT_DIR="Enter input directory: "
if not exist "%INPUT_DIR%" (
    echo ERROR: Directory not found: %INPUT_DIR%
    pause
    goto MENU
)

set /p OUTPUT_DIR="Enter output directory: "
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

echo.
echo Select mode:
echo   1. Normal
echo   2. Size-Conservative
echo   3. Maximum-Security
echo.
set /p MODE_CHOICE="Enter mode (1-3): "

if "%MODE_CHOICE%"=="1" set MODE_CONFIG=-l 2 -C 2
if "%MODE_CHOICE%"=="2" set MODE_CONFIG=-c config\size_conservative.yaml
if "%MODE_CHOICE%"=="3" set MODE_CONFIG=-c config\maximum_security.yaml

echo.
echo Processing files...
echo.

set COUNT=0
for %%f in ("%INPUT_DIR%\*.c") do (
    echo Obfuscating: %%~nxf
    %OBFUSCATOR% -i "%%f" -o "%OUTPUT_DIR%\%%~nf.exe" %MODE_CONFIG%
    set /a COUNT+=1
)

for %%f in ("%INPUT_DIR%\*.cpp") do (
    echo Obfuscating: %%~nxf
    %OBFUSCATOR% -i "%%f" -o "%OUTPUT_DIR%\%%~nf.exe" %MODE_CONFIG%
    set /a COUNT+=1
)

echo.
echo Processed %COUNT% files.
pause
goto MENU

:DOCUMENTATION
cls
echo.
echo ========================================
echo   Documentation
echo ========================================
echo.
echo Available documentation files:
echo.
echo   1. WINDOWS_GUIDE.md - Complete Windows guide
echo   2. USAGE_GUIDE.md - Usage instructions
echo   3. START_HERE.md - Quick start guide
echo   4. MAOS_COMPLETE_IMPLEMENTATION_REPORT.md - Technical details
echo   5. Return to menu
echo.
set /p DOC_CHOICE="Enter choice (1-5): "

if "%DOC_CHOICE%"=="1" start notepad WINDOWS_GUIDE.md
if "%DOC_CHOICE%"=="2" start notepad USAGE_GUIDE.md
if "%DOC_CHOICE%"=="3" start notepad START_HERE.md
if "%DOC_CHOICE%"=="4" start notepad MAOS_COMPLETE_IMPLEMENTATION_REPORT.md
if "%DOC_CHOICE%"=="5" goto MENU

pause
goto MENU

:CLEAN
cls
echo.
echo ========================================
echo   Clean Build Files
echo ========================================
echo.
echo This will delete:
echo   - build directory
echo   - test_results_windows directory
echo   - scientific_results_windows directory
echo.
set /p CONFIRM="Are you sure? (Y/N): "

if /i "%CONFIRM%"=="Y" (
    echo.
    echo Cleaning...
    if exist "build" (
        rmdir /s /q build
        echo Deleted: build\
    )
    if exist "test_results_windows" (
        rmdir /s /q test_results_windows
        echo Deleted: test_results_windows\
    )
    if exist "scientific_results_windows" (
        rmdir /s /q scientific_results_windows
        echo Deleted: scientific_results_windows\
    )
    echo.
    echo Clean complete!
) else (
    echo.
    echo Cancelled.
)

pause
goto MENU

:EXIT
cls
echo.
echo Thank you for using MAOS!
echo.
exit /b 0
