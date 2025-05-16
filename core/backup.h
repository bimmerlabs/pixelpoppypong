#pragma once
#include "objects/player.h"
#include "highscores.h"

typedef struct _SAVEGAME 
{
    GameOptions g_GameOptions;
    bool characterUnlocked[TOTAL_CHARACTERS];
    HighScoreEntry highScores[SCORE_ENTRIES];
    INPUT inputSettings[MAX_INPUTS];
} SaveGame;

extern SaveGame save_game;

void    save_game_backup(void);
bool    load_game_backup(void);