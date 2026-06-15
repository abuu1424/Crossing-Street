@echo off
setlocal

set PROJECT_NAME=CrossingGame
set BUILD_DIR=build_win
set SFML_DIR=C:\SFML-2.6.2

if "%1"=="clean" (
    if exist "%BUILD_DIR%" (
        echo [INFO] Removing build directory...
        rmdir /s /q "%BUILD_DIR%"
    )
)

cmake -S . -B "%BUILD_DIR%" ^
    -A x64 ^
    -DCMAKE_PREFIX_PATH="%SFML_DIR%"

if errorlevel 1 (
    echo.
    echo [ERROR] CMake configure failed!
    pause
    exit /b 1
)

cmake --build "%BUILD_DIR%" --config Release

if errorlevel 1 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b 1
)

echo [INFO] Copying SFML DLLs...
copy "%SFML_DIR%\bin\*.dll" "%BUILD_DIR%\Release\" /Y >nul

echo.
echo [OK] Build successful!

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