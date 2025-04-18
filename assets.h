#pragma once

#include <jo/jo.h>
#include "main.h"
#include "sprites.h"

#define NUM_PAW_SPRITES 5
#define NUM_POPPY_SPRITES 8

#define NUM_CHARACTER_SPRITES 12
#define NUM_PCURSOR_SPRITES 4

#define NUM_FONT_CHARS 40

#define NUM_TITLE_MENU_TEXT 12

#define NUM_GOAL_SPRITES 4
#define NUM_TIMER_SPRITES 11
#define NUM_HEART_SPRITES 5
#define NUM_SHIELD_SPRITES 7

#define NUM_MENUBG_SPRITES 9

#define NUM_BOMB_SPRITES 4
#define NUM_EXPLOD_SPRITES 6
#define NUM_FISH_SPRITES 4
#define NUM_SHROOM_SPRITES 8
    
// // sprites Z-depth
// // lower numbers are closer to the screen
// // larger numbers are farther away
// typedef enum _SPRITE_DEPTH
// {
    // SCORE_Z = 480,

    // CRACKS_Z = 490,
    // TITLE_MENU_Z = 491,

    // TITLE_SCREEN_Z = 495,

    // EXPLOSION_Z = 498,

    // PLAYER_Z = 500,

    // GRID_Z = 510,

    // SSMTF_GRID_Z = 510,

// } SPRITE_DEPTH;

// holds sprite and audio assets
// only really using these for animations?
typedef struct _assets
{
    // title screen
    int title;
    
    int font[NUM_FONT_CHARS]; // VDP1 font
    
    int menu[NUM_TITLE_MENU_TEXT];
    
    int timer[NUM_TIMER_SPRITES];
    int heart[NUM_HEART_SPRITES];
    
    int shield[NUM_SHIELD_SPRITES];
    // backgrounds for menus, player select, etc
    int menubg[NUM_MENUBG_SPRITES];

    // team selection
    int characters[NUM_CHARACTER_SPRITES];
    int pcursor[NUM_PCURSOR_SPRITES];
    
    // players sprites
    int paw1[NUM_PAW_SPRITES];
    int paw2[NUM_PAW_SPRITES];
    int paw3[NUM_PAW_SPRITES];
    int paw4[NUM_PAW_SPRITES];
    int paw5[NUM_PAW_SPRITES];
    int paw6[NUM_PAW_SPRITES];
    int paw7[NUM_PAW_SPRITES];
    int paw8[NUM_PAW_SPRITES];
    int paw9[NUM_PAW_SPRITES];
    int paw10[NUM_PAW_SPRITES];
    int paw11[NUM_PAW_SPRITES];
    
    int pixel_poppy1[NUM_POPPY_SPRITES];
    
    int goal1[NUM_GOAL_SPRITES];
    int goal2[NUM_GOAL_SPRITES];
    int goal3[NUM_GOAL_SPRITES];
    int goal4[NUM_GOAL_SPRITES];
    bool drawSingleGoal[MAX_PLAYERS];
    
    // items
    int bomb[NUM_BOMB_SPRITES];
    int explosion[NUM_EXPLOD_SPRITES];
    int fishtank[NUM_FISH_SPRITES];
    int shroom[NUM_SHROOM_SPRITES];

    // audio assets
    // CORE / MENU SOUNDS
    // short acceptPcm8; // EXITING MENUS?
    short cancelPcm8; // b button
    short cursorPcm8; // d-pad
    // short finishPcm8; // last menu selection
    short nextPcm8;   // normal menu selection
    short startPcm8;  // start button first press
    short tickPcm8;   // analog adjustment
    // short cursor1Pcm8; // back NEXT
    // short cursor2Pcm8; // move CURSOR
    // short plusPcm8; // next FINISH
    // short startPcm8; // accept/start
    
    // GAMEPLAY SOUNDS
    short gameOverPcm8;
    short scoreAddPcm8;
    short scoreTotalPcm8;
    short chain0Pcm8;
    short chain1Pcm8;
    short chain2Pcm8;
    short chain3Pcm8;
    short chain5Pcm8;
    short explod1Pcm8;
    short dropPcm8;
    short bouncePcm8;
    short shieldPcm8;
    short countdownPcm8;
    short bumpPcm16;
    
    // NAME ENTRY SOUNDS
    short name_ketPcm8;
    short name_curPcm8;
    short name_canPcm8;
    short name_brkPcm8;

} ASSETS, *PASSETS;

extern ASSETS g_Assets;
extern int paw_blank_id;

// initialize assets
// void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int num_tilesets, int frames, int w, int h, unsigned int spritesPerRow, bool animation1or2);
void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int frames, int w, int h, bool asset1or2);
void loadSoundAssets(void);
void loadCommonAssets(void);

void loadPPPLogoAssets(void);
void unloadPPPLogoAssets(void);

void loadNameEntryAssets(void);
void unloadNameEntryAssets(void);

void loadTitleScreenAssets(void);
void unloadTitleScreenAssets(void);

void loadCharacterAssets(void);
void loadGameAssets(void);
void unloadGameAssets(void);