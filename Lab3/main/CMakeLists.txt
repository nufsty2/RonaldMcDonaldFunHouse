cmake_minimum_required (VERSION 3.5)

project(Lab3)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(main ../hdmi/hdmi.c ../../Lab2/buttons/buttons.c ../../Lab2/switches/switches.c ../../Lab2/intc/intc.c ../draw/draw_generic.c ../scores/scores.c ../init/init.c ../draw/draw_alien.c ../draw/draw_ui.c ../draw/draw_game_over.c ../draw/draw_player.c ../globals/globals.c main.c)