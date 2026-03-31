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

add_library(rune_logger INTERFACE)

target_include_directories(rune_logger INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(rune_logger INTERFACE spdlog::spdlog)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_executable(rune-logger_example
        example.cpp
    )

    target_link_libraries(rune-logger_example PRIVATE rune_logger)
endif()
