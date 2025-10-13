@echo off
REM ============================================================================
REM MAOS - Quick Test (Windows)
REM ============================================================================
REM Quick verification that MAOS is working correctly
REM ============================================================================

echo ========================================
echo   MAOS Quick Test
echo ========================================
echo.

REM Find obfuscator
set OBFUSCATOR=build\Release\llvm-obfuscator.exe
if not exist "%OBFUSCATOR%" (
    set OBFUSCATOR=build\llvm-obfuscator.exe
)

if not exist "%OBFUSCATOR%" (
    echo ERROR: Obfuscator not found!
    echo Please run build_windows.bat first.
    pause
    exit /b 1
)

echo Found: %OBFUSCATOR%
echo.

REM Test 1: Help command
echo Test 1: Checking help output...
%OBFUSCATOR% --help > nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo [PASS] Help command works
) else (
    echo [FAIL] Help command failed
)
echo.

REM Test 2: Version command
echo Test 2: Checking version...
%OBFUSCATOR% --version
if %ERRORLEVEL% equ 0 (
    echo [PASS] Version command works
) else (
    echo [FAIL] Version command failed
)
echo.

REM Test 3: Simple obfuscation
if exist "tests\test_easy.c" (
    echo Test 3: Obfuscating test_easy.c...
    
    if exist "quick_test_output.exe" del "quick_test_output.exe"
    
    %OBFUSCATOR% -i tests\test_easy.c -o quick_test_output.exe -l 2 -C 2
    
    if exist "quick_test_output.exe" (
        echo [PASS] Obfuscation successful
        for %%F in ("quick_test_output.exe") do echo Output size: %%~zF bytes
        
        REM Clean up
        del "quick_test_output.exe" 2>nul
    ) else (
        echo [FAIL] Obfuscation failed
    )
) else (
    echo [SKIP] test_easy.c not found
)

echo.
echo ========================================
echo   Quick Test Complete!
echo ========================================
echo.

pause
