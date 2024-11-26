#pragma once

#define MINIMUM_FADE -254
#define NEUTRAL_FADE 0
#define MAXIMUM_FADE 254

extern unsigned int g_TransitionTimer;
extern Sint16 nbg0_rate;
extern Sint16 nbg1_rate;
extern Sint8 fadeDirection;
extern Sint8 fade_rate;
extern unsigned int g_FadeTimer;

void screenTransition_init(Sint16 nbg0, Sint16 nbg1, Sint16 spr);
void screenTransition(Sint16 nbg0_inc, Sint16 nbg1_inc, Sint16 nbg1_min, Uint16 transition_time);
bool fadeOut(Sint16 rate, Sint16 max);
bool fadeIn(Sint16 rate, Sint16 min);