cmake_minimum_required (VERSION 3.5)

project(Lab4)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(main ../config/config.c ../i2cps/i2cps.c main.c)