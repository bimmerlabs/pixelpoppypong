#include <jo/jo.h>
#include "main.h"
#include "main.h"
#include "assets.h"
#include "title_screen.h"
#include "screen_transition.h"
// #include "objects/player.h"
#include "BG_DEF/BG26.h"

// globals for menu options
int g_TitleScreenChoice = 0;
static int g_TitleScreenLastChoice = 0;
int g_OptionScreenChoice = 0;
int g_TitleScreenCursorColor = 0;

unsigned int g_TitleTimer = 0;
static bool draw_start_text = true;
static bool draw_option_mode  = true;
static bool draw_option_players  = true;
static bool draw_option_difficulty  = true;
static bool draw_option_start  = true;
static bool draw_option_options  = true;

static bool poppy_animation = false;
static int poppy_animation_frame = 0;
int logo1_pos;
int logo2_pos;
int logo_velocity = 0;
bool logo_falling = false;
bool logo_bounce = false;
static float poppy_scale = 0.1;
static float poppy_velocity = 0.0;

FIXED pos_x = 0; // for the background - should use the attribute instead
FIXED pos_y = 0;
Uint16 cursor_angle = 0;

//
// TITLE SCREEN
//

void titleScreen_init(void)
{
    if (g_Game.lastState == GAME_STATE_TEAM_SELECT || g_Game.lastState == GAME_STATE_GAMEPLAY) {
        unloadGameAssets();
        loadTitleScreenAssets();
    }
    
    g_Game.lastState = GAME_STATE_TITLE_SCREEN;
    // if (first_load) {
        // // init_bg0_img();
        // init_bg25_img();
        // first_load = false;
    // }
    
    
    g_TitleTimer = 0;
    
    logo1_pos = -400;
    logo2_pos = 400;
    logo_velocity = 0;
    logo_falling = true;
    logo_bounce = false;
    set_spr_position(&logo1, 0, logo1_pos, 97);
    set_spr_position(&logo2, 0, logo2_pos, 97);
    logo1.visible = true;
    logo2.visible = true;
    
    poppy_scale = 0.1;
    poppy_velocity = 0.0;
    set_spr_scale(&pixel_poppy, poppy_scale, poppy_scale);
    pixel_poppy.rot.z = 0;
    sprite_frame_reset(&pixel_poppy);
    
    slColOffsetOn(NBG0ON | NBG1ON | SPRON);
    slColOffsetAUse(NBG0ON);
    slColOffsetBUse(NBG1ON | SPRON);
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    // slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN); // not really being used..
    // init menu options
    g_TitleScreenChoice = 0;
    g_OptionScreenChoice = 0;
    g_TitleScreenLastChoice = TITLE_OPTION_GAME_MODE;

    // Defaults
    g_Game.gameMode = GAME_MODE_CLASSIC;
    g_Game.minTeams = 1;
    g_Game.maxTeams = 1;
    g_Game.numTeams = 0;
    g_Game.minPlayers = 0;
    g_Game.maxPlayers = 2;
    g_Game.numPlayers = 0;
    g_Game.gameDifficulty = GAME_DIFFICULTY_MEDIUM;
    g_Game.numPlayers = ONE_PLAYER;
    
    
    mosaic_in = true;
    music_in = true;
    fade_in = true;
    transition_in = true;
}

// handles input for the title screen
// only player one can control the title screen
void titleScreen_input(void)
{
    // select an option here
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        changeState(GAME_STATE_TITLE_MENU);
        if (game_options.mosaic_display) {
            mosaic_out = true;
        }
        fade_out = false;
        transition_out = true;
    }
}

void startScreen_update(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }
    g_TitleTimer++;
    
    // check if the frameAnim has expired
    if(g_TitleTimer > TITLE_TIMER)
    {
        transitionState(GAME_STATE_DEMO_LOOP);
        g_TitleTimer = 0;
    }
}

void titleScreen_update(void)
{    
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN && g_Game.gameState != GAME_STATE_TITLE_MENU)
    {
        return;
    }
    
    if (attrBg260.x_scroll > toFIXED(0)) {
        pos_x += attrBg260.x_scroll;
        if (pos_x > toFIXED(512.0))
            pos_x = toFIXED(0);
    }
    if (attrBg260.y_scroll > toFIXED(0)) {
        pos_y += attrBg260.y_scroll;
        if (pos_y > toFIXED(512.0))
            pos_y = toFIXED(0);
    }
    slScrPosNbg1(pos_x, pos_y);
    
    drawMenu();
    drawMenuCursor();
    drawTitle();
}

// draws title image + version number
void drawTitle(void)
{
    my_sprite_draw(&pixel_poppy);
    my_sprite_draw(&logo1);
    my_sprite_draw(&logo2);
    
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }
    // title graphic
    // jo_nbg0_printf(14, 23, "GAME LOGO HERE!");
    if (frame % 15 == 0) {
        draw_start_text = !draw_start_text;
    }
    if (draw_start_text) {
        jo_nbg0_printf(16, 27, "PRESS START");
    }
    if (game_options.debug_mode) {
        // version number
        jo_nbg0_printf(20, 28, "%s", VERSION);
    }

    if (poppy_scale < POPPY_MAX_SCALE) {
        if (poppy_velocity < 0.5) {
            poppy_velocity += 0.01;
        }
        poppy_scale += poppy_velocity;
        set_spr_scale(&pixel_poppy, poppy_scale, poppy_scale);
    }

    // LOGO ANIMATION
    // logo is falling
    if (logo_falling && !logo_bounce) {
        if (logo_velocity < LOGO_VELOCITY) {
            if (frame % 2 == 0) {
                logo_velocity++;
            }
        }
        if (logo1_pos < LOGO1_ONSCREEN_Y) {
            logo1_pos += logo_velocity;
            set_spr_position(&logo1, 0, logo1_pos, 97);
            logo2_pos -= logo_velocity;
            set_spr_position(&logo2, 0, logo2_pos, 97);
        }
        // logo has hit the "ground"
        else {
            logo_velocity = 0;
            logo_falling = false;
            logo_bounce = true;
        }
    }
    // logo is bouncing
    else if (logo_bounce && !logo_falling) {
        // bounce
        if (logo_velocity < LOGO_VELOCITY) {
            if (frame % 2 == 0) {
                logo_velocity++;
            }
        }
        if (logo1_pos > LOGO1_ONSCREEN_Y - 12) {
            logo1_pos -= logo_velocity;
            set_spr_position(&logo1, 0, logo1_pos, 97);
            logo2_pos += logo_velocity;
            set_spr_position(&logo2, 0, logo2_pos, 97);
        }
        // logo has hit it's resing spot
        else {
            logo_velocity = 0;
            logo_falling = true;
            logo_bounce = true;
        }
    }
    // first bounce has already happened
    else if (logo_bounce && logo_falling) {
        logo_falling = false;
        logo_bounce = false;
    }
    // falling again
    else if (!logo_bounce && !logo_falling) {
        if (logo_velocity < LOGO_VELOCITY) {
            logo_velocity++;
        }
        // final resting place
        if (logo1_pos < LOGO1_ONSCREEN_Y) {
            logo1_pos += logo_velocity;
            set_spr_position(&logo1, 0, logo1_pos, 97);
            logo2_pos -= logo_velocity;
            set_spr_position(&logo2, 0, logo2_pos, 97);
        }
    }
}

//
// MAIN MENU
//

void titleMenu_init(void)
{
    
    g_Game.lastState = GAME_STATE_TITLE_MENU;
    random_sprite_animation(&pixel_poppy, 0, 3);
    // static_sprite_animation(&pixel_poppy);
    jo_set_default_background_color(JO_COLOR_Black);
    if (game_options.mesh_display) {
        menu_bg1.mesh = MESHon;
    }
    else {
        menu_bg1.mesh = MESHoff;
    }
    menu_bg1.spr_id = menu_bg1.anim1.asset[6];
    set_spr_position(&menu_bg1, 0, MENU_Y+5, 95);
    set_spr_scale(&menu_bg1, 0, 0);
    // for debug
    // mosaic_in = true;
    music_in = true;
    fade_in = true;
    transition_in = true;
    poppy_animation = false;
}

void menuScreen_input(void)
{
    if (jo_is_pad1_key_down(JO_KEY_UP))
    {
        switch (g_TitleScreenChoice) {
            case TITLE_OPTION_GAME_MODE:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_MODE;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_PLAYERS;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_DIFFICULTY;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;
                
            case TITLE_OPTION_GAME_START:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_START;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;
                
            case TITLE_OPTION_GAME_OPTIONS:
                g_TitleScreenChoice = g_TitleScreenLastChoice;
                break;
        }
    }

    if (jo_is_pad1_key_down(JO_KEY_DOWN))
    {
        switch (g_TitleScreenChoice) {
            case TITLE_OPTION_GAME_MODE:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_MODE;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_PLAYERS;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_PLAYERS;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;
                
            case TITLE_OPTION_GAME_START:
                g_TitleScreenLastChoice = TITLE_OPTION_GAME_START;
                g_TitleScreenChoice = TITLE_OPTION_GAME_OPTIONS;
                break;
                
            case TITLE_OPTION_GAME_OPTIONS:
                g_TitleScreenChoice = g_TitleScreenLastChoice;
                break;
        }
    }

    if (jo_is_pad1_key_down(JO_KEY_LEFT))
    {
        switch(g_TitleScreenChoice)
        {
            case TITLE_OPTION_GAME_MODE:
                g_Game.gameMode--;
                sanitizeValue((int*)&g_Game.gameMode, 0, GAME_MODE_MAX);
                if (g_Game.gameMode == GAME_MODE_STORY) {
                    g_Game.minPlayers = 0;
                    g_Game.maxPlayers = 1;
                    g_Game.numPlayers = g_Game.minPlayers;
                    g_Game.minTeams = 1;
                    g_Game.maxTeams = 1;
                }
                else if (g_Game.gameMode == GAME_MODE_CLASSIC) {
                    g_Game.minPlayers = 0;
                    g_Game.maxPlayers = 2;
                    g_Game.numPlayers = g_Game.minPlayers;
                    g_Game.minTeams = 1;
                    g_Game.maxTeams = 1;
                }
                else {
                    g_Game.minPlayers = 1;
                    g_Game.maxPlayers = GAME_PLAYERS_MAX;
                    g_Game.numPlayers = g_Game.minPlayers;
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 2;
                }
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                g_Game.numPlayers--;
                sanitizeValue((int*)&g_Game.numPlayers, g_Game.minPlayers, g_Game.maxPlayers);
                if (g_Game.numPlayers == ONE_PLAYER) {
                    g_Game.minTeams = 1;
                    g_Game.maxTeams = 1;
                }
                else if (g_Game.numPlayers == TWO_PLAYER) {
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 2;
                }
                else if (g_Game.numPlayers == THREE_PLAYER) {
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 3;
                }
                else if (g_Game.numPlayers == FOUR_PLAYERS) {
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 4;
                }
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
                if (g_Game.gameMode == GAME_MODE_STORY) {
                    g_Game.minPlayers = 0;
                    g_Game.maxPlayers = 1;
                    g_Game.numPlayers = g_Game.minPlayers;
                    g_Game.minTeams = 1;
                    g_Game.maxTeams = 1;
                }
                else if (g_Game.gameMode == GAME_MODE_CLASSIC) {
                    g_Game.minPlayers = 0;
                    g_Game.maxPlayers = 2;
                    g_Game.numPlayers = g_Game.minPlayers;
                    g_Game.minTeams = 1;
                    g_Game.maxTeams = 1;
                }
                else {
                    g_Game.minPlayers = 1;
                    g_Game.maxPlayers = GAME_PLAYERS_MAX;
                    g_Game.numPlayers = g_Game.minPlayers;
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 2;
                }
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                g_Game.numPlayers++;
                sanitizeValue((int*)&g_Game.numPlayers, g_Game.minPlayers, g_Game.maxPlayers);
                if (g_Game.numPlayers == ONE_PLAYER) {
                    g_Game.minTeams = 1;
                    g_Game.maxTeams = 1;
                }
                else if (g_Game.numPlayers == TWO_PLAYER) {
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 2;
                }
                else if (g_Game.numPlayers == THREE_PLAYER) {
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 3;
                }
                else if (g_Game.numPlayers == FOUR_PLAYERS) {
                    g_Game.minTeams = 2;
                    g_Game.maxTeams = 4;
                }
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
            case TITLE_OPTION_GAME_MODE:
                g_TitleScreenLastChoice = g_TitleScreenChoice;
                g_TitleScreenChoice++;
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                g_TitleScreenLastChoice = g_TitleScreenChoice;
                g_TitleScreenChoice++;
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                g_TitleScreenLastChoice = g_TitleScreenChoice;
                g_TitleScreenChoice++;
                break;
                
            case TITLE_OPTION_GAME_START:
                static_sprite_animation(&pixel_poppy);
                transitionState(GAME_STATE_TEAM_SELECT);
                break;
                
            case TITLE_OPTION_GAME_OPTIONS:
                changeState(GAME_STATE_TITLE_OPTIONS);
                break;
                
            default:
                break;
        }
    }
    if (jo_is_pad1_key_down(JO_KEY_B) )
    {
        switch(g_TitleScreenChoice)
        {
            case TITLE_OPTION_GAME_MODE:
                changeState(GAME_STATE_TITLE_SCREEN);
                if (game_options.mosaic_display) {
                    mosaic_in = true;
                }
                fade_in = false;
                transition_in = true;
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                g_TitleScreenChoice--;
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                g_TitleScreenChoice--;
                break;
                
            case TITLE_OPTION_GAME_START:
                g_TitleScreenChoice--;
                break;
                
            case TITLE_OPTION_GAME_OPTIONS:
                g_TitleScreenChoice = g_TitleScreenLastChoice;
                break;
                
            default:
                break;
        }
    }
}

void drawMenu(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_MENU)
    {
        return;
    }
    
    // wink
    if (pixel_poppy.spr_id == pixel_poppy.anim1.asset[3]) 
    {
        if (poppy_animation && poppy_animation_frame % 20 == 0)
        {
            pixel_poppy.spr_id = pixel_poppy.anim1.asset[2];
            poppy_animation = false;
            poppy_animation_frame = 0;
        }
        poppy_animation = true;
        poppy_animation_frame += 1;
    }
    
    // make sure poppy continues to scale up if user presses start too fast
    if (poppy_scale < POPPY_MAX_SCALE) {
        if (poppy_velocity < 0.5) {
            poppy_velocity += 0.01;
        }
        poppy_scale += poppy_velocity;
        set_spr_scale(&pixel_poppy, poppy_scale, poppy_scale);
    }
    
    // Animate logo1 upwards
    if (logo1_pos > LOGO1_OFFSCREEN_Y) {
        logo1_pos -= 8;
        set_spr_position(&logo1, 0, logo1_pos, 97);
        if (logo1_pos <= LOGO1_OFFSCREEN_Y) {
            logo1.visible = false;
        }
    }

    // Animate logo2 downwards
    if (logo2_pos < LOGO2_OFFSCREEN_Y) {
        logo2_pos += 8;
        set_spr_position(&logo2, 0, logo2_pos, 97);
        if (logo2_pos >= LOGO2_OFFSCREEN_Y) {
            logo2.visible = false;
        }
    }
    
    if (menu_bg1.scl.x < toFIXED(352)) {
        menu_bg1.scl.x += toFIXED(8);
    }
    if (menu_bg1.scl.y < toFIXED(32)) {
        menu_bg1.scl.y += toFIXED(1);
    }

    switch (g_TitleScreenChoice) {
        case TITLE_OPTION_GAME_MODE:
            if (frame % 10 == 0) {
                draw_option_mode = !draw_option_mode;
            }
                draw_option_players = false;
                draw_option_difficulty = false;
                draw_option_start = false;
                draw_option_options = true;
            break;
            
        case TITLE_OPTION_GAME_PLAYERS:
            if (frame % 10 == 0) {
                draw_option_players = !draw_option_players;
            }
                draw_option_mode = false;
                draw_option_difficulty = false;
                draw_option_start = false;
                draw_option_options = true;
            break;

        case TITLE_OPTION_GAME_DIFFICULTY:
            if (frame % 10 == 0) {
                draw_option_difficulty = !draw_option_difficulty;
            }
                draw_option_mode = false;
                draw_option_players = false;
                draw_option_start = false;
                draw_option_options = true;
            break;

        case TITLE_OPTION_GAME_START:
            if (frame % 10 == 0) {
                draw_option_start = !draw_option_start;
            }
                draw_option_difficulty = false;
                draw_option_mode = false;
                draw_option_players = false;
                draw_option_options = true;
            // Start!
                
            break;
            
        case TITLE_OPTION_GAME_OPTIONS:
            switch (g_TitleScreenLastChoice) {
                case TITLE_OPTION_GAME_MODE:
                    draw_option_mode = true;
                    break;
                case TITLE_OPTION_GAME_PLAYERS:
                    draw_option_players = true;
                    break;
                case TITLE_OPTION_GAME_DIFFICULTY:
                    draw_option_difficulty = true;
                    break;
                case TITLE_OPTION_GAME_START:
                    draw_option_start = true;
                    break;
                default:
                    break;
            }
            if (frame % 10 == 0) {
                draw_option_options = !draw_option_options;
            }
            break;

        default:
            break;
    }    

    // Options
    menu_text.pos.x = toFIXED(0);
    menu_text.pos.y = toFIXED(MENU_Y-10);
    menu_text.zmode = _ZmCC;
    menu_text.spr_id = menu_text.anim1.asset[1];
    if (draw_option_options)
        my_sprite_draw(&menu_text); // options
        

            menu_text.pos.y = toFIXED(MENU_Y+20);
            
            menu_text.spr_id = menu_text.anim1.asset[g_Game.gameMode+MODE_OFFSET];
            if (draw_option_mode)
                my_sprite_draw(&menu_text); // classic, story, vs battle
                
            menu_text.spr_id = menu_text.anim1.asset[g_Game.numPlayers+PLAYER_OFFSET];
            if (draw_option_players)
                my_sprite_draw(&menu_text); // 1, 2, 3, 4
                
            menu_text.spr_id = menu_text.anim1.asset[g_Game.gameDifficulty+DIFF_OFFSET];
            if (draw_option_difficulty)
                my_sprite_draw(&menu_text); // easy, medium, hard
                
            menu_text.spr_id = menu_text.anim1.asset[0];
            if (draw_option_start)
                my_sprite_draw(&menu_text); // start


    my_sprite_draw(&menu_bg1); // shadow
}

//
// OPTIONS
//

void optionsScreen_init(void)
{    
    
    g_Game.lastState = GAME_STATE_TITLE_OPTIONS;
    slColOffsetOn(NBG1ON);
    slColOffsetAUse(OFF);
    slColOffsetBUse(NBG1ON);
    slColOffsetB(PAUSE_FADE, PAUSE_FADE, PAUSE_FADE);
    
    if (game_options.mesh_display) {
        menu_bg2.mesh = MESHon;
    }
    else {
        menu_bg2.mesh = MESHoff;
    }
    menu_bg2.zmode = _ZmCT;
    set_spr_position(&menu_bg2, 0, -120, 95);
    set_spr_scale(&menu_bg2, 200, 96);
}

void optionsScreen_input(void)
{
    if (jo_is_pad1_key_down(JO_KEY_UP))
    {
            g_OptionScreenChoice--;
    }

    if (jo_is_pad1_key_down(JO_KEY_DOWN))
    {
            g_OptionScreenChoice++;
    }

    if (jo_is_pad1_key_down(JO_KEY_LEFT) || jo_is_pad1_key_down(JO_KEY_RIGHT))
    {
        switch(g_OptionScreenChoice)
        {
            case OPTION_DEBUG_MODE:
                game_options.debug_mode = !game_options.debug_mode;
                break;
            case OPTION_DEBUG_TEXT:
                game_options.debug_display = !game_options.debug_display;
                break;
            case OPTION_DRAWMESH:
                game_options.mesh_display = !game_options.mesh_display;
                break;
            case OPTION_DRAWMOSAIC:
                game_options.mosaic_display = !game_options.mosaic_display;
                break;
            case OPTION_WIDESCREEN:
                game_options.widescreen = !game_options.widescreen;
                break;
            default:
                break;
        }
    }

    // keep title screen choice in range
    sanitizeValue(&g_OptionScreenChoice, 0, OPTION_MAX);

    // GO BACK
    if (jo_is_pad1_key_down(JO_KEY_START) || 
        jo_is_pad1_key_down(JO_KEY_A) ||
        jo_is_pad1_key_down(JO_KEY_C))
    {
        switch(g_OptionScreenChoice)
        {
            case OPTION_EXIT:
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                changeState(GAME_STATE_TITLE_MENU);
                break;
                
            default:
                break;
        }
    }
    if (jo_is_pad1_key_down(JO_KEY_B) )
    {
        slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
        changeState(GAME_STATE_TITLE_MENU);
    }
}

void optionsScreen_update(void)
{    
    if(g_Game.gameState != GAME_STATE_TITLE_OPTIONS)
    {
        return;
    }
    
    if (attrBg260.x_scroll > toFIXED(0)) {
        pos_x += attrBg260.x_scroll;
        if (pos_x > toFIXED(512.0))
            pos_x = toFIXED(0);
    }
    if (attrBg260.y_scroll > toFIXED(0)) {
        pos_y += attrBg260.y_scroll;
        if (pos_y > toFIXED(512.0))
            pos_y = toFIXED(0);
    }
    slScrPosNbg1(pos_x, pos_y);
    
    drawOptions();
    drawOptionsCursor();
}

void drawOptions(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_OPTIONS)
    {
        return;
    }
    int options_x = 28;
    int options_y = 4;
    
    jo_nbg0_printf(18, options_y, "OPTIONS");
    
    options_y += 4;
    jo_nbg0_printf(10, options_y, "DEBUG MODE:");
    if (game_options.debug_mode) {
        jo_nbg0_printf(options_x, options_y, "ON");
    }
    else {
        jo_nbg0_printf(options_x, options_y, "OFF");
    }
    
    options_y += 2;
    jo_nbg0_printf(10, options_y, "DEBUG DISPLAY:");
    if (game_options.debug_display) {
        jo_nbg0_printf(options_x, options_y, "ON");
    }
    else {
        jo_nbg0_printf(options_x, options_y, "OFF");
    }

    options_y += 2;
    jo_nbg0_printf(10, options_y, "MESH TRANSPARENCY:");
    if (game_options.mesh_display) {
        menu_bg1.mesh = MESHon; // good enough for now
        menu_bg2.mesh = MESHon;
        jo_nbg0_printf(options_x, options_y, "ON");
    }
    else {
        menu_bg1.mesh = MESHoff;
        menu_bg2.mesh = MESHoff;
        jo_nbg0_printf(options_x, options_y, "OFF");
    }

    options_y += 2;
    jo_nbg0_printf(10, options_y, "MOSAIC EFFECT:");
    if (game_options.mosaic_display) {
	slScrMosSize(MOSAIC_MAX, MOSAIC_MAX);
	slScrMosaicOn(NBG1ON);
        jo_nbg0_printf(options_x, options_y, "ON");
    }
    else {
        jo_nbg0_printf(options_x, options_y, "OFF");
	slScrMosSize(MOSAIC_MIN, MOSAIC_MIN);
	slScrMosaicOn(OFF);
    }
    
    options_y += 2;
    jo_nbg0_printf(10, options_y, "SCREEN MODE:");
    if (game_options.widescreen) {
        jo_nbg0_printf(options_x, options_y, "WIDE");
    }
    else {
        jo_nbg0_printf(options_x, options_y, "NORMAL");
    }
    
    options_y += 2;
    jo_nbg0_printf(10, options_y, "EXIT");
    
    my_sprite_draw(&menu_bg2); // shadow

}

//
// Title Screen Helpers
//

// menu screen cursor
void drawMenuCursor(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_MENU)
    {
        return;
    }
    
    FIXED offset = jo_fixed_mult(jo_fixed_sin(jo_fixed_deg2rad(toFIXED(cursor_angle))), toFIXED(8));
    cursor.pos.x = toFIXED(-130) + offset;
    if (g_TitleScreenChoice == TITLE_OPTION_GAME_OPTIONS) {
        cursor.pos.y = toFIXED(MENU_Y);
    }
    else {
        cursor.pos.y = toFIXED(MENU_Y+30);
    }
    
    my_sprite_draw(&cursor);
    cursor_angle += 8;
    if (cursor_angle > 360) {
        cursor_angle = 0;
    }
}

// options screen cursor
void drawOptionsCursor(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_OPTIONS)
    {
        return;
    }
    FIXED offset = jo_fixed_mult(jo_fixed_sin(jo_fixed_deg2rad(toFIXED(cursor_angle))), toFIXED(8));
    cursor.pos.x = toFIXED(-200) + offset;
    cursor.pos.y = toFIXED(-96 + (g_OptionScreenChoice * 32)); // vertical position varies based on selection
    my_sprite_draw(&cursor);
    cursor_angle += 8;
    if (cursor_angle > 360) {
        cursor_angle = 0;
    }
}
