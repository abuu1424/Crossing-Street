cmake_minimum_required(VERSION 3.16)
project(CrossingGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Tìm SFML
if(APPLE)
    set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/SFML-2.6.2")
endif()

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

#Thu thập source files
file(GLOB_RECURSE SOURCES
        "save/*cpp"
        "src/*.cpp"
        "src/core/*.cpp"
        "src/obstacles/*.cpp"
        "src/animals/*.cpp"
        "src/levels/*.cpp"
        "src/ui/*.cpp"
        "src/obstacles/lv1/animal_lv1/*.cpp"
        "src/obstacles/lv1/traffic_lv1/*.cpp"
)

#Tạo executable
add_executable(CrossingGame
        ${SOURCES}
        src/main.cpp
        # Core
        src/core/SaveData.cpp
        src/core/SaveData.h
        src/core/HighScore.cpp
        src/core/HighScore.h
        src/core/EntityManager.cpp
        src/core/EntityManager.h
        src/core/SoundManager.cpp
        src/core/SoundManager.h
        # Levels
        src/levels/LevelConfig.cpp
        # Level 1
        src/obstacles/lv1/traffic_lv1/CTRAFFIC_LV1.cpp
        src/obstacles/lv1/traffic_lv1/CTRAFFIC_LV1.h
        # Level 2
        src/obstacles/lv2/animal_lv2/CEAGLE.cpp
        src/obstacles/lv2/animal_lv2/CEAGLE.h
        src/obstacles/lv2/animal_lv2/CHORUS.cpp
        src/obstacles/lv2/animal_lv2/CHORUS.h
        src/obstacles/lv2/animal_lv2/CCAMEL.cpp
        src/obstacles/lv2/animal_lv2/CCAMEL.h
        src/obstacles/lv2/traffic_lv2/CTRAFFIC_LV2.cpp
        src/obstacles/lv2/traffic_lv2/CTRAFFIC_LV2.h
        # Level 3
        src/obstacles/lv3/vehicle_lv3/CRICKSHAW.cpp
        src/obstacles/lv3/vehicle_lv3/CRICKSHAW.h
        src/obstacles/lv3/vehicle_lv3/CHORSE.cpp
        src/obstacles/lv3/vehicle_lv3/CHORSE.h
        src/obstacles/lv3/vehicle_lv3/CWOOD.cpp
        src/obstacles/lv3/vehicle_lv3/CWOOD.h
        src/obstacles/lv3/animal_lv3/CROW.h
        src/obstacles/lv3/animal_lv3/CARROW.h
        src/obstacles/lv3/animal_lv3/CROW.cpp
        src/obstacles/lv3/animal_lv3/CARROW.cpp
        # Level 4
        src/obstacles/lv4/vehicle_lv4/CBUS.cpp
        src/obstacles/lv4/vehicle_lv4/CBUS.h
        src/obstacles/lv4/vehicle_lv4/CCAR.cpp
        src/obstacles/lv4/vehicle_lv4/CCAR.h
        src/obstacles/lv4/vehicle_lv4/CMOTOR.cpp
        src/obstacles/lv4/vehicle_lv4/CMOTOR.h
        src/obstacles/lv4/animal_lv4/CMISSILE.cpp
        src/obstacles/lv4/animal_lv4/CMISSILE.h
        src/obstacles/lv4/animal_lv4/CPLANE.cpp
        src/obstacles/lv4/animal_lv4/CPLANE.h
        # Level 5
        src/obstacles/lv5/vehicle_lv5/CECAR.cpp
        src/obstacles/lv5/vehicle_lv5/CECAR.h
        src/obstacles/lv5/vehicle_lv5/CETRAIN.cpp
        src/obstacles/lv5/vehicle_lv5/CETRAIN.h
        src/obstacles/lv5/animal_lv5/CDRONE.cpp
        src/obstacles/lv5/animal_lv5/CDRONE.h
        src/obstacles/lv5/animal_lv5/CFLYCAR.cpp
        src/obstacles/lv5/animal_lv5/CFLYCAR.h
        src/obstacles/lv5/animal_lv5/CUFO.cpp
        src/obstacles/lv5/animal_lv5/CUFO.h
)
#Include paths
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
        save
)

#Link SFML
target_link_libraries(CrossingGame PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

#Tắt warning trên macOS
if(APPLE)
    target_compile_options(CrossingGame PRIVATE -w)
endif()

file(COPY ${CMAKE_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_BINARY_DIR})

#Copy assets vào build
add_custom_command(TARGET CrossingGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:CrossingGame>/assets"
        COMMENT "Copying assets..."
)

#Tạo thư mục saves
add_custom_command(TARGET CrossingGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:CrossingGame>/saves"
        COMMENT "Creating saves directory..."
)

#Windows: copy SFML DLL vào cùng thư mục exe
if(WIN32)
    add_custom_command(TARGET CrossingGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CrossingGame>
            $<TARGET_FILE_DIR:CrossingGame>
            COMMAND_EXPAND_LISTS
    )
endif()