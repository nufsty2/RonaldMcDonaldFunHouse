cmake_minimum_required (VERSION 3.5)

project(Lab3)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(main ../hdmi/hdmi.c ../../Lab2/buttons/buttons.c ../../Lab2/switches/switches.c ../../Lab2/intc/intc.c ../draw/generic.c ../scores/scores.c ../init/init.c ../draw/alien.c ../draw/ui.c ../draw/game_over.c ../draw/player.c ../globals/globals.c ../draw/bunker.c main.c)