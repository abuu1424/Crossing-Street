@echo off

REM Cách dùng:
REM   build.bat          (build bình thường)
REM   build.bat clean    (xóa build cũ rồi build lại)
REM   build.bat run      (build rồi chạy game luôn)

setlocal enabledelayedexpansion

set PROJECT_NAME=CrossingGame
set BUILD_DIR=build_win

REM ── Đường dẫn vcpkg — SỬA CHỖ NÀY cho đúng máy ────────────────────────────
set VCPKG_ROOT=C:\vcpkg
REM Nếu vcpkg ở chỗ khác thì đổi lại, ví dụ:
REM set VCPKG_ROOT=C:\Users\YourName\vcpkg

set VCPKG_TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake

echo.
echo ╔══════════════════════════════════╗
echo ║   CrossingGame — Windows Build   ║
echo ╚══════════════════════════════════╝
echo.

REM ── Kiểm tra vcpkg ───────────────────────────────────────────────────────────
if not exist "%VCPKG_TOOLCHAIN%" (
    echo [ERROR] Khong tim thay vcpkg tai: %VCPKG_ROOT%
    echo.
    echo Huong dan cai vcpkg:
    echo   1. git clone https://github.com/microsoft/vcpkg
    echo   2. .\vcpkg\bootstrap-vcpkg.bat
    echo   3. .\vcpkg\vcpkg install sfml:x64-windows
    echo   4. .\vcpkg\vcpkg integrate install
    echo   5. Sua VCPKG_ROOT trong build.bat cho dung duong dan
    pause
    exit /b 1
)

REM ── Clean nếu có arg "clean" ──────────────────────────────────────────────────
if "%1"=="clean" (
    echo [INFO] Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
    echo [INFO] Done!
)

REM ── Tạo thư mục build ─────────────────────────────────────────────────────────
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

REM ── CMake configure ───────────────────────────────────────────────────────────
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

REM ── Build ─────────────────────────────────────────────────────────────────────
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

REM ── Chạy game nếu có arg "run" ────────────────────────────────────────────────
if "%1"=="run" goto :run
if "%2"=="run" goto :run
goto :end

:run
echo.
echo [INFO] Launching game...
"%BUILD_DIR%\Release\%PROJECT_NAME%.exe"

:end
pause
