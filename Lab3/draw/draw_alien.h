#ifndef DRAW_ALIEN_H
#define DRAW_ALIEN_H

#include "../globals/globals.h"

void draw_alien_init();

void draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens);

void draw_alien(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

void move_alien_army();

bool move_saucer(bool saucer_moving_local);

void move_alien_army();

bool draw_alien_detect_hit_army();

void draw_alien_detect_hit_saucer();

void draw_alien_check_alien_reset();

void erase_dead_aliens();

void draw_alien_trigger_bullets();

void draw_alien_debug_print();

void draw_alien_fire_alien_bullets();

#endif