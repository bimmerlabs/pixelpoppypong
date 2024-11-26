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


