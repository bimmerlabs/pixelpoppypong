#pragma once
#include "../sprites.h"
#include "../input.h"
#include "object.h"

#define NOT_PLAYING 0
#define PLAYING 1

#define X_SPEED_INC toFIXED(1.5)
#define Y_SPEED_INC toFIXED(5.0)
// #define X_FRICTION_SPEED toFIXED(0.375)

#define MAX_Y_SPEED               toFIXED(15.0)  // don't fall faster than this
// #define MAX_Y_SPEED2              toFIXED(-14.0)
#define MAX_X_SPEED               toFIXED(10.0)  // don't move faster than this
// #define MAX_X_SPEED2              toFIXED(-12.0) // don't move faster than this
#define AI_X_SPEED                toFIXED(10.0)  // don't move faster than this

#define PLAYER_HEIGHT toFIXED(20)
#define PLAYER_WIDTH toFIXED(40)

#define PLAYER_X 350
#define PLAYER_Y 120
#define PLAYER_DEPTH 120
#define PLAYER_RADIUS toFIXED(26)

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
    CHARACTER_POTTER,
    CHARACTER_SPARTA,
    CHARACTER_POPPY,
    CHARACTER_TJ,
    CHARACTER_GEORGE,
    CHARACTER_WUPPY,
    CHARACTER_WALRUS,
    CHARACTER_GARF,
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
    int  choice;
    bool selected;
    bool selectColor;
    jo_color _color;
} CHARACTER, *PCHARACTER;

typedef struct _TEAM
{   
    int choice;
    int oldTeam;
    bool selected;
} TEAM, *PTEAM;

// This structure represents the player
typedef struct _PLAYER
{
    GAME_OBJECT;
    PLAYER_STATE subState;
    SCORE score;
    CHARACTER character;
    TEAM team;
    
    // 0-11, controller ID (replace with actual inputs)
    PINPUT input;
    int playerID;

    // player
    bool startSelection;
    bool isReady;
    bool pressedB;
    bool pressedStart;
    bool isPlaying;
    bool onLeftSide;
    bool scored;
    bool isAI;
    
    int numLives;
    
    // Attributes from the selected character
    FIXED maxSpeed;
    FIXED acceleration;
    FIXED power;
    
    // timers (in frames)
    int invulnerabilityFrames; // how long player in invulerable
    int respawnFrames; // how long before respawning player after death
    int crackChoice; // style of cracked screen graphic

    // used for movement acceleration
    bool moveHorizontal;
    bool moveVertical;
    
    // sprites
    Sprite *_bg;
    Sprite *_cursor;
    Sprite *_sprite;
    Sprite *_portrait;

} PLAYER, *PPLAYER;

void drawPlayerSprite(PPLAYER player);
void speedLimitPlayer(PPLAYER player);
void boundPlayer(PPLAYER player);
void explodeNeighbors(PPLAYER player);

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



