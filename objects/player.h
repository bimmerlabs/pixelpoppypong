#pragma once
#include "../sprites.h"
#include "../input.h"
#include "object.h"
#include "teams.h"

#define NOT_PLAYING 0
#define PLAYING 1

#define X_SPEED_INC toFIXED(1.5)
#define Y_SPEED_INC toFIXED(5.0)

#define MAX_Y_SPEED toFIXED(15.0)
#define MAX_X_SPEED toFIXED(10.0)
#define AI_X_SPEED  toFIXED(10.0)

#define PLAYER_HEIGHT toFIXED(20)
#define PLAYER_WIDTH toFIXED(40)
#define PLAYER_BOUNDARY_RIGHT  toFIXED(328.0)
#define PLAYER_BOUNDARY_LEFT  toFIXED(-328.0)
#define PLAYER_BOUNDARY_MIDDLE  toFIXED(8)
#define PLAYER_BOUNDARY_MIDDLE_INACTIVE  toFIXED(120)

#define PLAYER_MOVEMENT_SPEED toFIXED(0.0015)

#define PLAYER_X 340
#define PLAYER_Y 110
#define PLAYER_DEPTH 110
#define PLAYER_RADIUS toFIXED(27)
#define SHIELD_RADIUS toFIXED(60)
#define SHIELD_OFFSET toFIXED(32)
#define SHIELD_POWER 26
#define SHIELD_REGEN_FAST 8 // POWER OF 2!!
#define SHIELD_REGEN_SLOW 16 // POWER OF 2!!
#define BOUNDARY_SHIELD_REGEN_FAST  toFIXED(280.0)
#define BOUNDARY_SHIELD_REGEN_SLOW  toFIXED(180.0)

#define PLAYER_RADIUS_SMALL toFIXED(10)
#define PLAYER_RADIUS_LARGE toFIXED(60)
#define SHIELD_RADIUS_SMALL toFIXED(30)
#define SHIELD_RADIUS_LARGE toFIXED(100)

#define ATTACK1 toFIXED(10)
#define ATTACK1_COST 4
#define ATTACK2 toFIXED(20)
#define ATTACK2_COST 6
#define ATTACK_FRAMES 8 // POWER OF 2!!

#define LARGE_PLAYER_SPRITE toFIXED(4)
#define SMALL_PLAYER_SPRITE toFIXED(1)
#define NORMAL_PLAYER_SPRITE toFIXED(2)
#define PLAYER_SPRITE_RATE toFIXED(0.1)

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
    Uint8 stars;
    Uint8 deaths; // used as a proxy for continues
    unsigned int total;  // total score (story mode)
    unsigned int points; // round score
    unsigned int lastMillion;

} SCORE, *PSCORE;

typedef struct _CHARACTER_ATTRIBUTES
{
    int maxSpeed;     // Speed on a scale of 1-10
    int acceleration; // Acceleration rate on a scale of 1-10
    int power;        // Power on a scale of 1-10
} CHARACTER_ATTRIBUTES;

typedef struct _SHIELD
{
    Uint8 power;
    bool activate;
} SHIELD;

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

extern bool characterUnlocked[TOTAL_CHARACTERS]; // distinction between selection and what gets saved in backup ram
extern bool characterAvailable[TOTAL_CHARACTERS];
// extern bool teamAvailable[MAX_TEAMS+1]; // because unselected is team 0 // could be part of a team struct

// Array to hold attributes for each character
extern const CHARACTER_ATTRIBUTES characterAttributes[];

typedef struct _CHARACTER
{
    int  choice;
    bool selected;
} CHARACTER, *PCHARACTER;

// This structure represents the player
typedef struct _PLAYER
{
    GAME_OBJECT;
    PLAYER_STATE subState;
    SCORE score;
    CHARACTER character;
    
    // 0-11, controller ID
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
    bool isExploded;
    bool isBig;
    bool isSmall;
    int numLives;
    int totalLives;
    
    // Team (replaces team struct values)
    short teamChoice;
    short teamOldTeam;
    bool teamSelected;
    
    // Attributes from the selected character
    FIXED maxSpeed;
    FIXED acceleration;
    FIXED basePower; // from the character
    FIXED power; // based on inputs / game factors
    
    // FOR GOAL MOVEMENT
    FIXED goalCenterThresholdMax;
    FIXED goalCenterThresholdMin;
    
    // ATTACK
    bool attack1;
    Uint8 attack1Frames;
    bool attack2;
    Uint8 attack2Frames;
    
    // SHIELD
    SHIELD shield;
    
    // timers (in frames)
    int invulnerabilityFrames; // how long player in invulerable
    int respawnFrames; // how long before respawning player after death
    int shroomFrames; // how long the mushroom effect lasts

    // used for movement acceleration
    bool moveHorizontal;
    bool moveVertical;
    
    // sprites
    Sprite *_bg;
    Sprite *_cursor;
    Sprite *_sprite;
    Sprite *_portrait;
    FIXED shield_pos;

} PLAYER, *PPLAYER;

extern PLAYER g_Players[MAX_PLAYERS];

void speedLimitPlayer(PPLAYER player);
void boundPlayer(PPLAYER player);
void explodeNeighbors(PPLAYER player);

void regenPlayerPower(PPLAYER player);
void playerAttack(PPLAYER player);
void getClassicModeInput(void);
void getPlayersInput(void);
void updatePlayers(void);

void initPlayers(void);
void switchPlayerPosition(PPLAYER player);
void initAiPlayers(void);
void initStoryCharacters(void);
void nextStoryCharacter(void);
void initVsModePlayers(void);
void initDemoPlayers(void);

void assignCharacterSprite(PPLAYER player);
void assignCharacterStats(PPLAYER player);

int getLives(PPLAYER player);
int getStars(void);
void getContinues(void);
void resetPlayerScores(void);
void resetPlayerAttacks(void);
void spawnPlayers(void);
bool explodePLayer(PPLAYER player);

void cacheInputDirectionPlayer(PPLAYER player, bool* up, bool* down, bool* left, bool *right);

static __jo_force_inline void drawPlayers(void)
{
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];

        if (player->objectState == OBJECT_STATE_INACTIVE || player->subState == PLAYER_STATE_DEAD) {
            continue;
        }
        my_sprite_draw(player->_sprite);
    }
}

static __jo_force_inline bool growPlayerSprite(PPLAYER player, FIXED targetSize)
{
    if (player->_sprite->scl.x < targetSize) {
        player->_sprite->scl.x = player->_sprite->scl.x + PLAYER_SPRITE_RATE;
        player->_sprite->scl.y = player->_sprite->scl.y + PLAYER_SPRITE_RATE;
        shield[player->playerID].scl.x = player->_sprite->scl.x;
        shield[player->playerID].scl.y = player->_sprite->scl.y;
        return true;
    }
    return false;
}

static __jo_force_inline bool shrinkPlayerSprite(PPLAYER player, FIXED targetSize)
{
    if (player->_sprite->scl.x > targetSize) {
        player->_sprite->scl.x = player->_sprite->scl.x - PLAYER_SPRITE_RATE;
        player->_sprite->scl.y = player->_sprite->scl.y - PLAYER_SPRITE_RATE;
        shield[player->playerID].scl.x = player->_sprite->scl.x;
        shield[player->playerID].scl.y = player->_sprite->scl.y;
        return true;
    }
    return false;
}
