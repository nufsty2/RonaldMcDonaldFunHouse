void draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens);

void draw_alien(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

void move_alien_army();

bool move_saucer(bool saucer_moving_local);