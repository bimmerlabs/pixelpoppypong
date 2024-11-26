#pragma once
#include "main.h"

// CD sound tracks
#define LOGO_TRACK                2
#define TITLE_TRACK               3 // temp
#define SELECT_TRACK              3
// #define GAME_TRACK                5
// #define PAUSE_TRACK               6
// #define VICTORY_TRACK             7
// #define DEATH_TRACK               8

#define AUDIO_FADE_TIMER (3 * 60)
#define INITIAL_STATE_VOLUME 32
#define MAX_VOLUME 127
#define HALF_VOLUME 64
#define QUARTER_VOLUME 32
#define MIN_VOLUME 0

extern bool cd_is_playing;
extern int volume;

void playCDTrack(int track, bool repeat);

void reset_audio(int new_volume);