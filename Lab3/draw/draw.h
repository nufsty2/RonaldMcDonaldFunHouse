#include "../globals/globals.h"

void draw_sprite(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

void draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens);

void draw_alien(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

void toggle_alien_sprite(const uint32_t* sprite_val, int16_t x, int16_t y);

void toggle_alien_sprite(const uint32_t* sprite_val, int16_t x, int16_t y);

void toggle_all_sprites();

void move_alien_army();

const uint32_t* get_sprite_from_digit(char digit);

char* get_score_digit(uint8_t digit);

char* get_selected_char();

void update_score(bool firstRun);

void init_score();

void update_lives(bool firstRun);

void move_saucer();

void blink_cursor(bool force);

void reset_counters();

void main_print_score(score_t score, uint32_t pos);

void main_print_scores();