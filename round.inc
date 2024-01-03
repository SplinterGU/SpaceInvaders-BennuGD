#ifndef __ROUND_H
#define __ROUND_H

#include "common.inc"
#include "utils.inc"
#include "sprites.inc"
#include "tables.inc"
#include "input.inc"
#include "sound.inc"

/* ********************************************* */

type _alienShotInfo
    int active;
    int aniFrame;
    int explodingCnt;
    double x, y;
end

/* ********************************************* */
 
/* ********************************************* */
/* Global Vars                                   */
/* ********************************************* */

/* Player */

double playerX, shotYOld;
int playerExplodingCnt = 0;
int playerFrames = 0; // Must be signed!!!

/* Player Shot */

double shotX, shotY; // shotX don't need be initialized
int shotExplodingCnt = 0;
int shotFrames = 0;

/* Aliens */

int alienAnimationStatus = 0;
int alienExplodingCnt = 0;
double alienExplodingX = 0, alienExplodingY = 0;
double tDeltaX;
int aliensFrames = 0;

/* Alien Shot */

int alienShotTimer = 0;
int alienShotFrameCnt = 000000001b;
int reloadCnt = 0;
//int aShotFrame = 0;

_alienShotInfo * asRol, * asPlu, * asSqu;
_alienShotInfo alienShotInfo[2];

/* Saucer */

int saucerStart = 0;
int saucerActive = 0;
int saucerExplodingCnt = 0;
double saucerX = 0;
double saucerDeltaX = 0;
int saucerFrames = 0;

/* Collision */

int aCol = 0, aRow = 0;

/* Demo */

int demoFrame = DEMOSPEED;
int demoCurCommand = 0;

/* Others */

int gameState = 0;

int alienSoundCnt = 0;

int anybodyExploding = 0;

struct state1
    int killed;
    int shotReady;

    int playerIsAlive;
    int waitForEventsComplete;
    int downFleet;
end

/* ********************************************* */

function ClearGameArea()
begin
    map_clear( 0, main_game_layer, 0, 4 * 8, 224, 30 * 8 - 1 );
end

/* ********************************************* */
// playerPtr.alienIdx is the current idx

function getAliensDeltaPos( double * dX, double * dY, int * row )
begin
    *row = playerPtr.alienIdx / 11; // / 11;
    *dX = ( playerPtr.alienIdx - ( *row * 11 ) ) * 16;
    *dY = -( ( 8 + FLEETDELTAY ) * *row );
end

/* ********************************************* */
// playerPtr.alienIdx is the next idx

function getAliensDeltaPos2( int idx, double * dX, double * dY )
begin
    double _dx = 0, _dy = 0;
    if ( idx >= playerPtr.alienIdx )
        if ( state1.downFleet )
            _dy = FLEETDELTAY;
        else
            _dx = playerPtr.aliensDeltaX < 0 ? -2 : playerPtr.numAliens > 1 ? 2 : 3;
        end
    end

    double r = idx / 11;
    *dX = ( idx - r * 11 ) * 16 - _dx;
    *dY = -( ( 8 + FLEETDELTAY ) * r + _dy );
end

/* ********************************************* */

function int getAlienColumn( double x )
begin
    int delta = x - playerPtr.aliensX;
    if ( delta < 0 || delta > 175 ) return -1; end
    delta /= 16; // divide by 16
    return delta;
end

/* ********************************************* */
// playerPtr.alienIdx is the next idx

function int getAlienIdx( double x, double y, int skipAdjust )
begin
    int idx = -1;

    int c = x - playerPtr.aliensX;
    if ( c < 0 || c > 175 ) return -1; end
    c /= 16; // divide by 16

    int r = playerPtr.aliensY - y;
    if ( r < 0 || r > ( 4 * ( 8 + FLEETDELTAY ) + 7 ) ) return -1; end
    r /= 8 + FLEETDELTAY;

    idx = r * 11 + c;

    if ( !skipAdjust && idx >= playerPtr.alienIdx )
        int _dx = 0, _dy = 0;
        if ( state1.downFleet )
            _dy = FLEETDELTAY;
        else
            _dx = playerPtr.aliensDeltaX < 0 ? -2 : playerPtr.numAliens > 1 ? 2 : 3;
        end
        return getAlienIdx( x + _dx, y + _dy, 1 );
    end

    aCol = c;
    aRow = r;

    return idx;
end

/* ********************************************* */

function int getLowestAlien( int col )
begin
    if ( col < 0 || col > 10 ) return -1; end

    int i;
    for ( i = 0; i < 55; i+=11 )
        if ( playerPtr.aliens[ col + i ] ) return col + i; end
    end
    return -1;
end

/* ********************************************* */

function int getReloadRate()
begin
    int n;
    if ( !playerPtr.score ) return ShotReloadRate[ 0 ]; end
    for ( n = 0; n < sizeof( AReloadScoreTab ) / sizeof ( AReloadScoreTab[ 0 ] ) && playerPtr.score > AReloadScoreTab[ n ]; n++ ); end
    return ShotReloadRate[ n - 1 ];
end

/* ********************************************* */

function resetRoundVars()
begin
    /* Aliens */

    alienAnimationStatus = 0;

    alienExplodingCnt = 0;

    /* Saucer */

    saucerStart = 0;
    saucerActive = 0;

    saucerExplodingCnt = 0;

    /* Player */

    playerX = 8; // 14;
    shotY = -1;
    state1.shotReady = 1;
    shotExplodingCnt = 0;

    playerExplodingCnt = 0;

    /* Alien Shot */

    alienShotTimer = 0;

    reloadCnt = 0;
//    aShotFrame = 0;

    /* General for round */

    saucerFrames = 0;
    aliensFrames = 0;
    playerFrames = -100;
    shotFrames = 0;

    state1.playerIsAlive = 1;
    state1.waitForEventsComplete = 0;
    state1.downFleet = 0;

    anybodyExploding = 0;

    for ( int i = 0; i < 3; i++ )
        alienShotInfo[ i ].active = 0;
        alienShotInfo[ i ].aniFrame = 0;
        alienShotInfo[ i ].explodingCnt = 0;
    end

end

/* ********************************************* */

function resetPlayer( int p, int isFirst )
begin
    int n;

    playerPtr = &player[ p ];

    if ( isFirst )
        playerPtr.score = 0;

        /* Alien Shot */

        playerPtr.pluShotColIdx = 0;
        playerPtr.squShotColIdx = 6;

        playerPtr.numShips = numLives;
        playerPtr.round = 0;

        playerPtr.reloadRate = demo_mode ? 8 : getReloadRate();

    else
        playerPtr.round++;
    end

    /* Aliens */

    playerPtr.numAliens = 55;

    playerPtr.leftLimitCol = 0;
    playerPtr.rightLimitCol = 10;

    playerPtr.leftLimit = 0;
    playerPtr.rightLimit = 224 - ( ( playerPtr.rightLimitCol * 16 ) + 16 ) - 1;

    playerPtr.aliensColMask = 11111111111b;

    for ( n = 0; n < playerPtr.numAliens; n++ ) playerPtr.aliens[ n ] = 1; end

    playerPtr.fleetTopBase = ( playerPtr.round == 0 ? FLEETPOSSTART : AlienStartTable[ ( playerPtr.round - 1 ) & 7 ] );

    playerPtr.aliensX = 40;
    playerPtr.aliensY = playerPtr.fleetTopBase;
    playerPtr.aliensDeltaX = 2;
    
    playerPtr.alienIdx = 0;

    /* Saucer */

    playerPtr.tillSaucer = SAUCERTIMER; // Original Arcade 0x0600
    playerPtr.sauScore = 0;

    /* Alien Shot */

    playerPtr.alienShotMask = 000101101b;

    /* Player */

    playerPtr.shotsCounter = 0;

    /* Shield */

    playerPtr.currentShieldTopY = SHIELDTOPY;
    playerPtr.shieldBackupSaved = 0;
    if ( !playerPtr.shieldBackup ) playerPtr.shieldBackup = map_new( 224,16 ); end

    /* Alien Shot Info */

    asRol = &alienShotInfo[ AS_ROL ];
    asPlu = &alienShotInfo[ AS_PLU ];
    asSqu = &alienShotInfo[ AS_SQU ];

    for ( n = 0; n < 3; n++ )
        alienShotInfo[ n ].active = 0;
        alienShotInfo[ n ].aniFrame = 0;
        alienShotInfo[ n ].explodingCnt = 0;
    end

    if ( !demo_mode ) SoundStop( -1 ); end
end

/* ********************************************* */

function DrawFloor()
begin
    for ( int n = 0; n < 32; n++ )
        PrintAt( n, 29, "_" );
    end
end

/* ********************************************* */

function DrawShields()
begin
    if ( playerPtr.shieldBackupSaved )
        map_block_copy(0, main_game_layer, 0, SHIELDTOPY, playerPtr.shieldBackup, 0, 0, 224, 16, B_NOCOLORKEY );
    else
        for ( int n = 0; n < 4; n++ )
            PutSprite( 32 + 45 * n, SHIELDTOPY, spr_shield );
        end
    end
end

/* ********************************************* */

function DrawShips()
begin
    PrintNumAt( 1, 30, playerPtr.numShips, 1 | NUM_LEFTZERO );
    int l = ( ( playerPtr.numShips > 6 ) ? 6 : playerPtr.numShips ) - 1;
    sprsheet_set_sprite( invaders_sprites_lib, spr_player );
    int c = 24;
    for ( int n = 0; n < l; n++, c+=16 )
        drawSpriteXY( c, 240 );
    end
    deleteSprite16x8( c, 240 );
end

/* ********************************************* */

function scoreUpdate( int points )
begin
    // Extra Life
    if ( playerPtr.score < bonusLife && ( playerPtr.score + points ) >= bonusLife )
        SoundPlay( SOUND_EXTRA_LIFE );

        playerPtr.numShips++;
        DrawShips();
    end

    playerPtr.score += points;
    PrintNumAt( !currPlayer ? 3 : 21, 3, playerPtr.score, 4 | NUM_LEFTZERO );

    playerPtr.reloadRate = getReloadRate();

end

/* ********************************************* */

function hiScoreUpdate( int score )
begin
    if ( score > hiScore )
        hiScore = score;
        PrintNumAt( 11, 3, hiScore, 4 | NUM_LEFTZERO | NUM_UNSIGNED );
    end
end

/* ********************************************* */

function createAlienShotCol( _alienShotInfo * asi, int * colIdx, int colMin, int colMax )
begin
    if ( !asi.active && !asi.explodingCnt && !shotExplodingCnt )
        int col = ColFireTable[ *colIdx ];

        int aidx = getLowestAlien( col );
        if ( aidx != -1 )
            getAliensDeltaPos2( aidx, &asi.x, &asi.y );
            asi.x += playerPtr.aliensX + 8 - 1;
            asi.y += playerPtr.aliensY + 16;
            asi.active = 1;
            asi.aniFrame = 0;
            reloadCnt = 0;
        end
        ( *colIdx )++;
        if ( *colIdx > colMax ) *colIdx = colMin; end
    end
end

/* ********************************************* */

function handleAlienShot( int aid )
begin
    _alienShotInfo * asi = &alienShotInfo[ aid ];

    int hit = 0;

    DeleteSprite( asi.x, asi.y, spr_alien_shot[ aid ][ 0 ], asi.aniFrame );

    asi.y += 2;
    asi.aniFrame = wrap( ++asi.aniFrame, 0, 3 );

    if ( asi.y <= PLAYERY + 8 )
        if ( asi.y + 8 > PLAYERY && asi.x < playerX + 14 && asi.x + 2 > playerX + 2 )
            asi.active = 0;
            if ( state1.playerIsAlive )
                playerExplodingCnt = 60; // 0x100;
                state1.waitForEventsComplete = 1;
                state1.playerIsAlive = 0;
                if ( !demo_mode )
                    SoundStop( SOUND_PLAYER_SHOT );
                    SoundPlay( SOUND_PLAYER_EXPLOSION );
                end
            end
        else
            // Shield Collision?
            if ( asi.y + 8 > playerPtr.currentShieldTopY - 4 && asi.y < SHIELDTOPY + 16 )
                byte r,g,b,a;
                int color = map_get_pixel( 0, main_game_layer, asi.x    , asi.y + 6 ) |
                            map_get_pixel( 0, main_game_layer, asi.x + 1, asi.y + 6 ) |
                            map_get_pixel( 0, main_game_layer, asi.x + 2, asi.y + 6 );
                rgba_get(color, &r, &g, &b, &a);
                if ( a )
                    asi.y += 2;

                    PutSpriteMerge( asi.x - 2, asi.y, spr_alien_shot_explosion );

                    // hay que borrar el escudo tambien

                    asi.active = 0;
                    asi.explodingCnt = 8; // 60;
                    hit = 1;
                end
            end

            if ( !hit ) PutSpriteMerge( asi.x, asi.y, spr_alien_shot[ aid ][ 0 ], asi.aniFrame ); end

        end
    else
        if ( asi.y > FLOORY )
            // Floor collision
            asi.y = FLOORY;
            PutSpriteMerge( asi.x - 2, asi.y, spr_alien_shot_explosion );
            asi.active = 0;
            asi.explodingCnt = 8; //60;
        end
    end
end

/* ********************************************* */

#define deleteOldPlayerShot()   if ( shotYOld < PLAYERY ) DeleteSprite( shotX, shotYOld, spr_player_shot ); end
#define drawShotExploding(y)    PutSpriteMerge( shotX - 4, y, spr_player_shot_explosion )

/* ********************************************* */

function handleAShotCollision( int aid, int aShotYH )
begin
    _alienShotInfo * asi = &alienShotInfo[ aid ];
    if ( ( shotX - asi.x ) < 3 && ( shotX - asi.x ) >= 0 && shotY < asi.y + aShotYH && shotY + 4 > asi.y )
        DeleteSprite( asi.x, asi.y, spr_alien_shot[ aid ][ 0 ], asi.aniFrame );
        deleteOldPlayerShot();
        PutSpriteMerge( asi.x - 3, asi.y + aShotYH, spr_alien_shot_explosion );

        asi.x = asi.x - 1;
        asi.y = asi.y + aShotYH;

        asi.active = 0;
        asi.explodingCnt = 8; //60;

        shotY = -( shotY - 2 );
        drawShotExploding( -shotY );
        shotExplodingCnt = 8; //60;
        state1.killed = 1;
    end
end

/* ********************************************* */

function handleDeleteAShotExplodingCnt( int aid )
begin
    _alienShotInfo * asi = &alienShotInfo[ aid ];
    if ( asi.explodingCnt )
        asi.explodingCnt--;
        if ( !asi.explodingCnt )
            DeleteSprite( asi.x - 2, asi.y, spr_alien_shot_explosion );
        end
    end
end

/* ********************************************* */

function handleSauScore()
begin
    playerPtr.shotsCounter++;
    playerPtr.sauScore++;
    if ( playerPtr.sauScore == sizeof( SaucerScrTab ) / sizeof( SaucerScrTab[ 0 ] ) )
        playerPtr.sauScore = 0;
    end
end

/* ********************************************* */

function int iterGame()
begin
    int n;

    anybodyExploding = 0;

    if ( demo_mode )
        if ( syskey & ( keyCredit | keyStart1UP | keyStart2UP | showSetup | showHelp ) ) return 2; end // Key pressed on demo mode
        if ( playerFrames >= 0 )
            demoFrame++;
            if ( demoFrame >= DEMOSPEED )
                demoFrame = 0;
                int k = DemoCommands[ demoCurCommand ];
                input  = ( k & 2 ) ? moveLeft : 0;
                input |= ( k & 1 ) ? moveRight : 0;
                input |= shot;
                state1.shotReady = 1;
                demoCurCommand++;
                if ( demoCurCommand > sizeof( DemoCommands ) / sizeof( DemoCommands[ 0 ] ) )
                    demoCurCommand = 0;
                end
            end
        end
    end

    if ( playerPtr.aliensY != playerPtr.fleetTopBase )
        playerPtr.tillSaucer--;
        if ( playerPtr.tillSaucer <= 0 )
            saucerStart = 1;
            playerPtr.tillSaucer = SAUCERTIMER; // Original Arcade 0x0600
        end
    end

    // Clear Alien Explosion
    if ( alienExplodingCnt )
        alienExplodingCnt--;
        if ( !alienExplodingCnt )
            deleteSprite16x8( alienExplodingX, alienExplodingY );
            if ( !playerPtr.numAliens )
                resetPlayer( currPlayer, 0 );
                return 3; // Next round
            end                
            handleSauScore();
        else
            anybodyExploding = 1;            
        end
    end

    // Clear Shot Explosion
    if ( shotExplodingCnt )
        shotExplodingCnt--;
        if ( !shotExplodingCnt )
            DeleteSprite( shotX - 4, -shotY /*+ 2*/, spr_player_shot_explosion );
            if ( saucerActive == 1 && shotY == -16 )
                // Restore deleted UFO sprite
                PutSprite( saucerX, SAUCERROW, spr_alien_explosion );
            end
            handleSauScore();
            shotY = -1;
        else
            anybodyExploding = 1;
        end
    end

    // Saucer Explosion
    if ( saucerActive < 0 )
        saucerExplodingCnt--;
        if ( saucerExplodingCnt == 24 )
            anybodyExploding = 1;
            handleSauScore();
            if ( !demo_mode ) scoreUpdate( SaucerScrTab[ playerPtr.sauScore ] ); end
            PrintNumAt( saucerX / 8, SAUCERROW / 8, SaucerScrTab[ playerPtr.sauScore ], 3 );
        elif ( !saucerExplodingCnt )
            deleteSprite24x8( saucerX, SAUCERROW );
            saucerActive = 0;
        end
    end

    handleDeleteAShotExplodingCnt( AS_ROL );
    handleDeleteAShotExplodingCnt( AS_PLU );
    handleDeleteAShotExplodingCnt( AS_SQU );

    if ( state1.playerIsAlive && playerPtr.numAliens )
//        handleNextAlien();

        aliensFrames++;
        if ( aliensFrames >= ALIENSPEED )
            aliensFrames = 0;

            for ( ; playerPtr.alienIdx < 55 && !playerPtr.aliens[ playerPtr.alienIdx ]; playerPtr.alienIdx++ ) end

            if ( playerPtr.alienIdx == 55 )
                for ( playerPtr.alienIdx = 0; playerPtr.alienIdx < 55 && !playerPtr.aliens[ playerPtr.alienIdx ]; playerPtr.alienIdx++ ) end

                // Alien Sound
                if ( !demo_mode )
                    if ( !SoundIsPlaying( SOUND_ALIEN_STEP1 + ( alienSoundCnt & 0x03 ) ) )
//                        SoundStop( SOUND_ALIEN_STEP1 + ( alienSoundCnt & 0x03 ) );
                        SoundPlay( SOUND_ALIEN_STEP1 + ( ( ++alienSoundCnt ) & 0x03 ) );
                    end
                end

                alienAnimationStatus ^= 1;            

                // Right to Left allways -2
                tDeltaX = playerPtr.aliensDeltaX < 0 ? -2 : playerPtr.numAliens > 1 ? 2 : 3;
                playerPtr.aliensX += tDeltaX;

                state1.downFleet = 0;

                if ( playerPtr.aliensX < playerPtr.leftLimit || playerPtr.aliensX > playerPtr.rightLimit )
                    state1.downFleet = 0x02;

                    playerPtr.aliensX -= tDeltaX;
                    playerPtr.aliensDeltaX = -playerPtr.aliensDeltaX;

                    playerPtr.aliensY += FLEETDELTAY;
                end
            end

            double dX, dY;
            int row;
            getAliensDeltaPos( &dX, &dY, &row );
            int posX = playerPtr.aliensX + dX, posY = playerPtr.aliensY + dY;
            if ( state1.downFleet )
                DeleteSprite16x8(   posX    , posY - FLEETDELTAY );

                if ( ( state1.downFleet & 0x02 ) && posY + 8 > playerPtr.currentShieldTopY )
                    if ( posY - SHIELDTOPY < 16 )
                        ClearRows( posY + 8 - FLEETDELTAY, FLEETDELTAY );
                    end
                    playerPtr.currentShieldTopY = posY + 8;
                    state1.downFleet = 0x01;
                end

                // Aliens reached the Player
                if ( posY >= PLAYERY ) 
                    playerExplodingCnt = 60; //0x100;
                    state1.waitForEventsComplete = 1;
                    state1.playerIsAlive = 0;

                    if ( !demo_mode )
                        SoundStop( SOUND_PLAYER_SHOT );
                        SoundPlay( SOUND_PLAYER_EXPLOSION );
                    end

                    if ( playerPtr.numShips ) playerPtr.numShips = 1; end
                end
            end
            PutSprite( posX, posY, spr_aliens[ row * 11 + ( playerPtr.alienIdx % 11 ) ], alienAnimationStatus );
            playerPtr.alienIdx++;
        end

        /* Player */

        playerFrames++;
        if ( playerFrames >= PLAYERSPEED )
            playerFrames = 0;

            if ( input & moveLeft  ) playerX--; end
            if ( input & moveRight ) playerX++; end

            playerX = clamp( playerX, 8, 201 );

            PutSprite( playerX, PLAYERY, spr_player );
        end

        if ( saucerStart && !asSqu.active && playerPtr.numAliens >= 8 )
            saucerActive = 1;
            if ( playerPtr.shotsCounter & 1 )
                saucerDeltaX = 2;
                saucerX = 17;
            else
                saucerDeltaX = -2;
                saucerX = 224;
            end
            saucerFrames = SAUCERSPEED;
            saucerStart = 0;
        end
    end

    if ( playerExplodingCnt )
        playerExplodingCnt--;
        if ( playerExplodingCnt )
            anybodyExploding = 1;
            if ( !( playerExplodingCnt & 0x03 ) )
                PutSprite( playerX, PLAYERY, spr_player_explosion[ 0 ], ( playerExplodingCnt & 0x04 ) ? 1 : 0 );
            end
        else
            deleteSprite16x8( playerX, PLAYERY );
            playerPtr.numShips--;
            DrawShips();
        end
    end

    if ( state1.waitForEventsComplete                                        &&
         !anybodyExploding                                                   &&
         !( asRol.explodingCnt | asPlu.explodingCnt | asSqu.explodingCnt )   &&
         !( asRol.active | asPlu.active | asSqu.active | saucerActive )      &&
         shotY == -1 
    )
        if ( !demo_mode ) SoundStop( -1 ); end

        if ( demo_mode ) return 0; end // demo complete

        if ( !playerPtr.numShips ) return 4; end // game over

        return 6; // player death, but have ships availables
    end

    if ( saucerActive == 1 )
        if ( !demo_mode && !SoundIsPlaying( SOUND_UFO ) ) SoundPlay( SOUND_UFO, -1 ); end
        saucerFrames++;
        if ( saucerFrames >= SAUCERSPEED )
            saucerFrames = 0;
            saucerX += saucerDeltaX;
            if ( saucerX < 0 || saucerX > 224 )
                deleteSprite24x8( saucerX - saucerDeltaX, SAUCERROW );
                saucerActive = 0;
                if ( !demo_mode )
                    SoundStop( SOUND_UFO );
                end
            else
                PutSprite( saucerX, SAUCERROW, spr_ufo );
            end
        end
    end

    shotFrames++;
    if ( playerFrames >= 0 /*shotFrames >= SHOTSPEED */ )
        shotFrames = 0;

        if ( !demo_mode )
            if ( !( input & shot ) ) state1.shotReady = 1; end
        end

        if ( ( input & shot ) && state1.shotReady && shotY == -1 && state1.playerIsAlive && !anybodyExploding )
            if ( !demo_mode ) SoundPlay( SOUND_PLAYER_SHOT ); end

            shotY = PLAYERY;
            shotX = playerX + 8;
            state1.shotReady = 0;
        end

        if ( shotY > 0 )
            shotYOld = shotY;
            shotY -= 4;

            state1.killed = 0;

            // Shot collision with aliens shots

            if ( asRol.active ) handleAShotCollision( AS_ROL, 7 ); end
            if ( asPlu.active ) handleAShotCollision( AS_PLU, 6 ); end
            if ( asSqu.active ) handleAShotCollision( AS_SQU, 7 ); end

            // Shot collision with saucer
            if ( !state1.killed && saucerActive > 0 && 
                  shotY + 4 > SAUCERROW && shotY < SAUCERROW + 8 &&
                  shotX >= saucerX + 4 && shotX < saucerX + 20
               )
                deleteOldPlayerShot();
                PutSprite( saucerX, SAUCERROW, spr_ufo_explosion );
                saucerActive = -1;
                saucerExplodingCnt = 48; // old 32

                if ( !demo_mode )
                    SoundStop( SOUND_UFO );
                    SoundPlay( SOUND_UFO_EXPLOSION );
                end

                shotY = -1;
                state1.killed = 1;
            end

            // Shot collision with shield
            if ( !state1.killed && shotY + 4 >= playerPtr.currentShieldTopY && shotY < SHIELDTOPY + 16 )
                byte r,g,b,a;
                int color = map_get_pixel( 0, main_game_layer, shotX, shotY     ) | 
                            map_get_pixel( 0, main_game_layer, shotX, shotY + 1 ) | 
                            map_get_pixel( 0, main_game_layer, shotX, shotY + 2 ) | 
                            map_get_pixel( 0, main_game_layer, shotX, shotY + 3 );
                rgba_get(color, &r, &g, &b, &a);
                if ( a )
                    deleteOldPlayerShot();

                    shotY = -( shotY - 2 );
                    drawShotExploding( -shotY );
                    shotExplodingCnt = 6; //60;
                    state1.killed = 1;
                end
            end

            // Shot collision with aliens
            if ( !state1.killed )
                int idx = getAlienIdx( shotX, shotY /*+ 4 - 6*/, 0 ), lowest;
                if ( idx >= 0 && idx < 55 && playerPtr.aliens[ idx ] )
// Alien A = 12px width (2 + 12 + 2)
// Alien B = 11px width (3 left + 11 alien + 2 right)
// Alien C = 8px width  (4 + 8 + 4)
                    getAliensDeltaPos2( idx, &alienExplodingX, &alienExplodingY );
                    alienExplodingX += playerPtr.aliensX;
                    alienExplodingY += playerPtr.aliensY;

                    int hit = 0;

                    switch ( aRow )
                        case 0,1:
                            if ( shotX >= alienExplodingX + 2 && shotX < alienExplodingX + 14 ) hit = 1; end
                        end

                        case 2,3:
                            if ( shotX >= alienExplodingX + 3 && shotX < alienExplodingX + 14 ) hit = 1; end
                        end

                        case 4:
                            if ( shotX >= alienExplodingX + 4 && shotX < alienExplodingX + 12 ) hit = 1; end
                        end

                    end

                    if ( hit )
                        deleteOldPlayerShot();
                        PutSprite( alienExplodingX, alienExplodingY, spr_alien_explosion );

                        playerPtr.aliens[ idx ] = 0;
                        playerPtr.numAliens--;
                        alienExplodingCnt = 8;
                        shotY = -1;
                        state1.killed = 1;

                        if ( !demo_mode )
                            scoreUpdate( AlienScores[ aRow ] );

//                            SoundStop( SOUND_ALIEN_STEP1 );
//                            SoundStop( SOUND_ALIEN_STEP2 );
//                            SoundStop( SOUND_ALIEN_STEP3 );
//                            SoundStop( SOUND_ALIEN_STEP4 );
//
                            SoundPlay( SOUND_ALIEN_EXPLOSION );
                        end

                        if ( playerPtr.numAliens < 9 ) playerPtr.alienShotMask = 0111111b; end

                        if ( ( lowest = getLowestAlien( aCol ) ) == -1 ) playerPtr.aliensColMask &= ~( 0x0001 << aCol ); end

                        if ( lowest && ( aCol == playerPtr.leftLimitCol || aCol == playerPtr.rightLimitCol ) )
                            if ( getLowestAlien( aCol ) == -1 )
                                int ll = 10, lr = 0;
                                uint bitl = 0x0400, bitr = 0x0001;

                                playerPtr.aliensColMask &= ~( 0x0001 << aCol );

                                for ( n = 0; n < 11; n++, bitl >>= 1, bitr <<= 1 )
                                    if ( playerPtr.aliensColMask & bitl ) ll = ( 10 - n ); end
                                    if ( playerPtr.aliensColMask & bitr ) lr = n; end
                                end

                                playerPtr.leftLimitCol = ll;
                                playerPtr.rightLimitCol = lr;

                                playerPtr.leftLimit  = -( ll * 16 );
                                playerPtr.rightLimit = 224 - ( ( lr * 16 ) + 16 ) - 1;
                            end
                        end
                    end
                end
            end

            if ( !state1.killed )
                deleteOldPlayerShot();
                if ( shotY <= SAUCERROW )
                    drawShotExploding( SAUCERROW );
                    shotY = -( SAUCERROW /*- 2*/ );
                    shotExplodingCnt = 4; //40;
                    state1.killed = 1;
                else
                    PutSpriteMerge( shotX, shotY, spr_player_shot );
                end
            end
        end
    end

    /* Aliens Shots */

//    aShotFrame++;
//    if ( aShotFrame >= ALIENSHOTSPEED )
//        aShotFrame = 0;
    reloadCnt++;
    if ( state1.playerIsAlive && playerFrames >= 0 )
        if ( reloadCnt >= playerPtr.reloadRate )
            reloadCnt = 0;
            switch ( alienShotTimer )
                case    0:  // rolling-shot
                    if ( !asRol.active && !asRol.explodingCnt && !shotExplodingCnt )
                        // Track the player
                        int aidx = getLowestAlien( getAlienColumn( playerX + 8 ) );

                        if ( aidx != -1 )
                            getAliensDeltaPos2( aidx, &asRol.x, &asRol.y );
                            asRol.x += playerPtr.aliensX + 8 - 1;
                            asRol.y += playerPtr.aliensY + 16;
                            asRol.active = 1;
                            asRol.aniFrame = 0;
                        end
                    end
                    alienShotTimer++;
                 end

                case    1:  // plunger-shot
                    if ( playerPtr.numAliens > 1 ) createAlienShotCol( asPlu, &playerPtr.pluShotColIdx, 0, 15 ); end
                    alienShotTimer++;
                end

                case    2:  // squiggly-shot or saucer
                    if ( !saucerActive ) createAlienShotCol( asSqu, &playerPtr.squShotColIdx, 6, 20 ); end
                    alienShotTimer = 0;
                end
            end
        end
    end
    
    if ( alienShotFrameCnt & playerPtr.alienShotMask )
        if ( asRol.active ) handleAlienShot( AS_ROL ); end
        if ( asPlu.active ) handleAlienShot( AS_PLU ); end
        if ( asSqu.active ) handleAlienShot( AS_SQU ); end
    end
    if ( alienShotFrameCnt == 000100000b ) alienShotFrameCnt = 000000001b;
    else                                   alienShotFrameCnt <<= 1; end

    return 1; // frame complete, return for continue

end

/* ********************************************* */

function int playGame( int _demo )
begin
    int n;

    demo_mode = _demo;

    /* Reset Players */

    resetPlayer( 1, 1 );
    resetPlayer( 0, 1 );

    /* Round */

    currPlayer = 0;

    if ( demo_mode ) playersInGame = 1; end

start_round:
    gameState = 0;

    ClearGameArea();

    DrawShips();
    
    if ( !demo_mode )
        DrawScoreHeaderAndCredits();
        PrintAt(   7,  14, "PLAY PLAYER<" + ( !currPlayer ? "1" : "2" ) + ">" );

        for ( n = 0; n < 17; n++ )
            PrintAt( !currPlayer ? 3 : 21, 3, "      " );
            DelayFrames(3);

            PrintNumAt( !currPlayer ? 3 : 21, 3, playerPtr.score, 4 | NUM_LEFTZERO );
            DelayFrames(3);
        end
        ClearGameArea();
    end
    
    DrawShields();

    DrawFloor();

    resetRoundVars();

    gameState = 1;

    // Main Loop
    while ( 1 )
        gameState = iterGame();

        switch ( gameState )
            case    0: // demo complete
                DelayFrames( 100 ); // 2 seconds
                ClearGameArea();
                demo_mode = 0;
                return 0;
            end

            case    1: // continue game
            end

            case    2: // key pressed on demo mode
                ClearGameArea();
                demo_mode = 0;
                return 2;
            end

            case    3: // next round
                DelayFrames( 100 ); // 2 seconds
                goto start_round;
            end

            case    4: // game over
                goto game_over;
            end

            case    6: // player death a life
                DelayFrames( 100 ); // 2 seconds
                reloadCnt = 0;
                if ( playersInGame > 1 )
                    if ( player[ currPlayer ^ 1 ].numShips > 0 )
                        // Backup Shield
                        map_block_copy(0, playerPtr.shieldBackup, 0, 0, main_game_layer, 0, SHIELDTOPY, 224, 16, B_NOCOLORKEY );
                        playerPtr.shieldBackupSaved = 1;
                        currPlayer ^= 1;
                        playerPtr = &player[ currPlayer ];
                        goto start_round;
                    end
                end
                state1.waitForEventsComplete = 0;
                state1.playerIsAlive = 1;
                playerX = 14;
                gameState = 1; // continue
            end
        end

        frame;
    end

game_over:
    hiScoreUpdate( playerPtr.score );

    if ( playersInGame == 1 )
        PrintAtDelay( 9, 7, "GAME OVER", 6 );
        DelayFrames( 100 ); // 2 seconds
        ClearGameArea();
        return 1;
    elif ( playersInGame == 2 )
        PrintAtDelay( 4, 28, "GAME OVER  PLAYER<" + ( !currPlayer ? "1" : "2" ) + ">", 6 );

        DelayFrames( 100 ); // 2 seconds

        currPlayer ^= 1;
        playerPtr = &player[ currPlayer ];

        if ( !playerPtr.numShips )
            ClearGameArea();
            return 1;
        end

        goto start_round;
    end

    gameState = 0;
    ClearGameArea();

    return 1;

end

/* ********************************************* */

#endif
