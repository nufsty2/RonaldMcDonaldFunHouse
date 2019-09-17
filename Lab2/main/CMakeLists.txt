cmake_minimum_required (VERSION 3.5)

project(Lab2)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(main_button_test ../buttons/buttons.c ../switches/switches.c ../intc/intc.c main_button_test.c)