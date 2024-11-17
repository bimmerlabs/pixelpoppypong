#pragma once

extern int g_PauseChoice;

void pause_input(void);
void pause_draw(void);
void pauseGame(void);
void convertNumberToDigits(int number, unsigned char* hunds, unsigned char* tens, unsigned char* ones);
