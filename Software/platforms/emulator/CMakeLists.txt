cmake_minimum_required(VERSION 3.15)
project(HHTV_emulator)

set(CMAKE_CXX_STANDARD 14)

# find_package(SDL2 REQUIRED)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})

#include_directories("../../api/include")

add_definitions( -DPLATFORM_PC=1 )

add_subdirectory("../../os/" HLM-os)

add_subdirectory("../../api/" HLM-api)

add_subdirectory("../../libs/" HLM-libs)

target_compile_definitions(HLM-os INTERFACE BAR)

add_executable(HHTVemulator "input.cpp" "main.cpp" "graphics.cpp" "driver/pcrandom.cpp" "driver/pcinput.cpp" "driver/pcbattery.cpp" "driver/pctime.cpp" "driver/pcstorage.cpp")

target_link_libraries(HHTVemulator ${SDL2_LIBRARIES})

target_link_libraries(HHTVemulator HLM-os)

target_link_libraries(HHTVemulator HLM-api)

target_link_libraries(HHTVemulator HLM-libs)
