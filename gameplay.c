#include <jo/jo.h>
#include "main.h"
#include "input.h"
#include "gameplay.h"
#include "physics.h"
#include "assets.h"
#include "screen_transition.h"
#include "objects/player.h"
#include "AI.h"
// background images
// #include "BG_DEF/BG25.h"
// #include "palettetools.h"
// #include "BG_DEF/bg0.h"
// #include "BG_DEF/hexagon.h" // normal map

extern PLAYER g_Players[MAX_PLAYERS];

unsigned int g_DemoTimer = 0;
bool start_timer = false;
bool round_start = false;
bool g_RoundOver = false;
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
    // jo_memset((void *)JO_VDP2_LAST_REG, 0, 0x40000);
    // jo_memset((void *)JO_VDP2_VRAM, 0, 0x40000);
    // jo_memset((void *)JO_VDP2_CRAM, 0, 0x0800);
    
    g_Game.lastState = GAME_STATE_GAMEPLAY;
        if (g_Game.nextState == GAME_STATE_GAMEPLAY)
    {
        // do different inits depending on game mode (demo etc)
        switch(g_Game.gameMode)
        {
            case GAME_MODE_BATTLE:
                initVsModePlayers();
                break;
            case GAME_MODE_CLASSIC:
                initVsModePlayers();
                break;
            case GAME_MODE_STORY:
                initVsModePlayers();
                break;
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
    
    // collision detection    sprite_count = 0;

    // debug only
    reset_ball(&pixel_poppy);
    
    // set_spr_position(&pixel_poppy, 0, 0, 100);
    // set_spr_scale(&pixel_poppy, 1.0, 1.1);
    sprite_frame_reset(&pixel_poppy);
    pixel_poppy.isColliding = false;
    // add_sprite_to_sweep_and_prune(&pixel_poppy);
    // update_bounding_box(&pixel_poppy);
    // add_sprite_to_sweep_and_prune(&macchi);
    // add_sprite_to_sweep_and_prune(&jelly);
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);
    // slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
    
    // FOR PALETTES
    // do_update = true;
    
    g_Game.isPaused = false;
    g_GameTimer = TIMEOUT;
    timer_num1.spr_id = timer_num1.anim1.asset[9];
    timer_num10.spr_id = timer_num10.anim1.asset[9];
    // NEED BETTER NAMES FOR THESE
    start_timer = false;
    g_GameBeginTimer = 0;
    times_up = false;
    round_start = false;
    g_RoundOver = false;
    
    slScrPosNbg0(toFIXED(0), toFIXED(0));
    
    menu_bg1.spr_id = menu_bg1.anim1.asset[4];
    set_spr_position(&menu_bg1, 0, -195, 85);
    set_spr_scale(&menu_bg1, 36, 20);
    
    reset_audio(MAX_VOLUME);
    playCDTrack(BEGIN_GAME_TRACK, false);
    
    // temporary - to be removed
    player1_score = 0;
    player2_score = 0;
}

void demo_init(void) {
    g_Game.lastState = GAME_STATE_DEMO_LOOP;
    unloadTitleScreenAssets();
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
    if (frame % 10 == 0) {
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
        transitionState(GAME_STATE_UNINITIALIZED);
        g_DemoTimer = 0;
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
        // if (game_options.debug_display) {
            start_ball_movement(&pixel_poppy);
        // }
        round_start = false;
    }
    
    // game timer
    Uint8 ones = g_GameTimer % 10; // Extracts the ones place
    Uint8 tens = g_GameTimer / 10; // Extracts the tens place
        
    timer_num10.spr_id = timer_num10.anim1.asset[tens];
    timer_num1.spr_id = timer_num1.anim1.asset[ones];
    
    if (g_GameTimer > 0 && frame % 60 == 0) {
        g_GameTimer--;
    }
    else if (g_GameTimer == 0) {
        times_up = true;
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
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        my_sprite_draw(&menu_bg1);     // shadow
                // field
        my_sprite_draw(&goal1);
        my_sprite_draw(&goal2);
                drawPlayers();
        
        update_ball(&pixel_poppy);
        my_sprite_draw(&pixel_poppy);
        
        // move to a physics module
        // pixel_poppy.rot.z++;
        
        jo_nbg0_printf(8, 2, "%09d", player1_score);
        jo_nbg0_printf(27, 2, "%09d", player2_score);
        
          
        for(unsigned int i = 0; i < (g_Game.numPlayers+1); i++)
        {
            PPLAYER player = &g_Players[i];
            // move to an animation module
            my_sprite_animation(player->_sprite);
            
            player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];    
            set_spr_scale(player->_portrait, 1, 1);
            if (player->onLeftSide == true) {
                set_spr_position(player->_portrait, -300, -190, 90); // PORTRAIT_DEPTH
            }
            else {
                set_spr_position(player->_portrait, 300, -190, 90); // PORTRAIT_DEPTH
            }
            my_sprite_draw(player->_portrait);
        }
    }
    else if (!g_Game.isPaused && times_up) {
        jo_nbg0_printf(17, 14, "TIME OUT");
    }
}

// void my_color_calc(void)
// {
    // if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    // {
        // return;
    // }
    // if (do_update) {
        // update_hexagon_color();
        // do_update = false;
        // update_palette = true;
    // }
// }
// void my_palette_update(void)
// {
    // if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    // {
        // return;
    // }
    // if (update_palette) {
        // // need to reference the game state here
        // update_hexagon_palette();
        // // update_bg25_palette();
    // }
// }

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