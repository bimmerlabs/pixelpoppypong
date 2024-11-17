#include <jo/jo.h>
#include "screen_transition.h"
#include "main.h"
#include "state.h"

unsigned int g_TransitionTimer = 0;
Sint16 nbg0_rate = -254;
Sint16 nbg1_rate = -254;
Sint16 spr_rate  = -254;
Sint8 fadeDirection = 1;
unsigned int g_FadeTimer = 0;

void screenTransition_init(Sint16 nbg0, Sint16 nbg1, Sint16 spr) {
    g_FadeTimer = 0;
    fadeDirection = 1;
    nbg0_rate = nbg0;
    nbg1_rate = nbg1;
    spr_rate  = spr;
}

void screenTransition(Sint16 nbg0_inc, Sint16 nbg1_inc, Sint16 nbg1_min, Uint16 transition_time) {
    // jo_nbg0_clear();
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

void fadeOut(Sint16 rate, Sint16 max) {
    if (nbg1_rate < max) {
        nbg0_rate -= rate;
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
        slColOffsetB(nbg0_rate, nbg0_rate, nbg0_rate);
    }
}

void fadeIn(Sint16 rate, Sint16 min) {
    if (nbg1_rate < min) {
        nbg0_rate += rate;
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
        slColOffsetB(nbg0_rate, nbg0_rate, nbg0_rate);
    }
}