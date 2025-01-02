#pragma once

#include <jo/jo.h>
#include "main.h"
#include "sprites.h"

// I haven't made any of my assets so this file is a mess

#define NUM_PAW_SPRITES 5
#define NUM_POPPY_SPRITES 4

// #define NUM_CURSORS_SPRITES 12
// #define NUM_FLAGS_SPRITES 12
// #define NUM_GRID_SPRITES 10
// #define NUM_MINE_SPRITES 5
// #define NUM_TEAM_SELECT_SPRITES 2
#define NUM_CHARACTER_SPRITES 12
#define NUM_PCURSOR_SPRITES 4
// #define NUM_PAUSE_LETTERS_SPRITES 6
// #define NUM_SCORE_DIGITS_SPRITES 10
// #define NUM_CRACKS_SPRITES 4
// #define NUM_SCORES_SPRITES 120

#define NUM_TITLE_MENU_TEXT 12
// #define NUM_TITLE_MODE_TEXT 3
// #define NUM_TITLE_PLAYER_TEXT 4
// #define NUM_TITLE_DIFFICULTY_TEXT 3

#define NUM_TIMER_SPRITES 11

#define NUM_MENUBG_SPRITES 9
// #define NUM_SHADOW_TITLE_TEXT_SPRITES 10
// #define NUM_PAUSE_TEXT_SPRITES 3
// #define NUM_EXPLOSION_SPRITES 6
    
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
typedef struct _assets
{
    // int randomizedColors[MAX_TEAMS]; // randomizes the order of the team colors once per boot

    // title screen
    int title;
    
    // individual tilesets are smaller, but this works for now..
    int menu[NUM_TITLE_MENU_TEXT];
    // int mode[NUM_TITLE_MODE_TEXT];
    // int player[NUM_TITLE_PLAYER_TEXT];
    // int difficulty[NUM_TITLE_DIFFICULTY_TEXT];
    
    int timer[NUM_TIMER_SPRITES];
    // backgrounds for menus, player select, etc
    int menubg[NUM_MENUBG_SPRITES];
    // // shadows
    // int mode_s;
    // int modes_s[4];
    // int difficulty_s;
    // int difficulties_s[3];
    // int start_s;

    // // grid
    // int closed;
    // int open;

    // // team selection
    int characters[NUM_CHARACTER_SPRITES];
    int pcursor[NUM_PCURSOR_SPRITES];
    // int open_select;
    // int mine_exploded_select;

    // cursor sprites
    // int cursor[1];
    
    // players sprites
    int paw1[NUM_PAW_SPRITES];
    int paw2[NUM_PAW_SPRITES];    
    
    // players sprites
    int pixel_poppy1[NUM_POPPY_SPRITES];
    int pixel_poppy2[NUM_POPPY_SPRITES];

    // // screen cracks
    // int cracks[4];

    // // mine
    // int mine;
    // int mine_exploded;
    // int mine_first;
    // int mine_wrong;
    // int mine_wrong_ts;

    // // explosions
    // int explosions[6];

    // // digits
    // int digits[9];

    // // flags
    // int flags[MAX_TEAMS];
    // int flags2[MAX_TEAMS]; // flags without the cell

    // // score
    // int pause_c;
    // int pause_d;
    // int pause_p;
    // int pause_r;
    // int pause_s;
    // int pause_t;
    // //int score_digits[10];
    // int scores[12][10];
    // int horizontal_seperator;
    // int vertical_seperator;

    // // score text
    // int resume;
    // int retry;
    // int exit;

    // // team selection
    // int horizontal_line;
    // int vertical_line;

    // // audio assets
    // jo_sound crackPCM;
    // jo_sound explodePCM;

} ASSETS, *PASSETS;

extern ASSETS g_Assets;
extern int paw_blank_id;

// initialize assets
void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int num_tilesets, int frames, int w, int h, unsigned int spritesPerRow);
void loadCommonAssets(void);

void loadTitleScreenAssets(void);
void unloadTitleScreenAssets(void);

void loadGameAssets(void);
void unloadGameAssets(void);