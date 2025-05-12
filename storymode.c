#include <jo/jo.h>
#include "main.h"
#include "storymode.h"
#include "assets.h"
#include "screen_transition.h"
#include "objects/player.h"
#include "BG_DEF/nbg1.h"

extern PLAYER g_Players[MAX_PLAYERS];

int character_pos_x = CHARACTER_POS_X;
int character_pos_y = 0;
int character_pos_selected = CHARACTER_POS_Y;
int character_offset = 0;
int g_StartStoryFrames = 0;
bool draw_story_cursor = false;

const char *fullCharacterNames[] = {
    "   MACCHIATO",
    "   JELLY BEAN",
    "  QUEEN PENNY",
    "  HAIRY POTTER",
    "    SPARTA",
    " PRINCESS POPPY",
    "    TOE JAM",
    "     GEORGE",
    "      WUPPY",
    "    CRAIG S.",
    "   GARFIELD",
};

const char *characterQuotes[] = {
    " I'M THE BEST!",
    " HELLO?  HELLO??",
    "NO, I'M THE BEST!",
    "THE CAT WHO LIVED",
    "  I LOVE LAMP!  ",
    " I'M SO FLUFFY!!",
    "ANYONE SEEN EARL?",
    " I'M NOT GRUMPY!",
    "GIVE ME THE BALL?",
    "  GREAT SHOT!!",
    "I HATE MONDAYS..",
};

void initStoryMode(void)
{    
    g_Game.selectStoryCharacter = true;
    character_pos_selected = CHARACTER_POS_Y;
    character_offset = (g_Game.countofRounds * CHARACTER_POS_OFFSET); // starts as 0
    
    if (g_GameOptions.mesh_display) {
        menu_bg2.mesh = MESHon;
    }
    else {
        menu_bg2.mesh = MESHoff;
    }
    menu_bg2.zmode = _ZmCC;
    menu_bg2.spr_id = menu_bg2.anim1.asset[4];
    set_spr_position(&menu_bg2, 0, 0, MENU_BG2_DEPTH);
    set_spr_scale(&menu_bg2, 54, 352);
    
    g_Game.countofRounds = 0;
    g_StartStoryFrames = CHARACTER_SELECT_TIMER;
    
    g_Game.numPlayers = TWO_PLAYER;
    reset_inputs();
    initPlayers();
    initTeams();
    g_Game.currentNumPlayers = 2;              

    PPLAYER player = &g_Players[0]; // macchi
    player->input = &g_Inputs[0];
    player->input->id = 0;
    player->input->isSelected = true;
    player->teamChoice = TEAM_1;
    player->teamSelected = true;
    g_Team.isAvailable[player->teamChoice] = false;
    g_Team.numTeams = ONE_TEAM;
    // TODO: move to character select
    player->_sprite = &macchi;
    player->character.selected = true;
    player->character.choice = CHARACTER_MACCHI;
    characterAvailable[player->character.choice] = false;
    player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];
    assignCharacterStats(player);
    
    set_spr_position(player->_cursor, FIXED_0, FIXED_0, CURSOR_DEPTH);
    player->objectState = OBJECT_STATE_ACTIVE;
    player->isPlaying = PLAYING;
    player->isAI = false;
    boundPlayer(player);
    draw_story_cursor = false;
    // resetSpriteColors();
    
    slScrPosNbg0(FIXED_0, FIXED_4);
    
    initStoryCharacters();
    initVsModePlayers();
}

void initContinue(void) {
    setGameTimer();
    g_Players[0].numLives = getLives(&g_Players[0]);
    g_Players[1].numLives = getLives(&g_Players[1]);
    g_Players[0].subState = PLAYER_STATE_ACTIVE;
    g_Players[1].subState = PLAYER_STATE_ACTIVE;
    g_Players[0].score.points = 0;
    g_Players[1].score.points = 0;
    g_Players[0].score.stars = 0;
    g_Players[1].score.stars = 0;
    g_Game.endDelayTimer = GAME_END_DELAY_TIMEOUT;
    g_Game.currentNumPlayers = 2;
    initVsModePlayers();
    resetTeamState();

    initPixelPoppy();
    g_Gameplay.start_gameplay_timer = false;
    g_Gameplay.round_start = false;
    g_Game.isBallActive = false;
    g_Game.isActive = false;
    g_Game.BeginTimer = 0;
    g_Game.winner = -2;
    initTouchCounter(1);

    boundPlayer(&g_Players[0]);
    boundPlayer(&g_Players[1]);
    g_StartStoryFrames = CHARACTER_SELECT_TIMER;
    g_Game.selectStoryCharacter = true;
    resetSpriteColors(); // doesn't do anything yet
    initGoalColors();
    g_Game.time_over = false;
    g_Game.isRoundOver = false;
    slScrPosNbg0(FIXED_0, FIXED_4);
    playCDTrack(BEGIN_GAME_TRACK, false);
}

void initNextRound(void) {
    setGameTimer();
    g_Players[1].numLives = getLives(&g_Players[1]);
    g_Players[0].score.points = 0;
    g_Players[1].score.points = 0;
    g_Players[0].score.stars = 0;
    g_Players[1].score.stars = 0;
    g_Players[1].score.continues = 0;
    g_Game.endDelayTimer = GAME_END_DELAY_TIMEOUT;
    g_Game.currentNumPlayers = 2;
    nextStoryCharacter();
    sprite_frame_reset(&pixel_poppy);
    g_StartStoryFrames = CHARACTER_SELECT_TIMER;
    g_Game.selectStoryCharacter = true;
    resetSpriteColors(); // doesn't do anything yet
    initGoalColors();
    
    g_Game.time_over = false;
    g_Game.isRoundOver = false;
    g_Game.winner = -2;
    slScrPosNbg0(FIXED_0, FIXED_4);
    playCDTrack(BEGIN_GAME_TRACK, false);
}

// main logic loop
void storySelectUpdate(void)
{
    character_offset = ((g_Game.countofRounds) * CHARACTER_POS_OFFSET);
    jo_nbg0_printf(2, 11, "%s", fullCharacterNames[0]);
    jo_nbg0_printf(26, 14, "%s", fullCharacterNames[g_Game.countofRounds+1]);
    
        character_portrait.spr_id = character_portrait.anim1.asset[0];
        set_spr_position(&character_portrait, -200, 0, PORTRAIT_DEPTH);
        set_spr_scale(&character_portrait, 2.0, 2.0);
        my_sprite_draw(&character_portrait);
        
    // scroll up list of characters
    if (character_pos_selected < character_offset) {
        character_pos_selected++;
    }
    else {
        jo_nbg0_printf(2, 19, "%s", characterQuotes[0]);
        jo_nbg0_printf(21, 19, "VS.");
        jo_nbg0_printf(26, 16, "%s", characterQuotes[g_Game.countofRounds+1]);
    }
    if (g_StartStoryFrames == 0)
    {
        g_Game.roundBeginTimer = ROUND_BEGIN_TIME_FAST;
        g_Game.dropBallTimer = DROP_BALL_TIME_FAST;
        g_Transition.all_in = true;
        if (g_GameOptions.mosaic_display) {
            g_Transition.mosaic_in = true;
        }
        g_Transition.fade_in = true;
        g_Game.selectStoryCharacter = false;
        slScrPosNbg0(FIXED_0, FIXED_0);
    }
    
    if (g_Game.frame % 3 == 0) { // modulus
        draw_story_cursor = !draw_story_cursor;
    }
    if (draw_story_cursor) {
        g_Players[0]._cursor->mesh = MESHon;
    }
    else {
        g_Players[0]._cursor->mesh = MESHoff;
    }
    drawCharacterList();
    g_StartStoryFrames--;
    
    if (attrNbg1.x_scroll > FIXED_0) {
        attrNbg1.x_pos += attrNbg1.x_scroll;
        if (attrNbg1.x_pos > toFIXED(512.0))
            attrNbg1.x_pos = FIXED_0;
    }
    slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);    
}

void tallyScore(void) {
    unsigned int thresholds[] = {100000, 10000, 1000, 50, 1};
    jo_nbg0_printf(10, 12, "ROUND SCORE:%09d", g_Players[0].score.points);
    jo_nbg0_printf(10, 14, "TOTAL SCORE:%09d", g_Players[0].score.total);
    if (g_Game.countofRounds < MAX_ROUNDS) {
        draw_heart_element(&heart, &g_Players[0], -64, 24, 16);
    }
    
    for (Uint8 i = 0; i < 5; i++) {
        unsigned int threshold = thresholds[i];
        if (g_Players[0].score.points > threshold) {
            pcm_play(g_Assets.scoreAddPcm8, PCM_PROTECTED, 7);
            g_Players[0].score.points -= threshold;
            g_Players[0].score.total += threshold;
            return;
        }
    }
    // additional lives    
    if (g_Game.endDelayTimer < LIFE_COUNT_DELAY_TIMEOUT && g_Game.countofRounds < MAX_ROUNDS) {
        unsigned int millions = g_Players[0].score.total / 1000000;
        if (millions > g_Players[0].score.lastMillion) {
            unsigned int livesToAdd = millions - g_Players[0].score.lastMillion;
            for (unsigned int i = 0; i < livesToAdd; i++) {
                if (g_Players[0].numLives < g_Players[0].totalLives * 2) {
                    g_Players[0].numLives += 1;
                }
            }
            g_Players[0].score.lastMillion = millions;      
        }
    }
    // additional time;
    if (g_Game.endDelayTimer < LIFE_COUNT_DELAY_TIMEOUT && g_Game.countofRounds < MAX_ROUNDS) {
        Uint16 addedTime = 0;
        switch(g_Game.gameDifficulty)
        {
            case GAME_DIFFICULTY_EASY:
                addedTime = 30;
                break;
            case GAME_DIFFICULTY_MEDIUM:
                addedTime = 20;
                break;
            case GAME_DIFFICULTY_HARD:
                addedTime = 10;
                break;
            default:
                break;
        }
        if (touchedBy[0].touchCount > addedTime) {
            addedTime = touchedBy[0].touchCount;     
        }
        jo_nbg0_printf(14, 18, "EXTRA TIME:%i", addedTime);
    }
    if (g_Game.endDelayTimer == LIFE_COUNT_DELAY_TIMEOUT && g_Game.countofRounds < MAX_ROUNDS) {
        pcm_play(g_Assets.startPcm8, PCM_PROTECTED, 7);
    }  
    else if (g_Game.endDelayTimer == WIN_GAME_DELAY_TIMEOUT && g_Game.countofRounds == MAX_ROUNDS) {
        pcm_play(g_Assets.winPcm8, PCM_PROTECTED, 7);
    }  

    if (g_Players[0].score.points > 0) {
        pcm_play(g_Assets.scoreTotalPcm8, PCM_PROTECTED, 7);
        g_Players[0].score.points--;
        g_Players[0].score.total++;
    } 
    else {
        g_Game.endDelayTimer--;
    }
}