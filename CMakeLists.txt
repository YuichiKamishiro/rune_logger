cmake_minimum_required(VERSION 3.16)

project(rune-logger LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

add_executable(rune-logger_example
    example.cpp
)

target_include_directories(rune-logger_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${spdlog_SOURCE_DIR}/include
)

target_link_libraries(rune-logger_example PRIVATE spdlog::spdlog)
