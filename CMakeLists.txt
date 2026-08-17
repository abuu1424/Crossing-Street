cmake_minimum_required(VERSION 3.16)
project(CrossingGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tìm SFML
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")
endif()

if(WIN32)
    if(DEFINED ENV{SFML_DIR})
        list(APPEND CMAKE_PREFIX_PATH "$ENV{SFML_DIR}")
    endif()
    if(DEFINED ENV{SFML_ROOT})
        list(APPEND CMAKE_PREFIX_PATH "$ENV{SFML_ROOT}")
    endif()
    list(APPEND CMAKE_PREFIX_PATH "C:/SFML-2.6.2" "C:/SFML-2.5.1" "C:/SFML")
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
    src/utils
    src/entities
    src/entities/base
    src/entities/obstacles
    src/entities/obstacles/lv1
    src/entities/obstacles/lv2
    src/entities/obstacles/lv3
    src/entities/obstacles/lv4
    src/entities/obstacles/lv5
    src/entities/obstacles/lv1/animal_lv1
    src/entities/obstacles/lv1/traffic_lv1
    src/entities/obstacles/lv2/animal_lv2
    src/entities/obstacles/lv2/traffic_lv2
    src/entities/obstacles/lv3/animal_lv3
    src/entities/obstacles/lv3/vehicle_lv3
    src/entities/obstacles/lv3/traffic_lv3
    src/entities/obstacles/lv4/animal_lv4
    src/entities/obstacles/lv4/vehicle_lv4
    src/entities/obstacles/lv4/traffic_lv4
    src/entities/obstacles/lv5/animal_lv5
    src/entities/obstacles/lv5/vehicle_lv5
    src/entities/obstacles/lv5/traffic_lv5
    src/cutscenes
    src/levels
    src/ui
    src/hazards
)

# Link SFML
target_link_libraries(CrossingGame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Compiler definitions
if(WIN32)
    target_compile_definitions(CrossingGame PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
endif()

# Cấu hình compiler warnings & UTF-8
if(MSVC)
    target_compile_options(CrossingGame PRIVATE /W4 /utf-8)
else()
    target_compile_options(CrossingGame PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# Visual Studio debugger: đặt Working Directory về thư mục gốc dự án để F5 debug load được assets/
set_target_properties(CrossingGame PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

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

# Windows: copy SFML & OpenAL DLLs vào cùng thư mục exe
if(WIN32)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.21")
        add_custom_command(TARGET CrossingGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CrossingGame>
            $<TARGET_FILE_DIR:CrossingGame>
            COMMAND_EXPAND_LISTS
        )
    endif()

    # Đảm bảo copy toàn bộ DLL từ thư mục bin của SFML (bao gồm openal32.dll)
    get_filename_component(SFML_BIN_DIR "${SFML_DIR}/../../../bin" ABSOLUTE)
    if(EXISTS "${SFML_BIN_DIR}")
        add_custom_command(TARGET CrossingGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SFML_BIN_DIR}"
            "$<TARGET_FILE_DIR:CrossingGame>"
            COMMENT "Copying SFML and OpenAL runtime DLLs..."
        )
    endif()
endif()