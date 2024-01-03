#ifndef __SOUND_H
#define __SOUND_H

#define SOUND_UFO               0
#define SOUND_PLAYER_SHOT       1
#define SOUND_PLAYER_EXPLOSION  2
#define SOUND_ALIEN_EXPLOSION   3
#define SOUND_ALIEN_STEP1       4
#define SOUND_ALIEN_STEP2       5
#define SOUND_ALIEN_STEP3       6
#define SOUND_ALIEN_STEP4       7
#define SOUND_UFO_EXPLOSION     8
#define SOUND_EXTRA_LIFE        9

struct sound_table[9]
    string name;
    int sound_id;
    int channel;
end = { 
    { "res/sounds/0.wav" , 0,  1 }, // SOUND_UFO
    { "res/sounds/1.wav" , 0,  2 }, // SOUND_PLAYER_SHOT
    { "res/sounds/2.wav" , 0,  3 }, // SOUND_PLAYER_EXPLOSION
    { "res/sounds/3.wav" , 0,  4 }, // SOUND_ALIEN_EXPLOSION
    { "res/sounds/4.wav" , 0,  5 }, // SOUND_ALIEN_STEP1
    { "res/sounds/5.wav" , 0,  5 }, // SOUND_ALIEN_STEP2
    { "res/sounds/6.wav" , 0,  5 }, // SOUND_ALIEN_STEP3
    { "res/sounds/7.wav" , 0,  5 }, // SOUND_ALIEN_STEP4
    { "res/sounds/8.wav" , 0,  6 }, // SOUND_UFO_EXPLOSION
    { "res/sounds/9.wav" , 0,  7 }  // SOUND_EXTRA_LIFE
};

function loadSounds()
begin
    soundsys_init();
    for ( int n = 0; n < sizeof( sound_table ) / sizeof( sound_table[0] ); n++ )
        sound_table[ n ].sound_id = sound_load( sound_table[ n ].name );
    end
end

function SoundPlay( int id, int _loop = 0 )
begin
    sound_play( sound_table[ id ].sound_id, _loop, sound_table[ id ].channel );
end

function SoundStop( int id )
begin
    sound_stop( sound_table[ id ].channel );
end

function int SoundIsPlaying( int id )
begin
    return sound_is_playing( sound_table[ id ].channel );
end

#endif
