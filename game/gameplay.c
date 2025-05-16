#include <jo/jo.h>
#include "../main.h"
#include "gameplay.h"
#include "storymode.h"
#include "AI.h"
#include "../core/input.h"
#include "../core/backup.h"
Item g_item = {0};
// TODO: implement this function
// static bool isRoundOver(void);

GAMEPLAY g_Gameplay = {0};void initGameplayStruct(void) {
    g_Gameplay.GameTimer = 0;
    // g_Gameplay.RoundOverTimer = 0; // NOT USED
    g_Gameplay.DemoTimer = 0;
    g_Gameplay.isGameOver = false; // NOT USED
    
    g_Gameplay.draw_demo_text = false;
    g_Gameplay.start_gameplay_timer = false;
    g_Gameplay.round_start = false;
}
void gameplay_init() {
    if (g_Game.nextState != GAME_STATE_DEMO_LOOP && g_Game.lastState != GAME_STATE_GAMEPLAY) {
        if (g_Game.gameMode == GAME_MODE_STORY) {
            unloadTitleScreenAssets();
            loadCharacterAssets();
        }
        loadGameAssets();
    }
    g_Game.lastState = GAME_STATE_GAMEPLAY;    if (g_Game.nextState == GAME_STATE_GAMEPLAY)
    {
        switch(g_Game.gameMode)
        {
            case GAME_MODE_BATTLE:
                initVsModePlayers();
                g_Transition.fade_in = true;
                break;
            case GAME_MODE_CLASSIC:
                initVsModePlayers();
                g_Transition.fade_in = true;
                break;
            case GAME_MODE_STORY:
                initStoryMode();
                g_Transition.story_fade_in = true;
                break;
            default:
                break;
        }
    }
    initGameplayStruct();
    resetTeamState(); // only needed if restarting the game
    
    resetSpriteColors();
    initGoalColors();
    hsl_incSprites[HSL_FISH].h += FISH_HUE_INCREMENT;
    do_update_fish = true;
    
    initGoals();
    setGoalSize();

    if (g_GameOptions.mosaic_display) {
        g_Transition.mosaic_in = true;
    }
    if (g_GameOptions.mesh_display) {
        menu_bg1.mesh = MESHon;
        menu_bg1.spr_id = menu_bg1.anim1.asset[4];
        // dead.spr_id = dead.anim1.asset[1];
    }
    else {
        menu_bg1.mesh = MESHoff;
        // dead.spr_id = dead.anim1.asset[0];
    }
    g_Transition.music_in = true;
    g_Transition.all_in = true;

    resetPlayerScores();
    getContinues();
    reset_ball_movement(&pixel_poppy);
    sprite_frame_reset(&pixel_poppy);
    pixel_poppy.isColliding = false;
    
    for (int i = 0; i < MAX_PLAYERS; i++) {
        sprite_frame_reset(&shield[0]);
    }
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);
    
    g_Game.isPaused = false;
    g_Game.isActive = false;
    g_Game.isBallActive = false;
        
    g_Game.isRoundOver = false;
    g_Game.countofRounds = 0;
    g_Game.isGoalScored = false;
    g_Game.winner = -2;
    g_Game.explodeBall = false;
    
    // reset timers
    setGameTimer();
    
    // slScrPosNbg1(FIXED_0, FIXED_0);
    
    menu_bg1.spr_id = menu_bg1.anim1.asset[4];
    set_spr_position(&menu_bg1, 0, -195, 85);
    set_spr_scale(&menu_bg1, 36, 20);
    
    g_Transition.explosion_flash = false;    
    reset_audio(MAX_VOLUME);
    playCDTrack(BEGIN_GAME_TRACK, false);
}

void demo_init(void) {
    g_Game.lastState = GAME_STATE_DEMO_LOOP;
    unloadTitleScreenAssets();
    loadCharacterAssets();
    loadGameAssets();
    initTeams();
    initDemoPlayers();
    g_Game.selectStoryCharacter = false;
    g_Gameplay.DemoTimer = 0;
    g_Game.gameMode = GAME_MODE_BATTLE;
    g_Transition.fade_in = true;
}
void setGameTimer(void) {
    if (g_Game.countofRounds == 0) {
        switch(g_Game.gameMode)
        {
            case GAME_MODE_BATTLE:
                g_Gameplay.GameTimer = TIMEOUT_BATTLE;
                break;
            case GAME_MODE_CLASSIC:
                g_Gameplay.GameTimer = TIMEOUT_CLASSIC;
                break;
            case GAME_MODE_STORY:
                switch(g_Game.gameDifficulty)
                {
                    case GAME_DIFFICULTY_EASY:
                        g_Gameplay.GameTimer = TIMEOUT_STORY_EASY;
                        break;
                    case GAME_DIFFICULTY_MEDIUM:
                        g_Gameplay.GameTimer = TIMEOUT_STORY_MEDIUM;
                        break;
                    case GAME_DIFFICULTY_HARD:
                        g_Gameplay.GameTimer = TIMEOUT_STORY_HARD;
                        break;
                    default:
                        g_Gameplay.GameTimer = TIMEOUT_STORY_MEDIUM;
                        break;
                }
                break;
            default:
                g_Gameplay.GameTimer = TIMEOUT_BATTLE;
                break;
        }
    }
    else if (g_Game.gameMode == GAME_MODE_STORY) {
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
            g_Gameplay.GameTimer += touchedBy[0].touchCount;
        }
        else {
            g_Gameplay.GameTimer += addedTime;
        }
        if (g_Gameplay.GameTimer > TIMER_MAX) {
            g_Gameplay.GameTimer = TIMER_MAX;
        }
    }
    g_Game.roundBeginTimer = ROUND_BEGIN_TIME_NORMAL;
    g_Game.dropBallTimer = DROP_BALL_TIME_NORMAL;
    g_Game.time_over = false;
    g_Gameplay.start_gameplay_timer = false;
    g_Gameplay.round_start = false;
    g_Game.endDelayTimer = GAME_END_DELAY_TIMEOUT;
    g_Game.BeginTimer = 0;
    
    for(unsigned int i = 0; i < MAX_PLAYERS; i++) // this is resetting the mushroom between rounds
    {
        if (!g_Players[i].isBig || !g_Players[i].isSmall) {
            g_item.timer[i] = 0;
        }
    }
}

void demo_update(void)
{
    if(g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (JO_MOD_POW2(g_Game.frame, 8) == 0) { // modulus
        g_Gameplay.draw_demo_text = !g_Gameplay.draw_demo_text;
    }
    if (g_Gameplay.draw_demo_text) {
        jo_nbg0_printf(20, 27, "DEMO!");
    }
    g_Gameplay.DemoTimer++;
    playerAI(&pixel_poppy);
    if(g_Gameplay.DemoTimer > DEMO_TIME)
    {
        g_Gameplay.DemoTimer = 0;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}void gameplay_timer(void)
{
    if (g_Game.BeginTimer > g_Game.roundBeginTimer && !g_Gameplay.start_gameplay_timer) {
        g_Gameplay.start_gameplay_timer = true;
        g_Game.roundBeginTimer = ROUND_BEGIN_TIME_NORMAL; // reset if the ball explodes
    }
    if (g_Game.BeginTimer > g_Game.dropBallTimer && !g_Game.isBallActive) {
        g_Game.isBallActive = drop_ball_animation(&pixel_poppy);
    }
    if (!g_Gameplay.start_gameplay_timer) {
        g_Game.BeginTimer++;
        g_Gameplay.round_start = true;
        return;
    }
    
    // start round    if (g_Gameplay.round_start) {
        g_Gameplay.round_start = startGameplay(); // returns true until ball is active
    }
    
    if (g_Game.isRoundOver) {
        return;
    }
        
    if (g_Gameplay.GameTimer > 0 && g_Game.frame % 60 == 0) { // modulus
        g_Gameplay.GameTimer--;
        if (g_Gameplay.GameTimer <= 10)
            pcm_play(g_Assets.countdownPcm8, PCM_PROTECTED, 7);
    }
    else if (g_Gameplay.GameTimer == 0) {
        g_Game.time_over = true;
    }
    
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        if (g_item.timer[i] > 0) {
            g_item.timer[i]--;
        }
    }
    if (!g_Game.explodeBall) {
        ballTtouchTimer++;
    }
    if (ballTtouchTimer == BALL_TOUCH_TIMEOUT && !g_Game.explodeBall) {
        stopBallMovement(&pixel_poppy);
        g_Game.explodeBall = true;
        g_Transition.explosion_flash = true;
        pixel_poppy.pos.r = EASY_BALL_RADIUS;
        pixel_poppy.scl.x = toFIXED(2.0);
        pixel_poppy.scl.y = toFIXED(2.0);
    }
    
    if (g_Game.explodeBall) {
        if (g_Transition.explosion_flash) {
            g_Transition.explosion_flash = explosionEffect();
        }
        g_Game.explodeBall = explode_animation(&pixel_poppy);
        if (!g_Game.explodeBall) {
            g_Game.roundBeginTimer = ROUND_BEGIN_TIME_FAST;
            g_Game.dropBallTimer = DROP_BALL_TIME_FAST;
            ballTtouchTimer = 0;
            initPixelPoppy();
            resetPlayerAttacks();
            g_Gameplay.start_gameplay_timer = false;
            g_Game.isBallActive = false;
            g_Game.isActive = false;
            g_Game.BeginTimer = 0;
        }
    }
    bombTimer();
}
void gameplay_draw(void)
{    
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    
    #if ENABLE_DEBUG_MODE == 1
    gameplayDebugText();
    #endif
    
    explodeGoals();
    if (g_Transition.fade_out) { // works for both ball and bomb..
        g_Transition.fade_out = fadeOut(8, NEUTRAL_FADE);
    }
                   
    animateGoalColor(&do_update_Goals[g_Game.goalID]);
    animateBombColor(&do_update_bomb);
    
    if (g_Game.endDelayTimer == 0) {
        // game ending
        if (g_Game.nextState == GAME_STATE_GAMEPLAY && g_Game.countofRounds == MAX_ROUNDS) {
            // TODO:  game ending
            transitionState(GAME_STATE_NAME_ENTRY);
        }
        // you ran out of time or died
        else if (g_Game.nextState == GAME_STATE_GAMEPLAY &&  g_Game.winner == -1) {
            initContinue();
        }
        // you finished off your opponent..
        else if (g_Game.nextState == GAME_STATE_GAMEPLAY &&  g_Game.winner == 0) {
            initNextRound();
        }
        else {
            if (g_Game.isRoundOver) {
                pcm_play(g_Assets.gameOverPcm8, PCM_PROTECTED, 7); // play different sound if player wins vs loses?
                g_Game.isRoundOver = false;
                g_Gameplay.isGameOver = true;
            }
            if (g_Game.frame == 240) { // this is a hack
                if (g_Game.gameMode == GAME_MODE_STORY &&  g_Players[0].subState == PLAYER_STATE_ACTIVE) {
                    g_Players[0].score.points = g_Players[0].score.total;
                    if (g_Game.nextState == GAME_STATE_NAME_ENTRY) {
                        // Unlock Stadler
                        if (!characterUnlocked[CHARACTER_WALRUS] &&
                            (g_GameOptions.craigTouchCounter >= 100 ||
                             g_Game.gameDifficulty == GAME_DIFFICULTY_MEDIUM ||
                              g_Players[0].score.points >= UNLOCK_CRAIG_SCORE))
                                {
                                    characterUnlocked[CHARACTER_WALRUS] = true;
                                }

                        // Unlock Garfield
                        if (!characterUnlocked[CHARACTER_GARF] &&
                            (g_GameOptions.garfTouchCounter >= 200 ||
                             g_Game.gameDifficulty == GAME_DIFFICULTY_HARD ||
                              g_Players[0].score.points >= UNLOCK_GARFIELD_SCORE))
                                {
                                    characterUnlocked[CHARACTER_GARF] = true;
                                }
                        // Unlock BIG HEAD MODE
                        if (!g_GameOptions.unlockBigHeadMode && 
                            (g_Game.gameDifficulty == GAME_DIFFICULTY_MEDIUM ||
                            g_Game.gameDifficulty == GAME_DIFFICULTY_HARD))
                                {
                                    g_GameOptions.unlockBigHeadMode = true;
                                }
                        save_game_backup();
                    }
                }                
                transitionState(g_Game.nextState);
            }
        }
    }
    else if (g_Game.currentNumPlayers == 1) {
        if (!g_Game.isRoundOver) {
            g_Game.isRoundOver = true;
            g_Game.frame = 0; // this is a hack
            g_Game.winner = determineWinner();
            g_Transition.all_out = true;
            g_Transition.story_fade_out = true;
        }       
            switch(g_Game.gameMode)
            {
                case GAME_MODE_BATTLE:
                    g_Game.endDelayTimer--;
                    if (g_Game.winner > -1) {
                        jo_nbg0_printf(17, 14, "GAME OVER!");
                        jo_nbg0_printf(15, 16, "PLAYER %i WINS!", g_Game.winner+1);
                        g_Game.nextState = GAME_STATE_NAME_ENTRY;
                    }
                    else {
                        jo_nbg0_printf(17, 14, "GAME OVER!");
                        jo_nbg0_printf(18, 16, "YOU LOSE");
                        g_Game.nextState = GAME_STATE_UNINITIALIZED;
                    }
                    break;
                case GAME_MODE_CLASSIC:
                    g_Game.endDelayTimer--;
                    if (g_Game.winner > -1) {
                        jo_nbg0_printf(17, 14, "GAME OVER!");
                        jo_nbg0_printf(15, 16, "PLAYER %i WINS!", g_Game.winner+1);
                        g_Game.nextState = GAME_STATE_NAME_ENTRY;
                    }
                    else {
                        jo_nbg0_printf(17, 14, "GAME OVER!");
                        jo_nbg0_printf(18, 16, "YOU LOSE");
                        g_Game.nextState = GAME_STATE_UNINITIALIZED;
                    }
                    break;
                case GAME_MODE_STORY:
                    if (g_Game.winner == 0 && g_Game.countofRounds == MAX_ROUNDS) {
                        jo_nbg0_printf(17, 10, "GAME OVER!");
                        tallyScore();                        
                        if (g_Game.endDelayTimer < WIN_GAME_DELAY_TIMEOUT)
                            jo_nbg0_printf(17, 18, "YOU WIN!!!");
                        g_Game.nextState = GAME_STATE_NAME_ENTRY;
                    }
                    else if (g_Game.winner == -1) {
                        g_Game.endDelayTimer--;
                        // check for continues
                        if (g_Players[0].score.continues > 0) {
                            g_Game.nextState = GAME_STATE_GAMEPLAY;
                            jo_nbg0_printf(17, 14, "TRY AGAIN!");
                            jo_nbg0_printf(12, 16, "REMAINING CONTINUES:%i", g_Players[0].score.continues);
                            
                        }
                        else {
                            jo_nbg0_printf(17, 14, "GAME OVER!");
                            jo_nbg0_printf(18, 16, "YOU LOSE");
                            g_Game.nextState = GAME_STATE_UNINITIALIZED;
                        }
                    }
                    else {
                        tallyScore();                        
                        if (g_Game.endDelayTimer < NEXT_BATTLE_DELAY_TIMEOUT)
                            jo_nbg0_printf(15, 20, "NEXT BATTLE..");
                        g_Game.nextState = GAME_STATE_GAMEPLAY;
                    }
                    break;
                default:
                    g_Game.endDelayTimer--;
                    break;
            }
    }
    else if (g_Game.time_over) {
        if (!g_Game.isRoundOver) {
            stopBallMovement(&pixel_poppy);
            g_Game.isRoundOver = true;
            g_Game.frame = 0; // this is a hack
            g_Transition.all_out = true;
            g_Transition.story_fade_out = true;
        }       
        jo_nbg0_printf(16, 14, "OUTTA TIME!");
        g_Gameplay.GameTimer = 60;
        g_Game.endDelayTimer--;
            switch(g_Game.gameMode)
            {
                case GAME_MODE_BATTLE:
                    jo_nbg0_printf(17, 16, "YOU LOSE..");
                    g_Game.nextState = GAME_STATE_UNINITIALIZED;
                    break;
                case GAME_MODE_CLASSIC:
                    jo_nbg0_printf(17, 16, "YOU LOSE..");
                    g_Game.nextState = GAME_STATE_UNINITIALIZED;
                    break;
                case GAME_MODE_STORY:
                    g_Game.winner = -1;
                    // kill player 1
                    if (g_Players[0].subState == PLAYER_STATE_ACTIVE) {
                        g_Players[0].score.continues--;
                        g_Players[0].subState = PLAYER_STATE_DEAD;
                    }
                    if (g_Players[0].score.continues >= 0) {
                        g_Game.nextState = GAME_STATE_GAMEPLAY;
                        jo_nbg0_printf(12, 16, "REMAINING CONTINUES:%i", g_Players[0].score.continues);
                    }
                    else {
                        jo_nbg0_printf(17, 16, "YOU LOSE..");
                        g_Game.nextState = GAME_STATE_UNINITIALIZED;
                    }
                    break;
                default:
                    break;
            }
    } 
    else if (g_Game.selectStoryCharacter) {
        if (g_Game.countofRounds > 0) {
            g_Transition.all_out = true;
            g_Transition.mosaic_out = true;
        }
        else {
            g_Transition.mosaic_in = false;
        }
        storySelectUpdate();
    } 
    else if (!g_Game.isPaused) {
        gameplay_timer();
        switch(g_Game.gameMode)
        {
            case GAME_MODE_BATTLE:
                drawVsMode();
                drawGameUI();
                break;
            case GAME_MODE_CLASSIC:
                drawClassicMode();
                drawGameUI();
                break;
            case GAME_MODE_STORY:
                drawVsMode();
                drawGameUI();
                break;
            default:
                break;
        }
    }
}

int determineWinner(void) {
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];
        if (!player->isAI && player->subState != PLAYER_STATE_DEAD) {
            return i;
        }
    }
    return -1;
}


void gameScore_draw(void) {
    if(g_Game.gameState != GAME_STATE_GAMEPLAY && g_Game.gameState != GAME_STATE_DEMO_LOOP)
    {
        return;
    }
    if (!g_Game.isPaused && !g_Game.time_over && !g_Game.selectStoryCharacter && g_Game.currentNumPlayers > 1) {
        for(unsigned int i = 0; i <= (g_Game.numPlayers); i++)
        {
            PPLAYER player = &g_Players[i];
            switch (g_Game.gameMode) 
            {
                case GAME_MODE_STORY: {
                    storymodeScore_draw(player);
                    break;
                }
                default:
                    gameplayScore_draw(player);
                    break;
            }
        }
    }
}

void gameplayUI_draw(PPLAYER player) {
    set_spr_scale(player->_portrait, 1.1, 1); // only really needs to do this once, after displaying the story/campaign screen
    // calculate power meter color
    hslSprites.color[p_rangePmenu[player->playerID].lower].h = player->shield.power*COLOR_MULTIPLIER;
    calculate_sprites_color(&p_rangePmenu[player->playerID]);
    do_update_Pmenu[player->playerID] = true;

    player->_bg->spr_id = player->_bg->anim1.asset[player->playerID];
    set_spr_scale(player->_bg, player->shield.power, POWER_METER_HEIGHT);        
    int heart_x = 0;
    int heart_y = 0;
    int star_x = 0;
    int offset = 0;    // TODO:  eliminate magic numbers
    switch (player->teamChoice)
    {
        case TEAM_1: {
            heart_x = -(GAMEPLAY_PORTRAIT_X - 54);
            heart_y = -(GAMEPLAY_PORTRAIT_Y - 8);
            star_x = -(GAMEPLAY_PORTRAIT_X - 208);
            offset = 16;
            set_spr_position(&dead, -GAMEPLAY_PORTRAIT_X, -GAMEPLAY_PORTRAIT_Y, 90);
            set_spr_position(player->_portrait, -GAMEPLAY_PORTRAIT_X, -GAMEPLAY_PORTRAIT_Y, 90);
            set_spr_position(player->_bg, (-GAMEPLAY_PORTRAIT_X-25), (-GAMEPLAY_PORTRAIT_Y+24), 80);
            break;
        }
        case TEAM_2: {
            heart_x = GAMEPLAY_PORTRAIT_X - 54;
            heart_y = -(GAMEPLAY_PORTRAIT_Y - 8);
            star_x = GAMEPLAY_PORTRAIT_X - 208;
            offset = -16;
            set_spr_position(&dead, GAMEPLAY_PORTRAIT_X, -GAMEPLAY_PORTRAIT_Y, 90);
            set_spr_position(player->_portrait, GAMEPLAY_PORTRAIT_X, -GAMEPLAY_PORTRAIT_Y, 90);
            set_spr_position(player->_bg, (GAMEPLAY_PORTRAIT_X-25), (-GAMEPLAY_PORTRAIT_Y+24), 80);
            break;
        }
        case TEAM_3: {
            heart_x = -(GAMEPLAY_PORTRAIT_X - 54);
            heart_y = GAMEPLAY_PORTRAIT_Y + 12;
            star_x = -(GAMEPLAY_PORTRAIT_X - 208);
            offset = 16;
            set_spr_position(&dead, -GAMEPLAY_PORTRAIT_X, GAMEPLAY_PORTRAIT_Y-2, 90);
            set_spr_position(player->_portrait, -GAMEPLAY_PORTRAIT_X, GAMEPLAY_PORTRAIT_Y-2, 90);
            set_spr_position(player->_bg, (-GAMEPLAY_PORTRAIT_X-25), (GAMEPLAY_PORTRAIT_Y+24), 80);
            break;
        }
        case TEAM_4: {
            heart_x = GAMEPLAY_PORTRAIT_X - 54;
            heart_y = GAMEPLAY_PORTRAIT_Y + 12;
            star_x = GAMEPLAY_PORTRAIT_X - 208;
            offset = -16;
            set_spr_position(&dead, GAMEPLAY_PORTRAIT_X, GAMEPLAY_PORTRAIT_Y-2, 90);
            set_spr_position(player->_portrait, GAMEPLAY_PORTRAIT_X, GAMEPLAY_PORTRAIT_Y-2, 90);
            set_spr_position(player->_bg, (GAMEPLAY_PORTRAIT_X-25), (GAMEPLAY_PORTRAIT_Y+24), 80);
            break;
        }
        default:
            break;
    }
    my_sprite_draw(player->_portrait);
    draw_heart_element(&heart, player, heart_x, heart_y, offset);
    if (player->subState == PLAYER_STATE_DEAD) {
        my_sprite_draw(&dead);
        return;
    }
    draw_star_element(&star, player->score.stars, star_x, heart_y, offset);
    set_spr_scale(player->_bg, player->shield.power, POWER_METER_HEIGHT);
    my_sprite_draw(player->_bg);
}

void reset_ball_movement(Sprite *ball) {
    ball_velocity = 0;
    ball_falling = true;
    ball_bounce = false;
    ball->pos.x = FIXED_0;
    ball->pos.y = toFIXED(-280);
    ball->vel.x = FIXED_0;
    ball->vel.y = FIXED_0;
    ball->vel.z = 0;
    
    // big head option (only with 2 players)    
    if(g_GameOptions.unlockBigHeadMode && g_GameOptions.bigHeadMode && g_Game.numPlayers < 3)
    {
        ball->pos.r = EASY_BALL_RADIUS;
        ball->scl.x = toFIXED(2.0);
        ball->scl.y = toFIXED(2.0);
    }
    else {
        ball->pos.r = NORMAL_BALL_RADIUS;
        ball->scl.x = toFIXED(1.0);
        ball->scl.y = toFIXED(1.1);
    }
}
// TODO:  implement function!
// static bool isRoundOver(void)
// {
    // // count number of players still active?
    
    // // int numTeams = 0;
    // // int numSquares = 0;

    // // // TODO: check for no time left in time attack
    // // numTeams = countActiveTeams();
    // // if(numTeams == 0)
    // // {
        // // // no more teams left, players lost
        // // g_RoundOver = true;
        // // g_RoundOverTimer = ROUND_OVER_TIME;
        // // playCDTrack(DEATH_TRACK);
        // // return true;
    // // }

    // // numSquares = countSquaresRemaining();
    // // if(numSquares == 0)
    // // {
        // // // no more squares left to open, players win
        // // g_RoundOver = true;
        // // g_RoundOverTimer = ROUND_OVER_TIME;
        // // playCDTrack(VICTORY_TRACK);
        // // return true;
    // // }

    // return false;
// }

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
        // TODO: implement this!!    
    // if(g_Game.isRoundOver == true)
    // {
        // g_Gameplay.RoundOverTimer--;

        // // check if it's time to go to the pause screen
        // if(g_Gameplay.RoundOverTimer <= 0)
        // {
            // g_Game.isPaused = true;
        // }
        // return;
    // }

    // g_Game.isRoundOver = isRoundOver();
}
void gameplay_input(void)
{
    if(g_Game.isPaused == true)
    {
        return;
    }
    check_inputs();
    switch (g_Game.gameMode) {
        case GAME_MODE_CLASSIC:
            getClassicModeInput();
            break;
        default:
            getPlayersInput();
            break;
    }
}

void    demo_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        g_Game.lastState = GAME_STATE_DEMO_LOOP;
        transitionState(GAME_STATE_TITLE_SCREEN);
        g_Gameplay.DemoTimer = 0;
    }
}

int ball_velocity = 0;
bool ball_falling = false;
bool ball_bounce = false;
bool drop_ball_animation(Sprite *ball) {
    // ball is falling
    if (ball_falling && !ball_bounce) {
        pcm_play(g_Assets.dropPcm8, PCM_PROTECTED, 6);
        if (ball_velocity < BALL_VELOCITY) {
            if (JO_MOD_POW2(g_Game.frame, 2) == 0) { // modulus
                ball_velocity += FIXED_1;
            }
        }              
        if (ball->pos.y < FIXED_0) {
            ball->pos.y += ball_velocity;
        }
        // ball has hit the "ground"
        else {
            ball_falling = false;
            ball_bounce = true;
        }
        return false;
    }
    // ball is bouncing
    if (ball_bounce && !ball_falling) {
        ball->pos.y -= ball_velocity;
        if (ball_velocity > 0) {
            ball_velocity -= toFIXED(1.5);
        }
        else {
            // ball_velocity = 0;
            pcm_play(g_Assets.bouncePcm8, PCM_VOLATILE, 5);
            ball_falling = true;
            ball_bounce = true;
        }
        return false;
    }
    // first bounce has already happened
    else if (ball_bounce && ball_falling) {
        ball_falling = false;
        ball_bounce = false;
        return false;
    }
    // falling again
    else if (!ball_bounce && !ball_falling) {
        if (ball_velocity < BALL_VELOCITY) {
            if (JO_MOD_POW2(g_Game.frame, 2) == 0) { // modulus
                ball_velocity += toFIXED(1.5);
            }
            
        }
        // final resting place
        if (ball->pos.y < FIXED_0) {
            ball->pos.y += ball_velocity;
        }
        else {
            pcm_play(g_Assets.bouncePcm8, PCM_VOLATILE, 5);
            g_Game.dropBallTimer = DROP_BALL_TIME_NORMAL;
            return true;
        }
    }
    return false;
}

void drawGameTimer(void) {
    convertSecondsToTime(g_Gameplay.GameTimer);
    // min_ones
    timer.spr_id =timer.anim1.asset[g_Timer.min_ones];
    set_spr_position(&timer, -30, -210, 80);
    my_sprite_draw(&timer);
    // :
    timer.spr_id = timer.anim1.asset[11];
    set_spr_position(&timer, -12, -210, 80);
    my_sprite_draw(&timer);
    // sec_tens
    timer.spr_id = timer.anim1.asset[g_Timer.sec_tens];
    set_spr_position(&timer, 6, -210, 80);
    my_sprite_draw(&timer);
    // sec_tens
    timer.spr_id = timer.anim1.asset[g_Timer.sec_ones];
    set_spr_position(&timer, 36, -210, 80);
    my_sprite_draw(&timer); 
}
