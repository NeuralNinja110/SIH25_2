@echo off
REM Phantron CLI Wrapper for Windows - Runs via WSL
REM Usage: phantron-wsl.bat [command] [options]

set PROJECT_PATH=/mnt/c/Users/abcra/OneDrive/Desktop/Phantron/SIH25_2

REM Pass all arguments to the Node.js CLI in WSL
wsl -d Ubuntu-24.04 -- bash -c "cd %PROJECT_PATH% && node cli/index.js %*"
