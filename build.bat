@echo off


setlocal enabledelayedexpansion

set PROJECT_NAME=CrossingGame
set BUILD_DIR=build_win

REM
set VCPKG_ROOT=C:\vcpkg


set VCPKG_TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake

echo.
echo ╔══════════════════════════════════╗
echo ║   CrossingGame — Windows Build   ║
echo ╚══════════════════════════════════╝
echo.

REM ── Clean nếu có arg "clean"
if "%1"=="clean" (
    echo [INFO] Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
    echo [INFO] Done!
)

REM ── Tạo thư mục build
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

REM ── CMake configure
echo [INFO] Configuring with CMake...
cmake .. ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%" ^
    -DVCPKG_TARGET_TRIPLET=x64-windows

if errorlevel 1 (
    echo [ERROR] CMake configure that bai!
    cd ..
    pause
    exit /b 1
)

REM ── Build
echo.
echo [INFO] Building...
cmake --build . --config Release

if errorlevel 1 (
    echo [ERROR] Build that bai!
    cd ..
    pause
    exit /b 1
)

echo.
echo [OK] Build thành công → %BUILD_DIR%\Release\%PROJECT_NAME%.exe
cd ..

REM ── Chạy game nếu có arg "run"git pull --no-rebase

if "%1"=="run" goto :run
if "%2"=="run" goto :run
goto :end

:run
echo.
echo [INFO] Launching game...
"%BUILD_DIR%\Release\%PROJECT_NAME%.exe"

:end
pause
