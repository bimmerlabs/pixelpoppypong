#include <jo/jo.h>
#include "goal.h"
#include "player.h"
#include "../main.h"

TEAM_OBJECT g_Team = {0};

void initTeams(void) {
    PTEAM_OBJECT team = &g_Team;
    for(unsigned int i = TEAM_1; i < TEAM_COUNT; i++)
    {
        team->isAvailable[i] = true;
        team->objectState[i] = OBJECT_STATE_INACTIVE;
    }
}

void resetTeamState(void) {
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];
        if (player->isPlaying) {
            g_Team.objectState[player->teamChoice] = OBJECT_STATE_ACTIVE;
            player->subState = PLAYER_STATE_ACTIVE;
        }
    }
}
