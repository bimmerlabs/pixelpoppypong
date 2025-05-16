#include <jo/jo.h>
#include "audio.h"
#include "../main.h"

AudioSettings g_Audio = {
    .goalScoredTrack = GOAL_SCORED_TRACK_1,
    .cdIsPlaying = false,
    .masterVolume = MAX_VOLUME,
};

void playCDTrack(int track, bool repeat)
{
    CDDA_PlaySingle(track, repeat);
    g_Audio.cdIsPlaying = true;
}

void reset_audio(int new_volume) {
    if (g_Audio.cdIsPlaying) {
        CDDA_Stop();
        g_Audio.masterVolume = volume_shift(new_volume);
        CDDA_SetVolume(g_Audio.masterVolume);
        g_Audio.cdIsPlaying = false;
    }
}

void nextGoalScoredTrack(void)
{
    g_Audio.goalScoredTrack++;
    if (g_Audio.goalScoredTrack == GOAL_SCORED_TRACK_MAX) {
        g_Audio.goalScoredTrack = GOAL1_TRACK;
    }
}