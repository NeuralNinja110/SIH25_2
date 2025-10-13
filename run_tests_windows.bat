@echo off
REM ============================================================================
REM MAOS - Windows Test Suite Runner
REM ============================================================================
REM Runs comprehensive tests across all modes and scenarios
REM ============================================================================

setlocal enabledelayedexpansion

echo ========================================
echo   MAOS Comprehensive Test Suite
echo   Windows Edition
echo ========================================
echo.

REM Configuration
set OBFUSCATOR=build\Release\llvm-obfuscator.exe
if not exist "%OBFUSCATOR%" (
    set OBFUSCATOR=build\llvm-obfuscator.exe
)

if not exist "%OBFUSCATOR%" (
    echo ERROR: Obfuscator executable not found!
    echo Expected: build\Release\llvm-obfuscator.exe
    echo Please run build_windows.bat first.
    pause
    exit /b 1
)

set TEST_DIR=tests
set RESULTS_DIR=test_results_windows
set TIMESTAMP=%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%
set TIMESTAMP=%TIMESTAMP: =0%

echo Test Configuration:
echo   Obfuscator: %OBFUSCATOR%
echo   Test Directory: %TEST_DIR%
echo   Results Directory: %RESULTS_DIR%
echo   Timestamp: %TIMESTAMP%
echo.

REM Create results directory
if not exist "%RESULTS_DIR%" mkdir "%RESULTS_DIR%"
if not exist "%RESULTS_DIR%\logs" mkdir "%RESULTS_DIR%\logs"
if not exist "%RESULTS_DIR%\binaries" mkdir "%RESULTS_DIR%\binaries"
if not exist "%RESULTS_DIR%\reports" mkdir "%RESULTS_DIR%\reports"

REM Test cases
set TEST_CASES=test_easy.c test_medium.c test_difficult.c test_variadic_functions.c test_function_pointers.c test_data_structures.c test_floating_point.c test_bit_manipulation.c

REM Test modes
set "MODE1=normal"
set "MODE2=size_conservative"
set "MODE3=maximum_security"

REM Counters
set TOTAL_TESTS=0
set PASSED_TESTS=0
set FAILED_TESTS=0

echo ========================================
echo   MODE 1: Normal Obfuscation
echo ========================================
echo.

set MODE_LABEL=normal
set MODE_CONFIG=-l 2 -C 2

for %%T in (%TEST_CASES%) do (
    if exist "%TEST_DIR%\%%T" (
        set /a TOTAL_TESTS+=1
        echo Testing: %%T [Normal Mode]
        
        set OUTPUT=%RESULTS_DIR%\binaries\%%~nT_normal.exe
        set LOG=%RESULTS_DIR%\logs\%%~nT_normal.log
        
        echo   Command: %OBFUSCATOR% -i %TEST_DIR%\%%T -o !OUTPUT! %MODE_CONFIG%
        
        REM Run obfuscation
        %OBFUSCATOR% -i "%TEST_DIR%\%%T" -o "!OUTPUT!" %MODE_CONFIG% > "!LOG!" 2>&1
        
        if !ERRORLEVEL! equ 0 (
            if exist "!OUTPUT!" (
                echo   [PASS] Binary created successfully
                set /a PASSED_TESTS+=1
                
                REM Get file size
                for %%F in ("!OUTPUT!") do set SIZE=%%~zF
                echo   Size: !SIZE! bytes
            ) else (
                echo   [FAIL] Binary not created
                set /a FAILED_TESTS+=1
            )
        ) else (
            echo   [FAIL] Obfuscation failed
            set /a FAILED_TESTS+=1
        )
        echo.
    )
)

echo ========================================
echo   MODE 2: Size-Conservative
echo ========================================
echo.

set MODE_LABEL=size_conservative
set MODE_CONFIG=-c config\size_conservative.yaml

for %%T in (%TEST_CASES%) do (
    if exist "%TEST_DIR%\%%T" (
        set /a TOTAL_TESTS+=1
        echo Testing: %%T [Size-Conservative Mode]
        
        set OUTPUT=%RESULTS_DIR%\binaries\%%~nT_size_conservative.exe
        set LOG=%RESULTS_DIR%\logs\%%~nT_size_conservative.log
        
        echo   Command: %OBFUSCATOR% -i %TEST_DIR%\%%T -o !OUTPUT! %MODE_CONFIG%
        
        REM Run obfuscation
        %OBFUSCATOR% -i "%TEST_DIR%\%%T" -o "!OUTPUT!" %MODE_CONFIG% > "!LOG!" 2>&1
        
        if !ERRORLEVEL! equ 0 (
            if exist "!OUTPUT!" (
                echo   [PASS] Binary created successfully
                set /a PASSED_TESTS+=1
                
                REM Get file size
                for %%F in ("!OUTPUT!") do set SIZE=%%~zF
                echo   Size: !SIZE! bytes
            ) else (
                echo   [FAIL] Binary not created
                set /a FAILED_TESTS+=1
            )
        ) else (
            echo   [FAIL] Obfuscation failed
            set /a FAILED_TESTS+=1
        )
        echo.
    )
)

echo ========================================
echo   MODE 3: Maximum-Security
echo ========================================
echo.

set MODE_LABEL=maximum_security
set MODE_CONFIG=-c config\maximum_security.yaml

for %%T in (%TEST_CASES%) do (
    if exist "%TEST_DIR%\%%T" (
        set /a TOTAL_TESTS+=1
        echo Testing: %%T [Maximum-Security Mode]
        
        set OUTPUT=%RESULTS_DIR%\binaries\%%~nT_maximum_security.exe
        set LOG=%RESULTS_DIR%\logs\%%~nT_maximum_security.log
        
        echo   Command: %OBFUSCATOR% -i %TEST_DIR%\%%T -o !OUTPUT! %MODE_CONFIG%
        
        REM Run obfuscation
        %OBFUSCATOR% -i "%TEST_DIR%\%%T" -o "!OUTPUT!" %MODE_CONFIG% > "!LOG!" 2>&1
        
        if !ERRORLEVEL! equ 0 (
            if exist "!OUTPUT!" (
                echo   [PASS] Binary created successfully
                set /a PASSED_TESTS+=1
                
                REM Get file size
                for %%F in ("!OUTPUT!") do set SIZE=%%~zF
                echo   Size: !SIZE! bytes
            ) else (
                echo   [FAIL] Binary not created
                set /a FAILED_TESTS+=1
            )
        ) else (
            echo   [FAIL] Obfuscation failed
            set /a FAILED_TESTS+=1
        )
        echo.
    )
)

REM Generate summary report
echo ========================================
echo   TEST SUMMARY
echo ========================================
echo.
echo Total Tests: %TOTAL_TESTS%
echo Passed: %PASSED_TESTS%
echo Failed: %FAILED_TESTS%
echo.

if %FAILED_TESTS% equ 0 (
    echo [SUCCESS] All tests passed!
) else (
    echo [WARNING] Some tests failed. Check logs in %RESULTS_DIR%\logs
)

REM Create summary report file
set REPORT=%RESULTS_DIR%\reports\test_summary_%TIMESTAMP%.txt
echo MAOS Test Suite Summary > "%REPORT%"
echo ======================= >> "%REPORT%"
echo. >> "%REPORT%"
echo Date: %date% %time% >> "%REPORT%"
echo Total Tests: %TOTAL_TESTS% >> "%REPORT%"
echo Passed: %PASSED_TESTS% >> "%REPORT%"
echo Failed: %FAILED_TESTS% >> "%REPORT%"
echo. >> "%REPORT%"
echo Test Results: >> "%REPORT%"
echo ------------- >> "%REPORT%"

REM List all results
for %%F in ("%RESULTS_DIR%\binaries\*.exe") do (
    echo %%~nxF - %%~zF bytes >> "%REPORT%"
)

echo.
echo Summary report saved to: %REPORT%
echo.
echo Results saved to: %RESULTS_DIR%
echo   - Binaries: %RESULTS_DIR%\binaries
echo   - Logs: %RESULTS_DIR%\logs
echo   - Reports: %RESULTS_DIR%\reports
echo.

pause
