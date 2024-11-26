#include <jo/jo.h>
#include "main.h"
#include "main.h"
#include "assets.h"
#include "title_screen.h"
#include "screen_transition.h"
// #include "objects/player.h"
#include "BG_DEF/BG25.h"

// globals for menu options
int g_TitleScreenChoice = 0;
int g_TitleScreenCursorColor = 0;

unsigned int g_TitleTimer = 0;
static bool draw_start_text = true;
static bool draw_option_mode  = true;
static bool draw_option_difficulty  = true;
static bool draw_option_start  = true;

FIXED pos_x = 0; // for the background - should use the attribute instead
FIXED pos_y = 0;
Uint16 cursor_angle = 0;

typedef enum _MENU_OPTIONS
{
    TITLE_OPTION_GAME_START = 0,
    TITLE_OPTION_GAME_DIFFICULTY = 1,
    TITLE_OPTION_GAME_MODE = 2,
    TITLE_OPTION_MAX,
} MENU_OPTIONS;

static void drawTitle(void);
static void drawMenuCursor(void);
static void drawMenu(void);

// // use the players as moving elements in the background
// extern PLAYER g_Players[MAX_PLAYERS];

void titleScreen_init(void)
{
    if (first_load) {
        // init_bg0_img();
        init_bg25_img();
        first_load = false;
    }
    g_TitleTimer = 0;
    pixel_poppy.scl.x = toFIXED(5);
    pixel_poppy.scl.y = toFIXED(5);
    pixel_poppy.rot.z = 0;
    
    slColOffsetOn(NBG0ON | NBG1ON | SPRON);
    slColOffsetAUse(NBG0ON);
    slColOffsetBUse(NBG1ON | SPRON);
    
    jo_set_default_background_color(JO_COLOR_DarkGray);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
    // init menu options
    g_TitleScreenChoice = 0;


    g_Game.gameMode = GAME_MODE_PRACTICE;
    g_Game.gameDifficulty = GAME_DIFFICULTY_MEDIUM;
    
    // menu_bg1.mesh = MESHon;
    set_spr_position(&menu_bg1, 0, 130, 95);
    
    set_spr_scale(&menu_bg1, 156, 50);

    return;
}

// handles input for the title screen
// only player one can control the title screen
void titleScreen_input(void)
{
    // select an option here
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        changeState(GAME_STATE_TITLE_MENU);
    }
}

// handles input for the title screen
// only player one can control the title screen
void titleScreen_menu(void)
{
    if (jo_is_pad1_key_down(JO_KEY_UP))
    {
            g_TitleScreenChoice++;
    }

    if (jo_is_pad1_key_down(JO_KEY_DOWN))
    {
            g_TitleScreenChoice--;
    }

    if (jo_is_pad1_key_down(JO_KEY_LEFT))
    {
        switch(g_TitleScreenChoice)
        {
            case TITLE_OPTION_GAME_MODE:
                g_Game.gameMode--;
                sanitizeValue((int*)&g_Game.gameMode, 0, GAME_MODE_MAX);
                break;

            case TITLE_OPTION_GAME_DIFFICULTY:
                g_Game.gameDifficulty--;
                sanitizeValue((int*)&g_Game.gameDifficulty, 0, GAME_DIFFICULTY_MAX);
                break;

            default:
                break;
        }
    }

    if (jo_is_pad1_key_down(JO_KEY_RIGHT))
    {
        switch(g_TitleScreenChoice)
            {
            case TITLE_OPTION_GAME_MODE:
                g_Game.gameMode++;
                sanitizeValue((int*)&g_Game.gameMode, 0, GAME_MODE_MAX);
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                g_Game.gameDifficulty++;
                sanitizeValue((int*)&g_Game.gameDifficulty, 0, GAME_DIFFICULTY_MAX);
                break;
            
            default:
                break;
        }
    }

    // keep title screen choice in range
    sanitizeValue(&g_TitleScreenChoice, 0, TITLE_OPTION_MAX);

    // select an option here
    if (jo_is_pad1_key_down(JO_KEY_START) || 
        jo_is_pad1_key_down(JO_KEY_A) ||
        jo_is_pad1_key_down(JO_KEY_C))
    {
        switch(g_TitleScreenChoice)
        {
            case TITLE_OPTION_GAME_START:
                transitionState(GAME_STATE_TEAM_SELECT);
                break;
                
            default:
                break;
        }
    }
    if (jo_is_pad1_key_down(JO_KEY_B) )
    {
        changeState(GAME_STATE_TITLE_SCREEN);
    }
}

void titleScreen_update(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }
    g_TitleTimer++;

    // standard transition-in
    if (!transition_complete && volume < MAX_VOLUME) {
        volume += 2;
        if (volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
        jo_audio_set_volume(volume);
    }
    if (!transition_complete) {
        transition_complete = fadeIn(fade_rate, NEUTRAL_FADE);
    }
    
    // check if the frameAnim has expired
    if(g_TitleTimer > TITLE_TIMER)
    {
        transitionState(GAME_STATE_DEMO_LOOP);
        g_TitleTimer = 0;
    }
}

void titleScreen_draw(void)
{    
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN && g_Game.gameState != GAME_STATE_TITLE_MENU)
    {
        return;
    }
    
    if (attrBg250.x_scroll > toFIXED(0)) {
        pos_x += attrBg250.x_scroll;
        if (pos_x > toFIXED(512.0))
            pos_x = toFIXED(0);
    }
    if (attrBg250.y_scroll > toFIXED(0)) {
        pos_y += attrBg250.y_scroll;
        if (pos_y > toFIXED(512.0))
            pos_y = toFIXED(0);
    }
    slScrPosNbg1(pos_x, pos_y);
    
    drawMenu();
    drawMenuCursor();
    drawTitle();
}

//
// Title screen helpers
//

// draws title image + version number
static void drawTitle(void)
{
    my_sprite_draw(&pixel_poppy);
    // version number
    jo_nbg0_printf(20, 27, "%s", VERSION);
    // title graphic
    switch (g_Game.gameState) {
        case GAME_STATE_TITLE_SCREEN:
            jo_nbg0_printf(13, 23, "PIXEL POPPY PONG!");
            // jo_nbg0_printf(14, 23, "GAME LOGO HERE!");
            if (frame % 15 == 0) {
                draw_start_text = !draw_start_text;
            }
            if (draw_start_text) {
                jo_nbg0_printf(16, 25, "PRESS START");
            }
            break;

        default:
            break;
    }
}

// Options menu + values
static void drawMenu(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_MENU)
    {
        return;
    }

    switch (g_TitleScreenChoice) {
        case TITLE_OPTION_GAME_MODE:
            if (frame % 10 == 0) {
                draw_option_mode = !draw_option_mode;
                draw_option_difficulty = true;
                draw_option_start = true;
            }
            break;

        case TITLE_OPTION_GAME_DIFFICULTY:
            if (frame % 10 == 0) {
                draw_option_difficulty = !draw_option_difficulty;
                draw_option_mode = true;
                draw_option_start = true;
            }
            break;

        case TITLE_OPTION_GAME_START:
            if (frame % 10 == 0) {
                draw_option_start = !draw_option_start;
                draw_option_difficulty = true;
                draw_option_mode = true;
            }
            break;

        default:
            break;
    }
    // game mode (practice, normal, hardcore, time attack)
    menu_text.pos.x = toFIXED(-5);
    menu_text.pos.y = toFIXED(100);
    menu_text.zmode = _ZmRC;
    menu_text.spr_id = menu_text.anim1.asset[MODE_ID];
    my_sprite_draw(&menu_text); // mode

    menu_text.pos.x = toFIXED(5);
    menu_text.zmode = _ZmLC;
    menu_text.spr_id = menu_text.anim1.asset[g_Game.gameMode+MODE_OFFSET];
    if (draw_option_mode)
        my_sprite_draw(&menu_text); // practice, normal, hardcore, time attack

    // Difficulty: (easy, medium, hard)
    menu_text.pos.x = toFIXED(-5);
    menu_text.pos.y = toFIXED(130);
    menu_text.zmode = _ZmRC;
    menu_text.spr_id = menu_text.anim1.asset[DIFF_ID];
    my_sprite_draw(&menu_text); // difficulty

    menu_text.pos.x = toFIXED(5);
    menu_text.zmode = _ZmLC;
    menu_text.spr_id = menu_text.anim1.asset[g_Game.gameDifficulty+DIFF_OFFSET];
    if (draw_option_difficulty)
        my_sprite_draw(&menu_text); // easy, medium, hard

    // // Start!
    menu_text.pos.x = toFIXED(0);
    menu_text.pos.y = toFIXED(160);
    menu_text.zmode = _ZmCC;
    menu_text.spr_id = menu_text.anim1.asset[START_ID];
    if (draw_option_start)
        my_sprite_draw(&menu_text); // start

    my_sprite_draw(&menu_bg1); // shadow

}

// flag used as the menu cursor
static void drawMenuCursor(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_MENU)
    {
        return;
    }
    FIXED offset = jo_fixed_mult(jo_fixed_sin(jo_fixed_deg2rad(toFIXED(cursor_angle))), toFIXED(8));
    cursor.pos.x = toFIXED(-147) + offset;
    cursor.pos.y = toFIXED(170 + (g_TitleScreenChoice * -30)); // vertical position varies based on selection
    my_sprite_draw(&cursor);
    cursor_angle += 8;
    if (cursor_angle > 360) {
        cursor_angle = 0;
    }
}
