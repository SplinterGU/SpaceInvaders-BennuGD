#ifndef __SCREENS_H
#define __SCREENS_H

#include "common.inc"
#include "utils.inc"
#include "input.inc"
#include "sprites.inc"

type scrtextA
    int x, y;
    string text;
end

/* ********************************************* */

scrtextA helpScreenTxt[] =
       9,    6,        "* HELP *"                   ,
       5,   10,    "START  FOR CREDITS"             ,
       8,   12,       "L1  FOR 1 PLAYER"            ,
       8,   14,       "R1  FOR 2 PLAYERS"           ,
       2,   16, "BUTTON Y  FOR CONFIGURE"           ,
       2,   18, "BUTTON X  FOR INSTRUCTIONS"        ,
       2,   20, "BUTTON B  FOR THIS HELP"
;

scrtextA helpScreenTxt2[] = 
       9,   24,        "VERSION BY"                 ,
       1,   26, "JUAN JOSE PONTEPRINO V1.7"
;

scrtextA scoreScreenTxt[] =
      10,   17, "=? MYSTERY"                        ,
      10,   19, "=30 POINTS"                        ,
      10,   21, "=20 POINTS"                        ,
      10,   23, "=10 POINTS"
;

scrtextA insertCoinScreenTxt[] = 
       6,   21, "*1 PLAYER  1 COIN "                ,
       6,   24, "*2 PLAYERS 2 COINS"
;

scrtextA setupScreenTxt[] =
       3,    6, "* CONFIGURE OPTIONS *"             ,
       1,   10, "LIVES"                             ,
       1,   12, "BONUS LIFE"                        ,
       1,   14, "DISPLAY COINAGE"                   ,
      13,   20, "OK"
;

scrtextA instructionsScreenTxt[] =
       4,    6, "SPACE INVADERS (TM)"               ,
       0,    8, "* INSERT COIN(S)"                  ,
       0,   10, "* SELECT 1 OR 2 PLAYERS."          ,
       0,   12, "* 2 PLAYERS ALTERNATE PLAY."       ,
       0,   14, "* USE CONTROL STICKS"              ,
       0,   15, "  TO POSITION LASER BASE."         ,
       0,   17, "* GAME ENDS WHEN PLAYERS"          ,
       0,   18, "  LASER BASES HAVE BEEN"           ,
       0,   19, "  ELIMINATED BY INVADERS"          ,
       0,   20, "  MISSILE OR WHEN THE"             ,
       0,   21, "  INVADERS OVERRUN THE BASE."      ,
       0,   23, "* 1 BONUS LASER BASE"              ,
       0,   26, "* 1 PLAY 1 COIN PER PLAYER."
;

/* ********************************************* */

int currentMenuScreen = 0;

/* ********************************************* */

function int PrintAtArr( scrtextA * st, int n )
begin
    while ( n-- )
        PrintAt( st.x, st.y, st.text );
        st++;
    end
    return 0;
end

/* ********************************************* */

function int PrintAtDelayArr( scrtextA * st, int n, int delay )
begin
    int ret = 0;
    while ( n-- && !ret )
        ret = PrintAtDelay( st.x, st.y, st.text, delay );
        st++;
    end
    return ret;
end

/* ********************************************* */

function ClearMenu()
begin
    map_clear( 0, main_game_layer, 0, 4 * 8, 224, 30 * 8 - 1 );
end

/* ********************************************* */

function DrawCredits()
begin
    PrintAt(     17, 30, "CREDIT " );
    PrintNumAt(  24, 30, numCredits, 2 | NUM_LEFTZERO );
end

/* ********************************************* */

function DrawScoreHeaderAndCredits()
begin
    PrintAt(          0,   1, " SCORE<1> HI-SCORE SCORE<2> " );

    PrintNumAt(       3,   3, player[ 0 ].score, 4 | NUM_LEFTZERO | NUM_UNSIGNED );
    PrintNumAt(      11,   3, hiScore          , 4 | NUM_LEFTZERO | NUM_UNSIGNED );

    if ( playersInGame != 1 )
        PrintNumAt(      21,   3, player[ 1 ].score, 4 | NUM_LEFTZERO | NUM_UNSIGNED );
    else
        PrintAt(         21,   3, "    " );
    end

    DrawCredits();
end

/* ********************************************* */

function int animateSpriteX( double y, double from_x, double to_x, double deltax, int sprite )
begin
    double ox = -1;
    int anim = 0;
    for ( x = from_x; ( deltax < 0 ) ? x >= to_x : x <= to_x; ox = x, x += deltax )
        if ( ox != -1 ) DeleteSprite( ox, y, sprite, anim ); end
        anim ^= 1;
        PutSpriteMerge( x, y, sprite, anim );
        if ( DelayFrames( 2 ) ) return 1; end
    end
    return 0;
end

/* ********************************************* */

function int animateAlienShot( double x, double from_y, double to_y, int sprite )
begin
    double oy = -1;
    int anim = 0;
    for ( y = from_y; y <= to_y; oy = y, y++ )
        if ( oy != -1 ) DeleteSprite( x, oy, sprite, anim % 4 ); end
        anim++;
        PutSpriteMerge( x, y, sprite, anim % 4 );
        if ( DelayFrames( 1 ) ) return 1; end
    end
    return 0;
end

/* ********************************************* */

function int scoreTable_Screen( int invertY )
begin
    ClearMenu();

    currentMenuScreen = 1;

    DrawScoreHeaderAndCredits();

    int ret = DelayFrames( 60 ); // 1 seconds

    if ( !ret ) ret = PrintAtDelay( 12,  8, invertY ? "PLAy" : "PLAY", 5 ); end

    if ( !ret ) ret = PrintAtDelay(  7, 11, "SPACE  INVADERS"        , 5 ); end

    if ( !ret ) ret = DelayFrames( 60 ); end // 1 seconds

    if ( !ret )
        PrintAt(   4,  15, "*SCORE ADVANCE TABLE*" );

        PutSprite(60, 136, spr_ufo);
        PutSprite(64, 152, spr_aliensC[0]);
        PutSprite(64, 168, spr_aliensB[0]);
        PutSprite(64, 184, spr_aliensA[0]);
    end
    
    if ( !ret ) ret = PrintAtDelayArr( &scoreScreenTxt, sizeof( scoreScreenTxt ) / sizeof( scoreScreenTxt[0] ), 5 ); end

    if ( ret ) ClearMenu(); end

    return ret;

end

/* ********************************************* */

function animateAlienCY()
begin
    int ret;
    
    ret = scoreTable_Screen( 1 );
    if ( !ret ) ret = DelayFrames( 60 ); end // 1 seconds
    if ( !ret ) ret = animateSpriteX( 64,          224,          122, -2, spr_aliensC[0] ); end
    if ( !ret ) PrintAt( 15,  8, "  " ); end
    if ( !ret ) ret = animateSpriteX( 64,          120,          222,  2, spr_alienCYInv[0] ); end
    if ( !ret ) ret = DelayFrames( 60 ); end // 1 seconds
    if ( !ret ) PrintAt( 27,  8, " " ); end
    if ( !ret ) ret = animateSpriteX( 64,          224,          120, -2, spr_alienCY[0] ); end
    if ( !ret ) ret = DelayFrames( 60 ); end // 1 seconds
    if ( !ret ) PrintAt( 15,  8, "Y " ); end

    if ( ret ) ClearMenu(); end

    return ret;
end

/* ********************************************* */

function inserCoin_Screen( int extraC )
begin
    int ret;

    ClearMenu();

    currentMenuScreen = 2;

    PrintAt(        8, 14, "INSERT  COIN" );

    if ( extraC )
        PrintAt(       15, 14, "C" );
    else
    end

    ret = PrintAtDelay(  6, 18, "<1 OR 2 PLAYERS>  " , 0 );

    if ( displayCoinage )
        if ( !ret ) ret = PrintAtDelayArr( &insertCoinScreenTxt, sizeof( insertCoinScreenTxt ) / sizeof( insertCoinScreenTxt[0] ), 5 ); end
    end

    if ( ret ) ClearMenu(); end

    return ret;
end

/* ********************************************* */

function inserCoin_ScreenC()
begin
    int ret;

    ret = inserCoin_Screen( 1 );

    if ( !ret ) ret = DelayFrames( 60 ); end // 1 seconds
    if ( !ret ) ret = animateSpriteX( 40, 2, 116, 2, spr_aliensC[0] ); end
    if ( !ret ) ret = animateAlienShot( 122,  48, 112, spr_alien_shot[ AS_SQU ][0] ); end

    if ( !ret )
        PutSpriteMerge( 120, 112, spr_alien_shot_explosion);
        if ( DelayFrames( 5 ) ) return 1; end
        PrintAt(       15,  14, " " );
    end

    if ( ret ) ClearMenu(); end

    return ret;
end

/* ********************************************* */

function pushPlayerButton_Screen()
begin
    ClearMenu();

    currentMenuScreen = 100;

    PrintAt( 12, 12, "PUSH " );
    PrintAt(  4, 15, ( numCredits == 1 ) ? "ONLY 1PLAYER  BUTTON" : "1 OR 2PLAYERS BUTTON" );

end

/* ********************************************* */

function setup_Screen()
begin
    int cursorPos = 0, flg = 0, ukey = 0;
    double x1 = 0, x2 = 0;

    y = 0;

    ClearMenu();

    currentMenuScreen = 3;

    PrintAtArr( &setupScreenTxt, sizeof( setupScreenTxt ) / sizeof( setupScreenTxt[0] ) );

    while ( 1 )
        flg = 0;

        if ( !ukey )
            switch ( cursorPos )
                case 0:
                    x1 = 24; x2 = 26; y = 10;

                    if ( input & moveLeft && numLives > 3 ) numLives--; end
                    if ( input & moveRight && numLives < 6 ) numLives++; end

                    if ( numLives > 3 ) flg |= 1; end
                    if ( numLives < 6 ) flg |= 2; end
                end

                case 1:
                    x1 = 21; x2 = 26; y = 12;

                    if ( input & moveLeft ) bonusLife = 1000; end
                    if ( input & moveRight ) bonusLife = 1500; end

                    if ( bonusLife == 1500 ) flg = 1; else flg = 2; end
                end

                case 2:
                    x1 = 23; x2 = 26; y = 14;

                    if ( input & moveLeft ) displayCoinage = 0; end
                    if ( input & moveRight ) displayCoinage = 1; end

                    if ( displayCoinage ) flg = 1; else flg = 2; end
                end

                case 3:
                    x1 = 15; x2 = 12; y = 20;
                    if ( input & shot )
                        return 0;
                    end
                    flg = 3;
                end
            end

            PrintAt( x1, y, " " );
            PrintAt( x2, y, " " );

            if ( input & ( moveUp | moveDown ) )
                cursorPos = wrap( cursorPos += ( input & moveUp ) ? -1 : 1, 0, 3 );
            end

            PrintNumAt( 25,  10, numLives,  1 );
            PrintNumAt( 22,  12, bonusLife, 4 );
               PrintAt( 23,  14, displayCoinage ? " ON" : "OFF" );

            if ( !( input & ( moveUp | moveDown ) ) )
                if ( flg & 1 ) PrintAt( x1, y, "<" ); end
                if ( flg & 2 ) PrintAt( x2, y, ">" ); end
            end

        end

        ukey = input & ( moveLeft | moveRight | moveUp | moveDown );

        frame;

    end

    return 0;
end

/* ********************************************* */

function int help_Screen( int delay = 300 ) // 5 seconds
begin
    ClearMenu();

    currentMenuScreen = 4;

    PrintAtArr( &helpScreenTxt, sizeof( helpScreenTxt ) / sizeof( helpScreenTxt[0] ) );
    PrintAtArr( &helpScreenTxt2, sizeof( helpScreenTxt2 ) / sizeof( helpScreenTxt2[0] ) );

    DelayFrames( delay );

    return 0;
end

/* ********************************************* */

function int instructions_Screen()
begin
    ClearMenu();

    currentMenuScreen = 5;

    PrintAtArr( &instructionsScreenTxt, sizeof( instructionsScreenTxt ) / sizeof( instructionsScreenTxt[0] ) );
    PrintAt( 0, 24, "  AWARDED AT " + bonusLife + " POINTS." );
    PrintAt( 0, 28, "* " + numLives + " LASER BASE PER PLAYER." );

    DelayFrames( 600 ); // 10 seconds

    return 0;
end

/* ********************************************* */

#endif
