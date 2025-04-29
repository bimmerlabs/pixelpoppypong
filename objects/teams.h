#pragma once
#include "object.h"

#define MAX_TEAMS 4

typedef enum _TEAM_SELECT
{
    TEAM_1 = 0,
    TEAM_2,
    TEAM_3,
    TEAM_4,
    TEAM_COUNT,
} TEAM_SELECT;

typedef enum _TEAMS_FOR_SCORING
{
    ONE_TEAM = 0,
    TWO_TEAMS = 1,
    THREE_TEAMS = 2,
    FOUR_TEAMS = 3,
} TEAMS_FOR_SCORING;

typedef struct _TEAM_OBJECT
{
    // number of teams
    short minTeams;
    short maxTeams;
    short numTeams;
    bool isAvailable[MAX_TEAMS];
    OBJECT_STATE objectState[MAX_TEAMS];

} TEAM_OBJECT, *PTEAM_OBJECT;

extern TEAM_OBJECT g_Team;

void initTeams(void);