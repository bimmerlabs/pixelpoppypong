#include <jo/jo.h>
#include "main.h"
#include "backup.h"
#include "input.h"
#include "assets.h"
#include "font.h"
#include "title_screen.h"
#include "screen_transition.h"
#include "BG_DEF/nbg1.h"
#include "BG_DEF/sprite_colors.h"

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
static int poppy_animation_id = 0;
int logo1_pos;
int logo2_pos;
int logo_velocity = 0;
bool logo_falling = false;
bool logo_bounce = false;
static float poppy_scale = 0.1;
static float poppy_velocity = 0.0;

bool increasing = true;
int h_value = 0;

Uint16 cursor_angle = 0;

//
// TITLE SCREEN
//

void titleScreen_init(void)
{
    if (g_Game.lastState == GAME_STATE_TEAM_SELECT 
     || g_Game.lastState == GAME_STATE_GAMEPLAY 
     || g_Game.lastState == GAME_STATE_DEMO_LOOP 
     || g_Game.lastState == GAME_STATE_CREDITS) {
        unloadGameAssets();
        loadTitleScreenAssets();
    }
    
    g_Game.lastState = GAME_STATE_TITLE_SCREEN;    
    
    g_TitleTimer = 0;
    
    // reset hsl colors
    reset_sprites();
    do_update_logo1 = true;
    do_update_menu3 = true;
    do_update_menu4 = true;
    
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
    set_spr_position(&pixel_poppy, 0, 0, 100);
    sprite_frame_reset(&pixel_poppy);
    poppy_animation_frame = 0;
    poppy_animation_id = 0;
    
    slColOffsetOn(NBG0ON | NBG1ON | SPRON);
    slColOffsetAUse(NBG0ON);
    slColOffsetBUse(NBG1ON | SPRON);
    
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    
    // init menu options
    g_TitleScreenChoice = TITLE_OPTION_GAME_MODE;
    g_OptionScreenChoice = 0;
    g_TitleScreenLastChoice = TITLE_OPTION_GAME_MODE;

    // INITIAL GAME MODE
    g_Game.gameMode = GAME_MODE_STORY;
    g_Game.minPlayers = ONE_PLAYER;
    g_Game.maxPlayers = ONE_PLAYER;
    g_Game.currentNumPlayers = 0;
    g_Game.numPlayers = ONE_PLAYER;
    
    g_Team.minTeams = -1;
    g_Team.maxTeams = -1;
    g_Team.numTeams = -1;
    
    g_Game.gameDifficulty = GAME_DIFFICULTY_MEDIUM;
    
    mosaic_in = true;
    music_in = true;
    fade_in = true;
    transition_in = true;
}

// only player one can control the title screen
void titleScreen_input(void)
{
    if (logo_bounce || logo_falling) {
        return;
    }
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        pcm_play(g_Assets.startPcm8, PCM_VOLATILE, 6);
        changeState(GAME_STATE_TITLE_MENU); 
        if (g_GameOptions.mosaic_display) {
            mosaic_out = true;
            transition_out = true;
        }
    }
}

void startScreen_update(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN)
    {
        return;
    }
    g_TitleTimer++;
    poppy_animation_id = my_random_range(0, 3); // delete??
    if (!logo_bounce && !logo_falling) {
        if (g_TitleTimer == LOGO_TIMER) {
            reset_sprites();
            h_value = 0;
            do_update_logo1 = true;
        }
        else if (g_TitleTimer >= LOGO_TIMER2 && g_TitleTimer < LOGO_TIMER) {
            hsl_incSprites.h -= 8;
            do_update_logo1 = true;
        }
        else if (frame % 5 == 0 && g_TitleTimer < LOGO_TIMER) { // modulus
            hsl_incSprites.h -= 20;
            do_update_logo1 = true;
        }
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[1];
    }
    // check if the frameAnim has expired
    if(g_TitleTimer > TITLE_TIMER)
    {
        transitionState(g_AttractScreenState);
        attract_screen_state();
        g_TitleTimer = 0;
    }
}

void titleScreen_update(void)
{    
    if(g_Game.gameState != GAME_STATE_TITLE_SCREEN && g_Game.gameState != GAME_STATE_TITLE_MENU)
    {
        return;
    }
    
    if (attrNbg1.x_scroll > toFIXED(0)) {
        attrNbg1.x_pos += attrNbg1.x_scroll;
        if (attrNbg1.x_pos > toFIXED(512.0))
            attrNbg1.x_pos = toFIXED(0);
    }
    slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);
    
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
    if (frame % 10 == 0) { // modulus
        draw_start_text = !draw_start_text;
    }
    if (draw_start_text) {
        jo_nbg0_printf(16, 27, "PRESS START");
    }
    if (releaseCanidate) {
        jo_nbg0_printf(18, 28, "%s RC", VERSION);
    }
    else {
        jo_nbg0_printf(20, 28, "%s", VERSION); // Regular version
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
            if (JO_MOD_POW2(frame, 2) == 0) { // modulus
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
            if (JO_MOD_POW2(frame, 2) == 0) { // modulus
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
    jo_set_default_background_color(JO_COLOR_Black);
    if (g_GameOptions.mesh_display) {
        menu_bg1.mesh = MESHon;
    }
    else {
        menu_bg1.mesh = MESHoff;
    }
    menu_bg1.spr_id = menu_bg1.anim1.asset[5];
    set_spr_position(&menu_bg1, 0, MENU_Y+5, 95);
    set_spr_scale(&menu_bg1, 0, 0);

    h_value = 0;
    
    music_in = true;
    fade_in = true;
    transition_in = true;
    poppy_animation = false;
}

void menuScreen_input(void)
{
    if (jo_is_pad1_key_down(JO_KEY_UP))
    {
        pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
        h_value = 0;
        reset_sprites();
        do_update_menu3 = true;
        do_update_menu4 = true;
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
        pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
        h_value = 0;
        reset_sprites();
        do_update_menu3 = true;
        do_update_menu4 = true;
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
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_Game.gameMode--;
                sanitizeValue((int*)&g_Game.gameMode, 0, GAME_MODE_MAX);
                selectGameMode();
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_Game.numPlayers--;
                sanitizeValue((int*)&g_Game.numPlayers, g_Game.minPlayers, (g_Game.maxPlayers+1));
                selectNumPlayers();
                break;

            case TITLE_OPTION_GAME_DIFFICULTY:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
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
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_Game.gameMode++;
                sanitizeValue((int*)&g_Game.gameMode, 0, GAME_MODE_MAX);
                selectGameMode();
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_Game.numPlayers++;
                sanitizeValue((int*)&g_Game.numPlayers, g_Game.minPlayers, (g_Game.maxPlayers+1));
                selectNumPlayers();
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
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
                pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 6);
                g_TitleScreenLastChoice = g_TitleScreenChoice;
                g_TitleScreenChoice++;
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 6);
                g_TitleScreenLastChoice = g_TitleScreenChoice;
                g_TitleScreenChoice++;
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 6);
                g_TitleScreenLastChoice = g_TitleScreenChoice;
                g_TitleScreenChoice++;
                break;
                
            case TITLE_OPTION_GAME_START: {
                pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 7);
                if (g_Game.gameMode == GAME_MODE_STORY) {
                    transitionState(GAME_STATE_GAMEPLAY);
                }
                else {
                    transitionState(GAME_STATE_TEAM_SELECT);
                }
                break;
            }
                
            case TITLE_OPTION_GAME_OPTIONS:
                pcm_play(g_Assets.startPcm8, PCM_VOLATILE, 6);
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
                if (g_GameOptions.mosaic_display) {
                    mosaic_in = true;
                }
                fade_in = false;
                transition_in = true;
                break;

            case TITLE_OPTION_GAME_PLAYERS:
                pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
                g_TitleScreenChoice--;
                break;
            
            case TITLE_OPTION_GAME_DIFFICULTY:
                pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
                g_TitleScreenChoice--;
                break;
                
            case TITLE_OPTION_GAME_START:
                pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
                g_TitleScreenChoice--;
                break;
                
            case TITLE_OPTION_GAME_OPTIONS:
                pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
                h_value = 0;
                game_palette.data[28] = JO_COLOR_White;
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

// SUBROUTINE FOR ANIMATION
    // GASP
    if (poppy_animation_id == 1) 
    {
        // PLAY SOUND EFFECT
        poppy_animation = true;
        poppy_animation_frame += 1;
        if (poppy_animation && poppy_animation_frame % 20 == 0) // modulus
        {
            // PLAY SOUND EFFECT
            pixel_poppy.spr_id = pixel_poppy.anim1.asset[0];
            poppy_animation = false;
            poppy_animation_frame = 0;
            poppy_animation_id = 0;
        }
    }
    
    // wink
    else if (poppy_animation_id == 2) 
    {
        // PLAY SOUND EFFECT
        poppy_animation = true;
        poppy_animation_frame += 1;
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[2];
        if (poppy_animation && poppy_animation_frame % 22 == 0) // modulus
        {
            pixel_poppy.spr_id = pixel_poppy.anim1.asset[1];
            poppy_animation = false;
            poppy_animation_frame = 0;
            poppy_animation_id = 0;
        }
    }
        
    // giggle
    else if (poppy_animation_id == 3) 
    {
        // PLAY SOUND EFFECT
        bool giggle = true;
        poppy_animation = true;
        poppy_animation_frame += 1;
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[3];
        if (poppy_animation && poppy_animation_frame % 40 == 0) // modulus
        {
            pixel_poppy.spr_id = pixel_poppy.anim1.asset[4];
            set_spr_position(&pixel_poppy, 0, 0, 100);
            poppy_animation = false;
            poppy_animation_frame = 0;
            poppy_animation_id = 0;
        }
        if (giggle && poppy_animation_frame % 10 == 0) { // modulus
            set_spr_position(&pixel_poppy, 0, 2, 100);
            giggle = false;
        }
        else if (poppy_animation_frame % 5 == 0) { // modulus
            set_spr_position(&pixel_poppy, 0, -2, 100);
            giggle = true;
        }
    }
    
    // // eyeroll
    // else if (poppy_animation_id == 4) 
    // {
        // // PLAY SOUND EFFECT
        // poppy_animation = true;
        // poppy_animation_frame += 1;
        // // pixel_poppy.spr_id = pixel_poppy.anim1.asset[6];
        // if (poppy_animation && poppy_animation_frame % 10 == 0) // modulus
        // {
            // pixel_poppy.spr_id = pixel_poppy.anim1.asset[6];
            // poppy_animation = false;
            // poppy_animation_frame = 0;
            // poppy_animation_id = 0;
        // }
    // }
    
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
                game_palette.data[28] = JO_COLOR_White;
                animateMenuColor(&do_update_menu4);
                draw_option_mode = true;
                draw_option_players = false;
                draw_option_difficulty = false;
                draw_option_start = false;
                draw_option_options = true;
            break;
            
        case TITLE_OPTION_GAME_PLAYERS:
                game_palette.data[28] = JO_COLOR_White;
                animateMenuColor(&do_update_menu4);
                draw_option_mode = false;
                draw_option_players = true;
                draw_option_difficulty = false;
                draw_option_start = false;
                draw_option_options = true;
            break;

        case TITLE_OPTION_GAME_DIFFICULTY:
                game_palette.data[28] = JO_COLOR_White;
                animateMenuColor(&do_update_menu4);
                draw_option_mode = false;
                draw_option_players = false;
                draw_option_difficulty = true;
                draw_option_start = false;
                draw_option_options = true;
            break;

        case TITLE_OPTION_GAME_START:
                game_palette.data[28] = JO_COLOR_White;
                animateMenuColor(&do_update_menu4);
                draw_option_difficulty = false;
                draw_option_mode = false;
                draw_option_players = false;
                draw_option_start = true;
                draw_option_options = true;
            // Start!
                
            break;
            
        case TITLE_OPTION_GAME_OPTIONS:
                game_palette.data[31] = JO_COLOR_White;
            animateMenuColor(&do_update_menu3);
            draw_option_options = true;
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
    g_TitleScreenChoice     = TITLE_OPTION_GAME_MODE;
    g_TitleScreenLastChoice = TITLE_OPTION_GAME_MODE;
    slColOffsetOn(NBG1ON);
    slColOffsetAUse(OFF);
    slColOffsetBUse(NBG1ON);
    slColOffsetB(QUARTER_FADE, QUARTER_FADE, QUARTER_FADE);
    
    if (g_GameOptions.mesh_display) {
        menu_bg2.mesh = MESHon;
    }
    else {
        menu_bg2.mesh = MESHoff;
    }
    menu_bg2.spr_id = menu_bg2.anim1.asset[4];
    menu_bg2.zmode = _ZmCC;
    set_spr_position(&menu_bg2, 0, 0, 95);
    set_spr_scale(&menu_bg2, 240, 480);
}

void optionsScreen_input(void)
{
    if (jo_is_pad1_key_down(JO_KEY_UP))
    {
        pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
        g_OptionScreenChoice--;
    }

    if (jo_is_pad1_key_down(JO_KEY_DOWN))
    {
        pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
        g_OptionScreenChoice++;
    }

    if (g_OptionScreenChoice == OPTION_ANALOG) {
        analogAdjustmentScreen_input();
    }
    
    if (jo_is_pad1_key_down(JO_KEY_LEFT) || jo_is_pad1_key_down(JO_KEY_RIGHT))
    {
        switch(g_OptionScreenChoice)
        {
            // case OPTION_DEBUG_MODE:
                // if (!releaseCanidate) {
                    // // pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                    // g_GameOptions.debug_mode = !g_GameOptions.debug_mode;
                // }
                // break;
            // case OPTION_DEBUG_TEXT:
                // if (!releaseCanidate) {
                    // // pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                    // g_GameOptions.debug_display = !g_GameOptions.debug_display;
                // }
                // break;
            // case OPTION_DEBUG_COLLISION:
                // if (!releaseCanidate) {
                    // // pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                    // g_GameOptions.testCollision = !g_GameOptions.testCollision;
                // }
                // break;
            case OPTION_DRAWMESH:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_GameOptions.mesh_display = !g_GameOptions.mesh_display;
                break;
            case OPTION_DRAWMOSAIC:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_GameOptions.mosaic_display = !g_GameOptions.mosaic_display;
                break;
            case OPTION_USE_RTC:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_GameOptions.use_rtc = !g_GameOptions.use_rtc;
                break;
            case OPTION_BIG_HEAD:
                if (g_GameOptions.unlockBigHeadMode) {
                    pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                    g_GameOptions.bigHeadMode = !g_GameOptions.bigHeadMode;
                }
                break;
            case OPTION_ITEMS:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_GameOptions.enableItems = !g_GameOptions.enableItems;
                break;
            case OPTION_MEOW:
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                g_GameOptions.enableMeows = !g_GameOptions.enableMeows;
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
                pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 7);
                save_game_backup();
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                changeState(GAME_STATE_TITLE_MENU);
                break;
                
            default:
                break;
        }
    }
    if (jo_is_pad1_key_down(JO_KEY_B) )
    {
        pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
        save_game_backup();
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
    
    if (attrNbg1.x_scroll > toFIXED(0)) {
        attrNbg1.x_pos += attrNbg1.x_scroll;
        if (attrNbg1.x_pos > toFIXED(512.0))
            attrNbg1.x_pos = toFIXED(0);
    }
    if (attrNbg1.y_scroll > toFIXED(0)) {
        attrNbg1.y_pos += attrNbg1.y_scroll;
        if (attrNbg1.y_pos > toFIXED(512.0))
            attrNbg1.y_pos = toFIXED(0);
    }
    slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);
    
    drawOptions();
    drawOptionsCursor();
}

void drawOptions(void)
{
    if(g_Game.gameState != GAME_STATE_TITLE_OPTIONS)
    {
        return;
    }
    int title_x = 8;
    int options_x = 27;
    int options_y = 2;
    
    jo_nbg0_printf(18, options_y, "OPTIONS");
    
    // options_y += 2;
    // jo_nbg0_printf(title_x, options_y, "DEBUG MODE:");
    // jo_nbg0_printf(options_x, options_y, g_GameOptions.debug_mode ? "ON" : "OFF");
    
    // options_y += 2;
    // jo_nbg0_printf(title_x, options_y, "DEBUG DISPLAY:");
    // jo_nbg0_printf(options_x, options_y, g_GameOptions.debug_display ? "ON" : "OFF");
    
    // options_y += 2;
    // jo_nbg0_printf(title_x, options_y, "DEBUG COLLISION:");
    // jo_nbg0_printf(options_x, options_y, g_GameOptions.testCollision ? "ON" : "OFF");

    options_y += 2;
    jo_nbg0_printf(title_x, options_y, "MESH TRANSPARENCY:");
    if (g_GameOptions.mesh_display) {
        menu_bg1.mesh = MESHon;
        menu_bg2.mesh = MESHon;
        jo_nbg0_printf(options_x, options_y, "ON");
    }
    else {
        menu_bg1.mesh = MESHoff;
        menu_bg2.mesh = MESHoff;
        jo_nbg0_printf(options_x, options_y, "OFF");
    }

    options_y += 2;
    jo_nbg0_printf(title_x, options_y, "MOSAIC EFFECT:");
    if (g_GameOptions.mosaic_display) {
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
    jo_nbg0_printf(title_x, options_y, "REAL TIME CLOCK:");
    jo_nbg0_printf(options_x, options_y, g_GameOptions.use_rtc ? "ON" : "OFF");
    
    options_y += 2;
    jo_nbg0_printf(title_x, options_y, "BIG HEAD MODE:");
    if (g_GameOptions.unlockBigHeadMode) {
        jo_nbg0_printf(options_x, options_y, g_GameOptions.bigHeadMode ? "ON" : "OFF");
    }
    else {
        jo_nbg0_printf(options_x, options_y, "OFF");
    }
    
    options_y += 2;
    jo_nbg0_printf(title_x, options_y, "POWER-UPS:");
    jo_nbg0_printf(options_x, options_y, g_GameOptions.enableItems ? "ON" : "OFF");
    
    options_y += 2;
    jo_nbg0_printf(title_x, options_y, "REAL TIME MEOW:");
    jo_nbg0_printf(options_x, options_y, g_GameOptions.enableMeows ? "ON" : "OFF");
    
    options_y += 2;
    jo_nbg0_printf(title_x, options_y, "ANALOG ADJUSTMENT:");
    options_y += 1;
    analogAdjustmentScreen_draw(title_x+5, options_y);     
    
    for(unsigned int i = 0; i < COUNTOF(g_Inputs); i++)
    {
        if (jo_is_input_available(i)) {
            options_y += 1;
        }
    }
    options_y += 1;
    jo_nbg0_printf(title_x, options_y, "EXIT");
    
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
    cursor.pos.x = toFIXED(-240) + offset;
    cursor.pos.y = toFIXED(-160 + (g_OptionScreenChoice * 32)); // vertical position varies based on selection
    if (g_OptionScreenChoice == OPTION_EXIT) 
    {
        for(unsigned int i = 0; i < COUNTOF(g_Inputs); i++)
        {
            if (jo_is_input_available(i)) {
                cursor.pos.y += toFIXED(16); 
            }
        }
    }
    my_sprite_draw(&cursor);
    cursor_angle += 8;
    if (cursor_angle > 360) {
        cursor_angle = 0;
    }
}
