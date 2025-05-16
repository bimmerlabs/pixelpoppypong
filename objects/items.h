#pragma once
#include "../core/assets.h"
#include "player.h"

#define SHROOM_TIMER (15 * 60)
#define BOMB_TIMER (9 * 60)
#define BOMB_FLASH_TIMER (3 * 60)
#define FISH_HUE_INCREMENT 50

// extern bool g_AnimateBomb;

typedef enum _GAME_ITEMS
{
    GAME_ITEM_BOMB = 0,
    GAME_ITEM_FISH,
    GAME_ITEM_SHROOM,
    GAME_ITEM_GARF,
    GAME_ITEM_CRAIG,
    GAME_ITEM_MAX,
} GAME_ITEMS;

typedef struct {
    unsigned int id;
    unsigned int timer[MAX_PLAYERS];
    bool isActive;
    Sprite *_sprite;
} Item;

extern Item g_item;

void animateBombColor(bool *_do_update);

void setItemPositions(void);
void drawGameItems(void);
void handlePlayerItemCollision(PPLAYER player);

void bombTimer(void);