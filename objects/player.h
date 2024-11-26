#pragma once
#include "../sprites.h"
#include "object.h"

#define NOT_PLAYING 0
#define PLAYING 1

#define X_SPEED_INC toFIXED(0.5)
#define Y_SPEED_INC toFIXED(4.0)
// #define X_FRICTION_SPEED toFIXED(0.375)

#define MAX_Y_SPEED               toFIXED(8.0)  // don't fall faster than this
#define MAX_Y_SPEED2              toFIXED(-8.0)
#define MAX_X_SPEED               toFIXED(7.0)  // don't move faster than this
#define MAX_X_SPEED2              toFIXED(-7.0) // don't move faster than this

// #define MAX_X_SPEED3               toFIXED(4.5)  // don't move faster than this
// #define MAX_Y_SPEED3              toFIXED(-5.0)


#define PLAYER_HEIGHT toFIXED(20)
#define PLAYER_WIDTH toFIXED(32)

#define SCREEN_RIGHT  toFIXED(352.0)
#define SCREEN_LEFT  toFIXED(-352.0)
#define SCREEN_MIDDLE  toFIXED(0)
#define SCREEN_WIDTH  toFIXED(704.0)

#define SCREEN_TOP toFIXED(-240.0)
#define SCREEN_BOTTOM toFIXED(240.0)

#define SCREEN_OFFSET toFIXED(100)
#define GAMEPLAY_ABOVE_SCREEN (SCREEN_TOP - SCREEN_OFFSET)
#define GAMEPLAY_BELOW_SCREEN (SCREEN_BOTTOM + SCREEN_OFFSET)

#define PLAYER_X 350
#define PLAYER_Y 120
#define PLAYER_DEPTH 120

// #define EXPLODING_FRAME_COUNT (2 * 30)
// #define EXPLODE_FRAME_COUNT (4 * 30)

typedef enum _PLAYER_STATE
{
    PLAYER_STATE_ACTIVE = 0,
    PLAYER_STATE_EXPLODING,
    PLAYER_STATE_EXPLODED,
    PLAYER_STATE_LOGO, // used for the cursor in the logo
    PLAYER_STATE_TITLE, // cursors in the background of the title screen
    PLAYER_STATE_DEAD,
} PLAYER_STATE;

typedef struct _SCORE
{
    unsigned int deaths; // last wave player was alive for
    unsigned int points; // number of boxes opened
    unsigned int team_points; // points of entire team

} SCORE, *PSCORE;

typedef struct _CHARACTER_ATTRIBUTES
{
    int maxSpeed;     // Speed on a scale of 1-10
    int acceleration; // Acceleration rate on a scale of 1-10
    int power;        // Power on a scale of 1-10
} CHARACTER_ATTRIBUTES;

typedef enum _CHARACTER_SELECT
{
    CHARACTER_MACCHI = 0,
    CHARACTER_JELLY,
    CHARACTER_PENNY,
    CHARACTER_POPPY, // used for the cursor in the logo
    CHARACTER_POTTER, // cursors in the background of the title screen
    CHARACTER_SPARTA,
    CHARACTER_TJ,
    CHARACTER_WUPPY,
    CHARACTER_NONE,
} CHARACTER_SELECT;

#define TOTAL_CHARACTERS (CHARACTER_NONE)

extern bool characterAvailable[TOTAL_CHARACTERS];

// Array to hold attributes for each character
extern const CHARACTER_ATTRIBUTES characterAttributes[];

typedef enum _TEAM_SELECT
{
    TEAM_CPU = 0,
    TEAM_1,
    TEAM_2,
    TEAM_3,
    TEAM_4,
} TEAM_SELECT;

extern int teamCount[MAX_TEAMS];

typedef struct _CHARACTER
{
    bool startSelection;
    int  choice;
    bool selected;
    bool selectColor;
    jo_color _color;
} CHARACTER, *PCHARACTER;

typedef struct _TEAM
{   
    bool startSelection;
    int choice;
    int oldTeam;
    bool selected;
} TEAM, *PTEAM;

// This structure represents the player
typedef struct _PLAYER
{
    GAME_OBJECT;
    PLAYER_STATE subState;

    // jo_fixed size; // DELETE?
    // jo_fixed ds; // DELETE

    // int angle; // DELETE??
    // int da; // DELETE?

    // int frameCount; // DELETE?

    // 0-11, controller ID
    int playerID;

    // 0 if not playing, 1 if playing
    bool isPlaying; // MAKE A BOOL?


    bool pressedB;
    bool pressedStart;
    
    // Index into the players array
    CHARACTER character;    
    // Attributes from the selected character
    int maxSpeed;
    int acceleration;
    int power;
    
    TEAM team;
    int teamID;

    int numLives;

    // timers (in frames)
    int invulnerabilityFrames; // how long player in invulerable
    int respawnFrames; // how long before respawning player after death
    int crackChoice; // style of cracked screen graphic

    SCORE score;

    // used for movement acceleration
    bool moveHorizontal;
    bool moveVertical;
    
    // sprite
    Sprite *_sprite;
    Sprite *_portrait;
    Sprite *_cursor;
    Sprite *_bg;

} PLAYER, *PPLAYER;

void getPlayersInput(void);
void updatePlayers(void);
void drawPlayers(void);
void explodePlayer(PPLAYER player, bool showExplosion, bool spreadExplosion);

void initPlayers(void);
void initVsModePlayers(void);
void initDemoPlayers(void);

void resetPlayerScores(void);
void spawnPlayers(void);
void cacheInputDirectionPlayer(PPLAYER player, bool* up, bool* down, bool* left, bool *right);



