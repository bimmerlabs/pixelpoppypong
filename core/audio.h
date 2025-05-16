#pragma once
#include "main.h"
#include "pcmsys.h"

// CD sound tracks
#define LOGO_TRACK                2 // TRACK 01
#define TITLE_TRACK               3
#define SELECT_TRACK              4
#define BEGIN_GAME_TRACK          5
#define CREDITS_TRACK             6
#define FINISH_TRACK              7
#define GOAL1_TRACK               8
#define GOAL2_TRACK               9
#define MATCH_TRACK              10
#define NAME_ENTRY_TRACK         11
#define CONTINUE_TRACK           12

#define AUDIO_FADE_TIMER (3 * 60)

#define INITIAL_STATE_VOLUME 32
#define MAX_VOLUME 127
#define LOWER_VOLUME 100
#define HALF_VOLUME 64
#define QUARTER_VOLUME 32
#define MIN_VOLUME 0

typedef struct {
    unsigned int goalScoredTrack;
    bool cdIsPlaying;
    int masterVolume;
} AudioSettings;

extern AudioSettings g_Audio;

typedef enum
{
    GOAL_SCORED_TRACK_1 = GOAL1_TRACK,
    GOAL_SCORED_TRACK_2 = FINISH_TRACK,
    GOAL_SCORED_TRACK_3 = GOAL2_TRACK,
    GOAL_SCORED_TRACK_4 = MATCH_TRACK,
    GOAL_SCORED_TRACK_MAX = MATCH_TRACK+1,
} GOAL_SCORED_TRACK;

void playCDTrack(int track, bool repeat);

void reset_audio(int new_volume);

void nextGoalScoredTrack(void);

static inline int volume_shift(int vol)
{
    return vol >> 4;
}