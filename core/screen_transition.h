#pragma once

#define MINIMUM_FADE -254
#define HALF_FADE -127
#define QUARTER_FADE -63
#define EIGTH_FADE -31
#define NEUTRAL_FADE 0
#define MAXIMUM_FADE 254 // 255?
#define PAUSE_FADE -75
#define STORY_FADE -100

#define MOSAIC_MIN  1
#define MOSAIC_MAX  16
#define MOSAIC_RANDOM_RATE  2
#define MOSAIC_SLOW_RATE  4
#define MOSAIC_FAST_RATE  2

extern Sint16 nbg0_rate;
extern Sint16 nbg1_rate;
extern Sint16 spr_rate;

typedef struct _TRANSITION
{
	unsigned int timer;
	
	bool all_out;
	bool all_in;
	
	Sint8 fade_in_rate;
	Sint8 fade_out_rate;
	bool fade_out;
	bool fade_in;
	bool story_fade_out;
	bool story_fade_in;
	bool slow_fade_in;
	bool explosion_flash;

	Uint8 mosaic_in_rate;
	unsigned short mosaic_x;
	unsigned short mosaic_y;
	bool mosaic_out;
	bool mosaic_in;

	bool music_out;
	bool music_in;
} TRANSITION;

extern TRANSITION g_Transition;

void initTransitionStruct(void);
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
bool explosionEffect(void);