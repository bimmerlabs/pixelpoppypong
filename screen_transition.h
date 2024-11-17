#pragma once

extern unsigned int g_TransitionTimer;
extern Sint16 nbg0_rate;
extern Sint16 nbg1_rate;
extern Sint8 fadeDirection;
extern unsigned int g_FadeTimer;

void screenTransition_init(Sint16 nbg0, Sint16 nbg1, Sint16 spr);
void screenTransition(Sint16 nbg0_inc, Sint16 nbg1_inc, Sint16 nbg1_min, Uint16 transition_time);
void fadeOut(Sint16 rate, Sint16 max);
void fadeIn(Sint16 rate, Sint16 min);