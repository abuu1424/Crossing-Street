@echo off
setlocal

set PROJECT_NAME=CrossingGame
set BUILD_DIR=build_win
set SFML_DIR=C:\SFML-2.6.2

:: Clean
if "%1"=="clean" (
    if exist "%BUILD_DIR%" (
        echo [INFO] Removing build directory...
        rmdir /s /q "%BUILD_DIR%"
    )
)

:: Create build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

:: Configure
cmake .. ^
    -G "Visual Studio 18 2026" ^
    -A x64 ^
    -DCMAKE_PREFIX_PATH="%SFML_DIR%"

if errorlevel 1 (
    echo.
    echo [ERROR] CMake configure failed!
    cd ..
    pause
    exit /b 1
)

:: Build
cmake --build . --config Release

if errorlevel 1 (
    echo.
    echo [ERROR] Build failed!
    cd ..
    pause
    exit /b 1
)

:: Copy SFML DLLs
echo [INFO] Copying SFML DLLs...
copy "%SFML_DIR%\bin\*.dll" "Release\" /Y >nul

:: Back to project root
cd ..

echo.
echo [OK] Build successful!

:: Run
if "%1"=="run" goto :run
if "%2"=="run" goto :run
goto :end

:run
echo.
echo [INFO] Starting game...
echo.

if exist "%BUILD_DIR%\Release\%PROJECT_NAME%.exe" (
    "%BUILD_DIR%\Release\%PROJECT_NAME%.exe"
    echo.
    echo [INFO] Exit code: %errorlevel%
) else (
    echo [ERROR] Cannot find:
    echo %BUILD_DIR%\Release\%PROJECT_NAME%.exe
)

:end
pause