#ifndef ALIEN_H
#define ALIEN_H

#include "../globals/globals.h"

void alien_init();

void alien_draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens);

void alien_draw(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

void alien_move_army();

bool alien_move_saucer(bool saucer_moving_local);

bool alien_detect_hit_army();

void alien_detect_hit_saucer();

void alien_check_alien_reset();

void alien_erase_dead();

void alien_trigger_bullets();

void alien_fire_bullets();

uint16_t alien_get_y_coord(uint32_t coord);

uint16_t alien_get_x_coord(uint32_t coord, uint16_t y_coord);

#endif