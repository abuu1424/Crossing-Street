#!/bin/bash

set -e  # Dừng ngay nếu có lỗi

PROJECT_NAME="CrossingGame"
BUILD_DIR="build"
SFML_PREFIX="/opt/homebrew"

# Màu terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}╔══════════════════════════════════╗${NC}"
echo -e "${CYAN}║   CrossingGame — Mac Build       ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════╝${NC}"

# ── Clean nếu có arg "clean"
if [ "$1" = "clean" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
    echo -e "${GREEN}Done!${NC}"
fi

# ── Tạo thư mục build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# ── CMake configure 
echo -e "\n${YELLOW}Configuring with CMake...${NC}"
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DSFML_DIR="$SFML_PREFIX/lib/cmake/SFML" \
    -Wno-dev \
    2>&1

# ── Build 
echo -e "\n${YELLOW}Building...${NC}"
cmake --build . --config Release -- -j$(sysctl -n hw.ncpu)

echo -e "\n${GREEN}✓ Build thành công! → build/$PROJECT_NAME${NC}"

cd ..
if [ "$1" = "run" ] || [ "$2" = "run" ]; then
    echo -e "\n${CYAN}Launching game...${NC}\n"
    "./$BUILD_DIR/$PROJECT_NAME"
fi
