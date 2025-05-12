#include <jo/jo.h>
#include "main.h"
#include "highscores.h"
#include "screen_transition.h"
#include "gameplay.h"
#include "sprites.h"
#include "name_entry.h"
#include "backup.h"
#include "BG_DEF/nbg1.h"

static unsigned int highScoreTimer = 0;

HighScoreEntry highScores[SCORE_ENTRIES];

void highScore_init(void) {
    highScores[0] = (HighScoreEntry){10000000, "CDS"}; // could put an actual score here
    highScores[1] = (HighScoreEntry){5000000, "GBA"};
    highScores[2] = (HighScoreEntry){4500000, "SES"};
    highScores[3] = (HighScoreEntry){4000000, "OCS"};
    highScores[4] = (HighScoreEntry){3500000, "WUP"};
    highScores[5] = (HighScoreEntry){3000000, "FOO"};
    highScores[6] = (HighScoreEntry){2500000, "CRS"};
    highScores[7] = (HighScoreEntry){2000000, "BAR"};
    highScores[8] = (HighScoreEntry){1500000, "PPP"};
    highScores[9] = (HighScoreEntry){1000000, "DAD"};
}

void init_scores(void)
{
    if (g_Game.lastState == GAME_STATE_NAME_ENTRY) {
        unloadNameEntryAssets();
    }
    g_Transition.fade_in = true;
    g_Transition.all_in = true;
    
    #if ENABLE_DEBUG_MODE == 1
    if (g_GameOptions.debug_mode) { // only needed if manually changing states
        reset_sprites();
        do_update_All = true;
        updateAllColors();
        updateAllPalette();
        set_spr_scale(&pixel_poppy, 6, 6);
        pixel_poppy.rot.z = 0;
        set_spr_position(&pixel_poppy, 0, 0, 100);
        sprite_frame_reset(&pixel_poppy);
    }
    #endif
    
    highScoreTimer = 0;
    
    if (g_GameOptions.mesh_display) {
        menu_bg2.mesh = MESHon;
    }
    else {
        menu_bg2.mesh = MESHoff;
    }
    menu_bg2.spr_id = menu_bg2.anim1.asset[4];
    menu_bg2.zmode = _ZmCC;
    set_spr_position(&menu_bg2, 0, 0, 95);
    set_spr_scale(&menu_bg2, 200, 480);
    
    sortHighScores(highScores);

    g_Audio.masterVolume = MAX_VOLUME;
    reset_audio(g_Audio.masterVolume);
    playCDTrack(FINISH_TRACK, false);
}
static bool draw_header_text = true;
void display_scores(void)
{
    if(g_Game.gameState != GAME_STATE_HIGHSCORES)
    {
        return;
    }
    int options_x = 12;
    int options_y = 4;
    highScoreTimer++;
    
    if (JO_MOD_POW2(g_Game.frame, 8) == 0) { // modulus
        draw_header_text = !draw_header_text;
    }
    if (draw_header_text) {
        jo_nbg0_printf(options_x+5, options_y, "HIGH SCORES");
    }
    options_y += 2;
    for (int i = 0; i < SCORE_ENTRIES; i++) {
        jo_nbg0_printf(options_x, options_y, "%2d. %s - %09d", i + 1, highScores[i].initials, highScores[i].score);
        options_y += 2;
    }
    my_sprite_draw(&menu_bg2);
    update_bg_position();    if (highScoreTimer == SCORE_DISPLAY_TIME) {
        if (g_Game.lastState == GAME_STATE_NAME_ENTRY) {
            g_Game.lastState = GAME_STATE_HIGHSCORES;
            transitionState(GAME_STATE_CREDITS);
        }
        else {
            g_Game.lastState = GAME_STATE_HIGHSCORES;
            transitionState(GAME_STATE_UNINITIALIZED);
        }
    }
}
static int backgroundAngle = 360;

// draw an ellipse
void update_bg_position(void) {
    if (JO_MOD_POW2(g_Game.frame, 2) == 0) {
        backgroundAngle -= 1;
        if (backgroundAngle == 0)
            backgroundAngle = 360;
        attrNbg1.x_pos = FIXED_127 + jo_fixed_mult(jo_cos(backgroundAngle), FIXED_127);
        slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);
    }
}

void    score_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START) && g_Game.lastState == GAME_STATE_NAME_ENTRY) {
        g_Game.lastState = GAME_STATE_HIGHSCORES;
        transitionState(GAME_STATE_CREDITS);
    }
    else if (jo_is_pad1_key_down(JO_KEY_START) && g_Game.lastState != GAME_STATE_NAME_ENTRY) {
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

void addHighScore(unsigned int newScore, const char *initials) {
    // Check if the new score qualifies
    if (newScore <= highScores[SCORE_ENTRIES - 1].score) {
        return;  // Score is too low, ignore
    }
    // Insert at the last position
    highScores[SCORE_ENTRIES - 1].score = newScore;
    
    for (int i = 0; i <= MAX_INITIAL; ++i) {
        highScores[SCORE_ENTRIES - 1].initials[i] = initials[i];
    }

    // Sort the list
    sortHighScores(highScores);
}

void calculateScore(Sprite *ball, Uint8 playerID) {
    unsigned int points = (JO_ABS(toINT(ball->vel.x)) * 50) 
                         + (JO_ABS(toINT(ball->vel.y)) * 500) 
                         + (JO_ABS(toINT(ball->vel.z)) * 1000);
    g_Players[playerID].score.points += points * touchedBy[playerID].touchCount;
    ballTtouchTimer = 0;
}

void updateScore(Sprite *ball, int playerID) {      
    if (g_Players[playerID].subState == PLAYER_STATE_DEAD) {
        return;
    }
    // Assign score if a valid player touched it
    if (lastTouchedBy != -1 && playerID != lastTouchedBy) {
        calculateScore(ball, lastTouchedBy);        
    }
    else {
        for (Uint8 i = 0; i < 3; i++) {
            Sint8 id = previouslyTouchedBy[i];
            if (id == -1) continue;
            if (g_Players[id].subState == PLAYER_STATE_DEAD) continue;
            if (g_Players[id].objectState == OBJECT_STATE_INACTIVE) continue;
            if (id == playerID) continue;
            if (g_Players[playerID].onLeftSide == g_Players[id].onLeftSide) continue;
            calculateScore(ball, id);
            break;
        }
    }
    updatePlayerLives(playerID);
    g_Game.isGoalScored = true; // still set to true - no score assigned to a player, but somebody loses a life
    playCDTrack(g_Audio.goalScoredTrack, false);
    nextGoalScoredTrack();
}
