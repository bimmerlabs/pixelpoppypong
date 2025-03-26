#pragma once
#include "objects/player.h"
#include "highscores.h"

extern int memory_access;
extern Uint8 backup_device;

typedef struct _SAVEGAME 
{
    GameOptions game_options;
    bool *characterUnlocked;
    HighScoreEntry highScores[SCORE_ENTRIES];
    INPUT inputSettings[MAX_INPUTS];
} SaveGame;

extern SaveGame save_game;

void    save_game_backup(void);
bool    load_game_backup(void);
bool    is_cart_mem_available(void);
bool    is_internal_mem_available(void);