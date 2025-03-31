#include <jo/jo.h>
#include <string.h>
#include "../main.h"
#include "../assets.h"
#include "player.h"
#include "../sprites.h"
#include "../gameplay.h"
#include "../AI.h"
#include "../physics.h"
#include "../team_select.h"
#include "../BG_DEF/sprite_colors.h"

// distance formula without the square root
bool checkDistance(PPLAYER a, PPLAYER b);

PLAYER g_Players[MAX_PLAYERS] = {0};

bool characterUnlocked[TOTAL_CHARACTERS] = {0};
bool characterAvailable[TOTAL_CHARACTERS] = {0};
bool teamAvailable[MAX_TEAMS+1] = {0};
int teamCount[MAX_TEAMS+1] = {0};

void respawnPlayer(PPLAYER player, bool deductLife);

const CHARACTER_ATTRIBUTES characterAttributes[] = {
  // s   a   p    // speed, acceleration, power - scale 0-100
    {70, 60, 55}, // MACCHI: High speed, medium acceleration, medium power
    {55, 70, 60}, // JELLY: Medium speed, high acceleration, medium-high power
    {60, 50, 70}, // PENNY: Medium-high speed, medium acceleration, high power
    {60, 45, 80}, // POPPY: Low attributes (for cursors) (was 4 4 4)
    {50, 60, 70}, // POTTER: Low attributes (for cursors) (was 4 4 4)
    {70, 80, 50}, // SPARTA: High acceleration, high speed, medium power
    {65, 65, 65}, // TJ: Balanced attributes
    {75, 50, 90}, // GEORGE: He's kinda mean
    {90, 62, 100},// WUPPY: High power, medium speed, low acceleration
    {88, 88, 88}, // THE WALRUS: Above average attributes
    {99, 99, 99}, // GARFIELD: Ultimate attributes
    {60, 60, 60}, // NONE: Medium attributes (for CPU)
};

// void spawnPlayers(void)
// {
    // PPLAYER player = NULL;

    // for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    // {
        // player = &g_Players[i];
        // respawnPlayer(player, false);
    // }
// }

void resetPlayerScores(void)
{
    
    // dss = getLives();
    g_Game.numStars = getStars();
    
    PPLAYER player = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        player = &g_Players[i];

        player->shield.power = SHIELD_POWER;
        player->score.stars  = 0;
        player->score.deaths = 0;
        player->score.points = 0;
        player->totalLives = getLives(player);
        player->numLives = player->totalLives;
        
        touchedBy[i].onLeftSide = false;
        touchedBy[i].hasTouched = false;
        touchedBy[i].touchCount = 0;
        touchedBy[i].teamChoice = 0;
    }
}

void resetPlayerAttacks(void)
{    
    PPLAYER player = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        player = &g_Players[i];

        player->shield.activate = false;
        player->_sprite->pos.r = PLAYER_RADIUS;
        sprite_frame_reset(&shield[i]);
        
        if (player->attack1) {
            if (player->onLeftSide) {
                player->_sprite->pos.x -= ATTACK1;
            }
            else {
                player->_sprite->pos.x += ATTACK1;
            }
            player->attack1 = false;
            player->attack1Frames = 0;
        }
        if (player->attack2) {
            if (player->onLeftSide) {
                player->_sprite->pos.x -= ATTACK2;
            }
            else {
                player->_sprite->pos.x += ATTACK2;
            }
            player->attack2 = false;
            player->attack2Frames = 0;
        }
        
    }
}

int getLives(PPLAYER player)
{
    int numLives = 0;
    
    switch(g_Game.gameMode)
    {
        case GAME_MODE_CLASSIC:
            numLives = 9;
            break;
        case GAME_MODE_STORY: {
            if (player->isAI) {
                switch(g_Game.gameDifficulty)
                {
                    case GAME_DIFFICULTY_EASY:
                        numLives = 4;
                        break;
                    case GAME_DIFFICULTY_MEDIUM:
                        numLives = 5;
                        break;
                    case GAME_DIFFICULTY_HARD:
                        numLives = 6;
                        break;
                    default:
                        break;
                }
            }
            else {
                switch(g_Game.gameDifficulty)
                {
                    case GAME_DIFFICULTY_EASY:
                        numLives = 6;
                        break;
                    case GAME_DIFFICULTY_MEDIUM:
                        numLives = 5;
                        break;
                    case GAME_DIFFICULTY_HARD:
                        numLives = 4;
                        break;
                    default:
                        break;
                }
            }
            break;
        }
        case GAME_MODE_BATTLE:
            numLives = 9;
            break;
        default:
            break;
    }

    return numLives;

}

int getStars(void)
{
    int numStars = 0;
    
    switch(g_Game.gameMode)
    {
        case GAME_MODE_CLASSIC:
            numStars = 1;
            break;
        case GAME_MODE_STORY: {
            switch(g_Game.gameDifficulty)
            {
                case GAME_DIFFICULTY_EASY:
                    numStars = 1;
                    break;
                case GAME_DIFFICULTY_MEDIUM:
                    numStars = 1;
                    break;
                case GAME_DIFFICULTY_HARD:
                    numStars = 2;
                    break;
                default:
                    break;
            }
            break;
        }
        case GAME_MODE_BATTLE:
            switch(g_Game.gameDifficulty)
            {
                case GAME_DIFFICULTY_EASY:
                    numStars = 1;
                    break;
                case GAME_DIFFICULTY_MEDIUM:
                    numStars = 2;
                    break;
                case GAME_DIFFICULTY_HARD:
                    numStars = 3;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return numStars;

}

void getContinues(void)
{
    switch(g_Game.gameMode)
    {
        case GAME_MODE_CLASSIC:
            g_Game.numContinues = 0;
            break;
        case GAME_MODE_STORY: {
            switch(g_Game.gameDifficulty)
            {
                case GAME_DIFFICULTY_EASY:
                    g_Game.numContinues = 3;
                    break;
                case GAME_DIFFICULTY_MEDIUM:
                    g_Game.numContinues = 2;
                    break;
                case GAME_DIFFICULTY_HARD:
                    g_Game.numContinues = 1;
                    break;
                default:
                    break;
            }
            break;
        }
        case GAME_MODE_BATTLE:
            g_Game.numContinues = 0;
            break;
        default:
            break;
    }

}

void initPlayers(void)
{
    PPLAYER player = NULL;

    memset(g_Players, 0, sizeof(g_Players));
    
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        
        player->playerID = i;
        player->objectState = OBJECT_STATE_ACTIVE;
        player->subState = PLAYER_STATE_ACTIVE;
        player->input->id = 0;
        player->input->isSelected = false;
        
        player->score.deaths = 0;
        player->score.points = 0;

        // PLAYER      
        player->startSelection = false;
        player->isReady = false;
        player->pressedB = false; // get rid of this?
        player->isPlaying = false; // get rid of this?
        player->scored = false;
        player->isAI = false;
        
        // GAMEPLAY
        player->numLives = 9;
        
        // CHARACTER
        player->character.choice = CHARACTER_NONE;
        player->character.selected = false;
        player->maxSpeed = toFIXED(0);
        player->acceleration = toFIXED(0);
        player->basePower = toFIXED(0);
        player->power = toFIXED(0);
        
        // TEAM
        player->teamChoice = TEAM_UNSELECTED;
        player->teamOldTeam = TEAM_UNSELECTED;
        player->teamSelected = false;
        g_goalPlayerId[player->teamChoice] = 0;
        
        player->moveHorizontal = false;
        player->moveVertical = false;
        
        // SPRITES
        // assign cursor & bg tile to each player
        player->_cursor = &player_cursor;
        player->_bg = &player_bg;
        player->_bg->zmode = _ZmLC;
        player->_sprite = &paw_blank;
        player->_sprite->spr_id = paw_blank_id; // not sure why this changes
        
        // cursors
        if (i == 1) {
            hsl_incSprites.h -= 90;
        }
        else if (i == 2) {
            hsl_incSprites.h -= 180;
        }
        else if (i == 3) {
            hsl_incSprites.h -= 270;
        }
        update_palette_Pmenu[i] = update_sprites_color(&p_rangePmenu[i]);
        
        // INPUTS
        player->curPos.dx = toFIXED(0);
        player->_sprite->vel.x = toFIXED(0);
        player->curPos.dy = toFIXED(0);
        player->_sprite->vel.y = toFIXED(0);
        
        player->attack1 = false;
        player->attack2 = false;
        player->attack1Frames = 0;
        player->attack2Frames = 0;
        
        player->_sprite->pos.r = PLAYER_RADIUS;
        player->shield.activate = false;
        
        player->_portrait = &character_portrait;
    }
}

void initAiPlayers(void)
{
    PPLAYER player = NULL;
    
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        if (player->isPlaying == PLAYING) {
            continue;
        }
        
        validateTeam(player);
        teamAvailable[player->teamChoice] = false;
        g_Game.numTeams++;
        g_Game.currentNumPlayers++;
        g_goalPlayerId[player->teamChoice-1] = i;
        
        player->_bg->spr_id = player->_bg->anim1.asset[i];
        player->character.choice = my_random_range(CHARACTER_MACCHI, CHARACTER_GARF);
        validateCharacters(player);
        characterAvailable[player->character.choice] = false;
                        
        assignCharacterSprite(player);        
        assignCharacterStats(player);
        
        player->_sprite->pos.r = PLAYER_RADIUS;
        player->shield.activate = false;
        
        player->isPlaying = PLAYING;
        player->objectState = OBJECT_STATE_ACTIVE;
        player->isAI = true;
    }
}

void initStoryCharacters(void)
{
    PPLAYER player = NULL;
    player = &g_Players[1];
    
    validateTeam(player);
    teamAvailable[player->teamChoice] = false;
    g_Game.numTeams++; 
    g_goalPlayerId[player->teamChoice-1] = 1;
    
    player->_bg->spr_id = player->_bg->anim1.asset[1];
    player->character.choice = my_random_range(CHARACTER_MACCHI, CHARACTER_GARF);
    characterAvailable[player->character.choice] = false;

    assignCharacterSprite(player);
    assignCharacterStats(player);
    
    player->isPlaying = PLAYING;
    player->objectState = OBJECT_STATE_ACTIVE;
    player->isAI = true;
    g_Game.numTeams = 2; // NEEDS TO BE SET DIFFERENTLY IF 2 PLAYERS ARE SELECTED?
}

void initVsModePlayers(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        
        player->_sprite->spr_id = player->_sprite->anim1.asset[0];
        set_spr_scale(player->_portrait, 1.1, 1);
        player->_bg->spr_id = player->_bg->anim1.asset[i];
        player->_sprite->isColliding = false;
        
        // assign goal to player for scoring
        g_goalPlayerId[player->teamChoice-1] = i;
        assignCharacterStats(player);
        
        // player_bg.mesh = MESHoff;
        // add_sprite_to_sweep_and_prune(player->_sprite);
        

        
        // PLAYER SPECIFIC ATTRIBUTES
        switch (player->teamChoice) {
            case TEAM_1: {
                player->_sprite->flip = sprNoflip;
                player->onLeftSide = true;
                if (g_Game.gameMode == GAME_MODE_CLASSIC || g_Game.gameMode == GAME_MODE_STORY || g_Game.numTeams == 2) {
                    g_Assets.drawSingleGoal[0] = true;
                    player->goalCenterThresholdMax = LARGE_GOAL_THRESHOLD_MAX;
                    player->goalCenterThresholdMin = LARGE_GOAL_THRESHOLD_MIN;
                    set_spr_position(player->_sprite, -1*PLAYER_X, 0, PLAYER_DEPTH);
                }
                else {
                    g_Assets.drawSingleGoal[0] = false;
                    set_spr_position(player->_sprite, -1*PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
                    player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
                    player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;
                }
                player->shield_pos = SHIELD_OFFSET;
                shield[i].flip = sprNoflip;
                break;
            } 
            case TEAM_2: {
                player->_sprite->flip = sprHflip;
                player->onLeftSide = false;
                if (g_Game.gameMode == GAME_MODE_CLASSIC || g_Game.gameMode == GAME_MODE_STORY || g_Game.numTeams < 4) {
                    g_Assets.drawSingleGoal[1] = true;
                    player->goalCenterThresholdMax = LARGE_GOAL_THRESHOLD_MAX;
                    player->goalCenterThresholdMin = LARGE_GOAL_THRESHOLD_MIN;
                    set_spr_position(player->_sprite, PLAYER_X, 0, PLAYER_DEPTH);
                }
                else {
                    g_Assets.drawSingleGoal[1] = false;
                    player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
                    player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;
                    set_spr_position(player->_sprite, PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
                }
                player->shield_pos = -SHIELD_OFFSET;
                shield[i].flip = sprHflip;
                break;
            }
            case TEAM_3: {
                player->_sprite->flip = sprVflip;
                player->onLeftSide = true;
                g_Assets.drawSingleGoal[2] = false;
                player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
                player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;
                set_spr_position(player->_sprite, -1*PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
                player->shield_pos = SHIELD_OFFSET;
                shield[i].flip = sprNoflip;
                break;
            }
            case TEAM_4: {
                player->_sprite->flip = sprHVflip;
                player->onLeftSide = false;
                g_Assets.drawSingleGoal[3] = false;
                player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
                player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;
                set_spr_position(player->_sprite, PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
                player->shield_pos = -SHIELD_OFFSET;
                shield[i].flip = sprHflip;
                break;
            }
            default:
                break;
        }
    }
}

void initDemoPlayers(void)
{
    PPLAYER player = NULL;
        
    g_Game.numPlayers = my_random_range(ONE_PLAYER, FOUR_PLAYER);
    g_Game.numTeams = 0;
    g_Game.gameMode = GAME_MODE_BATTLE;
    initPlayers();
    
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        // add_sprite_to_sweep_and_prune(player->_sprite);
        if (i == 0) {
            player->_sprite = &macchi;
            player->_sprite->spr_id = player->_sprite->anim1.asset[0];
            player->_sprite->flip = sprNoflip;
            player->onLeftSide = true;
            player->teamChoice = TEAM_1;
            // g_Assets.drawSingleGoal[i] = false;
            // set_spr_position(player->_sprite, -1*PLAYER_X, -100, PLAYER_DEPTH);
            if (g_Game.numPlayers >= THREE_PLAYER) {
                g_Assets.drawSingleGoal[0] = false;
                set_spr_position(player->_sprite, -1*PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
            }
            else {
                g_Assets.drawSingleGoal[0] = true;
                set_spr_position(player->_sprite, -1*PLAYER_X, 0, PLAYER_DEPTH);
            }
            player->character.choice = i;
            player->shield_pos = SHIELD_OFFSET;
            g_Game.numTeams++;
        }
        else if (i == 1) {
            if (JO_MOD_POW2(jo_random(999), 2)) { // modulus
                player->_sprite = &jelly;
                player->character.choice = 1;
            }
            else if (JO_MOD_POW2(jo_random(999), 9)) { // modulus
                player->_sprite = &garfield;
                player->character.choice = 10;
            }
            else {
                player->_sprite = &sparta;
                player->character.choice = 4;
            }
            player->_sprite->spr_id = player->_sprite->anim1.asset[0];
            player->_sprite->flip = sprHflip;
            player->onLeftSide = false;
            player->teamChoice = TEAM_2;
            // set_spr_position(player->_sprite, PLAYER_X, -100, PLAYER_DEPTH);
            
            if (g_Game.numPlayers <= THREE_PLAYER) {
                g_Assets.drawSingleGoal[i] = true;
                set_spr_position(player->_sprite, PLAYER_X, 0, PLAYER_DEPTH);
            }
            else {
                g_Assets.drawSingleGoal[i] = false;
                set_spr_position(player->_sprite, PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
            }
            player->shield_pos = -SHIELD_OFFSET;
            g_Game.numTeams++;
        }
        else if (i == 2) { // set up player 3 last (we need to know the team count)
            if (JO_MOD_POW2(jo_random(999), 2)) { // modulus
                player->_sprite = &poppy;
                player->character.choice = 5;
            }
            else if (JO_MOD_POW2(jo_random(999), 9)) { // modulus
                player->_sprite = &stadler;
                player->character.choice = 9;
            }
            else {
                player->_sprite = &penny;
                player->character.choice = 2;
            }
            player->_sprite->spr_id = player->_sprite->anim1.asset[0];
            player->_sprite->flip = sprVflip;
            player->onLeftSide = true;
            player->teamChoice = TEAM_3;
            set_spr_position(player->_sprite, -1*PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
            g_Assets.drawSingleGoal[i] = true;
            player->shield_pos = SHIELD_OFFSET;
            g_Game.numTeams++;
        }
        else if (i == 3) {
            if (JO_MOD_POW2(jo_random(999), 2)) { // modulus
                player->_sprite = &potter;
                player->character.choice = 3;
            }
            else if (JO_MOD_POW2(jo_random(999), 9)) { // modulus
                player->_sprite = &wuppy;
                player->character.choice = 8;
            }
            else {
                player->_sprite = &tj;
                player->character.choice = 6;
            }
            player->_sprite->spr_id = player->_sprite->anim1.asset[0];
            player->_sprite->flip = sprHVflip;
            player->onLeftSide = false;
            player->teamChoice = TEAM_4;
            set_spr_position(player->_sprite, PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
            g_Assets.drawSingleGoal[i] = true;
            player->shield_pos = -SHIELD_OFFSET;
            g_Game.numTeams++;
        }
        
        assignCharacterStats(player);
        
        g_goalPlayerId[player->teamChoice-1] = i;
        player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
        player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;

        player->_sprite->pos.r = PLAYER_RADIUS;
        player->shield.activate = false;
        
        player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];
        set_spr_scale(player->_portrait, 1.1, 1);
        player->objectState = OBJECT_STATE_ACTIVE;
        player->isAI = true;
        player->_sprite->isColliding = false;
        boundPlayer(player);
    }
}

void assignCharacterSprite(PPLAYER player) {
    switch (player->character.choice)
    {
        case CHARACTER_MACCHI:
            player->_sprite = &macchi;
            break;
        case CHARACTER_JELLY:
            player->_sprite = &jelly;
            break;
        case CHARACTER_PENNY:
            player->_sprite = &penny;
            break;
        case CHARACTER_POPPY:
            player->_sprite = &poppy;
            break;
        case CHARACTER_POTTER:
            player->_sprite = &potter;
            break;
        case CHARACTER_SPARTA:
            player->_sprite = &sparta;
            break;
        case CHARACTER_TJ:
            player->_sprite = &tj;
            break;
        case CHARACTER_GEORGE:
            player->_sprite = &george;
            break;
        case CHARACTER_WUPPY:
            player->_sprite = &wuppy;
            break;
        case CHARACTER_WALRUS:
            player->_sprite = &stadler;
            break;
        case CHARACTER_GARF:
            player->_sprite = &garfield;
            break;
        default:
        break;
    }
    player_bg.mesh = MESHoff;
}

void assignCharacterStats(PPLAYER player) {
    player->maxSpeed = jo_fixed_mult(toFIXED(characterAttributes[player->character.choice].maxSpeed), toFIXED(0.1));
    player->acceleration = toFIXED(characterAttributes[player->character.choice].acceleration);
    player->basePower = jo_fixed_mult(toFIXED(characterAttributes[player->character.choice].power), toFIXED(.05)); // this could be a difficulty factor?
    player->power = player->basePower;
    
    if (player->isAI) {
        switch (g_Game.gameDifficulty) {
            case GAME_DIFFICULTY_EASY: {
                player->acceleration = jo_fixed_mult(player->acceleration, EASY_AI_MOVEMENT_SPEED);
                break;
            }
            case GAME_DIFFICULTY_MEDIUM: {
                player->acceleration = jo_fixed_mult(player->acceleration, MEDIUM_AI_MOVEMENT_SPEED);
                break;
            }
            case GAME_DIFFICULTY_HARD: {
                player->acceleration = jo_fixed_mult(player->acceleration, HARD_AI_MOVEMENT_SPEED);
                break;
            }
            default:
                player->acceleration = jo_fixed_mult(player->acceleration, MEDIUM_AI_MOVEMENT_SPEED);
                break;
        }
    }
    else {
        player->acceleration = jo_fixed_mult(player->acceleration, PLAYER_MOVEMENT_SPEED);
    }
}

void initPlayerGoals(void)
{
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        // set initial player colors (player 0 uses the default)
        if (i == 1) {
            hsl_incSprites.h -= 90;
        }
        else if (i == 2) {
            hsl_incSprites.h -= 180;
        }
        else if (i == 3) {
            hsl_incSprites.h -= 270;
        }
        update_palette_Goals[i] = update_sprites_color(&p_rangeGoals[i]);
    }
}

void getClassicModeInput(void)
{
    PPLAYER player = NULL;

    // check inputs for all players
    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE || player->isAI == true)
        {
            // TODO: hit a button to respawn??
            continue;
        }
        
        // switch state for game mode (handle inputs differently)?
        
        if (player->input->isAnalog) {
            player->curPos.dy = jo_fixed_mult(player->input->axis_y, toFIXED(0.5));
            player->moveVertical = true;
        }
        else {
            player->moveVertical = false;

            if (jo_is_input_key_pressed(player->input->id, JO_KEY_UP))
            {
                player->curPos.dy -= X_SPEED_INC;
                player->moveVertical = true;
            }
            if (jo_is_input_key_pressed(player->input->id, JO_KEY_DOWN))
            {
                player->curPos.dy += X_SPEED_INC;
                player->moveVertical = true;
            }
        }
        
        if(player->moveVertical == false)
        {
            player->curPos.dy = 0;
            player->_sprite->vel.y = 0;
        }
        
        regenPlayerPower(player);
        
        if (!g_Game.isBallActive) {
            return;
        }
        
        playerAttack(player);
        
    }
}

void getPlayersInput(void)
{
    PPLAYER player = NULL;
    
    // check inputs for all players
    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE || player->isAI == true || player->subState == PLAYER_STATE_DEAD)
        {
            // TODO: hit a button to respawn??
            continue;
        }
        
        // switch state for game mode (handle inputs differently)?       
        if (player->input->isAnalog) {
            player->curPos.dx = player->input->axis_x;
            // don't process in classic mode?
            player->curPos.dy = jo_fixed_mult(player->input->axis_y, toFIXED(0.5));
            player->moveHorizontal = true;
            player->moveVertical = true;
        }
        else {
            player->moveHorizontal = false;
            player->moveVertical = false;
            // don't process in classic mode?
            if (jo_is_input_key_pressed(player->input->id, JO_KEY_LEFT))
            {
                player->curPos.dx -= X_SPEED_INC;
                player->moveHorizontal = true;
            }
            if (jo_is_input_key_pressed(player->input->id, JO_KEY_RIGHT))
            {
                player->curPos.dx += X_SPEED_INC;
                player->moveHorizontal = true;
            }

            if (jo_is_input_key_pressed(player->input->id, JO_KEY_UP))
            {
                player->curPos.dy -= X_SPEED_INC;
                player->moveVertical = true;
            }
            if (jo_is_input_key_pressed(player->input->id, JO_KEY_DOWN))
            {
                player->curPos.dy += X_SPEED_INC;
                player->moveVertical = true;
            }
        }
        
        if(player->moveHorizontal == false)
        {
            player->curPos.dx = 0;
            player->_sprite->vel.x = 0;
        }
        if(player->moveVertical == false)
        {
            player->curPos.dy = 0;
            player->_sprite->vel.y = 0;
        }
        
        regenPlayerPower(player);
        
        if (!g_Game.isBallActive) {
            return;
        }
       
        playerAttack(player);
        
        // SHIELD
        if (jo_is_input_key_pressed(player->input->id, JO_KEY_B) && player->shield.power > 0)
        {
            if (player->shield.power > 1) {
                player->shield.activate = true;
                player->_sprite->pos.r = SHIELD_RADIUS;
            }
            player->shield.power--;
        }
        else {
            player->shield.activate = false;
            player->_sprite->pos.r = PLAYER_RADIUS;
        }
    }
}

void playerAttack(PPLAYER player) {
        // ATTACK1
        if (jo_is_input_key_down(player->input->id, JO_KEY_A) && player->shield.power > ATTACK1_COST && !player->attack1)
        {
            if (player->onLeftSide) {
                player->_sprite->pos.x += ATTACK1;
            }
            else {
                player->_sprite->pos.x -= ATTACK1;
            }
            player->attack1 = true;
            player->shield.power -= ATTACK1_COST;
        }
        else if (player->attack1 && player->attack1Frames == ATTACK_FRAMES) {
            if (player->onLeftSide) {
                player->_sprite->pos.x -= ATTACK1;
            }
            else {
                player->_sprite->pos.x += ATTACK1;
            }
            player->attack1Frames = 0;
            player->attack1 = false;
        }
        else if (player->attack1 && player->attack1Frames < ATTACK_FRAMES) {
            player->attack1Frames++;
        }
        // ATTACK2
        if (jo_is_input_key_down(player->input->id, JO_KEY_C) && player->shield.power > ATTACK2_COST && !player->attack2)
        {
            if (player->onLeftSide) {
                player->_sprite->pos.x += ATTACK2;
            }
            else {
                player->_sprite->pos.x -= ATTACK2;
            }
            player->attack2 = true;
            player->shield.power -= ATTACK2_COST;
        }
        else if (player->attack2 && player->attack2Frames == ATTACK_FRAMES) {
            if (player->onLeftSide) {
                player->_sprite->pos.x -= ATTACK2;
            }
            else {
                player->_sprite->pos.x += ATTACK2;
            }
            player->attack2Frames = 0;
            player->attack2 = false;
        }
        else if (player->attack2 && player->attack2Frames < ATTACK_FRAMES) {
            player->attack2Frames++;
        }
}

void regenPlayerPower(PPLAYER player)
{
    if (!jo_is_input_key_pressed(player->input->id, JO_KEY_A) || 
        !jo_is_input_key_pressed(player->input->id, JO_KEY_B) || 
        !jo_is_input_key_pressed(player->input->id, JO_KEY_C)) {
        if (player->shield.power < SHIELD_POWER) {
            if (JO_MOD_POW2(frame, SHIELD_REGEN) == 0) { // modulus
                player->shield.power++;
            }
        }
    }
}

void updatePlayers(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        if(player->objectState == OBJECT_STATE_INACTIVE || player->isAI == true || player->subState == PLAYER_STATE_DEAD)
        {
            continue;
        }

        // if(player->subState == PLAYER_STATE_EXPLODING)
        // {
            // player->frameCount--;
            // if(player->frameCount <= 0)
            // {
                // // transition from exploding to exploded state
                // //player->objectState = OBJECT_STATE_INACTIVE;
                // player->curPos.dx = 0;
                // player->curPos.dy = 0;
                // player->ds = 0;
                // player->da = 0;

                // player->frameCount = EXPLODE_FRAME_COUNT;
                // player->subState = PLAYER_STATE_EXPLODED;
                // jo_audio_play_sound(&g_Assets.crackPCM);

            // }
            // //continue;
        // }

        // if(player->subState == PLAYER_STATE_EXPLODED)
        // {
            // player->frameCount--;
            // if(player->frameCount <= 0)
            // {
                // respawnPlayer(player, true);
            // }
        // }

        speedLimitPlayer(player);

        // move the player
        player->_sprite->pos.y += player->curPos.dy;
        player->_sprite->pos.x += player->curPos.dx;
        player->_sprite->vel.x += player->curPos.dx;
        player->_sprite->vel.y += player->curPos.dy;
        // update_bounding_box(player->_sprite);
        detect_player_ball_collision(&pixel_poppy, player);
        boundPlayer(player);
    }
}


void speedLimitPlayer(PPLAYER player)
{
    FIXED bonusSpeed = toFIXED(0);
    FIXED y_up_speed;
    FIXED y_dn_speed;
    FIXED x_l_speed;
    FIXED x_r_speed;
    if (player->isAI == true) {
        // y_up_speed = AI_X_SPEED;
        // y_dn_speed = -AI_X_SPEED;
        // x_l_speed  = MAX_X_SPEED;
        // x_r_speed  = -MAX_X_SPEED;
        y_up_speed = player->maxSpeed;
        y_dn_speed = -player->maxSpeed;
        x_l_speed  = MAX_X_SPEED;
        x_r_speed  = -MAX_X_SPEED;
    }
    else {
        y_up_speed = MAX_Y_SPEED;
        y_dn_speed = -MAX_Y_SPEED;
        x_l_speed  = MAX_X_SPEED;
        x_r_speed  = -MAX_X_SPEED;
    }
    /*
    if(player->exceedLimitTimer > 0)
    {
        player->exceedLimitTimer--;
        bonusSpeed = toFIXED(4.0);
    }
    */

    // validate speeds
    if(player->curPos.dx > x_l_speed + bonusSpeed)
    {
        player->curPos.dx = x_l_speed + bonusSpeed;
    }

    if(player->curPos.dx < x_r_speed - bonusSpeed)
    {
        player->curPos.dx = x_r_speed - bonusSpeed;
    }

    if(player->curPos.dy > y_up_speed + bonusSpeed)
    {
        player->curPos.dy = y_up_speed + bonusSpeed;
    }

    if(player->curPos.dy < y_dn_speed - bonusSpeed)
    {
        player->curPos.dy = y_dn_speed - bonusSpeed;
    }
}

void boundPlayer(PPLAYER player)
{
    // screen boundaries
    if (player->onLeftSide == true) {
      if (g_GameOptions.testCollision) {
        if(player->_sprite->pos.x > PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH;
        }
      }
      else {
        if(player->_sprite->pos.x > -PLAYER_BOUNDARY_MIDDLE - PLAYER_WIDTH)
        {
            player->_sprite->pos.x = -PLAYER_BOUNDARY_MIDDLE - PLAYER_WIDTH;
        }
      }
        if(player->_sprite->pos.x < PLAYER_BOUNDARY_LEFT + PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_LEFT + PLAYER_WIDTH;
        }
    }
    else {
        if(player->_sprite->pos.x > PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH;
        }
      if (g_GameOptions.testCollision) {
        if(player->_sprite->pos.x < PLAYER_BOUNDARY_LEFT + PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_LEFT + PLAYER_WIDTH;
        }
      }
      else {
        if(player->_sprite->pos.x < PLAYER_BOUNDARY_MIDDLE + PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_MIDDLE + PLAYER_WIDTH;
        }
      }
    }

    if(player->_sprite->pos.y > SCREEN_BOTTOM - PLAYER_HEIGHT)
    {
        player->_sprite->pos.y = SCREEN_BOTTOM - PLAYER_HEIGHT;
    }

    if(player->_sprite->pos.y < SCREEN_TOP + PLAYER_HEIGHT)
    {
        player->_sprite->pos.y = SCREEN_TOP + PLAYER_HEIGHT;
    }
}

// void explodeNeighbors(PPLAYER player)
// {
    // bool neighbor = false;

    // // for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    // // {
        // // PPLAYER victim = &g_Players[i];

        // // if(victim->objectState != OBJECT_STATE_ACTIVE)
        // // {
            // // continue;
        // // }

        // // if(victim->subState != PLAYER_STATE_ACTIVE)
        // // {
            // // continue;
        // // }

        // // if(player->input->id == victim->playerID)
        // // {
            // // continue;
        // // }

        // // // check if player is nearby
        // // neighbor = checkDistance(player, victim);
        // // if(neighbor == true)
        // // {
            // // explodePlayer(victim, false, false);
        // // }
    // // }
// }

void respawnPlayer(PPLAYER player, bool deductLife)
{
    if(player->objectState != OBJECT_STATE_ACTIVE)
    {
        return;
    }

    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        // only respawn during gameplay
        player->subState = PLAYER_STATE_DEAD;
        return;
    }

    if(player->numLives <= 0)
    {
        // no lives
        player->subState = PLAYER_STATE_DEAD;
        return;
    }

    if(deductLife == true)
    {
        player->numLives--;
    }

    // set position
    player->curPos.x = toFIXED(jo_random(660) - 330);
    player->curPos.y = toFIXED(jo_random(330) - 150);
    // player->angle = 0;
    // player->size = toFIXED(1);

    // reset speed to zero
    player->curPos.dx = 0;
    player->curPos.dy = 0;

    player->subState = PLAYER_STATE_ACTIVE;
}

// distance formula without the square root
bool checkDistance(PPLAYER a, PPLAYER b)
{
    int dist = 36*36;

    int x_dist = toINT(a->curPos.x) - toINT(b->curPos.x);
    x_dist = x_dist * x_dist;

    int y_dist = toINT(a->curPos.y) - toINT(b->curPos.y);
    y_dist = y_dist * y_dist;

    if(dist > x_dist + y_dist)
    {
        return true;
    }

    return false;
}
