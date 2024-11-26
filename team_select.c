#include <jo/jo.h>
#include "main.h"
#include "team_select.h"
// #include "objects/flag.h"
#include "assets.h"
#include "screen_transition.h"
#include "objects/player.h"
// #include "objects/explosion.h"
#include "BG_DEF/BG25.h"

extern PLAYER g_Players[MAX_PLAYERS];

int g_StartGameFrames = 0;
bool g_TeamSelectPressedStart = false;
bool draw_cursor = false;
bool draw_portrait = false;

bool validateTeams(void);
// void updateTeamSelectPlayers(void);

const char *characterNames[] = {
    "MACCHI",
    "JELLY",
    "PENNY",
    "POPPY",
    "POTTER",
    "SPARTA",
    "T.J.",
    "WUPPY",
    "CPU"
};

void drawCharacterSelectGrid(void)
{    
    int portrait_x = PORTRAIT_X;
    int portrait_y = PORTRAIT_Y;
    int paw_x = PAW_X;
    int text_x = 2;
    int text_y = 2;
    
    if (frame % 3 == 0) {
        draw_cursor = !draw_cursor;
    }
    if (frame % 20 == 0) {
        draw_portrait = !draw_portrait;
    }
    
    
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];
        
        if (!debug_display) {
            jo_nbg0_printf(text_x, text_y, "PLAYER %i:", i+1);
            if (player->character.selected) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X1, text_y, "TEAM:");
            }
            
            jo_nbg0_printf(text_x, text_y+CHARACTER_TEXT_Y, "%s", characterNames[player->character.choice]);
            
            if (player->character.selected) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X2, text_y+TEAM_TEXT_Y, "%i", player->team.choice);
            }
        }

        // WARNING: doesn't work on hardware (VDP1 is too slow)
        // HORIZONTAL STRIPE
        // LEFT
        player->_bg->spr_id = player->_bg->anim1.asset[i];
        set_spr_position(player->_bg, -352, portrait_y, PLAYER_BG2_DEPTH);
        set_spr_scale(player->_bg, 90, 52);
        player->_bg->zmode = _ZmLC;
        my_sprite_draw(player->_bg);
        // RIGHT
        set_spr_position(player->_bg, 352, portrait_y, PLAYER_BG2_DEPTH);
        set_spr_scale(player->_bg, 210, 52);
        player->_bg->zmode = _ZmRC;
        my_sprite_draw(player->_bg);

        
        // SELECTION BOX
        switch (player->character.selected) {
            case false:
                set_spr_position(player->_cursor, portrait_x, portrait_y, CURSOR_DEPTH);
                break;
            case true:
                set_spr_position(player->_cursor, paw_x, portrait_y, CURSOR_DEPTH);
                break;
        }
        if (draw_cursor) {
            player->_cursor->mesh = MESHon;
        }
        else {
            player->_cursor->mesh = MESHoff;
        }
        player->_cursor->spr_id = player->_cursor->anim1.asset[i];
        my_sprite_draw(player->_cursor);
        
        // PORTRAIT
        player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];
        set_spr_position(player->_portrait, portrait_x, portrait_y, PORTRAIT_DEPTH);
        if (player->character.choice == CHARACTER_NONE) {
            if (draw_portrait) {
                my_sprite_draw(player->_portrait);
            }
        }
        else {
                my_sprite_draw(player->_portrait);
        }
        
        if (player->character.choice == CHARACTER_NONE) {
            set_spr_position(&menu_bg1, portrait_x, portrait_y, MENU_BG1_DEPTH);
            my_sprite_draw(&menu_bg1);
        }
        if (player->character.selected) {
            set_spr_position(&menu_bg1, paw_x, portrait_y, MENU_BG1_DEPTH);
            my_sprite_draw(&menu_bg1);
        }
                
        //PAW
        if (player->character.selected) {
                my_sprite_animation(player->_sprite);
        }
        set_spr_position(player->_sprite, paw_x, portrait_y, PORTRAIT_DEPTH);
        my_sprite_draw(player->_sprite);
        
        // CHARACTER METER
        if (player->character.startSelection) {
            jo_nbg0_printf(text_x+METER_TEXT_X, text_y,   "SPEED:%i", player->maxSpeed);
            // yellow
            meter.spr_id = meter.anim1.asset[6];
            set_spr_scale(&meter, (player->maxSpeed), METER_HEIGHT);
            set_spr_position(&meter, METER_X, portrait_y-METER_Y1, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // red
            meter.spr_id = meter.anim1.asset[7];
            set_spr_scale(&meter, (METER_WIDTH-player->maxSpeed), METER_HEIGHT);
            set_spr_position(&meter, (METER_X+(2*player->maxSpeed)), portrait_y-METER_Y1, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            
            jo_nbg0_printf(text_x+METER_TEXT_X, text_y+METER_TEXT_Y2, "ACCEL:%i", player->acceleration); 
            // yellow       
            meter.spr_id = meter.anim1.asset[6];
            set_spr_scale(&meter, (player->acceleration), METER_HEIGHT);
            set_spr_position(&meter, METER_X, portrait_y-METER_Y2, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // red
            meter.spr_id = meter.anim1.asset[7];
            set_spr_scale(&meter, (METER_WIDTH-player->acceleration), METER_HEIGHT);
            set_spr_position(&meter, (METER_X+(2*player->acceleration)), portrait_y-METER_Y2, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            
            jo_nbg0_printf(text_x+METER_TEXT_X, text_y+METER_TEXT_Y3, "POWER:%i", player->power);
            // yellow
            meter.spr_id = meter.anim1.asset[6];
            set_spr_scale(&meter, (player->power), METER_HEIGHT);
            set_spr_position(&meter, METER_X, portrait_y+METER_Y3, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // red
            meter.spr_id = meter.anim1.asset[7];
            set_spr_scale(&meter, (METER_WIDTH-player->power), METER_HEIGHT);
            set_spr_position(&meter, (METER_X+(2*player->power)), portrait_y+METER_Y3, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
        }
        
        portrait_y += PLAYER_OFFSET_Y;
        text_y += PLAYER_TEXT_OFFSET_Y;
    }
}

void drawTeamSelectGrid(void)
{
    // maybe this is where I draw the background sprites?
}

// initializations for PPP screen
void teamSelect_init(void)
{
    
    // slColOffsetOn(NBG0ON | NBG1ON | SPRON);
    // slColOffsetAUse(NBG0ON);
    // slColOffsetBUse(NBG1ON);
    // screenTransition_init(0, 0, 0);
    // slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
    // slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    initPlayers();
    g_TeamSelectPressedStart = false;
    g_StartGameFrames = TEAM_SELECT_TIMER;
}

void assign_team(int oldTeam, int newTeam) {
    if (oldTeam >= TEAM_1 && oldTeam <= TEAM_4) {
        teamCount[oldTeam - TEAM_1]--;
    }
    if (newTeam >= TEAM_1 && newTeam <= TEAM_4) {
        teamCount[newTeam - TEAM_1]++;
    }
}

void characterSelect_input(void)
{
    // int characterSelectChoice = 0;
    
    // if (!transition_complete) {
        // return;
    // }
    
    if (jo_is_pad1_key_down(JO_KEY_B) && g_Players[0].character.startSelection == false && g_Players[0].pressedB == false)
    {
        changeState(GAME_STATE_TITLE_SCREEN);
    }
    
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];
      
        if (player->character.startSelection && !player->character.selected) {
            // did the player hit a direction key
            if (jo_is_input_key_down(player->playerID, JO_KEY_LEFT))
            {
                do
                {
                    player->character.choice--;
                    if (player->character.choice < CHARACTER_MACCHI)
                    {
                        player->character.choice = CHARACTER_WUPPY;
                    }
                } while (!characterAvailable[player->character.choice]);
            }

            if (jo_is_input_key_down(player->playerID, JO_KEY_RIGHT))
            {
                do
                {
                    player->character.choice++;
                    if (player->character.choice > CHARACTER_WUPPY)
                    {
                        player->character.choice = CHARACTER_MACCHI;
                    }
                } while (!characterAvailable[player->character.choice]);
            }
            
            // player->_sprite = &paw_blank;

                player->maxSpeed = characterAttributes[player->character.choice].maxSpeed;
                player->acceleration = characterAttributes[player->character.choice].acceleration;
                player->power = characterAttributes[player->character.choice].power;
            
            if (jo_is_input_key_down(player->playerID, JO_KEY_B) && player->pressedB == false)
            {
                player->character.startSelection = false;
                characterAvailable[player->character.choice] = true;
                player->character.choice = CHARACTER_NONE;
                player->maxSpeed = 0;
                player->acceleration = 0;
                player->power = 0;
            }
            
            if (jo_is_input_key_down(player->playerID, JO_KEY_START) ||
                jo_is_input_key_down(player->playerID, JO_KEY_A) ||
                jo_is_input_key_down(player->playerID, JO_KEY_C))
            {
                // assign to a default team (left vs right)
                if (i %2 == 0) {
                    player->team.choice = TEAM_1;
                    player->_sprite->flip = sprNoflip;
                }
                else {
                    player->team.choice = TEAM_2;
                    player->_sprite->flip = sprHflip;
                }
                player->character.selected = true;
                characterAvailable[player->character.choice] = false;
            }
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
                    player->_sprite = &macchi;
                    break;
                 case CHARACTER_WUPPY:
                    player->_sprite = &jelly;
                    break;
                 default:
                    break;
                }
                // set to first frame
                g_Players[i]._sprite->spr_id = g_Players[i]._sprite->anim1.asset[0];
        }
        // wait for player to press start to begin character selection
        if (!player->character.startSelection) {
            if (jo_is_input_key_down(player->playerID, JO_KEY_START))
            {
                player->character.startSelection = true;
                player->character.choice = CHARACTER_MACCHI; // maybe make it random?
                // only select available characters // works great as a random selection lol
                while (!characterAvailable[player->character.choice]) {
                        player->character.choice++;
                        if (player->character.choice > CHARACTER_WUPPY)
                        {
                            player->character.choice = CHARACTER_MACCHI;
                        }
               }
            }
        }
        if (!player->character.selected) {
            player->_sprite->flip = sprNoflip;
            // make sure 2 people don't have the same character selected
            if (player->character.startSelection) {
                while (!characterAvailable[player->character.choice]) {
                        player->character.choice++;
                        if (player->character.choice > CHARACTER_WUPPY)
                        {
                            player->character.choice = CHARACTER_MACCHI;
                        }
               }
           }
        }
    }
    g_Players[0].pressedB = false; // button press expires
}


// handles input for the title screen menu
// only player one can control the title screen
void teamSelect_input(void)
{
    // already pressed start, nothing to do
    if(g_TeamSelectPressedStart == true)
    {
        return;
    }

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];
        player->pressedB = false; // button press expires
        if (player->character.selected == true && player->team.selected == false) {
            
            // did the player hit a direction key
            if (jo_is_input_key_down(player->playerID, JO_KEY_LEFT)) {
                do {
                    player->team.choice--;
                    if (player->team.choice < TEAM_1) {
                        player->team.choice = TEAM_4;
                    }
                } while (teamCount[player->team.choice - TEAM_1] >= MAX_TEAM_MEMBERS); // Skip full teams

                // Flip the sprite based on even/odd team
                if (player->team.choice % 2 == 0) {
                    player->_sprite->flip = sprHflip;
                } else {
                    player->_sprite->flip = sprNoflip;
                }

                return;
            }

            if (jo_is_input_key_down(player->playerID, JO_KEY_RIGHT)) {
                do {
                    player->team.choice++;
                    if (player->team.choice > TEAM_4) {
                        player->team.choice = TEAM_1;
                    }
                } while (teamCount[player->team.choice - TEAM_1] >= MAX_TEAM_MEMBERS); // Skip full teams

                // Flip the sprite based on even/odd team
                if (player->team.choice % 2 == 0) {
                    player->_sprite->flip = sprHflip;
                } else {
                    player->_sprite->flip = sprNoflip;
                }

                return;
            }
            
                
        }
        
        if (player->character.selected == true && player->team.selected == false) {
            if (jo_is_input_key_down(player->playerID, JO_KEY_START) ||
                jo_is_input_key_down(player->playerID, JO_KEY_A) ||
                jo_is_input_key_down(player->playerID, JO_KEY_C))
            {
                assign_team(player->team.oldTeam, player->team.choice);
                player->team.oldTeam = player->team.choice;
                
                player->team.selected = true;
                return;
            }
            if (jo_is_input_key_down(player->playerID, JO_KEY_B))
            {
                // g_Player1pressedB = true;
                player->pressedB = true;
                player->character.selected = false;
                characterAvailable[player->character.choice] = true;
                    // maybe need a helper for this..
                    g_Players[i]._sprite->anim1.frame = 0;
                    g_Players[i]._sprite->spr_id = g_Players[i]._sprite->anim1.asset[g_Players[i]._sprite->anim1.frame];
                player->_sprite = &paw_blank;
                player->team.choice = TEAM_CPU;
                return;
            }
        }
        
        if (jo_is_pad1_key_down(JO_KEY_START) && 
            g_Players[0].team.selected == true)
        {
            bool result;
            result = validateTeams();
            if(!result)
            {
                // // todo: play bad noise
                // // jo_audio_play_sound(&g_Assets.crackPCM);
                return;
            }

            // changeState(GAME_STATE_GAMEPLAY);
            // g_StartGameFrames = TRANSITION_TIMER;
            g_TeamSelectPressedStart = true;
            return;
        }
        
        if (player->character.selected == true && 
            player->team.selected == true && 
            jo_is_input_key_down(player->playerID, JO_KEY_B))
        {
            player->team.selected = false;
            player->team.oldTeam = TEAM_CPU;
            player->pressedB = true;
            // g_Player1pressedB = true;
            return;
        }
    }
}

void teamSelect_update(void)
{
    
    if(g_Game.gameState != GAME_STATE_TEAM_SELECT)
    {
        return;
    }
    
    // standard transition-in
    if (!transition_complete && volume < MAX_VOLUME) {
        volume += 2;
        if (volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
        jo_audio_set_volume(volume);
    }
    if (!transition_complete) {
        transition_complete = fadeIn(fade_rate, NEUTRAL_FADE);
    }
    
    if(g_TeamSelectPressedStart == true)
    {
        g_StartGameFrames--;
        
        if(g_StartGameFrames <= 0)
        {
            for(unsigned int i = 0; i < COUNTOF(g_Players); i++)
            {
                PPLAYER player = &g_Players[i];
                if(player->isPlaying != PLAYING)
                {
                    // player didn't pick a team and thus isn't playing
                    player->objectState = OBJECT_STATE_INACTIVE;
                }
            }
            
            transitionState(GAME_STATE_GAMEPLAY);
        }
    }

    // updateTeamSelectPlayers();
    updatePlayers();
}

void characterSelectBg(void) {
    // vertical stripe
    set_spr_position(&menu_bg2, -120, 240, MENU_BG2_DEPTH);
    set_spr_scale(&menu_bg2, 54, 352); // ONLY HAPPENS ONCE?
    menu_bg2.zmode = _ZmCB;
    my_sprite_draw(&menu_bg2);
}

// draws the team select menu
void teamSelect_draw(void)
{
    if(g_Game.gameState != GAME_STATE_TEAM_SELECT)
    {
        return;
    }
        
    if (attrBg250.x_scroll > toFIXED(0)) {
        attrBg250.x_pos += attrBg250.x_scroll;
        if (attrBg250.x_pos > toFIXED(512.0))
            attrBg250.x_pos = toFIXED(0);
    }
    if (attrBg250.y_scroll > toFIXED(0)) {
        attrBg250.y_pos += attrBg250.y_scroll;
        if (attrBg250.y_pos > toFIXED(512.0))
            attrBg250.y_pos= toFIXED(0);
    }
    slScrPosNbg1(attrBg250.x_pos, attrBg250.y_pos);
    
    set_spr_scale(&menu_bg1, 44, 44);
    // menu_bg1.mesh = MESHoff;
    
    drawCharacterSelectGrid();
    characterSelectBg();
    // drawTeamSelectGrid();
    // drawPlayers();
}

// adds a non player to an empty team
// looks better on the score page
static void addNonPlayerToEmptyTeam(int* teams, int numTeams, PPLAYER player, int playerID)
{
    int teamID = 0;

    if(teams[playerID] == 0)
    {
        // your own team color is empty
        // use that
        teamID = playerID;
    }
    else
    {
        // search for an empty team
        for(int i = 0; i < numTeams; i++)
        {
            if(teams[i] == 0)
            {
                // found empty team
                teams[i] = 1;
                teamID = i;
                break;
            }
        }
    }

    teams[teamID] = 1;
    // assign player to an empty team
    player->teamID = teamID;

}

// at least one team is required
bool validateTeams(void)
{
    int numTeams = 0;
    int teams[MAX_TEAMS] = {0};
    int minTeams = 0;

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];
        int teamID = 0;

        if(player->team.choice == 0)
        {
            continue;
        }
        // else if(player->team.choice > 4)
        // {
            // teamID = player->team.choice - 1;
        // }
        else
        {
            teamID = player->team.choice - 1;
        }

        teams[teamID] = teams[teamID] + 1;
        player->teamID = teamID;
    }

    //jo_printf(0, 0, "P[0] team: %d", g_Players[0].teamID);

    for(int i = 0; i < MAX_TEAMS; i++)
    {
        if(teams[i] > 0)
        {
            numTeams++;
        }
    }

    minTeams = 1;

    if(numTeams < minTeams)
    {
        return false;
    }

    // // destroy the flag of empty teams
    // for(int i = 0; i < MAX_TEAMS; i++)
    // {
        // if(teams[i] == 0)
        // {
            // destroyTeamSelectFlag(i);
        // }
    // }

    // drop the players who aren't in
    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->team.choice == 0)
        {
            // explodePlayer(player, true, false);
            player->numLives = 0;
            addNonPlayerToEmptyTeam(teams, COUNTOF(teams), player, i);
            player->isPlaying = NOT_PLAYING;
            continue;
        }
        else
        {
            player->isPlaying = PLAYING;
        }
    }
    return true;
}

// void updateTeamSelectPlayers(void)
// {
    // if(g_TeamSelectPressedStart == true)
    // {
        // return;
    // }

    // for(int i = 0; i < MAX_PLAYERS; i++)
    // {
        // PPLAYER player = &g_Players[i];
        // int playerX = 0;
        // int playerY = 0;

        // playerX = -242 + (player->team.choice * 42);
        // playerY = -200 + 9 + ((i + 1) * 28);

        // player->curPos.x = toFIXED(playerX);
        // player->curPos.y = toFIXED(playerY);

        // if(i >= 6)
        // {
            // player->curPos.y += toFIXED(28);
        // }
    // }

    // return;
// }
