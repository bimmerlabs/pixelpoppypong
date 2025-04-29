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
    
    set_spr_position(player->_cursor, toFIXED(0), toFIXED(0), CURSOR_DEPTH);
    player->objectState = OBJECT_STATE_ACTIVE;
    player->isPlaying = PLAYING;
    player->isAI = false;
    boundPlayer(player);
    draw_story_cursor = false;
    // resetSpriteColors();
    
    slScrPosNbg0(toFIXED(0), toFIXED(4));
    
    initStoryCharacters();
    initVsModePlayers();
}

// main logic loop
void storySelectUpdate(void)
{
    character_offset = ((g_Game.countofRounds) * CHARACTER_POS_OFFSET);
    jo_nbg0_printf(2, 14, "%s", fullCharacterNames[0]);
    jo_nbg0_printf(26, 14, "%s", fullCharacterNames[g_Game.countofRounds+1]);
    // scroll up list of characters
    if (character_pos_selected < character_offset) {
        character_pos_selected++;
    }
    else {
        jo_nbg0_printf(2, 16, "%s", characterQuotes[0]);
        jo_nbg0_printf(21, 19, "VS.");
        jo_nbg0_printf(26, 16, "%s", characterQuotes[g_Game.countofRounds+1]);
    }
    if (g_StartStoryFrames == 0)
    {
        g_Game.roundBeginTimer = ROUND_BEGIN_TIME_FAST;
        g_Game.dropBallTimer = DROP_BALL_TIME_FAST;
        transition_in = true;
        if (g_GameOptions.mosaic_display) {
            mosaic_in = true;
        }
        fade_in = true;
        g_Game.selectStoryCharacter = false;
        slScrPosNbg0(toFIXED(0), toFIXED(0));
    }
    
    if (frame % 3 == 0) { // modulus
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
    
    if (attrNbg1.x_scroll > toFIXED(0)) {
        attrNbg1.x_pos += attrNbg1.x_scroll;
        if (attrNbg1.x_pos > toFIXED(512.0))
            attrNbg1.x_pos = toFIXED(0);
    }
    slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);    
}

void resetSpriteColors(void) {
    reset_sprites();
    do_update_All = true;
    updateAllColors();
    updateAllPalette();
}

