#include <jo/jo.h>
#include "main.h"
#include "highscores.h"
#include "screen_transition.h"
#include "gameplay.h"
#include "sprites.h"
#include "BG_DEF/nbg1.h"

unsigned int g_ScoreTimer = 0;

HighScoreEntry highScores[SCORE_ENTRIES];

void highScore_init(void) {
    highScores[0] = (HighScoreEntry){500000, "CDS"};
    highScores[1] = (HighScoreEntry){450000, "BUB"};
    highScores[2] = (HighScoreEntry){400000, "SES"};
    highScores[3] = (HighScoreEntry){350000, "DAD"};
    highScores[4] = (HighScoreEntry){300000, "OCS"};
    highScores[5] = (HighScoreEntry){250000, "FOO"};
    highScores[6] = (HighScoreEntry){200000, "BAR"};
    highScores[7] = (HighScoreEntry){150000, "PPP"};
    highScores[8] = (HighScoreEntry){125000, "ITS"};
    highScores[9] = (HighScoreEntry){100000, "WUP"};
}

void init_scores(void)
{
    music_in = true;
    fade_in = true;
    transition_in = true;

    g_Game.lastState = GAME_STATE_HIGHSCORES;

    g_ScoreTimer = 0;
    
    if (game_options.mesh_display) {
        menu_bg2.mesh = MESHon;
    }
    else {
        menu_bg2.mesh = MESHoff;
    }
    menu_bg2.spr_id = menu_bg2.anim1.asset[4];
    menu_bg2.zmode = _ZmCC;
    set_spr_position(&menu_bg2, 0, 0, 95);
    set_spr_scale(&menu_bg2, 180, 480);
    
    sortHighScores(highScores);

    playCDTrack(BEGIN_GAME_TRACK, false);
}

static bool draw_header_text = true;
void display_scores(void)
{
    if(g_Game.gameState != GAME_STATE_HIGHSCORES)
    {
        return;
    }
    int options_x = 14;
    int options_y = 4;
    g_ScoreTimer++;
    
    if (JO_MOD_POW2(frame, 8) == 0) { // modulus
        draw_header_text = !draw_header_text;
    }
    if (draw_header_text) {
        jo_nbg0_printf(options_x+3, options_y, "HIGH SCORES");
    }
    options_y += 2;
    for (int i = 0; i < SCORE_ENTRIES; i++) {
        jo_nbg0_printf(options_x, options_y, "%2d. %s - %d", i + 1, highScores[i].initials, highScores[i].score);
        options_y += 2;
    }
    my_sprite_draw(&menu_bg2);
    update_bg_position();    if (g_ScoreTimer == SCORE_DISPLAY_TIME) {
        g_Game.lastState = GAME_STATE_HIGHSCORES;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}

static int backgroundAngle = 360;

// draw an ellipse
void update_bg_position(void) {
    if (JO_MOD_POW2(frame, 2) == 0) {
        backgroundAngle -= 1;
        if (backgroundAngle == 0)
            backgroundAngle = 360;
        attrNbg1.x_pos = FIXED_127 + jo_fixed_mult(jo_cos(backgroundAngle), FIXED_127);
        slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);
    }
}

void    score_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        g_Game.lastState = GAME_STATE_HIGHSCORES;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}

// Sorting function (Descending order)
void sortHighScores(HighScoreEntry scores[]) {
    for (int i = 0; i < SCORE_ENTRIES - 1; i++) {
        for (int j = i + 1; j < SCORE_ENTRIES; j++) {
            if (scores[j].score > scores[i].score) {
                HighScoreEntry temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}
// void addHighScore(Uint16 newScore, const char *initials) {
    // // Check if the new score qualifies
    // if (newScore <= highScores[SCORE_ENTRIES - 1].score) {
        // return;  // Score is too low, ignore
    // }

    // // Insert at the last position
    // highScores[SCORE_ENTRIES - 1].score = newScore;
    // highScores[SCORE_ENTRIES - 1].initials = initials; // causes error

    // // Sort the list
    // sortHighScores(highScores);
// }