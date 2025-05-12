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
#include "../backup.h"
#include "../BG_DEF/sprite_colors.h"

PLAYER g_Players[MAX_PLAYERS] = {0};

bool characterUnlocked[TOTAL_CHARACTERS] = {0};
bool characterAvailable[TOTAL_CHARACTERS] = {0};

const CHARACTER_ATTRIBUTES characterAttributes[] = {
  // s   a   p    // speed, acceleration, power - scale 0-100
    {70, 60, 60}, // MACCHI: High speed, medium acceleration, medium power
    {60, 70, 60}, // JELLY: Medium speed, high acceleration, medium-high power
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

void resetPlayerScores(void)
{
    // g_Game.numStars = getStars();
    initTouchCounter(1);
    
    PPLAYER player = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        player = &g_Players[i];
        player->shield.power = SHIELD_POWER;
        player->score.stars  = 0;
        player->score.continues = 0;
        player->score.total  = 0;
        player->score.points = 0;
        player->score.lastMillion = 0;
        player->totalLives = getLives(player);
        player->numLives = player->totalLives;
        set_spr_scale(player->_sprite, 2, 2);
        set_spr_scale(&shield[i], 2, 2);
        player->isBig = false;
        player->isSmall = false;
        player->shroomFrames = SHROOM_TIMER;
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
        player->isExploded = false;
        
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
        case GAME_MODE_BATTLE:
            numLives = 9;
            // numLives = 3;
            break;
        case GAME_MODE_CLASSIC:
            switch(g_Game.gameDifficulty)
            {
                case GAME_DIFFICULTY_EASY:
                    numLives = 9;
                    break;
                case GAME_DIFFICULTY_MEDIUM:
                    numLives = 6;
                    break;
                case GAME_DIFFICULTY_HARD:
                    numLives = 4;
                    break;
                default:
                    break;
            }
            break;
        default:
            if (player->isAI) {
                switch(g_Game.gameDifficulty)
                {
                    case GAME_DIFFICULTY_EASY:
                        numLives = 4;
                        // numLives = 1;
                        break;
                    case GAME_DIFFICULTY_MEDIUM:
                        numLives = 6;
                        // numLives = 3;
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
                        // numLives = 1;
                        break;
                    case GAME_DIFFICULTY_MEDIUM:
                        numLives = 6;
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
                    numStars = 1;
                    break;
                case GAME_DIFFICULTY_HARD:
                    numStars = 1;
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
    if (g_Game.gameMode != GAME_MODE_STORY) {
        return;
    }
    switch(g_Game.gameDifficulty)
    {
        case GAME_DIFFICULTY_EASY:
            g_Players[0].score.continues = 3;
            break;
        case GAME_DIFFICULTY_MEDIUM:
            g_Players[0].score.continues = 2;
            break;
        case GAME_DIFFICULTY_HARD:
            g_Players[0].score.continues = 1;
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
        
        player->score.continues = 0;
        player->score.points = 0;
        player->score.total = 0;
        player->score.lastMillion = 0;

        // PLAYER      
        player->startSelection = false;
        player->isReady = false;
        player->pressedB = false; // get rid of this?
        player->isPlaying = NOT_PLAYING;
        player->scored = false;
        player->isAI = false;
        player->isExploded = false;
        player->onLeftSide = false;
        
        // GAMEPLAY
        player->numLives = 9;
        
        // CHARACTER
        player->character.choice = CHARACTER_NONE;
        player->character.selected = false;
        player->maxSpeed = FIXED_0;
        player->acceleration = FIXED_0;
        player->basePower = FIXED_0;
        player->power = FIXED_0;
        
        // TEAM
        player->teamChoice = TEAM_COUNT;
        player->teamOldTeam = TEAM_COUNT;
        player->teamSelected = false;
        
        player->moveHorizontal = false;
        player->moveVertical = false;
        
        // SPRITES
        // assign cursor & bg tile to each player
        player->_cursor = &player_cursor;
        player->_bg = &player_bg;
        player->_bg->zmode = _ZmLC;
        player->_sprite = &paw_blank;
        set_spr_scale(player->_sprite, 2, 2);
        player->_sprite->spr_id = paw_blank_id; // not sure why this changes
        player->_sprite->isColliding = false;
        
        // cursors
        if (i == 1) {
            hsl_incSprites[HSL_CURSOR].h -= 90;
        }
        else if (i == 2) {
            hsl_incSprites[HSL_CURSOR].h -= 180;
        }
        else if (i == 3) {
            hsl_incSprites[HSL_CURSOR].h -= 270;
        }
        update_palette_Pmenu[i] = update_sprites_color(&p_rangePmenu[i], HSL_CURSOR);
        
        // INPUTS
        player->curPos.dx = FIXED_0;
        player->_sprite->vel.x = FIXED_0;
        player->curPos.dy = FIXED_0;
        player->_sprite->vel.y = FIXED_0;
        
        player->attack1 = false;
        player->attack2 = false;
        player->attack1Frames = 0;
        player->attack2Frames = 0;
        
        player->_sprite->pos.r = PLAYER_RADIUS;
        player->shield.activate = false;
        
        player->_portrait = &character_portrait;
        
        player->isBig = false;
        player->isSmall = false;
        player->shroomFrames = SHROOM_TIMER;
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
        g_Team.isAvailable[player->teamChoice] = false;
        g_Team.objectState[player->teamChoice] = OBJECT_STATE_ACTIVE;
        g_Team.numTeams++;
        g_Game.currentNumPlayers++;
        
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
    
    player->teamChoice = TEAM_2;
    g_Team.isAvailable[player->teamChoice] = false; 
    g_Team.objectState[player->teamChoice] = OBJECT_STATE_ACTIVE;
    
    player->_bg->spr_id = player->_bg->anim1.asset[1];
    player->character.choice = g_Game.countofRounds +1;
    characterAvailable[player->character.choice] = false;

    assignCharacterSprite(player);
    assignCharacterStats(player);
    
    player->isPlaying = PLAYING;
    player->objectState = OBJECT_STATE_ACTIVE;
    player->subState = PLAYER_STATE_ACTIVE;
    player->isAI = true;
    g_Team.numTeams = TWO_TEAMS;
}

void nextStoryCharacter(void)
{
    PPLAYER player = NULL;
    player = &g_Players[1];
    
    player->character.choice = g_Game.countofRounds + 1;
    player->objectState = OBJECT_STATE_ACTIVE;
    player->subState = PLAYER_STATE_ACTIVE;

    assignCharacterSprite(player);
    initVsModePlayers();
    boundPlayer(player);
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
        assignCharacterStats(player);
        
        // PLAYER SPECIFIC ATTRIBUTES
        switch (player->teamChoice) {
            case TEAM_1: {
                player->_sprite->flip = sprNoflip;
                player->onLeftSide = true;
                if (g_Game.gameMode == GAME_MODE_CLASSIC || g_Game.gameMode == GAME_MODE_STORY || g_Team.numTeams == TWO_TEAMS) {
                    player->goalCenterThresholdMax = LARGE_GOAL_THRESHOLD_MAX;
                    player->goalCenterThresholdMin = LARGE_GOAL_THRESHOLD_MIN;
                    set_spr_position(player->_sprite, -1*PLAYER_X, 0, PLAYER_DEPTH);
                }
                else {
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
                if (g_Game.gameMode == GAME_MODE_CLASSIC || g_Game.gameMode == GAME_MODE_STORY || g_Team.numTeams < FOUR_TEAMS) {
                    player->goalCenterThresholdMax = LARGE_GOAL_THRESHOLD_MAX;
                    player->goalCenterThresholdMin = LARGE_GOAL_THRESHOLD_MIN;
                    set_spr_position(player->_sprite, PLAYER_X, 0, PLAYER_DEPTH);
                }
                else {
                    player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
                    player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;
                    set_spr_position(player->_sprite, PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
                }
                player->shield_pos = -SHIELD_OFFSET;
                shield[i].flip = sprHflip;
                break;
            }
            case TEAM_3: {
                // set based on status of team 1
                player->_sprite->flip = sprVflip;
                player->onLeftSide = true;
                player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
                player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;
                set_spr_position(player->_sprite, -1*PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
                player->shield_pos = SHIELD_OFFSET;
                shield[i].flip = sprNoflip;
                break;
            }
            case TEAM_4: {
                // set based on status of team 2
                player->_sprite->flip = sprHVflip;
                player->onLeftSide = false;
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

void switchPlayerPosition(PPLAYER player)
{       
        // PLAYER SPECIFIC ATTRIBUTES
        switch (player->teamChoice) {
            case TEAM_3: {
                // set based on status of team 1
                player->_sprite->flip = sprHflip;
                player->onLeftSide = false;
                set_spr_position(player->_sprite, PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
                player->shield_pos = -SHIELD_OFFSET;
                shield[player->playerID].flip = sprHflip;
                break;
            }
            case TEAM_4: {
                // set based on status of team 2
                player->_sprite->flip = sprNoflip;
                player->onLeftSide = true;
                set_spr_position(player->_sprite, -1*PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
                player->shield_pos = SHIELD_OFFSET;
                shield[player->playerID].flip = sprNoflip;
                break;
            }
            default:
                break;
        }
        player->goalCenterThresholdMax = LARGE_GOAL_THRESHOLD_MAX;
        player->goalCenterThresholdMin = LARGE_GOAL_THRESHOLD_MIN;
}

void initDemoPlayers(void)
{
    PPLAYER player = NULL;
        
    g_Game.numPlayers = my_random_range(ONE_PLAYER, FOUR_PLAYER);
    g_Team.numTeams = -1;
    g_Game.gameMode = GAME_MODE_BATTLE;
    initPlayers();
    
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        if (i == 0) {
            player->_sprite = &macchi;
            player->_sprite->spr_id = player->_sprite->anim1.asset[0];
            player->_sprite->flip = sprNoflip;
            player->onLeftSide = true;
            player->teamChoice = TEAM_1;
            if (g_Game.numPlayers >= THREE_PLAYER) {
                set_spr_position(player->_sprite, -1*PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
            }
            else {
                set_spr_position(player->_sprite, -1*PLAYER_X, 0, PLAYER_DEPTH);
            }
            player->character.choice = i;
            player->shield_pos = SHIELD_OFFSET;
            g_Team.numTeams++;
            g_Game.currentNumPlayers++;
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
            
            if (g_Game.numPlayers <= THREE_PLAYER) {
                set_spr_position(player->_sprite, PLAYER_X, 0, PLAYER_DEPTH);
            }
            else {
                set_spr_position(player->_sprite, PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
            }
            player->shield_pos = -SHIELD_OFFSET;
            g_Team.numTeams++;
            g_Game.currentNumPlayers++;
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
            player->shield_pos = SHIELD_OFFSET;
            g_Team.numTeams++;
            g_Game.currentNumPlayers++;
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
            player->shield_pos = -SHIELD_OFFSET;
            g_Team.numTeams++;
            g_Game.currentNumPlayers++;
        }
        
        assignCharacterStats(player);
        player->goalCenterThresholdMax = SMALL_GOAL_THRESHOLD_MAX;
        player->goalCenterThresholdMin = SMALL_GOAL_THRESHOLD_MIN;

        player->_sprite->pos.r = PLAYER_RADIUS;
        player->shield.activate = false;
        
        player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];
        set_spr_scale(player->_portrait, 1.1, 1);
        player->objectState = OBJECT_STATE_ACTIVE;
        player->isPlaying = PLAYING;
        player->isAI = true;
        player->_sprite->isColliding = false;
        boundPlayer(player);
    }
    resetTeamState();
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
    if (player->isAI) {
        switch(g_Game.gameDifficulty)
            {
                case GAME_DIFFICULTY_EASY:
                    player->maxSpeed = jo_fixed_mult(toFIXED(characterAttributes[player->character.choice].maxSpeed), toFIXED(0.06));
                    break;
                case GAME_DIFFICULTY_MEDIUM:
                    player->maxSpeed = jo_fixed_mult(toFIXED(characterAttributes[player->character.choice].maxSpeed), toFIXED(0.08));
                    break;
                case GAME_DIFFICULTY_HARD:
                    player->maxSpeed = jo_fixed_mult(toFIXED(characterAttributes[player->character.choice].maxSpeed), toFIXED(0.1));
                    break;
                default:
                    break;
            }
    }
    else {        
        player->maxSpeed = jo_fixed_mult(toFIXED(characterAttributes[player->character.choice].maxSpeed), toFIXED(0.1));
    }
    player->acceleration = toFIXED(characterAttributes[player->character.choice].acceleration);
    player->basePower = jo_fixed_mult(toFIXED(characterAttributes[player->character.choice].power), toFIXED(.05));
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

void getClassicModeInput(void)
{
    PPLAYER player = NULL;
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        if(player->objectState == OBJECT_STATE_INACTIVE || player->subState == PLAYER_STATE_DEAD || player->isAI == true)
        {
            continue;
        }
        player->moveHorizontal = false;
        player->moveVertical = false;
        if (player->input->isAnalog) {
            player->curPos.dy = jo_fixed_mult(player->input->axis_y, FIXED_HALF);
            if (player->curPos.dy > FIXED_0 || player->curPos.dy < FIXED_0) {
                player->moveVertical = true;
            }
        }
        else {
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
        playerAttack(player);
    }
}

void getPlayersInput(void)
{
    PPLAYER player = NULL;
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        player = &g_Players[i];
        if(player->objectState == OBJECT_STATE_INACTIVE || player->subState == PLAYER_STATE_DEAD || player->isAI == true)
        {
            continue;
        }
        player->moveHorizontal = false;
        player->moveVertical = false;
        if (player->input->isAnalog) {
            player->curPos.dx = player->input->axis_x;
            player->curPos.dy = jo_fixed_mult(player->input->axis_y, FIXED_HALF);
            if (player->curPos.dx > FIXED_0 || player->curPos.dx < FIXED_0) {
                player->moveHorizontal = true;
            }
            if (player->curPos.dy > FIXED_0 || player->curPos.dy < FIXED_0) {
                player->moveVertical = true;
            }
        }
        else {
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
        playerAttack(player);
    }
}

void playerAttack(PPLAYER player) {
        if (!g_Game.isActive) {
            return;
        }        
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
        // SHIELD
        if (jo_is_input_key_pressed(player->input->id, JO_KEY_B) && player->shield.power > 0)
        {
            if (player->shield.power > 1) {
                pcm_play(g_Assets.shieldPcm8, PCM_PROTECTED, 6);
                if (!player->shield.activate) {                
                    if (player->isBig) {
                        player->_sprite->pos.r = SHIELD_RADIUS_LARGE;
                    }
                    else if (player->isSmall) {
                        player->_sprite->pos.r = SHIELD_RADIUS_SMALL;
                    }
                    else {
                        player->_sprite->pos.r = SHIELD_RADIUS;
                    }
                }
                player->shield.activate = true;
            }
            player->shield.power--;
        }
        else {
            if (player->shield.activate) {  
                if (player->isBig) {
                    player->_sprite->pos.r = PLAYER_RADIUS_LARGE;
                }
                else if (player->isSmall) {
                    player->_sprite->pos.r = PLAYER_RADIUS_SMALL;
                }
                else {
                    player->_sprite->pos.r = PLAYER_RADIUS;
                }
            }
            player->shield.activate = false;
        }
}

void regenPlayerPower(PPLAYER player)
{
    // TODO: draw some sort of visual effect when the shield is charging (maybe a sound effect)
    if (player->onLeftSide && player->_sprite->pos.x > -BOUNDARY_SHIELD_REGEN_SLOW) {
        return;
    }
    if (!player->onLeftSide && player->_sprite->pos.x < BOUNDARY_SHIELD_REGEN_SLOW) {
        return;
    }
    Uint8 regen_speed = SHIELD_REGEN_SLOW;
    if (player->onLeftSide && player->_sprite->pos.x < -BOUNDARY_SHIELD_REGEN_FAST) {
        regen_speed = SHIELD_REGEN_FAST;
    }
    if (!player->onLeftSide && player->_sprite->pos.x > BOUNDARY_SHIELD_REGEN_FAST) {
        regen_speed = SHIELD_REGEN_FAST;
    }
    if (!jo_is_input_key_pressed(player->input->id, JO_KEY_A) || 
        !jo_is_input_key_pressed(player->input->id, JO_KEY_B) || 
        !jo_is_input_key_pressed(player->input->id, JO_KEY_C)) {
        if (player->shield.power < SHIELD_POWER) {
            if (JO_MOD_POW2(g_Game.frame, regen_speed) == 0) { // modulus
                // pcm_play(g_Assets.rechargePcm8, PCM_PROTECTED, 6); // don't think this works because it keeps playing
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
        if(player->objectState == OBJECT_STATE_INACTIVE || player->subState == PLAYER_STATE_DEAD || player->isAI == true)
        {
            continue;
        }

        speedLimitPlayer(player);

        // move the player
        player->_sprite->pos.y += player->curPos.dy;
        player->_sprite->vel.y += player->curPos.dy;
        player->_sprite->pos.x += player->curPos.dx;
        player->_sprite->vel.x += player->curPos.dx;
        boundPlayer(player);
        
        // ball collision
        detect_player_ball_collision(&pixel_poppy, player);
        
        // item collision
        if (g_item.isActive) { // don't allow collision unless player can see item
            g_item._sprite->isColliding = checkItemDistance(player->_sprite, g_item._sprite);
        }
        else {
            g_item._sprite->isColliding = false;
        }
        if (g_item._sprite->isColliding) {
            handlePlayerItemCollision(player);
        }
        
        if (player->isBig) {
            if (g_item.timer[i] == 0) {
                pcm_play(g_Assets.shrinkPcm8, PCM_PROTECTED, 7);
                player->isBig = shrinkPlayerSprite(player, NORMAL_PLAYER_SPRITE);
            }
            else {
                growPlayerSprite(player, LARGE_PLAYER_SPRITE);
            }
        }
        if (player->isSmall) {
            if (g_item.timer[i] == 0) {
                pcm_play(g_Assets.growPcm8, PCM_PROTECTED, 7);
                player->isSmall = growPlayerSprite(player, NORMAL_PLAYER_SPRITE);
            }
            else {
                shrinkPlayerSprite(player, SMALL_PLAYER_SPRITE);
            }
        }
    }
}

void speedLimitPlayer(PPLAYER player)
{
    FIXED bonusSpeed = FIXED_0;
    FIXED y_up_speed;
    FIXED y_dn_speed;
    FIXED x_l_speed;
    FIXED x_r_speed;
    if (player->isAI == true) {
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
    FIXED boundary_middle;
    if (g_Game.isActive) {
        boundary_middle = PLAYER_BOUNDARY_MIDDLE;
    }
    else {
        boundary_middle = PLAYER_BOUNDARY_MIDDLE_INACTIVE;
    }
    // screen boundaries
    if (player->onLeftSide == true) {
      if (g_GameOptions.testCollision) {
        if(player->_sprite->pos.x > PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH;
        }
      }
      else {
        if(player->_sprite->pos.x > -boundary_middle - PLAYER_WIDTH)
        {
            player->_sprite->pos.x = -boundary_middle - PLAYER_WIDTH;
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
        if(player->_sprite->pos.x < boundary_middle + PLAYER_WIDTH)
        {
            player->_sprite->pos.x = boundary_middle + PLAYER_WIDTH;
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

bool explodePLayer(PPLAYER player)
{
    if (player->numLives > 0) {
        player->numLives--;
        touchedBy[player->playerID].touchCount = 0;
        if (player->numLives == 0) {
            // kill player
            player->score.continues--;
            player->subState = PLAYER_STATE_DEAD;
            g_Team.objectState[player->teamChoice] = OBJECT_STATE_INACTIVE;
            g_Game.currentNumPlayers--;
            if (player->isAI && g_Game.gameMode == GAME_MODE_STORY) {
                g_Game.countofRounds++; // for story mode only
            }
            if (g_Game.gameMode != GAME_MODE_STORY && g_Game.currentNumPlayers > 1) {
                setGoalSize(); // not in story mode
            }
        }
    }
    return true;
}

void killPlayer(Sint8 playerID) {
    g_Players[playerID].score.continues--;
    g_Players[playerID].subState = PLAYER_STATE_DEAD;
    g_Team.objectState[g_Players[playerID].teamChoice] = OBJECT_STATE_INACTIVE;
    g_Game.currentNumPlayers--;
    if (g_Players[playerID].isAI && g_Game.gameMode == GAME_MODE_STORY) {
        g_Game.countofRounds++;
        // unlock character once you've beaten them
        if (!characterUnlocked[g_Game.countofRounds] && g_Game.gameDifficulty > GAME_DIFFICULTY_EASY) {
            characterUnlocked[g_Game.countofRounds] = true;
            save_game_backup();
        }
    }
    if (g_Game.gameMode != GAME_MODE_STORY && g_Game.currentNumPlayers > 1) {
        setGoalSize();
    }
    // player lost all their lives, assign stars to scoring player
    if (lastTouchedBy != -1 && lastTouchedBy != playerID) {
        g_Players[lastTouchedBy].score.stars++;
    }
    else {
        for (Uint8 i = 0; i < 3; i++) {
            Sint8 id = previouslyTouchedBy[i];
            if (id == -1) continue;
            if (g_Players[id].subState == PLAYER_STATE_DEAD) continue;
            if (g_Players[id].objectState == OBJECT_STATE_INACTIVE) continue;
            if (id == playerID) continue;
            if (g_Players[playerID].onLeftSide == g_Players[id].onLeftSide) continue;
            g_Players[id].score.stars++;
            break;
        }
    }
}

void updatePlayerLives(Uint8 scoredOnPlayerID)
{
    if (g_Players[scoredOnPlayerID].numLives > 0) {
        g_Players[scoredOnPlayerID].numLives--;
        touchedBy[scoredOnPlayerID].touchCount = 0;
    }
    if (g_Players[scoredOnPlayerID] .numLives == 0) {
        killPlayer(scoredOnPlayerID);
    }
}