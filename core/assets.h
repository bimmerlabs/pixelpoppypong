#pragma once

#include <jo/jo.h>
#include "../main.h"
#include "sprites.h"
#include "pcmsys.h"

#define NUM_PAW_SPRITES 5
#define NUM_POPPY_SPRITES 7

#define NUM_CHARACTER_SPRITES 12
#define NUM_X_SPRITES 1
#define NUM_PCURSOR_SPRITES 4

#define NUM_FONT_CHARS 40

#define NUM_TITLE_MENU_TEXT 12

#define NUM_GOAL_SPRITES 4
#define NUM_TIMER_SPRITES 12
#define NUM_HEART_SPRITES 4
#define NUM_SHIELD_SPRITES 7

#define NUM_MENUBG_SPRITES 9

#define NUM_BOMB_SPRITES 4
#define NUM_EXPLOD_SPRITES 6
#define NUM_FISH_SPRITES 4
#define NUM_SHROOM_SPRITES 8

typedef enum _MEOW
{
    MEOW1 = 0,
    MEOW2,
    MEOW3,
    MEOW4,
    MEOW5,
    MEOW6,
    MEOW7,
    MEOW8,
    MEOW9,
} MEOW;

// holds sprite and audio assets
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
    int dead[NUM_X_SPRITES];
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
    
    // items
    int bomb[NUM_BOMB_SPRITES];
    int explosion[NUM_EXPLOD_SPRITES];
    int fishtank[NUM_FISH_SPRITES];
    int shroom[NUM_SHROOM_SPRITES];

    // audio assets
    // CORE / MENU SOUNDS
    short cancelPcm8; // b button
    short cursorPcm8; // d-pad
    short nextPcm8;   // normal menu selection
    short startPcm8;  // start button first press
    short tickPcm8;   // analog adjustment
    
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
    short growPcm8;
    short shrinkPcm8;
    short bloopPcm8;
    short stadlerPcm8;
    short dropPcm8;
    short bouncePcm8;
    short shieldPcm8;
    // short rechargePcm8;
    short countdownPcm8;
    short winPcm8;
    short bumpPcm16;
    bool GameplaySoundsLoaded;
    
    // CAT SOUNDS
    short meowPcm8[9];
    short meowID;
    
    // NAME ENTRY SOUNDS
    short name_ketPcm8;
    short name_curPcm8;
    short name_canPcm8;
    short name_brkPcm8;
    bool NameEntrySoundsLoaded;

} ASSETS, *PASSETS;

extern ASSETS g_Assets;
extern int paw_blank_id;

// create sprite / animation object
void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int frames, int w, int h, bool asset1or2);

// assets
void loadCommonAssets(void);
void loadCoreSoundAssets(void);

void loadPPPLogoAssets(void);
void unloadPPPLogoAssets(void);

void loadNameEntryAssets(void);
void unloadNameEntryAssets(void);
bool loadNameEntrySoundAssets(void);

void loadTitleScreenAssets(void);
void unloadTitleScreenAssets(void);

void loadCharacterAssets(void);
void loadGameAssets(void);
void unloadGameAssets(void);
bool loadGameplaySoundAssets(void);