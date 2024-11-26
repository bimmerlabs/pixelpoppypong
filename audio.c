#include <jo/jo.h>
#include "audio.h"
#include "main.h"

bool cd_is_playing = false;
int volume = MAX_VOLUME;

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
