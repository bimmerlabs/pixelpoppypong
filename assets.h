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

    // // audio assets
    #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
    short gameOverPcm8;
    short bumpPcm16;
    #else
    jo_sound gameOverPcm8;
    jo_sound bumpPcm16;
    #endif

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