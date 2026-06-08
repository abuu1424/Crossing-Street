@echo off
setlocal

set PROJECT_NAME=CrossingGame
set BUILD_DIR=build_win
set SFML_DIR=C:\SFML-2.6.2

if "%1"=="clean" (
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

cmake .. ^
    -DCMAKE_PREFIX_PATH="%SFML_DIR%"

if errorlevel 1 (
    cd ..
    pause
    exit /b 1
)

cmake --build . --config Release

if errorlevel 1 (
    cd ..
    pause
    exit /b 1
)

cd ..

if "%1"=="run" goto :run
if "%2"=="run" goto :run
goto :end

:run
if exist "%BUILD_DIR%\Release\%PROJECT_NAME%.exe" (
    "%BUILD_DIR%\Release\%PROJECT_NAME%.exe"
) else (
    "%BUILD_DIR%\%PROJECT_NAME%.exe"
)

:end
pause