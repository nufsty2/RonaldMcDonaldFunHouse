cmake_minimum_required (VERSION 3.5)

project(Lab3)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(main ../hdmi/hdmi.c main.c)