#include <jo/jo.h>
#include "screen_transition.h"
#include "main.h"
#include "state.h"
#include "math.h"
#include "assets.h"

// leave separate for now
Sint16 nbg0_rate = MINIMUM_FADE;
Sint16 nbg1_rate = MINIMUM_FADE;
Sint16 spr_rate  = MINIMUM_FADE;

static int rand_max = MOSAIC_MAX;

TRANSITION g_Transition = {0};

void initTransitionStruct(void) {
    g_Transition.timer = 0;
    
    g_Transition.all_out = false;
    g_Transition.all_in = false;
    
    g_Transition.fade_in_rate = 8;
    g_Transition.fade_out_rate = 4;
    g_Transition.fade_out = false;
    g_Transition.fade_in = false;    
    g_Transition.story_fade_out = false;
    g_Transition.story_fade_in = false;
    g_Transition.slow_fade_in = false;
    g_Transition.explosion_flash = false;

    g_Transition.mosaic_in_rate = MOSAIC_SLOW_RATE;
    g_Transition.mosaic_x = MOSAIC_MIN;
    g_Transition.mosaic_y = MOSAIC_MIN;
    g_Transition.mosaic_out = false;
    g_Transition.mosaic_in = false;

    g_Transition.music_out = false;
    g_Transition.music_in = false;
}


void screenTransition_init(Sint16 nbg0, Sint16 nbg1, Sint16 spr) {
    nbg0_rate = nbg0;
    nbg1_rate = nbg1;
    spr_rate  = spr;
}

void screenTransition_update(void) {
    if (!g_Transition.all_out && !g_Transition.all_in) {
        return;
    }
    if (g_Transition.all_out) {
        g_Transition.all_out = transitionOut();
    }
    else if (g_Transition.all_in) {
        g_Transition.all_in = transitionIn();
    }
}

bool transitionOut(void) {
    if (!g_Transition.fade_out && !g_Transition.story_fade_out && !g_Transition.mosaic_out && !g_Transition.music_out) {
        return false;
    }
    if (g_Transition.fade_out) {
        g_Transition.fade_out = fadeOut(g_Transition.fade_out_rate, MINIMUM_FADE);
    }
    if (g_Transition.story_fade_out) {
        g_Transition.story_fade_out = fadeOut(g_Transition.fade_out_rate, STORY_FADE);
    }
    if (g_Transition.mosaic_out) {
        g_Transition.mosaic_out = mosaicOut(NBG1ON);
    }
    if (g_Transition.music_out) {
        g_Transition.music_out = musicOut();
    }
    return true;
}

bool transitionIn(void) {
    if (!g_Transition.fade_in && !g_Transition.mosaic_in && !g_Transition.music_in && !g_Transition.slow_fade_in && !g_Transition.story_fade_in) {
        return false;
    }
    if (g_Transition.fade_in) {
        g_Transition.fade_in = fadeIn(g_Transition.fade_in_rate, NEUTRAL_FADE);
    }
    if (g_Transition.slow_fade_in) {
        g_Transition.slow_fade_in = slowFadeIn(g_Transition.fade_in_rate, QUARTER_FADE);
    }
    if (g_Transition.story_fade_in) {
        g_Transition.story_fade_in = fadeIn(g_Transition.fade_in_rate, STORY_FADE);
    }
    if (g_Transition.mosaic_in) {
        g_Transition.mosaic_in = mosaicIn(NBG1ON);
    }
    if (g_Transition.music_in) {
        g_Transition.music_in = musicIn();
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
    if (nbg1_rate < max && JO_MOD_POW2(g_Game.frame, 4) == 0) { // modulus
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
    g_Transition.mosaic_x = MOSAIC_MAX;
    g_Transition.mosaic_y = MOSAIC_MAX;
    slScrMosSize(g_Transition.mosaic_x, g_Transition.mosaic_y);
    slScrMosaicOn(screens);
}

bool mosaicOut(jo_scroll_screen screens) {
    if (g_Transition.mosaic_x < MOSAIC_MAX && g_Transition.mosaic_y < MOSAIC_MAX) {
        if (g_Game.frame % MOSAIC_FAST_RATE == 0) { // modulus
            g_Transition.mosaic_x++;
            if (g_Transition.mosaic_x > MOSAIC_MAX)
                g_Transition.mosaic_x = MOSAIC_MAX;
            g_Transition.mosaic_y++;
            if (g_Transition.mosaic_y > MOSAIC_MAX)
                g_Transition.mosaic_y = MOSAIC_MAX;
        }
	slScrMosSize(g_Transition.mosaic_x, g_Transition.mosaic_y);
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
    if (g_Transition.mosaic_x > MOSAIC_MIN+1 && g_Transition.mosaic_y > MOSAIC_MIN+1) {
        if (g_Game.frame % g_Transition.mosaic_in_rate == 0) { // modulus
            g_Transition.mosaic_x--;
            if (g_Transition.mosaic_x < MOSAIC_MIN)
                g_Transition.mosaic_x = MOSAIC_MIN;
            g_Transition.mosaic_y--;
            if (g_Transition.mosaic_y < MOSAIC_MIN)
                g_Transition.mosaic_y = MOSAIC_MIN;
        }
	slScrMosSize(g_Transition.mosaic_x, g_Transition.mosaic_y);
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
    if (g_Game.frame % MOSAIC_RANDOM_RATE == 0) { // modulus
        g_Transition.mosaic_x = my_random_range(1, rand_max);
        g_Transition.mosaic_y = my_random_range(1, rand_max);
	slScrMosSize(g_Transition.mosaic_x, g_Transition.mosaic_y);
	slScrMosaicOn(screens);
    }
    if (g_Game.frame % 80 == 0 && rand_max > 7) { // modulus (could use 64 as a power of 2)
	rand_max--;
    }
}

bool musicOut(void) {
    if (g_Audio.masterVolume > MIN_VOLUME) {
        g_Audio.masterVolume--;
        if (g_Audio.masterVolume < MIN_VOLUME) {
            g_Audio.masterVolume = MIN_VOLUME;
        }
        CDDA_SetVolume(volume_shift(g_Audio.masterVolume));
        return true;
    }
    else {
        return false;
    }
}

bool musicIn(void) {
    if (g_Audio.masterVolume < MAX_VOLUME) {
        g_Audio.masterVolume += 4;
        if (g_Audio.masterVolume > MAX_VOLUME) {
            g_Audio.masterVolume = MAX_VOLUME;
        }
        CDDA_SetVolume(volume_shift(g_Audio.masterVolume));
        return true;
    }
    else {
        return false;
    }
}

bool explosionEffect(void) {
    pcm_play(g_Assets.explod1Pcm8, PCM_PROTECTED, 7);
    g_Transition.fade_out = true;
    nbg1_rate = MAXIMUM_FADE;
    if (!g_GameOptions.debug_display) {
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
    }
    slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    return false;
}