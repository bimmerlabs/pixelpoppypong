#include <jo/jo.h>
#include <string.h>
#include "../main.h"
#include "../assets.h"
#include "player.h"
// #include "grid.h"
// #include "explosion.h"
// #include "../team_select.h"
#include "../sprites.h"
#include "../gameplay.h"
#include "../util.h"

// distance formula without the square root
bool checkDistance(PPLAYER a, PPLAYER b);

PLAYER g_Players[MAX_PLAYERS] = {0};

bool characterAvailable[TOTAL_CHARACTERS] = {
    true, true, true, true, true, true, true, true
};

void drawPlayerSprite(PPLAYER player);
void speedLimitPlayer(PPLAYER player);
void boundPlayer(PPLAYER player);
void explodeNeighbors(PPLAYER player);

void respawnPlayer(PPLAYER player, bool deductLife);

static int getLives(GAME_MODE mode);

// const CHARACTER_ATTRIBUTES characterAttributes[] = {
   // //s  a  p   // speed, acceleration, power - scale 0-100
    // {70, 60, 55}, // MACCHI: High speed, medium acceleration, medium power
    // {55, 70, 60}, // JELLY: Medium speed, high acceleration, medium-high power
    // {60, 50, 70}, // PENNY: Medium-high speed, medium acceleration, high power
    // {60, 45, 80}, // POPPY: Low attributes (for cursors) (was 4 4 4)
    // {50, 60, 70}, // POTTER: Low attributes (for cursors) (was 4 4 4)
    // {70, 80, 50}, // SPARTA: High acceleration, high speed, medium power
    // {65, 65, 65}, // TJ: Balanced attributes
    // {70, 50, 85}, // WUPPY: High power, medium speed, low acceleration
    // {60, 60, 60}, // NONE: Medium attributes (for CPU)
// };
const CHARACTER_ATTRIBUTES characterAttributes[] = {
   //s   a   p   // speed, acceleration, power - scale 0-80
    {56, 48, 44}, // MACCHI: High speed, medium acceleration, medium power
    {44, 56, 48}, // JELLY: Medium speed, high acceleration, medium-high power
    {48, 40, 56}, // PENNY: Medium-high speed, medium acceleration, high power
    {48, 36, 64}, // POPPY: Low attributes (for cursors)
    {40, 48, 56}, // POTTER: Low attributes (for cursors)
    {56, 64, 40}, // SPARTA: High acceleration, high speed, medium power
    {52, 52, 52}, // TJ: Balanced attributes
    {56, 40, 68}, // WUPPY: High power, medium speed, low acceleration
    {48, 48, 48}, // NONE: Medium attributes (for CPU)
};

int teamCount[MAX_TEAMS] = {0};

void spawnPlayers(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];
        respawnPlayer(player, false);
    }
}

void resetPlayerScores(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        player->score.deaths = 0;
        player->score.points = 0;
        player->score.team_points = 0;

        if(player->isPlaying)
        {
            player->numLives = getLives(g_Game.gameMode);
        }
        else
        {
            player->numLives = 0;
        }
    }
}

static int getLives(GAME_MODE mode)
{
    int numLives = 0;

    switch(mode)
    {
        case GAME_MODE_PRACTICE:
            numLives = 1000; // basically infinite lives
            break;
        case GAME_MODE_NORMAL:
            numLives = 3;
            break;
        case GAME_MODE_HARDCORE:
            numLives = 1;
            break;
        case GAME_MODE_TIME_ATTACK:
            numLives = 3;
            break;
        default:
            numLives = 0; // should never get here
            break;
    }

    return numLives;

}

void initPlayers(void)
{
    PPLAYER player = NULL;

    memset(g_Players, 0, sizeof(g_Players));
    
    for (int i = 0; i < TOTAL_CHARACTERS; i++)
    {
        characterAvailable[i] = true;
    }
    
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        player = &g_Players[i];
        
        player->objectState = OBJECT_STATE_ACTIVE;
        player->playerID = i;
        
        player->score.deaths = 0;
        player->score.points = 0;
        player->score.team_points = 0;

        // PLAYER      
        player->startSelection = false;
        player->isReady = false;
        player->pressedB = false; // get rid of this?
        player->isPlaying = false; // get rid of this?
        
        // GAMEPLAY
        player->numLives = 9;
        
        // CHARACTER
        player->character.choice = CHARACTER_NONE;
        player->character.selected = false;
        player->maxSpeed = 0;
        player->acceleration = 0;
        player->power = 0;
        
        // TEAM
        player->team.choice = TEAM_CPU;
        player->team.oldTeam = TEAM_CPU;
        player->team.selected = false;
        
        player->moveHorizontal = false;
        player->moveVertical = false;
        
        // SPRITES
        // assign cursor & bg tile to each player
        player->_cursor = &player_cursor;
        player->_bg = &player_bg;
        player->_sprite = &paw_blank;
        player->_sprite->spr_id = paw_blank_id; // not sure why this changes
        // character portrait
        player->_portrait = &character_portrait;
        player->_portrait->spr_id = player->_portrait->anim1.asset[0];
    }
}

void initVsModePlayers(void)
{
    PPLAYER player = NULL;

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        player = &g_Players[i];
        
        g_Players[i]._sprite->spr_id = g_Players[i]._sprite->anim1.asset[0];
        
        // Flip the sprite based on even/odd team
        if (i == 0) {
            player->_sprite->flip = sprNoflip;
            set_spr_position(player->_sprite, -1*PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
        } 
        else if (i == 1) {
            player->_sprite->flip = sprHflip;
            set_spr_position(player->_sprite, PLAYER_X, -1*PLAYER_Y, PLAYER_DEPTH);
        }
        else if (i == 2) {
            player->_sprite->flip = sprNoflip;
            set_spr_position(player->_sprite, -1*PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
        }
        else if (i == 3) {
            player->_sprite->flip = sprHflip;
            set_spr_position(player->_sprite, PLAYER_X, PLAYER_Y, PLAYER_DEPTH);
        }
        
    }
}

void initDemoPlayers(void)
{
    PPLAYER player = NULL;

    memset(g_Players, 0, sizeof(g_Players));

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        player = &g_Players[i];
        
    }
}

void getPlayersInput(void)
{
    PPLAYER player = NULL;

    // check inputs for all players
    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            // TODO: hit a button to respawn??
            continue;
        }

        // if(player->subState == PLAYER_STATE_EXPLODING ||
           // player->subState == PLAYER_STATE_EXPLODED ||
           // player->subState == PLAYER_STATE_TITLE ||
           // player->subState == PLAYER_STATE_DEAD)
        // {
            // // uncontrollable in these states
            // continue;
        // }

        // if(player->subState == PLAYER_STATE_LOGO)
        // {
            // // moving towards the center
            // if(toINT(player->curPos.x) < 0)
            // {
                // player->curPos.dx += X_SPEED_INC;
            // }
            // else if(toINT(player->curPos.x) > 0)
            // {
                // player->curPos.dx -= X_SPEED_INC;
            // }

            // // moving towards the center
            // if(toINT(player->curPos.y) < 0)
            // {
                // player->curPos.dy += X_SPEED_INC;
            // }
            // else if(toINT(player->curPos.y) > 0)
            // {
                // player->curPos.dy -= Y_SPEED_INC;
            // }

            // continue;
        // }

        /*
        if(player->animation == PLAYER_ANIMATION_ELECTROCUTING ||
           player->animation == PLAYER_ANIMATION_CRYING ||
           player->animation == PLAYER_ANIMATION_DROPPING ||
           player->animation == PLAYER_ANIMATION_EATING )
        {
            continue;
        }
        */

        player->moveHorizontal = false;
        if (jo_is_input_key_pressed(player->playerID, JO_KEY_LEFT))
        {
                player->curPos.dx -= X_SPEED_INC;
                player->moveHorizontal = true;
        }

        if (jo_is_input_key_pressed(player->playerID, JO_KEY_RIGHT))
        {
                player->curPos.dx += X_SPEED_INC;
                player->moveHorizontal = true;
        }
        if(player->moveHorizontal == false)
        {
            player->curPos.dx = 0;
        }

        player->moveVertical = false;
        if (jo_is_input_key_pressed(player->playerID, JO_KEY_UP))
        {
                player->curPos.dy -= X_SPEED_INC;
                player->moveVertical = true;
        }
        if (jo_is_input_key_pressed(player->playerID, JO_KEY_DOWN))
        {
                player->curPos.dy += X_SPEED_INC;
                player->moveVertical = true;
        }
        if(player->moveVertical == false)
        {
            player->curPos.dy = 0;
        }

        // did the player click
        if (jo_is_input_key_down(player->playerID, JO_KEY_A) ||
            jo_is_input_key_down(player->playerID, JO_KEY_C))
        {
            start_timer = !start_timer;

        }
        
        // did the player plant a flag
        if (jo_is_input_key_down(player->playerID, JO_KEY_B))
        {
            start_timer = !start_timer;
        }
    }
}

void updatePlayers(void)
{
    PPLAYER player = NULL;

    for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    {
        player = &g_Players[i];
        if(player->objectState == OBJECT_STATE_INACTIVE)
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
        // player->size += player->ds;
        // player->angle += player->da;

        boundPlayer(player);
    }
}

void drawPlayers(void)
{
    for(unsigned int i = 0; i <  COUNTOF(g_Players); i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->objectState == OBJECT_STATE_INACTIVE)
        {
            continue;
        }
        my_sprite_draw(player->_sprite);
    }
}

void drawPlayerSprite(PPLAYER player)
{
    int playerSprite = 0;
    int crackSprite = 0;

    // playerSprite = g_Assets.cursors[g_Assets.randomizedColors[player->playerID]];

    // don't draw dead players
    if(player->subState == PLAYER_STATE_DEAD)
    {
        return;
    }

    if(player->subState == PLAYER_STATE_EXPLODING || player->subState == PLAYER_STATE_EXPLODED)
    {


        // if(player->subState == PLAYER_STATE_EXPLODED)
        // {
            // crackSprite = g_Assets.cracks[player->crackChoice];
            // jo_sprite_change_sprite_scale_xy_fixed(toFIXED(3), toFIXED(3));
            // jo_sprite_draw3D(crackSprite, toINT(player->curPos.x), toINT(player->curPos.y), CRACKS_Z);
            // jo_sprite_restore_sprite_scale();
        // }

        // jo_sprite_change_sprite_scale_xy_fixed(player->size, player->size);


        // jo_sprite_draw3D_and_rotate(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z, player->angle);

        jo_sprite_restore_sprite_scale();
    }

    // jo_sprite_draw3D(playerSprite, toINT(player->curPos.x), toINT(player->curPos.y), PLAYER_Z);


}


void speedLimitPlayer(PPLAYER player)
{
    jo_fixed bonusSpeed = 0;

    /*
    if(player->exceedLimitTimer > 0)
    {
        player->exceedLimitTimer--;
        bonusSpeed = toFIXED(4.0);
    }
    */

    // validate speeds
    if(player->curPos.dx > MAX_X_SPEED + bonusSpeed)
    {
        player->curPos.dx = MAX_X_SPEED + bonusSpeed;
    }

    if(player->curPos.dx < MAX_X_SPEED2 - bonusSpeed)
    {
        player->curPos.dx = MAX_X_SPEED2 - bonusSpeed;
    }

    if(player->curPos.dy > MAX_Y_SPEED + bonusSpeed)
    {
        player->curPos.dy = MAX_Y_SPEED + bonusSpeed;
    }

    if(player->curPos.dy < MAX_Y_SPEED2 - bonusSpeed)
    {
        player->curPos.dy = MAX_Y_SPEED2 - bonusSpeed;
    }
}

void boundPlayer(PPLAYER player)
{
    /*
    if(player->animation == PLAYER_ANIMATION_DROPPING)
    {
        // a dying player creates a splash after they hit the water level
        if(player->hasSplashed == false && toINT(player->curPos.y) >= 200)
        {
            player->hasSplashed = true;
            initSplash(player->curPos.x);
        }
        return;
    }
    */


    // // ignore wrap on logo state?
    // if(player->subState == PLAYER_STATE_LOGO)
    // {
        // return;
    // }

    // if(player->subState == PLAYER_STATE_TITLE)
    // {
        // if(player->curPos.x > SCREEN_RIGHT || player->curPos.y < SCREEN_TOP)
        // {
            // //player->curPos.x = SCREEN_LEFT;
            // // randomizeTitlePlayerPosition(&player->curPos.x, &player->curPos.y);
        // }



        // return;
    // }

    // screen boundaries
    // will need to differentiate between left/right side of the screen
    // if(player->_sprite->pos.x > SCREEN_MIDDLE - PLAYER_WIDTH)
    // {
        // player->_sprite->pos.x = SCREEN_MIDDLE - PLAYER_WIDTH;
    // }
    if(player->_sprite->pos.x > SCREEN_RIGHT - PLAYER_WIDTH)
    {
        player->_sprite->pos.x = SCREEN_RIGHT - PLAYER_WIDTH;
    }

    if(player->_sprite->pos.x < SCREEN_LEFT + PLAYER_WIDTH)
    {
        player->_sprite->pos.x = SCREEN_LEFT + PLAYER_WIDTH;
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

void explodePlayer(PPLAYER player, bool showExplosion, bool spreadExplosion)
{
    int rand = 0;

    player->subState = PLAYER_STATE_EXPLODING;
    // player->frameCount = EXPLODING_FRAME_COUNT + jo_random(EXPLODING_FRAME_COUNT);
    player->score.deaths++;

    player->curPos.dx = jo_random(0x40000) - 0x20000;
    player->curPos.dy = jo_random(0x40000) - 0x20000;

    // player->size = toFIXED(1);
    // player->ds = toFIXED(.07);

    // player->angle = jo_random(360);


    // player->da = 45 - jo_random(30);

    // 50% chance to flip the direction
    rand = jo_random(2);
    if(rand == 1)
    {
        // player->da *= -1;
    }

    // if(showExplosion)
    // {
        // // draw an explosion on the player
        // spawnExplosion(player);
    // }

    // if(spreadExplosion)
    // {
        // // spread the explosion to nearby players
        // explodeNeighbors(player);
    // }
}

void explodeNeighbors(PPLAYER player)
{
    bool neighbor = false;

    // for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
    // {
        // PPLAYER victim = &g_Players[i];

        // if(victim->objectState != OBJECT_STATE_ACTIVE)
        // {
            // continue;
        // }

        // if(victim->subState != PLAYER_STATE_ACTIVE)
        // {
            // continue;
        // }

        // if(player->playerID == victim->playerID)
        // {
            // continue;
        // }

        // // check if player is nearby
        // neighbor = checkDistance(player, victim);
        // if(neighbor == true)
        // {
            // explodePlayer(victim, false, false);
        // }
    // }
}

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
    // player->da = 0;
    // player->ds = 0;

    player->subState = PLAYER_STATE_ACTIVE;
}

// distance formula without the square root
bool checkDistance(PPLAYER a, PPLAYER b)
{
    // int dist = 36*36;

    // int x_dist = toINT(a->curPos.x) - toINT(b->curPos.x);
    // x_dist = x_dist * x_dist;

    // int y_dist = toINT(a->curPos.y) - toINT(b->curPos.y);
    // y_dist = y_dist * y_dist;

    // if(dist > x_dist + y_dist)
    // {
        // return true;
    // }

    return false;
}
