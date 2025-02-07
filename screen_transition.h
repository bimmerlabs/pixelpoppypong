#pragma once

#define MINIMUM_FADE -254
#define HALF_FADE -127
#define QUARTER_FADE -63
#define EIGTH_FADE -31
#define NEUTRAL_FADE 0
#define MAXIMUM_FADE 254
#define PAUSE_FADE -75

#define MOSAIC_MIN  1
#define MOSAIC_MAX  16
#define MOSAIC_RANDOM_RATE  2
#define MOSAIC_SLOW_RATE  4
#define MOSAIC_FAST_RATE  2

extern unsigned int g_TransitionTimer;
extern bool transition_out;
extern bool transition_in;
extern Sint16 nbg1_rate;
extern Sint16 nbg1_rate;
extern Sint8 fadeDirection;
extern Sint8 fade_in_rate;
extern Sint8 fade_out_rate;
extern Uint8 mosaic_in_rate;
extern unsigned int g_FadeTimer;
extern bool fade_out;
extern bool fade_in;
extern bool slow_fade_in;

extern unsigned short mosaic_x;
extern unsigned short mosaic_y;
extern bool mosaic_out;
extern bool mosaic_in;

extern bool music_out;
extern bool music_in;

void screenTransition_init(Sint16 nbg0, Sint16 nbg1, Sint16 spr);
void screenTransition_update(void);
bool transitionOut(void);
bool transitionIn(void);
bool fadeOut(Sint16 rate, Sint16 max);
bool fadeIn(Sint16 rate, Sint16 min);
bool slowFadeIn(Sint16 rate, Sint16 max);
void mosaicInit(jo_scroll_screen screens);
bool mosaicOut(jo_scroll_screen screens);
bool mosaicIn(jo_scroll_screen screens);
void mosaicRandom(jo_scroll_screen screens);
bool musicOut(void);
bool musicIn(void);