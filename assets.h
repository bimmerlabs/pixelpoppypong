#pragma once

#include <jo/jo.h>
#include "main.h"
#include "sprites.h"

// sprites Z-depth
// lower numbers are closer to the screen
// larger numbers are farther away
typedef enum _SPRITE_DEPTH
{
    SCORE_Z = 480,

    CRACKS_Z = 490,
    TITLE_MENU_Z = 491,

    TITLE_SCREEN_Z = 495,

    EXPLOSION_Z = 498,

    PLAYER_Z = 500,

    GRID_Z = 510,

    SSMTF_GRID_Z = 510,

} SPRITE_DEPTH;

// holds sprite and audio assets
typedef struct _assets
{
    // int randomizedColors[MAX_TEAMS]; // randomizes the order of the team colors once per boot

    // title screen
    int title;
    int options[10];
    int timer[11];

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
    // int open_select;
    // int mine_exploded_select;

    // cursor sprites
    // int cursor[1];
    
    // players sprites
    int paw1[5];
    int paw2[5];    
    
    // players sprites
    int poppy1[11];
    int poppy2[6];

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

// initialize assets
void loadAssets(void);
void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int num_tilesets, int frames, int w, int h, unsigned int spritesPerRow);
void loadSpriteAssets(void);