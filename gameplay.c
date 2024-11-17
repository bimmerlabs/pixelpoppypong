#include <jo/jo.h>
#include "main.h"
#include "gameplay.h"
#include "assets.h"
#include "font.h"
#include "input.h"
#include "objects/player.h"
// background images
// #include "BG_DEF/BG25.h"
// #include "palettetools.h"
// #include "BG_DEF/bg0.h"
#include "BG_DEF/hexagon.h" // normal map

extern PLAYER g_Players[MAX_PLAYERS];

unsigned int g_DemoTimer = 0;
static Bool draw_demo_text = true;
Bool start_timer = false;
static Bool times_up = false;
Bool g_RoundOver = false;
Uint16 g_RoundOverTimer = 0;

#define ROUND_OVER_TIME_FRAMES (30*4)

static bool isRoundOver(void);
// static void drawStats(void);
// static void drawScore(void);

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

    // check if the frameAnim has expired
    if(g_DemoTimer > DEMO_TIMER)
    {
        changeState(GAME_STATE_PPP_LOGO);
        g_DemoTimer = 0;
    }
}void game_timer(void)
{
    if (!start_timer) {
        return;
    }
    // game timer
    Uint8 ones = g_GameTimer % 10; // Extracts the ones place
    Uint8 tens = g_GameTimer / 10; // Extracts the tens place
    // Uint8 hnds = g_GameTimer / 100; // Extracts the hundreds place
        
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
        
        game_timer();
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        my_sprite_draw(&menu_bg);     // shadow
        
        my_sprite_draw(&macchi);
        my_sprite_draw(&jelly);
        my_sprite_draw(&penny);
        my_sprite_draw(&potter);
        my_sprite_draw(&sparta);
        my_sprite_draw(&player1);
        my_sprite_draw(&boss1);
        my_sprite_draw(&boss2);
        my_sprite_draw(&poppy);
        
                // move to a physics module
        poppy.rot.z++;
        
        // move to an animation module
        if (frame % FRAMERATE == 0) {
            macchi.anim1.frame++;
            if (macchi.anim1.frame > macchi.anim1.max) {
                macchi.anim1.frame = 0;
            }
            macchi.spr_id = macchi.anim1.asset[macchi.anim1.frame];
            
            jelly.anim1.frame++;
            if (jelly.anim1.frame > jelly.anim1.max) {
                jelly.anim1.frame = 0;
            }
            jelly.spr_id = jelly.anim1.asset[jelly.anim1.frame];
        }
    }
    else if (!g_Game.isPaused && times_up) {
        jo_nbg0_printf(16, 14, "TIME OUT");
    }
}

void my_color_calc(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (do_update) {
        update_hexagon_color();
        do_update = false;
        update_palette = true;
    }
}
void my_palette_update(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (update_palette) {
        // need to reference the game state here
        update_hexagon_palette();
        // update_bg25_palette();
    }
}

void load_gameplay_assets(void) {
    // jo_vdp2_clear_bitmap_nbg1(0);
    // sprites
    loadAssets();
    init_font();
    // init_bg0_img();
    // init_hexagon_img();
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);
    slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
}

void gameplay_init() {
    // jo_memset((void *)JO_VDP2_LAST_REG, 0, 0x40000);
    // jo_memset((void *)JO_VDP2_VRAM, 0, 0x40000);
    // jo_memset((void *)JO_VDP2_CRAM, 0, 0x0800);
    resetPlayerScores();

    poppy.scl.x = toFIXED(1.0);
    poppy.scl.y = toFIXED(1.1);
    macchi.pos.x = toFIXED(-350);
    macchi.pos.y = toFIXED(0);
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);
    slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
    
    slColOffsetAUse(OFF);
    slColOffsetBUse(NBG1ON);
    // slColOffsetA(0,0,0);
    slColOffsetB(0,0,0);
    
    // FOR PALETTES
    do_update = true;
    
    g_Game.isPaused = false;
    g_GameTimer = TIMEOUT;
    timer_num1.spr_id = timer_num1.anim1.asset[9];
    timer_num10.spr_id = timer_num10.anim1.asset[9];
    // NEED BETTER NAMES FOR THESE
    start_timer = false;
    times_up = false;
    
    g_RoundOver = false;
    
    slScrPosNbg0(toFIXED(0), toFIXED(0));
    SET_SPR_POSITION(menu_bg.pos, 0, -200, 85);
    SET_SPR_SCALE(menu_bg.scl, 18, 20);
}

void demo_init(void) {
    start_timer = true;
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
        // g_RoundOverTimer = ROUND_OVER_TIME_FRAMES;
        // playCDTrack(DEATH_TRACK);
        // return true;
    // }

    // numSquares = countSquaresRemaining();
    // if(numSquares == 0)
    // {
        // // no more squares left to open, players win
        // g_RoundOver = true;
        // g_RoundOverTimer = ROUND_OVER_TIME_FRAMES;
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
    if(g_Game.isPaused == true)
    {
        return;
    }

    getPlayersInput();
}