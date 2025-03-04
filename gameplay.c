#include <jo/jo.h>
#include "main.h"
#include "input.h"
#include "gameplay.h"
#include "physics.h"
#include "assets.h"
#include "screen_transition.h"
#include "AI.h"
#include "BG_DEF/sprite_colors.h"
unsigned int g_DemoTimer = 0;
bool start_timer = false;
bool round_start = false;
bool g_RoundOver = false;
static bool play_battle_is_over = true;
static bool draw_bomb = true;
static bool explode_bomb = false;
Uint16 g_GameBeginTimer = 0;
Uint16 g_RoundOverTimer = 0;

int player1_score = 0;
int player2_score = 0;

static bool draw_demo_text = true;
static bool times_up = false;

static bool isRoundOver(void);
// static void drawStats(void);
// static void drawScore(void);

void gameplay_init() {    
    if (g_Game.nextState != GAME_STATE_DEMO_LOOP && g_Game.lastState != GAME_STATE_GAMEPLAY) {
        if (g_Game.gameMode == GAME_MODE_STORY) {
            unloadTitleScreenAssets();
            loadCharacterAssets();
        }
        loadGameAssets();
    }
    
    g_Game.lastState = GAME_STATE_GAMEPLAY;
        if (g_Game.nextState == GAME_STATE_GAMEPLAY)
    {
        // do different inits depending on game mode (demo etc)
        switch(g_Game.gameMode)
        {
            case GAME_MODE_BATTLE:
                initVsModePlayers();
                break;
            case GAME_MODE_CLASSIC: {
                initVsModePlayers();
                break;
            }
            case GAME_MODE_STORY: {
                initStoryMode();
                break;
            }
            default:
                break;
        }
    }

    if (game_options.mosaic_display) {
        mosaic_in = true;
    }
    music_in = true;
    fade_in = true;
    transition_in = true;

    resetPlayerScores();
    
    // collision detection    // sprite_id = 0;

    // debug only
    reset_ball(&pixel_poppy);
    
    sprite_frame_reset(&pixel_poppy);
    pixel_poppy.isColliding = false;
    // add_sprite_to_sweep_and_prune(&pixel_poppy);
    // update_bounding_box(&pixel_poppy);
    // add_sprite_to_sweep_and_prune(&macchi);
    // add_sprite_to_sweep_and_prune(&jelly);
    
    explode_bomb = false;
    draw_bomb = true;
    sprite_frame_reset(&bomb_item);
    sprite_frame_reset(&fishtank_item);
    sprite_frame_reset(&shroom_item);
    sprite_frame_reset(&shield1);

    reset_sprites();
    do_update_shroom = true;
    do_update_PmenuAll = true;
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);
    // slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
    
    g_Game.isPaused = false;
    g_Game.isActive = false;
    g_GameTimer = TIMEOUT;
    timer_num1.spr_id = timer_num1.anim1.asset[9];
    timer_num10.spr_id = timer_num10.anim1.asset[9];
    // NEED BETTER NAMES FOR THESE
    start_timer = false;
    g_GameBeginTimer = 0;
    times_up = false;
    round_start = false;
    g_RoundOver = false;
    
    slScrPosNbg1(toFIXED(0), toFIXED(0));
    
    menu_bg1.spr_id = menu_bg1.anim1.asset[4];
    set_spr_position(&menu_bg1, 0, -195, 85);
    set_spr_scale(&menu_bg1, 36, 20);
    
    // set_spr_scale(&goal2, 2, 0.1); // need a different sprite for 4 teams

    set_item_position(&bomb_item);
    set_item_position(&fishtank_item);
    set_item_position(&shroom_item);
    set_item_position(&garfield_item);
    set_item_position(&craig_item);    
    reset_audio(MAX_VOLUME);
    playCDTrack(BEGIN_GAME_TRACK, false);
}

void demo_init(void) {
    g_Game.lastState = GAME_STATE_DEMO_LOOP;
    unloadTitleScreenAssets();
    loadCharacterAssets();
    loadGameAssets();
    initDemoPlayers();
    g_Game.gameMode = GAME_MODE_BATTLE;
}

void demo_update(void)
{
    if(g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (JO_MOD_POW2(frame, 8) == 0) { // modulus
        draw_demo_text = !draw_demo_text;
    }
    if (draw_demo_text) {
        jo_nbg0_printf(20, 26, "DEMO!");
    }
    g_DemoTimer++;
    if (!round_start) {
        playerAI(&pixel_poppy);
        // sweep_and_prune();
        // updatePlayers();
    }
    // check if the frameAnim has expired
    if(g_DemoTimer > DEMO_TIME)
    {
        g_DemoTimer = 0;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}void game_timer(void)
{
    // wait 3 seconds to start the game
    if (g_GameBeginTimer > GAME_BEGIN_TIME) {
        start_timer = true;
    }
    if (!start_timer) {
        g_GameBeginTimer++;
        round_start = true;
        return;
    }
    
    // start round    if (round_start) {
        start_ball_movement(&pixel_poppy);
        g_Game.isActive = true;
        round_start = false;
    }
    
    // game timer
    Uint8 ones = g_GameTimer % 10; // Extracts the ones place // modulus
    Uint8 tens = g_GameTimer / 10; // Extracts the tens place
        
    timer_num10.spr_id = timer_num10.anim1.asset[tens];
    timer_num1.spr_id = timer_num1.anim1.asset[ones];
    
    if (g_GameTimer > 0 && frame % 60 == 0) { // modulus
        g_GameTimer--;
    }
    else if (g_GameTimer == 0) {
        times_up = true;
    }
    // test
    if (g_GameTimer == 80) {
        explode_bomb = true;
    }
    
}
void gameplay_draw(void)
{    
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (!g_Game.isPaused && !times_up) {
                // move to draw/HUD timer function
        game_timer();
        
            // SPRITES
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
                // field
        my_sprite_draw(&goal1);
        my_sprite_draw(&goal2);
        
        drawPlayers();
        
        // items
        if (draw_bomb) {
            my_sprite_draw(&bomb_item);
        }
        if (fishtank_item.visible)
            my_sprite_draw(&fishtank_item);
        if (shroom_item.visible)
            my_sprite_draw(&shroom_item);
        if (garfield_item.visible)
            my_sprite_draw(&garfield_item);
        if (craig_item.visible)
            my_sprite_draw(&craig_item);
                
    //ANIMATIONS
        if (!explode_bomb) {
            looped_animation_pow(&bomb_item, 4);
        }
        else {
            explode_bomb = explode_animation(&bomb_item);
            if (explode_bomb == false) {
                draw_bomb = false;
            }
        }
        looped_animation_pow(&fishtank_item, 8);
        looped_animation_pow(&shroom_item, 4);
        hsl_incSprites.h += 2;
        do_update_shroom = true;
                
        update_ball(&pixel_poppy);
        my_sprite_draw(&pixel_poppy);
        
    // TEXT / UI
        for(unsigned int i = 0; i < (g_Game.numPlayers+1); i++)
        {
            PPLAYER player = &g_Players[i];
            looped_animation_pow(player->_sprite, 4);
            player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];            gameplayUI_draw(player);
        }
    }
    else if (!g_Game.isPaused && times_up) {
        if (play_battle_is_over) {
            #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
            pcm_play(g_Assets.gameOverPcm8, PCM_PROTECTED, 6);
            #else
            jo_audio_play_sound_on_channel(&g_Assets.gameOverPcm8, 6);
            #endif
            play_battle_is_over = false;
        }
        jo_nbg0_printf(17, 14, "TIME OUT");
        // transition?  end game?  add scores?  enter initials?
    }
}

void gameScore_draw(void) {
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (!g_Game.isPaused && !times_up) {
        for(unsigned int i = 0; i <= (g_Game.numPlayers); i++)
        {
            PPLAYER player = &g_Players[i];
            gameplayScores_draw(player);
        }
    }
}

void gameplayUI_draw(PPLAYER player) {
    static int portrait_x = 300;
    static int portrait_y = 190;
    static int color_multiplier = 5;
    static int power_meter_height = 3;
    switch (player->teamChoice) 
    {
        case TEAM_1: {
            int heart_x = -(portrait_x - 40);
            int heart_y = -(portrait_y - 12);
            int star_y = -(portrait_y + 12);
            set_shield_position(player->_sprite, &shield1, player->shield_pos);
            looped_animation_pow(&shield1, 4);
            my_sprite_draw(&shield1);

            set_spr_position(player->_portrait, -portrait_x, -portrait_y, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, 16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, 16);
            
            // calculate power meter color            hslSprites.color[p_rangePmenu[0].lower].h = player->shield.power*color_multiplier;            calculate_sprites_color(&p_rangePmenu[0]);
            do_update_Pmenu[0] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[0];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (-portrait_x-25), (-portrait_y+24), 80);
            my_sprite_draw(player->_bg); // have a shield meter for each player?
            break;
        }
        case TEAM_2: {
            int heart_x = portrait_x - 40;
            int heart_y = -(portrait_y - 12);
            int star_y = -(portrait_y + 12);
            set_shield_position(player->_sprite, &shield2, player->shield_pos);
            looped_animation_pow(&shield2, 4);
            my_sprite_draw(&shield2);

            set_spr_position(player->_portrait, portrait_x, -portrait_y, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, -16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, -16);
            
            // calculate power meter color
            hslSprites.color[p_rangePmenu[1].lower].h = player->shield.power*color_multiplier;
            calculate_sprites_color(&p_rangePmenu[1]);
            do_update_Pmenu[1] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[1];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (portrait_x-25), (-portrait_y+24), 80);
            my_sprite_draw(player->_bg);
            break;
        }
        case TEAM_3: {
            int heart_x = -(portrait_x - 40);
            int heart_y = portrait_y + 12;
            int star_y = portrait_y - 12;
            set_shield_position(player->_sprite, &shield3, player->shield_pos);
            looped_animation_pow(&shield3, 4);
            my_sprite_draw(&shield3);

            set_spr_position(player->_portrait, -portrait_x, portrait_y, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, 16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, 16);
            
            // calculate power meter color
            hslSprites.color[p_rangePmenu[2].lower].h = player->shield.power*color_multiplier;
            calculate_sprites_color(&p_rangePmenu[2]);
            do_update_Pmenu[2] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[2];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (-portrait_x-25), (portrait_y+24), 80);
            my_sprite_draw(player->_bg);
            break;
        }
        case TEAM_4: {
            int heart_x = portrait_x - 40;
            int heart_y = portrait_y + 12;
            int star_y = portrait_y - 12;
            set_shield_position(player->_sprite, &shield4, player->shield_pos);
            looped_animation_pow(&shield4, 4);
            my_sprite_draw(&shield4);

            set_spr_position(player->_portrait, portrait_x, portrait_y, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, -16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, -16);
            
            // calculate power meter color
            hslSprites.color[p_rangePmenu[3].lower].h = player->shield.power*color_multiplier;
            calculate_sprites_color(&p_rangePmenu[3]);
            do_update_Pmenu[3] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[3];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (portrait_x-25), (portrait_y+24), 80);
            my_sprite_draw(player->_bg);
            break;
        }
        default:
            break;
    }
    my_sprite_draw(player->_portrait);
}

static bool isRoundOver(void)
{
    // int numTeams = 0;
    // int numSquares = 0;

    // // TODO: check for no time left in time attack
    // numTeams = countActiveTeams();
    // if(numTeams == 0)
    // {
        // // no more teams left, players lost
        // g_RoundOver = true;
        // g_RoundOverTimer = ROUND_OVER_TIME;
        // playCDTrack(DEATH_TRACK);
        // return true;
    // }

    // numSquares = countSquaresRemaining();
    // if(numSquares == 0)
    // {
        // // no more squares left to open, players win
        // g_RoundOver = true;
        // g_RoundOverTimer = ROUND_OVER_TIME;
        // playCDTrack(VICTORY_TRACK);
        // return true;
    // }

    return false;
}

void gameplay_update(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    // don't do anything if the game is paused
    if(g_Game.isPaused == true)
    {
        return;
    }

    updatePlayers();
    playerAI(&pixel_poppy);
    // sweep_and_prune();
    
    if(g_RoundOver == true)
    {
        g_RoundOverTimer--;

        // check if it's time to go to the pause screen
        if(g_RoundOverTimer <= 0)
        {
            g_Game.isPaused = true;
        }
        return;
    }

    g_RoundOver = isRoundOver();
}
void gameplay_input(void)
{
    if(g_Game.isPaused == true || start_timer == false)
    {
        return;
    }
    check_inputs();
    getPlayersInput();
}

void    demo_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        g_Game.lastState = GAME_STATE_DEMO_LOOP;
        transitionState(GAME_STATE_TITLE_SCREEN);
        g_DemoTimer = 0;
    }
}
