#ifndef __INPUT_H
#define __INPUT_H

#include "common.inc"

/* ********************************************* */
/* Input */

#define moveLeft            0x0001
#define moveRight           0x0002
#define moveUp              0x0004
#define moveDown            0x0008
#define shot                0x0010

int input;

/* ********************************************* */

#define keyCredit           0x0001
#define keyStart1UP         0x0002
#define keyStart2UP         0x0004
#define showSetup           0x0008
#define showHelp            0x0010
#define showInstructions    0x1020

int syskey;

/* ********************************************* */

/* Input */

int keyCreditEnable = 0;

/* ********************************************* */

process InputManager()
begin
    priority = 1000;

    while( 1 )
        syskey = 0;

        if ( !demo_mode ) input = 0; end

        if ( !demo_mode && playerPtr )
            int joy = currPlayer;
            if ( os_id == OS_SWITCH || !joy_query( currPlayer, JOY_QUERY_ATTACHED ) )
                joy = joy_query( joy_query_first_attached );
            end

            if ( joy != -1 )
                input |= ( joy_query( joy, JOY_BUTTON_DPAD_LEFT ) || joy_query( joy, JOY_AXIS_LEFTX ) < -16384 ) ? moveLeft : 0;
                input |= ( joy_query( joy, JOY_BUTTON_DPAD_RIGHT ) || joy_query( joy, JOY_AXIS_LEFTX ) > 16383 ) ? moveRight : 0;
                input |= ( joy_query( joy, JOY_BUTTON_DPAD_UP ) || joy_query( joy, JOY_AXIS_LEFTY ) < -16384 ) ? moveUp : 0;
                input |= ( joy_query( joy, JOY_BUTTON_DPAD_DOWN ) || joy_query( joy, JOY_AXIS_LEFTY ) > 16383 ) ? moveDown : 0;
                input |=   joy_query( joy, JOY_BUTTON_A ) ? shot : 0;
            end

            input |= key( _LEFT ) ? moveLeft : 0;
            input |= key( _RIGHT ) ? moveRight : 0;
            input |= key( _UP ) ? moveUp : 0;
            input |= key( _DOWN ) ? moveDown : 0;
            input |= ( keyboard.shift_status & STAT_SHIFT || keyboard.shift_status & STAT_CTRL ) ? shot : 0;
        end

        int startP1 = key( _1 ) || joy_query( 0, JOY_BUTTON_LEFTSHOULDER  ) || joy_query( 1, JOY_BUTTON_LEFTSHOULDER  );
        int startP2 = key( _2 ) || joy_query( 0, JOY_BUTTON_RIGHTSHOULDER ) || joy_query( 1, JOY_BUTTON_RIGHTSHOULDER );

        if ( startP1 || startP2 || joy_query( 0, JOY_BUTTON_START ) || joy_query( 1, JOY_BUTTON_START ) || key( _0 ) ) // Key Credits (+/k)
            int coins = 0;

            if ( demo_mode || !playersInGame )
                if ( ( startP2 || joy_query( 1, JOY_BUTTON_START ) ) && numCredits < 2 )
                    coins = 2 - numCredits;
                elif ( startP1 && numCredits < 1 )
                    coins = 1;
                elif ( !startP1 && !startP2 )
                    coins = 1;
                end
            else
                coins = 1;
            end

            if ( keyCreditEnable )
                keyCreditEnable = 0;
                syskey |= keyCredit;
                if ( numCredits < 99 )
                    numCredits += coins;
                    DrawCredits();
                end
            end
        else
            keyCreditEnable = 1;
        end

        if ( numCredits > 0 )
            if ( startP1 ) // One Player (1)
                syskey |= keyStart1UP;
            end

            if ( numCredits > 1 && startP2 ) // Two Player (2)
                syskey |= keyStart2UP;
            end
        end

        if ( joy_query( 0, JOY_BUTTON_Y ) || joy_query( 1, JOY_BUTTON_Y ) || key( _C ) || key( _F9 ) ) syskey |= showSetup; end
        if ( joy_query( 0, JOY_BUTTON_B ) || joy_query( 1, JOY_BUTTON_B ) || key_down( _H ) || key_down( _F1 ) ) syskey |= showHelp; end
        if ( joy_query( 0, JOY_BUTTON_X ) || joy_query( 1, JOY_BUTTON_X ) || key( _I ) ) syskey |= showInstructions; end

        if ( key( _ESC ) ) let_me_alone(); return; end

        frame;
    end
end

#endif
