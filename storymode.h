#pragma once

#include <jo/jo.h>
#include "assets.h"
#include "sprites.h"
#include "gameplay.h"
#include "team_select.h"

#define CHARACTER_POS_X  0
#define CHARACTER_POS_Y  -128
#define CHARACTER_POS_OFFSET 128
#define CHARACTER_SELECT_TIMER (6 * 60)

extern int character_pos_x;
extern int character_pos_y;
extern int character_pos_selected;
extern int character_offset;
extern int g_StartStoryFrames;
extern const char *fullCharacterNames[];
extern const char *characterQuotes[];
void initStoryMode(void);
void storySelectUpdate(void);

static __jo_force_inline void drawCharacterList(void) {
    my_sprite_draw(&menu_bg2);
    my_sprite_draw(g_Players[0]._cursor);
    character_pos_y = character_pos_selected;
    for(unsigned int i = 1; i <= MAX_ROUNDS; i++) {
        character_portrait.spr_id = character_portrait.anim1.asset[i];
        set_spr_position(&character_portrait, character_pos_x, character_pos_y, PORTRAIT_DEPTH);
        set_spr_scale(&character_portrait, 2.0, 2.0);
        my_sprite_draw(&character_portrait);
        character_pos_y -= CHARACTER_POS_OFFSET;
    }
}

void resetSpriteColors(void);