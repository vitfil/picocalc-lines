# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)

project(lcdspi
        VERSION 0.1.0
        DESCRIPTION "lcdspi"
        )

add_library(lcdspi INTERFACE)

target_sources(lcdspi INTERFACE
        lcdspi.c
        gfx.c
        )

target_link_libraries(lcdspi INTERFACE  pico_stdlib hardware_spi)

target_include_directories(lcdspi INTERFACE ${CMAKE_CURRENT_LIST_DIR})