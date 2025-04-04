#include <jo/jo.h>
#include "audio.h"
#include "main.h"

bool cd_is_playing = false;
int volume = MAX_VOLUME;

unsigned int g_GoalScoredTrack = GOAL_SCORED_TRACK_1;

#ifdef JO_COMPILE_WITH_AUDIO_SUPPORT
void playCDTrack(int track, bool repeat)
{
    jo_audio_play_cd_track(track, track, repeat);
    cd_is_playing = true;
}

void reset_audio(int new_volume) {
    if (cd_is_playing) {
        jo_audio_stop_cd();
        volume = new_volume;
        jo_audio_set_volume(volume);
        cd_is_playing = false;
    }
}
#else
void playCDTrack(int track, bool repeat)
{
    CDDA_PlaySingle(track, repeat);
    cd_is_playing = true;
}

void reset_audio(int new_volume) {
    if (cd_is_playing) {
        CDDA_Stop();
        volume = new_volume;
        // set_master_volume(volume >> 4);
        CDDA_SetVolume(volume >> 4);
        cd_is_playing = false;
    }
}
#endif

void nextGoalScoredTrack(void)
{
    g_GoalScoredTrack++;
    if (g_GoalScoredTrack == GOAL_SCORED_TRACK_MAX) {
        g_GoalScoredTrack = GOAL1_TRACK;
    }
}