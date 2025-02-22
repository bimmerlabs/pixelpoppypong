#pragma once

#include "gameplay.h"

#define SCORE_DISPLAY_TIME (15 * 60)

extern unsigned int g_ScoreTimer;

void init_scores(void);
void display_scores(void);
void score_input(void);
void sortHighScores(HighScoreEntry scores[]);