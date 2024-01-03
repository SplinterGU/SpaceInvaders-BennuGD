#ifndef __SPRITES_INC
#define __SPRITES_INC

#include "sprsheet.inc"

#define AS_ROL  0
#define AS_PLU  1
#define AS_SQU  2

/* ********************************************* */

#define drawSprite()                map_block_copy(0, main_game_layer, x, y, graph, clip.x, clip.y, clip.w, clip.h, B_NOCOLORKEY)
#define drawSpriteMerge()           map_block_copy(0, main_game_layer, x, y, graph, clip.x, clip.y, clip.w, clip.h, 0)
#define drawSpriteXY(a,b)           map_block_copy(0, main_game_layer, a, b, graph, clip.x, clip.y, clip.w, clip.h, B_NOCOLORKEY)
#define drawSpriteXYMerge(a,b)      map_block_copy(0, main_game_layer, a, b, graph, clip.x, clip.y, clip.w, clip.h, 0)

// #define deleteSprite()              map_block_copy(0, main_game_layer, x, y, 0, graph, clip.x, clip.y, clip.w, clip.h, B_NOCOLORKEY, 255, 255, 255, 255, BLEND_ALPHA_MASK )
// #define deleteSpriteMerge()         map_block_copy(0, main_game_layer, x, y, 0, graph, clip.x, clip.y, clip.w, clip.h, 0, 255, 255, 255, 255, BLEND_ALPHA_MASK )
// #define deleteSpriteXY(a,b)         map_block_copy(0, main_game_layer, a, b, 0, graph, clip.x, clip.y, clip.w, clip.h, B_NOCOLORKEY, 255, 255, 255, 255, BLEND_ALPHA_MASK )
// #define deleteSpriteXYMerge(a,b)    map_block_copy(0, main_game_layer, a, b, 0, graph, clip.x, clip.y, clip.w, clip.h, 0, 255, 255, 255, 255, BLEND_ALPHA_MASK )

#define deleteSprite8x8(x,y)        map_clear( 0, main_game_layer, x, y, x + 7, y + 7 )
#define deleteSprite16x8(x,y)       map_clear( 0, main_game_layer, x, y, x + 15, y + 7 )
#define deleteSprite24x8(x,y)       map_clear( 0, main_game_layer, x, y, x + 23, y + 7 )

#define ClearRows(y,h)              map_clear( 0, main_game_layer, 0, y, 256, y + h )

/* ********************************************* */

int spr_aliensA                 [1] = 0, 0;
int spr_aliensB                 [1] = 0, 0;
int spr_aliensC                 [1] = 0, 0;
int spr_aliensC2                [1] = 0, 0;
int spr_alienCYInv              [1] = 0, 0;
int spr_alienCY                 [1] = 0, 0;
int spr_player                      = 0;
int spr_ufo                         = 0;
int spr_alien_explosion             = 0;
int spr_alien_shot_explosion        = 0;
int spr_player_explosion        [1] = 0,0;
int spr_player_shot_explosion       = 0;
int spr_ufo_explosion               = 0;
int spr_alien_shot           [2][3] = { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 };
int spr_player_shot                 = 0;
int spr_shield                      = 0;

int spr_aliens[54];

/* ********************************************* */

__SPRSHEET_LIB * invaders_sprites_lib = NULL;

/* ********************************************* */

function int loadSprites()
begin
    invaders_sprites_lib        = sprsheet_load("res/sprites/sprites.png");

    if ( !invaders_sprites_lib ) return 1; end

    spr_aliensA[0]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprA-1");
    spr_aliensA[1]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprA-2");
    spr_aliensB[0]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprB-1");
    spr_aliensB[1]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprB-2");
    spr_aliensC[0]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprC-1");
    spr_aliensC[1]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprC-2");
    spr_alienCYInv[0]           = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprCYInv-1");
    spr_alienCYInv[1]           = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprCYInv-2");
    spr_alienCY[0]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprCY-1");
    spr_alienCY[1]              = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienSprCY-2");
    spr_player                  = sprsheet_get_id_by_name(invaders_sprites_lib, "PlayerSprite");
    spr_ufo                     = sprsheet_get_id_by_name(invaders_sprites_lib, "SpriteSaucer");
    spr_alien_explosion         = sprsheet_get_id_by_name(invaders_sprites_lib, "AlienExplode");
    spr_alien_shot_explosion    = sprsheet_get_id_by_name(invaders_sprites_lib, "AShotExplo");
    spr_player_explosion[0]     = sprsheet_get_id_by_name(invaders_sprites_lib, "PlrBlowupSprites-1");
    spr_player_explosion[1]     = sprsheet_get_id_by_name(invaders_sprites_lib, "PlrBlowupSprites-2");
    spr_player_shot_explosion   = sprsheet_get_id_by_name(invaders_sprites_lib, "ShotExploding");
    spr_ufo_explosion           = sprsheet_get_id_by_name(invaders_sprites_lib, "SpriteSaucerExp");
    spr_player_shot             = sprsheet_get_id_by_name(invaders_sprites_lib, "PlayerShotSpr");
    spr_alien_shot[ AS_ROL ][0] = sprsheet_get_id_by_name(invaders_sprites_lib, "RollShot-1");
    spr_alien_shot[ AS_ROL ][1] = sprsheet_get_id_by_name(invaders_sprites_lib, "RollShot-2");
    spr_alien_shot[ AS_ROL ][2] = sprsheet_get_id_by_name(invaders_sprites_lib, "RollShot-3");
    spr_alien_shot[ AS_ROL ][3] = sprsheet_get_id_by_name(invaders_sprites_lib, "RollShot-4");
    spr_alien_shot[ AS_PLU ][0] = sprsheet_get_id_by_name(invaders_sprites_lib, "PlungerShot-1");
    spr_alien_shot[ AS_PLU ][1] = sprsheet_get_id_by_name(invaders_sprites_lib, "PlungerShot-2");
    spr_alien_shot[ AS_PLU ][2] = sprsheet_get_id_by_name(invaders_sprites_lib, "PlungerShot-3");
    spr_alien_shot[ AS_PLU ][3] = sprsheet_get_id_by_name(invaders_sprites_lib, "PlungerShot-4");
    spr_alien_shot[ AS_SQU ][0] = sprsheet_get_id_by_name(invaders_sprites_lib, "SquiglyShot-1");
    spr_alien_shot[ AS_SQU ][1] = sprsheet_get_id_by_name(invaders_sprites_lib, "SquiglyShot-2");
    spr_alien_shot[ AS_SQU ][2] = sprsheet_get_id_by_name(invaders_sprites_lib, "SquiglyShot-3");
    spr_alien_shot[ AS_SQU ][3] = sprsheet_get_id_by_name(invaders_sprites_lib, "SquiglyShot-4");
    spr_shield                  = sprsheet_get_id_by_name(invaders_sprites_lib, "ShieldImage");

    // 55 sprites
    for ( int n = 0; n < 11; n++ )
        spr_aliens[0*11+n] = spr_aliensA[0];
        spr_aliens[1*11+n] = spr_aliensA[0];
        spr_aliens[2*11+n] = spr_aliensB[0];
        spr_aliens[3*11+n] = spr_aliensB[0];
        spr_aliens[4*11+n] = spr_aliensC[0];
    end

    return 0;
end

/* ********************************************* */

function PutSprite(double x, double y, int sprite, int anim = 0)
begin
    sprsheet_set_sprite( invaders_sprites_lib, sprite + anim );
    drawSprite();
end

/* ********************************************* */

function PutSpriteMerge(double x, double y, int sprite, int anim = 0)
begin
    sprsheet_set_sprite( invaders_sprites_lib, sprite + anim );
    drawSpriteMerge();
end

/* ********************************************* */

function DeleteSprite(double x, double y, int sprite, int anim = 0)
begin
    sprsheet_set_sprite( invaders_sprites_lib, sprite + anim );
    map_block_copy(0, main_game_layer, x, y, 0, graph, clip.x, clip.y, clip.w, clip.h, 0, 255, 255, 255, 255, BLEND_ALPHA_MASK );
end

/* ********************************************* */

#endif
