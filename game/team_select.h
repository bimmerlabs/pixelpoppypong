#pragma once

#include <jo/jo.h>
#include "../objects/player.h"

#define TEAM_SELECT_TIMER (1 * 60)

// UI locations
#define PORTRAIT_X -120
#define PORTRAIT_Y -166
#define CHARACTER_TEXT_Y 2
#define PAW_X 247
#define TEAM_TEXT_X1 33
#define TEAM_TEXT_X2 35
#define TEAM_TEXT_Y 4
#define METER_TEXT_X 18
#define METER_TEXT_Y2 2
#define METER_TEXT_Y3 4
#define METER_X -40
#define METER_Y1 34
#define METER_Y2 3
#define METER_Y3 29
#define METER_HEIGHT 12
#define METER_WIDTH 100
#define PLAYER_OFFSET_Y 112
#define PLAYER_TEXT_OFFSET_Y 7
// asset draw depth
#define PLAYER_BG2_DEPTH 95
#define MENU_BG2_DEPTH 95
#define MENU_BG1_DEPTH 92
#define PORTRAIT_DEPTH 90
#define CURSOR_DEPTH 80

extern const char *characterNames[];
extern int g_StartGameFrames; // for debugging
extern bool g_TeamSelectPressedStart; // for debugging
extern bool all_players_ready; // for debugging

void resetTeams(void);
void initUnlockedCharacters(void);
void initAvailableCharacters(void);
void drawCharacterSelectGrid(void);  // re-organize these to be in order
void characterSelect_input(void);
void teamSelect_init(void);
// void assign_team(int oldTeam, int newTeam);
void teamSelect_input(void);
void validateCharacters(PLAYER *player);
void validateTeam(PLAYER *player);
bool playerReadyState(void);
void resetReadyState(void);
bool validateTeamCount(void);
void teamSelect_update(void);
void teamSelect_draw(void);
