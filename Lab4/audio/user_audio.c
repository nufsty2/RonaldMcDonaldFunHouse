#include "user_audio.h"

static uint32_t file;
static int32_t invader_die[SUPER_BIG];
static int32_t laser[SUPER_BIG];
static int32_t player_die[SUPER_BIG];
static int32_t ufo_die[SUPER_BIG];
static int32_t ufo[SUPER_BIG];
static int32_t walk1[SUPER_BIG];
static int32_t walk2[SUPER_BIG];
static int32_t walk3[SUPER_BIG];
static int32_t walk4[SUPER_BIG];
static uint32_t invader_die_size;
static uint32_t laser_size;
static uint32_t player_die_size;
static uint32_t ufo_die_size;
static uint32_t ufo_size;
static uint32_t walk1_size;
static uint32_t walk2_size;
static uint32_t walk3_size;
static uint32_t walk4_size;

void user_audio_init()
{
    file = open("/dev/audio", O_RDWR);
    if (file == FILE_NOT_FOUND)
        printf("File not found\n\r");

    // Open audio driver
    open_audio_driver();

    // config the codec
    config_audio_pll(I2C_INDEX);
    config_audio_codec(I2C_INDEX);

    // load sound data
    invader_die_size    = wav_read_data(invader_die,    "../wav_files/invader_die.wav");
    laser_size          = wav_read_data(laser,          "../wav_files/laser.wav");
    player_die_size     = wav_read_data(player_die,     "../wav_files/player_die.wav");
    ufo_die_size        = wav_read_data(ufo_die,        "../wav_files/ufo_die.wav");
    ufo_size            = wav_read_data(ufo,            "../wav_files/ufo.wav");
    walk1_size          = wav_read_data(walk1,          "../wav_files/walk1.wav");
    walk2_size          = wav_read_data(walk2,          "../wav_files/walk2.wav");
    walk3_size          = wav_read_data(walk3,          "../wav_files/walk3.wav");
    walk4_size          = wav_read_data(walk4,          "../wav_files/walk4.wav");
}

void user_audio_play(uint32_t file, int32_t audio_data[], uint32_t size) 
{
    write(file, audio_data, size);
    while(!read(file, audio_data, size));
}

void user_audio_play_sound(uint8_t sound_num)
{
    switch (sound_num)
    {
        case SOUND_INVADER_DIE:
            user_audio_play(file, invader_die, invader_die_size);
            break;
        case SOUND_LASER:
            user_audio_play(file, laser, laser_size);
            break;      
        case SOUND_PLAYER_DIE:
            user_audio_play(file, player_die, player_die_size);
            break; 
        case SOUND_UFO_DIE:
            user_audio_play(file, ufo_die, ufo_die_size);
            break;    
        case SOUND_UFO:
            user_audio_play(file, ufo, ufo_size);
            break;        
        case SOUND_WALK_1:
            user_audio_play(file, walk1, walk1_size);
            break;     
        case SOUND_WALK_2:
            user_audio_play(file, walk2, walk2_size);
            break;     
        case SOUND_WALK_3:
            user_audio_play(file, walk3, walk3_size);
            break;     
        case SOUND_WALK_4:
            user_audio_play(file, walk4, walk4_size);
            break;
    }
}