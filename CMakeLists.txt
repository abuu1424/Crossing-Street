cmake_minimum_required(VERSION 3.16)
project(CrossingGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tìm SFML
if(APPLE)
    set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/SFML-2.6.2")
endif()

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Tự động thu thập toàn bộ source & header trong src/ (tự re-configure khi thêm/xóa file)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.h"
)

# Tạo executable
add_executable(CrossingGame ${SOURCES})

# Include paths
target_include_directories(CrossingGame PRIVATE
    src
    src/core
    src/obstacles
    src/obstacles/lv1
    src/obstacles/lv2
    src/obstacles/lv3
    src/obstacles/lv4
    src/obstacles/lv5
    src/obstacles/lv1/animal_lv1
    src/obstacles/lv1/traffic_lv1
    src/obstacles/lv2/animal_lv2
    src/obstacles/lv2/traffic_lv2
    src/obstacles/lv3/animal_lv3
    src/obstacles/lv3/vehicle_lv3
    src/obstacles/lv3/traffic_lv3
    src/obstacles/lv4/animal_lv4
    src/obstacles/lv4/vehicle_lv4
    src/obstacles/lv4/traffic_lv4
    src/obstacles/lv5/animal_lv5
    src/obstacles/lv5/vehicle_lv5
    src/obstacles/lv5/traffic_lv5
    src/vehicle
    src/animals
    src/levels
    src/ui
)

# Link SFML
target_link_libraries(CrossingGame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Cấu hình compiler warnings
if(MSVC)
    target_compile_options(CrossingGame PRIVATE /W4)
else()
    target_compile_options(CrossingGame PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# Copy assets vào build
add_custom_command(TARGET CrossingGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:CrossingGame>/assets"
    COMMENT "Copying assets..."
)

# Tạo thư mục saves
add_custom_command(TARGET CrossingGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    "$<TARGET_FILE_DIR:CrossingGame>/saves"
    COMMENT "Creating saves directory..."
)

# Windows: copy SFML DLL vào cùng thư mục exe
if(WIN32)
    add_custom_command(TARGET CrossingGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:CrossingGame>
        $<TARGET_FILE_DIR:CrossingGame>
        COMMAND_EXPAND_LISTS
    )
endif()