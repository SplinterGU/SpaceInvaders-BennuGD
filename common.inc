#ifndef __COMMON_H
#define __COMMON_H

/* ********************************************* */

#define SHIELDTOPY      192
#define PLAYERY         216

#define FLOORY          232

#define SAUCERSPEED     3
#define SHOTSPEED       5
#define PLAYERSPEED     0 

#define SAUCERTIMER     0x600 
#define SAUCERROW       40

#define ALIENSPEED      1 
#define ALIENSHOTSPEED  2

#define FLEETDELTAY     8

#define FLEETPOSSTART   128

#define DEMOSPEED       50

/* ********************************************* */

int font = 0;

int demo_mode = 0;

int numLives = 3;
int bonusLife = 1500;
int displayCoinage = 1;

int numCredits = 0;
int hiScore = 0;
int playersInGame = 0;
int currPlayer = 0;

type _player
    /* Aliens */

    int     numAliens;

    int     leftLimitCol,               
            rightLimitCol;

    int     leftLimit,                  
            rightLimit;

    int     aliensColMask;

    int     aliens[55];

    int     fleetTopBase;

    double  aliensX,                    
            aliensY,                    
            aliensDeltaX;

    int     alienIdx;

    int     tillSaucer;                 // = SAUCERTIMER; // Original Arcade 0x0600
    
    int     sauScore;

    int     alienShotMask;

    /* Player */

    int     shotsCounter;

    int     score;

    /* Alien Shot */

    int     reloadRate;

    int     pluShotColIdx;
    int     squShotColIdx;

    /* Others */

    int     numShips;

    int     round;

    /* Shield */

    double  currentShieldTopY;          // = SHIELDTOPY;

    int     shieldBackupSaved;
    int     shieldBackup;
end

/* ********************************************* */

_player player[1];
_player * playerPtr = NULL;

#endif
