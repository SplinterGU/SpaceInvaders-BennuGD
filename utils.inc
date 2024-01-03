#ifndef __UTILS_H
#define __UTILS_H

// PrintNumAt Flags
// bits 0-3 digits
// bit 6 fill leftzero
// bit 7 unsigned

#define NUM_UNSIGNED 0x80
#define NUM_LEFTZERO 0x60

#include "common.inc"
#include "input.inc"

/* ********************************************* */

int _ExitIfKeyPressed = 0;

/* ********************************************* */

function int DelayFrames( int frames )
begin
    while ( frames-- )
        frame;
        if ( _ExitIfKeyPressed && syskey ) return 1; end
    end
    return 0;
end

/* ********************************************* */

function PrintNumAt( double x, double y, int num, int flags )
begin
    x *= 8;
    y *= 8;
    text.flags = B_NOCOLORKEY;
    write_in_map( 0, main_game_layer, font, x, y, substr( ( ( flags & NUM_LEFTZERO ) ? "000000" : "      " ) + abs( num ), - ( flags & 7 ) ), ALIGN_TOP_LEFT );
end

/* ********************************************* */

function int PrintAt( double x, double y, string msg )
begin
    x *= 8;
    y *= 8;
    text.flags = B_NOCOLORKEY;
    write_in_map( 0, main_game_layer, font, x, y, msg, ALIGN_TOP_LEFT );
end

/* ********************************************* */

function int PrintAtDelay( double x, double y, string msg, int delay )
begin
    x *= 8;
    y *= 8;
    text.flags = B_NOCOLORKEY;
    for ( int i = 0; i < strlen( msg ); i++ )
        text.flags = B_NOCOLORKEY;
        write_in_map( 0, main_game_layer, font, x, y, substr( msg, 0, i + 1 ), ALIGN_TOP_LEFT );
        if ( DelayFrames( delay ) ) return 1; end
    end
    return 0;
end

/* ********************************************* */

#endif
