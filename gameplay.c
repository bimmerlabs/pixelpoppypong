#include <jo/jo.h>
#include "main.h"
#include "input.h"
#include "gameplay.h"
#include "screen_transition.h"
#include "AI.h"
bool start_timer = false;
bool round_start = false;
static bool play_battle_is_over = true;

static bool draw_demo_text = true;
static bool time_over = false;

static unsigned char hunds;
static unsigned char tens;
static unsigned char ones;
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
    setGoalSize();
    
    // collision detection    // sprite_id = 0;

    // debug only
    reset_ball_movement(&pixel_poppy);
    
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
    initPlayerGoals();
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);
    // slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
    
    g_Game.isPaused = false;
    g_Game.isActive = false;
    // reset timer
    g_Game.GameTimer = TIMEOUT;
    g_Game.BombTimer = BOMB_TIMER;
    convertNumberToDigits(g_Game.GameTimer, &hunds, &tens, &ones);
    timer_num100.spr_id = timer_num10.anim1.asset[hunds];
    timer_num10.spr_id = timer_num10.anim1.asset[tens];
    timer_num1.spr_id = timer_num1.anim1.asset[ones];
    // NEED BETTER NAMES FOR THESE
    start_timer = false;
    g_Game.BeginTimer = 0;
    time_over = false;
    round_start = false;
    g_Game.isRoundOver = false;
    g_Game.isGoalScored = false;
    
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
    g_Game.DemoTimer = 0;
    g_Game.gameMode = GAME_MODE_BATTLE;
}void setGoalSize(void)
{
    switch(g_Game.gameMode)
    {
        case GAME_MODE_CLASSIC:
            g_Game.goalYPosTop = GOAL_Y_POS_MEDIUM;
            g_Game.goalYPosMid = GOAL_CENTER_POS;
            g_Game.goalYPosBot = GOAL_Y_POS_MEDIUM;
            g_Game.goalScale = GOAL_SCALE_MEDIUM;
            break;
        case GAME_MODE_STORY: {
            switch(g_Game.gameDifficulty)
            {
                case GAME_DIFFICULTY_EASY:
                    g_Game.goalYPosTop = GOAL_Y_POS_EASY;
                    g_Game.goalYPosMid = GOAL_CENTER_POS;
                    g_Game.goalYPosBot = GOAL_Y_POS_EASY;
                    g_Game.goalScale = GOAL_SCALE_EASY;
                    break;
                case GAME_DIFFICULTY_MEDIUM:
                    g_Game.goalYPosTop = GOAL_Y_POS_MEDIUM;
                    g_Game.goalYPosMid = GOAL_CENTER_POS;
                    g_Game.goalYPosBot = GOAL_Y_POS_MEDIUM;
                    g_Game.goalScale = GOAL_SCALE_MEDIUM;
                    break;
                case GAME_DIFFICULTY_HARD:
                    g_Game.goalYPosTop = GOAL_Y_POS_HARD;
                    g_Game.goalYPosMid = GOAL_CENTER_POS;
                    g_Game.goalYPosBot = GOAL_Y_POS_HARD;
                    g_Game.goalScale = GOAL_SCALE_HARD;
                    break;
                default:
                    break;
            }
            break;
        }
        case GAME_MODE_BATTLE:
            g_Game.goalYPosTop = GOAL_Y_POS_MEDIUM;
            g_Game.goalYPosMid = GOAL_CENTER_POS;
            g_Game.goalYPosBot = GOAL_Y_POS_MEDIUM;
            g_Game.goalScale = GOAL_SCALE_MEDIUM;
            break;
        default:
            break;
    }
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
        jo_nbg0_printf(20, 27, "DEMO!");
    }
    g_Game.DemoTimer++;
    if (!round_start) {
        playerAI(&pixel_poppy);
        // sweep_and_prune();
        // updatePlayers();
    }
    // check if the frameAnim has expired
    if(g_Game.DemoTimer > DEMO_TIME)
    {
        g_Game.DemoTimer = 0;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}void game_timer(void)
{
    // could move to vblank?
    // wait 3 seconds to start the game
    if (g_Game.BeginTimer > GAME_BEGIN_TIME) {
        start_timer = true;
    }
    if (!start_timer) {
        g_Game.BeginTimer++;
        round_start = true;
        return;
    }
    
    // start round    if (round_start) {
        startGameplay();
        g_Game.isActive = true;
        round_start = false;
    }
    
    if (g_Game.isRoundOver) { // NEED ANOTHER WAY TO STOP THE TIMER
        return;
    }
    
    // game timer
    convertNumberToDigits(g_Game.GameTimer, &hunds, &tens, &ones);
    timer_num100.spr_id = timer_num10.anim1.asset[hunds];
    timer_num10.spr_id = timer_num10.anim1.asset[tens];
    timer_num1.spr_id = timer_num1.anim1.asset[ones];
    
    if (g_Game.GameTimer > 0 && frame % 60 == 0) { // modulus
        g_Game.GameTimer--;
    }
    else if (g_Game.GameTimer == 0) {
        time_over = true;
    }
    // test
    if (g_Game.BombTimer == 0) {
        explode_bomb = true;
    }
    else {
        g_Game.BombTimer--;
    }
    
}
void gameplay_draw(void)
{    
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (!g_Game.isPaused && !time_over) {
        // move to draw/HUD timer function
        game_timer();
        switch(g_Game.gameMode)
        {
            case GAME_MODE_BATTLE:
                drawVsMode();
                drawGameUI();
                break;
            case GAME_MODE_CLASSIC: {
                drawClassicMode();
                drawGameUI();
                break;
            }
            case GAME_MODE_STORY: {
                drawStoryMode();
                drawGameUI();
                break;
            }
            default:
                break;
        }
    }
    else if (!g_Game.isPaused && time_over) {
        if (play_battle_is_over) {
            #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
            pcm_play(g_Assets.gameOverPcm8, PCM_PROTECTED, 6);
            #else
            jo_audio_play_sound_on_channel(&g_Assets.gameOverPcm8, 6);
            #endif
            play_battle_is_over = false;
        }
        jo_nbg0_printf(17, 14, "GAME OVER!");
        // transition?  
        // add scores?
        // enter initials?
        // end game?
    }
}

void gameScore_draw(void) {
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (!g_Game.isPaused && !time_over) {
        for(unsigned int i = 0; i <= (g_Game.numPlayers); i++)
        {
            PPLAYER player = &g_Players[i];
            gameplayScores_draw(player);
        }
    }
}

// todo: move things from switch statement that are the same in each case to the end
void gameplayUI_draw(PPLAYER player) {
    static int color_multiplier = 5;
    static int power_meter_height = 3;
    
    switch (player->teamChoice) 
    {
        case TEAM_1: {
            int heart_x = -(GAMEPLAY_PORTRAIT_X - 40);
            int heart_y = -(GAMEPLAY_PORTRAIT_Y - 12);
            int star_y = -(GAMEPLAY_PORTRAIT_Y + 12);
            set_shield_position(player->_sprite, &shield1, player->shield_pos);
            looped_animation_pow(&shield1, 4);
            my_sprite_draw(&shield1);
                        // If speed is an issue, can create a sprite object for each portrait during gameplay
            set_spr_position(player->_portrait, -GAMEPLAY_PORTRAIT_X, -GAMEPLAY_PORTRAIT_Y, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, 16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, 16);
            
            // calculate power meter color            hslSprites.color[p_rangePmenu[0].lower].h = player->shield.power*color_multiplier;            calculate_sprites_color(&p_rangePmenu[0]);
            do_update_Pmenu[0] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[0];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (-GAMEPLAY_PORTRAIT_X-25), (-GAMEPLAY_PORTRAIT_Y+24), 80);
            // my_sprite_draw(player->_bg); // have a shield meter for each player?
            break;
        }
        case TEAM_2: {
            int heart_x = GAMEPLAY_PORTRAIT_X - 40;
            int heart_y = -(GAMEPLAY_PORTRAIT_Y - 12);
            int star_y = -(GAMEPLAY_PORTRAIT_Y + 12);
            set_shield_position(player->_sprite, &shield2, player->shield_pos);
            looped_animation_pow(&shield2, 4);
            my_sprite_draw(&shield2);

            set_spr_position(player->_portrait, GAMEPLAY_PORTRAIT_X, -GAMEPLAY_PORTRAIT_Y, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, -16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, -16);
            
            // calculate power meter color
            hslSprites.color[p_rangePmenu[1].lower].h = player->shield.power*color_multiplier;
            calculate_sprites_color(&p_rangePmenu[1]);
            do_update_Pmenu[1] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[1];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (GAMEPLAY_PORTRAIT_X-25), (-GAMEPLAY_PORTRAIT_Y+24), 80);
            // my_sprite_draw(player->_bg);
            break;
        }
        case TEAM_3: {
            int heart_x = -(GAMEPLAY_PORTRAIT_X - 40);
            int heart_y = GAMEPLAY_PORTRAIT_Y + 12;
            int star_y = GAMEPLAY_PORTRAIT_Y - 12;
            set_shield_position(player->_sprite, &shield3, player->shield_pos);
            looped_animation_pow(&shield3, 4);
            my_sprite_draw(&shield3);

            set_spr_position(player->_portrait, -GAMEPLAY_PORTRAIT_X, GAMEPLAY_PORTRAIT_Y-2, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, 16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, 16);
            
            // calculate power meter color
            hslSprites.color[p_rangePmenu[2].lower].h = player->shield.power*color_multiplier;
            calculate_sprites_color(&p_rangePmenu[2]);
            do_update_Pmenu[2] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[2];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (-GAMEPLAY_PORTRAIT_X-25), (GAMEPLAY_PORTRAIT_Y+24), 80);
            // my_sprite_draw(player->_bg);
            break;
        }
        case TEAM_4: {
            int heart_x = GAMEPLAY_PORTRAIT_X - 40;
            int heart_y = GAMEPLAY_PORTRAIT_Y + 12;
            int star_y = GAMEPLAY_PORTRAIT_Y - 12;
            set_shield_position(player->_sprite, &shield4, player->shield_pos);
            looped_animation_pow(&shield4, 4);
            my_sprite_draw(&shield4);

            set_spr_position(player->_portrait, GAMEPLAY_PORTRAIT_X, GAMEPLAY_PORTRAIT_Y-2, 90);
            draw_heart_element(&heart, player->numLives, heart_x, heart_y, -16);
            draw_ui_element(&star, player->score.stars, heart_x, star_y, -16);
            
            // calculate power meter color
            hslSprites.color[p_rangePmenu[3].lower].h = player->shield.power*color_multiplier;
            calculate_sprites_color(&p_rangePmenu[3]);
            do_update_Pmenu[3] = true;
            
            player->_bg->spr_id = player->_bg->anim1.asset[3];
            set_spr_scale(player->_bg, player->shield.power, power_meter_height);
            set_spr_position(player->_bg, (GAMEPLAY_PORTRAIT_X-25), (GAMEPLAY_PORTRAIT_Y+24), 80);
            // my_sprite_draw(player->_bg);
            break;
        }
        default:
            break;
    }
    set_spr_scale(player->_bg, player->shield.power, power_meter_height);
    my_sprite_draw(player->_bg);
    my_sprite_draw(player->_portrait);
}

static bool isRoundOver(void)
{
    // count number of players still active?
    
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
    
    if(g_Game.isRoundOver == true)
    {
        g_Game.RoundOverTimer--;

        // check if it's time to go to the pause screen
        if(g_Game.RoundOverTimer <= 0)
        {
            g_Game.isPaused = true;
        }
        return;
    }

    g_Game.isRoundOver = isRoundOver();
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
        g_Game.DemoTimer = 0;
    }
}
