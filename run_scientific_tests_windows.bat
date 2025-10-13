@echo off
REM ============================================================================
REM MAOS - Windows Scientific Testing Suite
REM ============================================================================
REM Performs detailed scientific testing with metrics collection
REM ============================================================================

setlocal enabledelayedexpansion

echo ========================================
echo   MAOS Scientific Test Suite
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
    pause
    exit /b 1
)

set TEST_DIR=tests
set RESULTS_DIR=scientific_results_windows
set TIMESTAMP=%date:~-4,4%%date:~-10,2%%date:~-7,2%_%time:~0,2%%time:~3,2%%time:~6,2%
set TIMESTAMP=%TIMESTAMP: =0%

echo Creating results directories...
if not exist "%RESULTS_DIR%" mkdir "%RESULTS_DIR%"
if not exist "%RESULTS_DIR%\baseline" mkdir "%RESULTS_DIR%\baseline"
if not exist "%RESULTS_DIR%\size_conservative" mkdir "%RESULTS_DIR%\size_conservative"
if not exist "%RESULTS_DIR%\maximum_security" mkdir "%RESULTS_DIR%\maximum_security"
if not exist "%RESULTS_DIR%\comparisons" mkdir "%RESULTS_DIR%\comparisons"

REM Test cases
set TEST_CASES=test_easy.c test_medium.c test_difficult.c test_variadic_functions.c test_function_pointers.c

echo.
echo ========================================
echo   PHASE 1: Baseline Testing
echo ========================================
echo.

for %%T in (%TEST_CASES%) do (
    if exist "%TEST_DIR%\%%T" (
        echo Testing: %%T [Baseline]
        
        set INPUT=%TEST_DIR%\%%T
        set OUTPUT=%RESULTS_DIR%\baseline\%%~nT.exe
        set METRICS=%RESULTS_DIR%\baseline\%%~nT.json
        
        REM Get original size
        for %%F in ("!INPUT!") do set ORIG_SIZE=%%~zF
        
        REM Measure time
        set START_TIME=!time!
        
        %OBFUSCATOR% -i "!INPUT!" -o "!OUTPUT!" -l 2 -C 2 > nul 2>&1
        
        set END_TIME=!time!
        
        if exist "!OUTPUT!" (
            for %%F in ("!OUTPUT!") do set OBF_SIZE=%%~zF
            
            REM Calculate size increase
            set /a SIZE_INCREASE=(!OBF_SIZE! - !ORIG_SIZE!) * 100 / !ORIG_SIZE!
            
            echo   [PASS] Original: !ORIG_SIZE! bytes, Obfuscated: !OBF_SIZE! bytes, Increase: !SIZE_INCREASE!%%
            
            REM Create JSON metrics
            echo { > "!METRICS!"
            echo   "test_case": "%%T", >> "!METRICS!"
            echo   "mode": "baseline", >> "!METRICS!"
            echo   "original_size": !ORIG_SIZE!, >> "!METRICS!"
            echo   "obfuscated_size": !OBF_SIZE!, >> "!METRICS!"
            echo   "size_increase_percent": !SIZE_INCREASE! >> "!METRICS!"
            echo } >> "!METRICS!"
        ) else (
            echo   [FAIL] Obfuscation failed
        )
        echo.
    )
)

echo.
echo ========================================
echo   PHASE 2: Size-Conservative Testing
echo ========================================
echo.

for %%T in (%TEST_CASES%) do (
    if exist "%TEST_DIR%\%%T" (
        echo Testing: %%T [Size-Conservative]
        
        set INPUT=%TEST_DIR%\%%T
        set OUTPUT=%RESULTS_DIR%\size_conservative\%%~nT.exe
        set METRICS=%RESULTS_DIR%\size_conservative\%%~nT.json
        
        REM Get original size
        for %%F in ("!INPUT!") do set ORIG_SIZE=%%~zF
        
        %OBFUSCATOR% -i "!INPUT!" -o "!OUTPUT!" -c config\size_conservative.yaml > nul 2>&1
        
        if exist "!OUTPUT!" (
            for %%F in ("!OUTPUT!") do set OBF_SIZE=%%~zF
            
            REM Calculate size increase
            set /a SIZE_INCREASE=(!OBF_SIZE! - !ORIG_SIZE!) * 100 / !ORIG_SIZE!
            
            echo   [PASS] Original: !ORIG_SIZE! bytes, Obfuscated: !OBF_SIZE! bytes, Increase: !SIZE_INCREASE!%%
            
            REM Check constraint
            if !SIZE_INCREASE! LEQ 15 (
                echo   [OK] Size constraint met (≤15%%)
            ) else (
                echo   [WARNING] Size constraint exceeded (target ≤15%%)
            )
            
            REM Create JSON metrics
            echo { > "!METRICS!"
            echo   "test_case": "%%T", >> "!METRICS!"
            echo   "mode": "size_conservative", >> "!METRICS!"
            echo   "original_size": !ORIG_SIZE!, >> "!METRICS!"
            echo   "obfuscated_size": !OBF_SIZE!, >> "!METRICS!"
            echo   "size_increase_percent": !SIZE_INCREASE!, >> "!METRICS!"
            echo   "constraint_met": !SIZE_INCREASE! LEQ 15 >> "!METRICS!"
            echo } >> "!METRICS!"
        ) else (
            echo   [FAIL] Obfuscation failed
        )
        echo.
    )
)

echo.
echo ========================================
echo   PHASE 3: Maximum-Security Testing
echo ========================================
echo.

for %%T in (%TEST_CASES%) do (
    if exist "%TEST_DIR%\%%T" (
        echo Testing: %%T [Maximum-Security]
        
        set INPUT=%TEST_DIR%\%%T
        set OUTPUT=%RESULTS_DIR%\maximum_security\%%~nT.exe
        set METRICS=%RESULTS_DIR%\maximum_security\%%~nT.json
        
        REM Get original size
        for %%F in ("!INPUT!") do set ORIG_SIZE=%%~zF
        
        %OBFUSCATOR% -i "!INPUT!" -o "!OUTPUT!" -c config\maximum_security.yaml > nul 2>&1
        
        if exist "!OUTPUT!" (
            for %%F in ("!OUTPUT!") do set OBF_SIZE=%%~zF
            
            REM Calculate size increase
            set /a SIZE_INCREASE=(!OBF_SIZE! - !ORIG_SIZE!) * 100 / !ORIG_SIZE!
            
            echo   [PASS] Original: !ORIG_SIZE! bytes, Obfuscated: !OBF_SIZE! bytes, Increase: !SIZE_INCREASE!%%
            
            REM Create JSON metrics
            echo { > "!METRICS!"
            echo   "test_case": "%%T", >> "!METRICS!"
            echo   "mode": "maximum_security", >> "!METRICS!"
            echo   "original_size": !ORIG_SIZE!, >> "!METRICS!"
            echo   "obfuscated_size": !OBF_SIZE!, >> "!METRICS!"
            echo   "size_increase_percent": !SIZE_INCREASE! >> "!METRICS!"
            echo } >> "!METRICS!"
        ) else (
            echo   [FAIL] Obfuscation failed
        )
        echo.
    )
)

echo.
echo ========================================
echo   PHASE 4: Comparison Analysis
echo ========================================
echo.

set COMPARISON_REPORT=%RESULTS_DIR%\comparisons\comparison_report_%TIMESTAMP%.txt

echo MAOS Comparison Report > "%COMPARISON_REPORT%"
echo ====================== >> "%COMPARISON_REPORT%"
echo. >> "%COMPARISON_REPORT%"
echo Date: %date% %time% >> "%COMPARISON_REPORT%"
echo. >> "%COMPARISON_REPORT%"
echo Test Results Summary: >> "%COMPARISON_REPORT%"
echo --------------------- >> "%COMPARISON_REPORT%"
echo. >> "%COMPARISON_REPORT%"

for %%T in (%TEST_CASES%) do (
    echo Test Case: %%T >> "%COMPARISON_REPORT%"
    echo -------------- >> "%COMPARISON_REPORT%"
    
    if exist "%RESULTS_DIR%\baseline\%%~nT.exe" (
        for %%F in ("%RESULTS_DIR%\baseline\%%~nT.exe") do echo   Baseline: %%~zF bytes >> "%COMPARISON_REPORT%"
    )
    
    if exist "%RESULTS_DIR%\size_conservative\%%~nT.exe" (
        for %%F in ("%RESULTS_DIR%\size_conservative\%%~nT.exe") do echo   Size-Conservative: %%~zF bytes >> "%COMPARISON_REPORT%"
    )
    
    if exist "%RESULTS_DIR%\maximum_security\%%~nT.exe" (
        for %%F in ("%RESULTS_DIR%\maximum_security\%%~nT.exe") do echo   Maximum-Security: %%~zF bytes >> "%COMPARISON_REPORT%"
    )
    
    echo. >> "%COMPARISON_REPORT%"
)

echo.
echo ========================================
echo   Testing Complete!
echo ========================================
echo.
echo Results saved to: %RESULTS_DIR%
echo   - Baseline: %RESULTS_DIR%\baseline
echo   - Size-Conservative: %RESULTS_DIR%\size_conservative
echo   - Maximum-Security: %RESULTS_DIR%\maximum_security
echo   - Comparisons: %RESULTS_DIR%\comparisons
echo.
echo Comparison report: %COMPARISON_REPORT%
echo.

pause
