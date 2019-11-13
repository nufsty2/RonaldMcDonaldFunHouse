#ifndef ALIEN_H
#define ALIEN_H

#include "../globals/globals.h"

// This is one of our init function for the alien, it just sets the random variables
void alien_init();

// This is our main function for drawing the whole alien army
// @param - pos - pos we want to draw at
// @param width - width of the sprite
// @param sprite_row - which row we are currently drawing
// @param alien_y - alien y value
// @param pixel_size - size of the pixel
// @param color - color we want the drawing to be
// @param erase_aliens - if true, erase aliens, if false, draw alien
void alien_draw_lots_o_aliens(uint32_t pos, uint32_t width, uint32_t sprite_row, uint32_t alien_y, uint16_t pixel_size, char color[], bool erase_aliens);

// This is our main draw function, we use this to draw a lot of different things
// @param sprite - sprite we want to draw
// @param pos - position on the screen we want to draw to 
// @param width - width of sprite
// @param height - height of the sprite
// @param pixel_size - size we want to draw
// @param color - color the sprite will have
void alien_draw(const uint32_t sprite[], uint32_t pos, uint32_t width, uint32_t height, uint16_t pixel_size, char color[]);

// This is our move alien army function, this gets called repeatly because aliens are moving all the time
void alien_move_army();

// This is our move saucer function, it will move the saucer every 30 seconds
// @param saucer_moving_local - this is what we pass in to the move saucer function to see if moving
// @return - returns false when saucer is done moving, else keep moving
bool alien_move_saucer(bool saucer_moving_local);

// This function will be called when a bullet is fired and consistently check if an alien has been hit
// @return - returns true if an alien has been hits, false if ain't
bool alien_detect_hit_army();

// This function detects a hit on the saucer
void alien_detect_hit_saucer();

// This function makes sure all aliens are dead
void alien_check_alien_reset();

// This function checks to see if an alien is dead by checking the bool array, if it is, erase
void alien_erase_dead();

// This function will draw the alien bullets, should be a max of 4 at a time
void alien_trigger_bullets();

// This function fires the alien bullet
// @param bullet_num - which bullet to shoot from the array (0-3)
void alien_fire_bullets();

// This function gets the y coord of whatever we put in (alien, player, bunker)
// @param coord - gets the position on screen
// @return - returns the y coord
uint16_t alien_get_y_coord(uint32_t coord);

// This function gets the x coord of whatever, depejnds on Y coord
// @param coord - the position on the screen
// @param y_coord - y coordinate we calculate from function above
// @return - returns the x value
uint16_t alien_get_x_coord(uint32_t coord, uint16_t y_coord);

#endif