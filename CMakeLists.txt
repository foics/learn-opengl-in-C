cmake_minimum_required(VERSION 3.5)
project(main.exe)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SDL2 SDL2main SDL2_mixer m)
