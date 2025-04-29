#include <jo/jo.h>
#include "main.h"
#include "team_select.h"
#include "assets.h"
#include "screen_transition.h"
#include "physics.h"
#include "objects/player.h"
#include "BG_DEF/nbg1.h"
#include "BG_DEF/sprite_colors.h"

extern PLAYER g_Players[MAX_PLAYERS];

int g_StartGameFrames = 0;
Uint8 currentNumPlayers = 0;
bool g_TeamSelectPressedStart = false;
bool draw_cursor = false;
bool draw_portrait = false;
bool all_players_ready = false;

// TODO: move to characters.h?
const char *characterNames[] = {
    "MACCHI",
    "JELLY",
    "PENNY",
    "POTTER",
    "SPARTA",
    "POPPY",
    "T.J.",
    "GEORGE",
    "WUPPY",
    "CRAIG",
    "GARF.",
    "CPU"
};

// need to test/fix so debug mode doesn't have to be turned on
void initUnlockedCharacters(void) {
    // Unlock the first four characters by default, others remain locked
    for (int i = 0; i < TOTAL_CHARACTERS; i++) {
        if (i <= CHARACTER_POTTER || i == CHARACTER_NONE) {
            characterUnlocked[i] = true;
        }
        else {
            characterUnlocked[i] = false;
        }
    }
}

void initAvailableCharacters(void) {
    if (g_GameOptions.debug_mode) {
        for (int i = 0; i < TOTAL_CHARACTERS; i++) {
            characterAvailable[i] = true;
        }
    }
    // Copy values from characterUnlocked to characterAvailable
    else {
        for (int i = 0; i < TOTAL_CHARACTERS; i++) {
            characterAvailable[i] = characterUnlocked[i];
        }
    }
}

void teamSelect_init(void)
{
    g_Game.lastState = GAME_STATE_TEAM_SELECT;
    unloadTitleScreenAssets();
    loadCharacterAssets();
    reset_inputs();
    initPlayers();
    initAvailableCharacters();

    initTeams();
    all_players_ready = false;
    g_TeamSelectPressedStart = false;
    g_StartGameFrames = TEAM_SELECT_TIMER;
    g_Team.numTeams = -1;
    g_Team.minTeams = TEAM_1;
    
    if (g_GameOptions.mesh_display) {
        menu_bg1.mesh = MESHon;
        menu_bg2.mesh = MESHon;
        player_bg.mesh = MESHon;
    }
    else {
        menu_bg1.mesh = MESHoff;
        menu_bg2.mesh = MESHoff;
        player_bg.mesh = MESHoff;
    }       
    
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    
    // some assets don't change in scale
    menu_bg1.spr_id = menu_bg1.anim1.asset[4];
    set_spr_scale(&menu_bg1, 46, 46);
    menu_bg2.spr_id = menu_bg2.anim1.asset[5];
    set_spr_position(&menu_bg2, -120, 240, MENU_BG2_DEPTH);
    set_spr_scale(&menu_bg2, 54, 352);
    
    mosaic_in = true;
    music_in = true;
    fade_in = true;
    transition_in = true;
}

// main logic loop
void teamSelect_update(void)
{
    if(g_Game.gameState != GAME_STATE_TEAM_SELECT)
    {
        return;
    }
    
    // EVERYONE PRESSED START
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
                    if (g_Game.numPlayers >= g_Game.currentNumPlayers) {
                        initAiPlayers();               
                    }
                    else {
                        // player didn't pick a team and thus isn't playing
                        player->objectState = OBJECT_STATE_INACTIVE;
                    }
                }
            }
            transitionState(GAME_STATE_GAMEPLAY);
        }
    }
}

// main draw routine
void teamSelect_draw(void)
{
    if(g_Game.gameState != GAME_STATE_TEAM_SELECT)
    {
        return;
    }
        
    if (attrNbg1.x_scroll > toFIXED(0)) {
        attrNbg1.x_pos += attrNbg1.x_scroll;
        if (attrNbg1.x_pos > toFIXED(512.0))
            attrNbg1.x_pos = toFIXED(0);
    }
    slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);
    
    drawCharacterSelectGrid();
}

void drawCharacterSelectGrid(void)
{    
    int portrait_x = PORTRAIT_X;
    int portrait_y = PORTRAIT_Y;
    int paw_x = PAW_X;
    int text_x = 2;
    int text_y = 2;
    
    if (g_Game.numPlayers == ONE_PLAYER) {
        portrait_y = -6;
        text_y = 12;
    }
    else if (g_Game.numPlayers == TWO_PLAYER) {
        portrait_y = -70;
        text_y = 8;
    }
    else if (g_Game.numPlayers == THREE_PLAYER) {
        portrait_y = -118;
        text_y = 5;
    }
    
    if (frame % 3 == 0) { // modulus
        draw_cursor = !draw_cursor;
    }
    if (frame % 20 == 0) { // modulus
        draw_portrait = !draw_portrait;
    }
    
    for(unsigned int i = 0; i < (g_Game.numPlayers+1); i++)
    {
        PPLAYER player = &g_Players[i];
        
        if (!g_GameOptions.debug_display) {
            jo_nbg0_printf(text_x, text_y, "PLAYER %i:", i+1);
            if (player->character.choice != CHARACTER_NONE || g_Game.numPlayers > ONE_PLAYER) {
                jo_nbg0_printf(text_x, text_y+CHARACTER_TEXT_Y, "%s", characterNames[player->character.choice]);
            }           
            if (player->teamSelected && g_Game.numPlayers > ONE_PLAYER) {
                jo_nbg0_printf(text_x, text_y+CHARACTER_TEXT_Y+2, "TEAM %i", player->teamChoice+1);
            }
        }
            
        if (player->character.selected) {
            if (player->isReady) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X1, text_y, "READY");
            }
            else if (player->teamSelected && g_Team.numTeams >= g_Team.minTeams && !player->isReady && !draw_portrait) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X1, text_y, "PRESS");
            }
            else if (!player->teamSelected && g_Game.numPlayers > ONE_PLAYER) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X1, text_y, "TEAM:");
            }

            if (player->teamSelected && g_Team.numTeams >= g_Team.minTeams && !player->isReady && !draw_portrait) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X1, text_y+TEAM_TEXT_Y, "START");
            }
            else if (player->teamSelected && g_Team.numTeams < g_Team.minTeams) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X1, text_y+TEAM_TEXT_Y, "WAIT.");
            }
            else if (!player->teamSelected && g_Game.numPlayers > ONE_PLAYER) {
                jo_nbg0_printf(text_x+TEAM_TEXT_X2, text_y+TEAM_TEXT_Y, "%i", player->teamChoice+1);
            }
        }

        // VERTICAL STRIPE (animate?)
        menu_bg2.zmode = _ZmCB;
        my_sprite_draw(&menu_bg2);
                
        // SELECTION BOX
        switch (player->character.selected) {
            case false: // SELECT CHARACTER
                set_spr_position(player->_cursor, portrait_x, portrait_y, CURSOR_DEPTH);
                
                // CURSOR BACKGROUND
                set_spr_position(&menu_bg1, portrait_x, portrait_y, MENU_BG1_DEPTH);
                my_sprite_draw(&menu_bg1);
                // PAW
                set_spr_position(player->_sprite, paw_x, portrait_y, PORTRAIT_DEPTH);
                my_sprite_draw(player->_sprite);
                break;
            case true: // SELECT TEAM
                set_spr_position(player->_cursor, paw_x, portrait_y, CURSOR_DEPTH);
                
                // CURSOR BACKGROUND
                set_spr_position(&menu_bg1, paw_x, portrait_y, MENU_BG1_DEPTH);
                my_sprite_draw(&menu_bg1);
                // PAW
                looped_animation_pow(player->_sprite, 4);
                set_spr_position(player->_sprite, paw_x, portrait_y, PORTRAIT_DEPTH);
                my_sprite_draw(player->_sprite);
                break;
        }
        
        // CURSOR
        if (draw_cursor) {
            player->_cursor->mesh = MESHon;
        }
        else {
            player->_cursor->mesh = MESHoff;
        }
        player->_cursor->spr_id = player->_cursor->anim1.asset[i];
        my_sprite_draw(player->_cursor);
        
        // PORTRAIT
        set_spr_scale(player->_portrait, 2, 2);
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
        
        // CHARACTER METER
        if (player->startSelection) {
            // SPEED
            if (g_GameOptions.debug_mode) {
                jo_nbg0_printf(text_x+METER_TEXT_X, text_y,   "SPEED:%i", player->maxSpeed);
            }
            else {
                jo_nbg0_printf(text_x+METER_TEXT_X, text_y,   "SPEED");
            }
            // yellow
            meter.spr_id = meter.anim1.asset[7];
            set_spr_scale(&meter, player->maxSpeed, METER_HEIGHT);
            set_spr_position(&meter, METER_X, portrait_y-METER_Y1, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // red
            meter.spr_id = meter.anim1.asset[8];
            set_spr_scale(&meter, (METER_WIDTH-player->maxSpeed), METER_HEIGHT);
            set_spr_position(&meter, (METER_X+(2*player->maxSpeed)), portrait_y-METER_Y1, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // ACCELERATION
            Uint8 acceleration = JO_FIXED_TO_INT(player->acceleration);
            if (g_GameOptions.debug_mode) {
                jo_nbg0_printf(text_x+METER_TEXT_X, text_y+METER_TEXT_Y2, "ACCEL:%i", acceleration); 
            }
            else {
                jo_nbg0_printf(text_x+METER_TEXT_X, text_y+METER_TEXT_Y2, "ACCEL."); 
            }
            // yellow       
            meter.spr_id = meter.anim1.asset[7];
            set_spr_scale(&meter, acceleration, METER_HEIGHT);
            set_spr_position(&meter, METER_X, portrait_y-METER_Y2, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // red
            meter.spr_id = meter.anim1.asset[8];
            set_spr_scale(&meter, (METER_WIDTH-acceleration), METER_HEIGHT);
            set_spr_position(&meter, (METER_X+(2*acceleration)), portrait_y-METER_Y2, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // POWER
            if (g_GameOptions.debug_mode) {
                jo_nbg0_printf(text_x+METER_TEXT_X, text_y+METER_TEXT_Y3, "POWER:%i", player->power);
            }
            else {
                jo_nbg0_printf(text_x+METER_TEXT_X, text_y+METER_TEXT_Y3, "POWER");
            }
            // yellow
            meter.spr_id = meter.anim1.asset[7];
            set_spr_scale(&meter, player->power, METER_HEIGHT);
            set_spr_position(&meter, METER_X, portrait_y+METER_Y3, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
            // red
            meter.spr_id = meter.anim1.asset[8];
            set_spr_scale(&meter, (METER_WIDTH-player->power), METER_HEIGHT);
            set_spr_position(&meter, (METER_X+(2*player->power)), portrait_y+METER_Y3, PORTRAIT_DEPTH);
            my_sprite_draw(&meter);
        }
        
        portrait_y += PLAYER_OFFSET_Y;
        text_y += PLAYER_TEXT_OFFSET_Y;
    }
}

void characterSelect_input(void)
{    
    // ONLY PLAYER 1 CAN EXIT TO TITLE SCREEN
    if (jo_is_pad1_key_down(JO_KEY_B) && g_Players[0].startSelection == false && g_Players[0].pressedB == false)
    {
        g_Game.lastState = GAME_STATE_TEAM_SELECT;
        transitionState(GAME_STATE_TITLE_SCREEN);
    }
    
    for(unsigned int i = 0; i < (g_Game.numPlayers+1); i++)
    {
        PPLAYER player = &g_Players[i];
      
        if (player->startSelection && !player->character.selected) {
            // CHOOSE CHARACTER
            if (jo_is_input_key_down(player->input->id, JO_KEY_LEFT))
            {
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                do
                {
                    player->character.choice--;
                    if (player->character.choice < CHARACTER_MACCHI)
                    {
                        player->character.choice = TOTAL_CHARACTERS;
                    }
                } while (!characterAvailable[player->character.choice]);
            }
            if (jo_is_input_key_down(player->input->id, JO_KEY_RIGHT))
            {
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                do
                {
                    player->character.choice++;
                    if (player->character.choice > TOTAL_CHARACTERS)
                    {
                        player->character.choice = CHARACTER_MACCHI;
                    }
                } while (!characterAvailable[player->character.choice]);
            }
            // ASSIGN STATS
            player->maxSpeed = characterAttributes[player->character.choice].maxSpeed;
            player->acceleration = toFIXED(characterAttributes[player->character.choice].acceleration);
            player->power = characterAttributes[player->character.choice].power;
            
            // GO BACK
            if (jo_is_input_key_down(player->input->id, JO_KEY_B) && player->pressedB == false)
            {
                pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
                player->startSelection = false;
                characterAvailable[player->character.choice] = true;
                player->_sprite = &paw_blank;
                player->character.choice = CHARACTER_NONE;
                player->maxSpeed = 0;
                player->acceleration = 0;
                player->power = 0;
                return;
            }
            
            // SELECT CHARACTER
            if (jo_is_input_key_down(player->input->id, JO_KEY_START) ||
                jo_is_input_key_down(player->input->id, JO_KEY_A) ||
                jo_is_input_key_down(player->input->id, JO_KEY_C))
            {
                pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 6);
                // assign to a default team (left vs right)
                if (i %2 == 0) { // modulus (replace with jo function)
                    player->teamChoice = TEAM_1;
                    player->_sprite->flip = sprNoflip;
                }
                else {
                    player->teamChoice = TEAM_2;
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
                // set to first frame and make sure the scale is normal
                set_spr_scale(player->_sprite, 2, 2);
               player->_sprite->spr_id = player->_sprite->anim1.asset[0];
        }
        // BEGIN CHARACTER SELECTION
        if (!player->startSelection) {
            // Once a player starts selection, they shouldn't be able to assign a new id
            if (player->input->isSelected && jo_is_input_key_down(player->input->id, JO_KEY_START)) {
                pcm_play(g_Assets.startPcm8, PCM_VOLATILE, 6);
                player->startSelection = true;
                player->character.choice = CHARACTER_MACCHI;
                validateCharacters(player);
            }
            else {
                for(unsigned int ip = 0; ip < COUNTOF(g_Inputs); ip++)
                {
                    // Once a player starts selection, they shouldn't be able to assign a new id
                    if (g_Inputs[ip].isSelected) {
                        continue;
                    }
                    if (jo_is_input_key_down(ip, JO_KEY_START))
                    {
                        pcm_play(g_Assets.startPcm8, PCM_VOLATILE, 6);
                        player->input = &g_Inputs[ip];
                        player->input->id = ip;
                        player->input->isSelected = true;
                        
                        player->startSelection = true;
                        player->character.choice = CHARACTER_MACCHI;
                        validateCharacters(player);
                        player->teamChoice = TEAM_1;
                        validateTeam(player);
                       return;
                    }
                }
            }
        }
        // DEFAULT CHARACTER SETUP
        if (!player->character.selected) {
            player->_sprite->flip = sprNoflip;
            // make sure 2 people don't have the same character selected
            if (player->startSelection) {
                validateCharacters(player);
           }
        }
    }
    g_Players[0].pressedB = false; // button press expires
}


// once a character has been selected all players can select their teams
void teamSelect_input(void)
{
    // already pressed start, nothing to do
    if(g_TeamSelectPressedStart == true)
    {
        return;
    }

    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];
        
        player->pressedB = false; // button press expires
        
        if (player->teamSelected == false && player->character.selected) {
            if (g_Game.numPlayers == ONE_PLAYER) {
                // default team
                player->teamChoice = TEAM_1;
                g_Team.numTeams++;
            }
            // CHOOSE A TEAM
            if (jo_is_input_key_down(player->input->id, JO_KEY_LEFT) && g_Game.numPlayers != ONE_PLAYER) {
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                do {
                    player->teamChoice--;
                    if (player->teamChoice < TEAM_1) {
                        player->teamChoice = g_Team.maxTeams;
                    }
                } while (!g_Team.isAvailable[player->teamChoice]); // Skip full teams // replace with teamAvailable

                // Flip the sprite based on even/odd team
                if (player->teamChoice == TEAM_2 || player->teamChoice == TEAM_4) {
                    player->_sprite->flip = sprHflip;
                } else {
                    player->_sprite->flip = sprNoflip;
                }

                return;
            }
            if (jo_is_input_key_down(player->input->id, JO_KEY_RIGHT) && g_Game.numPlayers != ONE_PLAYER) {
                pcm_play(g_Assets.cursorPcm8, PCM_VOLATILE, 6);
                do {
                    player->teamChoice++;
                    if (player->teamChoice > g_Team.maxTeams) {
                        player->teamChoice = TEAM_1;
                    }
                } while (!g_Team.isAvailable[player->teamChoice]); // Skip full teams // replace with teamAvailable


                // Flip the sprite based on even/odd team
                if (player->teamChoice == TEAM_2 || player->teamChoice == TEAM_4) {
                    player->_sprite->flip = sprHflip;
                } else {
                    player->_sprite->flip = sprNoflip;
                }

                return;
            }

            // GO BACK
            if (jo_is_input_key_down(player->input->id, JO_KEY_B))
            {
                pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
                player->pressedB = true;
                characterAvailable[player->character.choice] = true;
                player->character.selected = false;
                return;
            }
            
            // SELECT TEAM
            if (jo_is_input_key_down(player->input->id, JO_KEY_START) ||
                jo_is_input_key_down(player->input->id, JO_KEY_A) ||
                jo_is_input_key_down(player->input->id, JO_KEY_C))
            {
                pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 6);
                // assign_team(player->teamOldTeam, player->teamChoice);
                player->teamOldTeam = player->teamChoice;
                player->teamSelected = true;
                g_Team.isAvailable[player->teamChoice] = false;
                g_Team.numTeams++;
                return;
            }
        }
        
        if (player->teamSelected == true) {
            // START GAME
            if (playerReadyState())
            {
                g_TeamSelectPressedStart = true;
                return;
            }
            
            // PRESS START TO BE "READY"
            if (jo_is_input_key_down(player->input->id, JO_KEY_START))
            {
                if(!validateTeamCount())
                {
                    return;
                }
                 pcm_play(g_Assets.nextPcm8, PCM_VOLATILE, 7);
                 player->isReady = true;
                 g_Game.currentNumPlayers++;
                 return;
            }
            
            // GO BACK
            if (jo_is_input_key_down(player->input->id, JO_KEY_B))
            {   
                pcm_play(g_Assets.cancelPcm8, PCM_VOLATILE, 6);
                resetReadyState();
                all_players_ready = false;
                player->isReady = false;
                player->teamSelected = false;
                g_Team.isAvailable[player->teamChoice] = true;
                // player->teamChoice = TEAM_1;
                // player->teamOldTeam = TEAM_COUNT;
                player->pressedB = true;
                g_Team.numTeams--;
                if (g_Game.currentNumPlayers > 0)
                    g_Game.currentNumPlayers--;
                return;
            }
        }
    }
}

// VALIDATION CHECKS

// only select available characters 
void validateCharacters(PLAYER *player) {
    while (!characterAvailable[player->character.choice]) {
        player->character.choice++;
        if (player->character.choice > TOTAL_CHARACTERS)
        {
            player->character.choice = CHARACTER_MACCHI;
        }
    }
}

void validateTeam(PLAYER *player)
{
    while (!g_Team.isAvailable[player->teamChoice])
    {
        player->teamChoice++;
        if (player->teamChoice >= TEAM_COUNT)
            player->teamChoice = TEAM_1;
    }
}

// DON'T START GAME UNTIL EVERONE IS READY
bool playerReadyState(void) {
     for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];

        if (player->startSelection && !player->isReady)
        {
            all_players_ready = false;
            return false;
        }
    }
    all_players_ready = true;
    return true;
}

void resetReadyState(void) {
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];
        player->isReady = false;
    }
}

// // ENSURES THAT AT LEAST 1 PERSON IS ON A DIFFERENT TEAM
// // only required if multiple people can be on 1 team
// void assign_team(int oldTeam, int newTeam) {
    // if (oldTeam >= TEAM_1 && oldTeam <= TEAM_4) {
        // teamCount[oldTeam]--;
    // }
    // if (newTeam >= TEAM_1 && newTeam <= TEAM_4) {
        // teamCount[newTeam]++;
    // }
// }

// AT LEAST 2 TEAMS ARE REQUIRED
bool validateTeamCount(void)
{
    if(g_Team.numTeams < g_Team.minTeams)
    {
        return false;
    }
    
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];

        if(player->teamChoice == TEAM_COUNT)
        {
            player->numLives = 0;
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
