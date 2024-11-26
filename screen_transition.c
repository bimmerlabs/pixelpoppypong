#include <jo/jo.h>
#include "screen_transition.h"
#include "main.h"
#include "state.h"

unsigned int g_TransitionTimer = 0;
Sint16 nbg0_rate = MINIMUM_FADE;
Sint16 nbg1_rate = MINIMUM_FADE;
Sint16 spr_rate  = MINIMUM_FADE;
Sint8 fadeDirection = 1;
Sint8 fade_rate = 4;
unsigned int g_FadeTimer = 0;

void screenTransition_init(Sint16 nbg0, Sint16 nbg1, Sint16 spr) {
    g_FadeTimer = 0;
    fadeDirection = 1;
    nbg0_rate = nbg0;
    nbg1_rate = nbg1;
    spr_rate  = spr;
}

// mainly for logo screen (rename?)
void screenTransition(Sint16 nbg0_inc, Sint16 nbg1_inc, Sint16 nbg1_min, Uint16 transition_time) {
    if (fadeDirection == 1 && nbg0_rate < 0) {
        nbg0_rate += nbg0_inc;
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
    }
    if (fadeDirection == 1 && nbg1_rate < 0) {
        nbg1_rate += nbg1_inc;
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        if (nbg1_rate == nbg1_min) {
            fadeDirection = -1; // Switch to fade in
        }
    }
    if (fadeDirection == -1 && g_FadeTimer < transition_time) {
        g_FadeTimer++;
    }
    if (g_FadeTimer == transition_time && nbg0_rate > -254) {
        nbg0_rate -= nbg0_inc;
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
    }
    if (g_FadeTimer == transition_time && nbg1_rate > -254) {
        nbg1_rate -= nbg1_inc;
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    }
}

bool fadeOut(Sint16 rate, Sint16 min) {
    if (nbg0_rate > min) {
        nbg0_rate -= rate;
        if (nbg0_rate < min) {
            nbg0_rate = min;
        }
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
        slColOffsetB(nbg0_rate, nbg0_rate, nbg0_rate);
        return false;
    }
    else {
        return true;
    }
}

bool fadeIn(Sint16 rate, Sint16 max) {
    if (nbg0_rate < max) {
        nbg0_rate += rate;
        if (nbg0_rate > max) {
            nbg0_rate = max;
        }
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
        slColOffsetB(nbg0_rate, nbg0_rate, nbg0_rate);
        return false;
    }
    else {
        return true;
    }
}