#include <jo/jo.h>
#include "goal.h"
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
