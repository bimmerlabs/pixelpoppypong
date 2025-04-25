#include <jo/jo.h>
#include "screen_transition.h"
#include "main.h"
#include "state.h"
#include "math.h"
#include "assets.h"

unsigned int g_TransitionTimer = 0;
bool transition_out = false;
bool transition_in = false;
Sint16 nbg0_rate = MINIMUM_FADE;
Sint16 nbg1_rate = MINIMUM_FADE;
Sint16 spr_rate  = MINIMUM_FADE;
Sint8 fadeDirection = 1;
Sint8 fade_in_rate = 8;
Sint8 fade_out_rate = 4;
Uint8 mosaic_in_rate = MOSAIC_SLOW_RATE;
unsigned int g_FadeTimer = 0;
bool fade_out = false;
bool story_fade_out = false;
bool fade_in = false;
bool slow_fade_in = false;
bool story_fade_in = false;
bool explosion_flash = false;

unsigned short mosaic_x = MOSAIC_MIN;
unsigned short mosaic_y = MOSAIC_MIN;
bool mosaic_out = false;
bool mosaic_in = false;
static int rand_max = MOSAIC_MAX;

bool music_out = false;
bool music_in = false;

void screenTransition_init(Sint16 nbg0, Sint16 nbg1, Sint16 spr) {
    g_FadeTimer = 0;
    fadeDirection = 1;
    nbg0_rate = nbg0;
    nbg1_rate = nbg1;
    spr_rate  = spr;
}

void screenTransition_update(void) {
    if (!transition_out && !transition_in) {
        return;
    }
    if (transition_out) {
        transition_out = transitionOut();
    }
    else if (transition_in) {
        transition_in = transitionIn();
    }
}

bool transitionOut(void) {
    if (!fade_out && !story_fade_out && !mosaic_out && !music_out) {
        return false;
    }
    if (fade_out) {
        fade_out = fadeOut(fade_out_rate, MINIMUM_FADE);
    }
    if (story_fade_out) {
        story_fade_out = fadeOut(fade_out_rate, STORY_FADE);
    }
    if (mosaic_out) {
        mosaic_out = mosaicOut(NBG1ON);
    }
    if (music_out) {
        music_out = musicOut();
    }
    return true;
}

bool transitionIn(void) {
    if (!fade_in && !mosaic_in && !music_in && !slow_fade_in && !story_fade_in) {
        return false;
    }
    if (fade_in) {
        fade_in = fadeIn(fade_in_rate, NEUTRAL_FADE);
    }
    if (slow_fade_in) {
        slow_fade_in = slowFadeIn(fade_in_rate, QUARTER_FADE);
    }
    if (story_fade_in) {
        story_fade_in = fadeIn(fade_in_rate, STORY_FADE);
    }
    if (mosaic_in) {
        mosaic_in = mosaicIn(NBG1ON);
    }
    if (music_in) {
        music_in = musicIn();
    }
    return true;
}

bool fadeOut(Sint16 rate, Sint16 min) {
    if (nbg1_rate > min) {
        nbg1_rate -= rate;
        if (nbg1_rate < min) {
            nbg1_rate = min;
        }
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        return true;
    }
    else {
        return false;
    }
}

bool fadeIn(Sint16 rate, Sint16 max) {
    if (nbg1_rate < max) {
        nbg1_rate += rate;
        if (nbg1_rate > max) {
            nbg1_rate = max;
        }
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        return true;
    }
    else {
        return false;
    }
}

bool slowFadeIn(Sint16 rate, Sint16 max) {
    if (nbg1_rate < max && JO_MOD_POW2(frame, 4) == 0) { // modulus
        nbg1_rate += rate;
        if (nbg1_rate > max) {
            nbg1_rate = max;
        }
        if (!g_GameOptions.debug_display) {
            slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        }
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        return true;
    }
    else {
        return false;
    }
}

// MOSAIC
void mosaicInit(jo_scroll_screen screens) {
    rand_max = MOSAIC_MAX;
    mosaic_x = MOSAIC_MAX;
    mosaic_y = MOSAIC_MAX;
    slScrMosSize(mosaic_x, mosaic_y);
    slScrMosaicOn(screens);
}

bool mosaicOut(jo_scroll_screen screens) {
    if (mosaic_x < MOSAIC_MAX && mosaic_y < MOSAIC_MAX) {
        if (frame % MOSAIC_FAST_RATE == 0) { // modulus
            mosaic_x++;
            if (mosaic_x > MOSAIC_MAX)
                mosaic_x = MOSAIC_MAX;
            mosaic_y++;
            if (mosaic_y > MOSAIC_MAX)
                mosaic_y = MOSAIC_MAX;
        }
	slScrMosSize(mosaic_x, mosaic_y);
	slScrMosaicOn(screens);
        return true;
    }
    else {
	slScrMosSize(MOSAIC_MAX, MOSAIC_MAX);
	slScrMosaicOn(screens);
        return false;
    }
}

bool mosaicIn(jo_scroll_screen screens) {
    if (mosaic_x > MOSAIC_MIN+1 && mosaic_y > MOSAIC_MIN+1) {
        if (frame % mosaic_in_rate == 0) { // modulus
            mosaic_x--;
            if (mosaic_x < MOSAIC_MIN)
                mosaic_x = MOSAIC_MIN;
            mosaic_y--;
            if (mosaic_y < MOSAIC_MIN)
                mosaic_y = MOSAIC_MIN;
        }
	slScrMosSize(mosaic_x, mosaic_y);
	slScrMosaicOn(screens);
        return true;
    }
    else {
	slScrMosSize(MOSAIC_MIN, MOSAIC_MIN);
	slScrMosaicOn(OFF);
        return false;
    }
}

void mosaicRandom(jo_scroll_screen screens) {
    if (frame % MOSAIC_RANDOM_RATE == 0) { // modulus
        mosaic_x = my_random_range(1, rand_max);
        mosaic_y = my_random_range(1, rand_max);
	slScrMosSize(mosaic_x, mosaic_y);
	slScrMosaicOn(screens);
    }
    if (frame % 80 == 0 && rand_max > 7) { // modulus (could use 64 as a power of 2)
	rand_max--;
    }
}

bool musicOut(void) {
    if (volume > MIN_VOLUME) {
        volume--;
        if (volume < MIN_VOLUME) {
            volume = MIN_VOLUME;
        }
        #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        CDDA_SetVolume(volume >> 4);
        #else
        jo_audio_set_volume(volume);
        #endif
        return true;
    }
    else {
        return false;
    }
}

bool musicIn(void) {
    if (volume < MAX_VOLUME) {
        volume += 4;
        if (volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
        #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        CDDA_SetVolume(volume >> 4);
        #else
        jo_audio_set_volume(volume);
        #endif
        return true;
    }
    else {
        return false;
    }
}

bool explosionEffect(void) {
    pcm_play(g_Assets.explod1Pcm8, PCM_PROTECTED, 7);
    fade_out = true;
    nbg1_rate = MAXIMUM_FADE;
    if (!g_GameOptions.debug_display) {
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
    }
    slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    return false;
}