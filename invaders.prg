import "mod_gfx";
import "mod_input";
import "mod_misc";
import "mod_sound";

int main_game_layer = 0;

#include "utils.inc"
#include "sprites.inc"
#include "common.inc"
#include "input.inc"
#include "screens.inc"
#include "fontsheet.inc"
#include "screens.inc"
#include "round.inc"
#include "sound.inc"

string extra_resources[] = "res/fonts/invaders.font", "res/sprites/sprites.spr";

int scrw = 0;
int scrh = 0;

double global_size = 100;

process overlay()
begin
    priority = -10000;
    z = -10000;

    int overlay_map = map_new(224,256);

    map_clear( 0, overlay_map, 0 );
    map_clear( 0, overlay_map, 0, 4*8, 256-1, 8*8 - 1, rgba( 255, 102, 0, 255 ) );
    map_clear( 0, overlay_map, 0, 23*8, 256-1, 30*8 - 1, rgba( 0, 255, 0, 255 ) );
    map_clear( 0, overlay_map, 3*8, 30*8, 17*8-1, 32*8 - 1, rgba( 0, 255, 0, 255 ) );

    while(1)
        map_put( 0, main_game_layer, 0, overlay_map, 112, 128, 0, 100, 100, 0, 255, 255, 255, 255, BLEND_NORMAL_KEEP_ALPHA );
        frame;
    end
end

process bezel()
begin
    z = -15000;
    graph = map_load("res/images/bezel.png");

//    center.x = 0; 
//    center.y = 0; 

    size = global_size;

    x = SCRW / 2;
    y = SCRH / 2;

    while(1)
        frame;
    end
end

process dark_layer()
begin
    z = 10000;

    graph = map_new(SCRW,SCRH);

    map_clear( 0, graph, rgba( 0,0,0,128 ) );

    x = SCRW / 2;
    y = SCRH / 2;

    while(1)
        frame;
    end
end

process crt_light()
begin
    z = -2000;

    graph = map_new(224,256);

    map_clear( 0, graph, rgba( 255, 255, 255, 8 ) );

    size = remap( 200, 0, 768, 0, SCRH );

    x = SCRW / 2.0;
    y = SCRH / 2.0 + remap( 54, 0, 768, 0, SCRH );

    while(1)
        frame;
    end
end

process int main()
begin
    z = -1000;

    screen.fullscreen = 1;

    set_mode( 0, 0 );

    desktop_get_size( &scrw, &scrh );

    global_size = SCRH * 100 / 1080;

    set_fps( 60,0 );

    texture_set_quality(Q_BEST);

    background.graph = map_load("res/images/bennugd.png");
    background.size = SCRH * 100 / 513;

    fade_off( 0 ); frame; fade_on( 3000 ); while(fade_info.fading) frame; end
    DelayFrames( 180 );
    fade_off( 3000 ); while(fade_info.fading) frame; end

    map_unload( 0, background.graph );

    background.graph = map_load("res/images/splash.png");
    background.size = SCRH * 100 / 800;

    fade_off( 0 ); frame; fade_on( 3000 ); while(fade_info.fading) frame; end
    DelayFrames( 180 );
    fade_off( 3000 ); while(fade_info.fading) frame; end

   	/* Configuración de diseño de fuente */
    
    font = fontsheet_load("res/fonts/invaders_8x8.png");

	loadSprites();
    loadSounds();

    map_unload( 0, background.graph );

    background.graph = map_load("res/images/background.png");
    background.size = global_size;

    center_set( 0, background.graph, 1365 / 2, 837 / 2 - 130 );

    main_game_layer = map_new(224,256);

    graph = main_game_layer;

    size = remap( 200, 0, 768, 0, SCRH );

    x = SCRW / 2.0;
    y = SCRH / 2.0 + remap( 54, 0, 768, 0, SCRH );

    blendmode = BLEND_DISABLED;

    alpha = 200;

    overlay();
    bezel();
    dark_layer();
    crt_light();

    help_Screen(0);
    fade_on( 3000 ); while(fade_info.fading) frame; end
    DelayFrames( 180 );

    InputManager();

    numCredits = 0;

    int menu = 0;

    while( 1 )
        resetPlayer( 1, 1 );
        resetPlayer( 0, 1 );

        int ret = 0;

        if ( !numCredits )
            repeat
    		    playersInGame = 0;

                _ExitIfKeyPressed = 1;

                switch ( menu )
                    case 0:
                        if ( !( ret = scoreTable_Screen( 0 ) ) )
                            menu++;
                        end
                    end

                    case 1,4:
    				    ClearMenu();
                        if ( !( ret = playGame( 1 ) ) )
                            menu++;
                        end
                    end

                    case 2:
                        if ( !( ret = inserCoin_Screen( 0 ) ) )
                            menu++;
                        end
                    end

                    case 3:
                        if ( !( ret = animateAlienCY() ) )
                            menu++;
                        end
                    end

                    case 5:
                        if ( !( ret = inserCoin_ScreenC() ) )
                            menu = 0;
                        end
                    end

                end
                
                if ( !ret )
                    ret = DelayFrames( 120 ); // 2 seconds
                end

                _ExitIfKeyPressed = 0;
                if ( syskey & showSetup         ) ret = setup_Screen(); end
                if ( syskey & showHelp          ) ret = help_Screen(); end
                if ( syskey & showInstructions  ) ret = instructions_Screen(); end
                frame;

            until ( ret || numCredits );
        end

        playersInGame = 0;
        syskey = 0;
        pushPlayerButton_Screen();
        repeat
            if ( syskey & showSetup         ) ret = setup_Screen();         syskey |= keyCredit; end
            if ( syskey & showHelp          ) ret = help_Screen();          syskey |= keyCredit; end
            if ( syskey & showInstructions  ) ret = instructions_Screen();  syskey |= keyCredit; end
            if ( syskey & keyCredit         ) pushPlayerButton_Screen(); end
            frame;
        until ( ( syskey & ( keyStart1UP | keyStart2UP ) ) );

        playersInGame = ( syskey & keyStart2UP ) ? 2 : 1;

        numCredits -= playersInGame;

        currentMenuScreen = 0;

        ret = playGame( 0 );

        playersInGame = 0;
        syskey = 0;

        if ( menu < 2 )
            menu = 2;
        else
            menu = 5;
        end

    end

    return 0;
end
