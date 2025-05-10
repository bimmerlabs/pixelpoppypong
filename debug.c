#include <jo/jo.h>
#include "main.h"
#include "backup.h"
#include "screen_transition.h"
#include "ppp_logo.h"
#include "title_screen.h"
#include "team_select.h"
#include "gameplay.h"
#include "pause.h"
#include "assets.h"
#include "physics.h"
#include "objects/player.h"
#include "BG_DEF/nbg1.h"
#include "credits.h"

extern PLAYER g_Players[MAX_PLAYERS];

void debux_text(void)
{
    if (!g_GameOptions.debug_display) {
        return;
    }
        jo_getdate(&now);
        jo_nbg0_printf(2, 2, "STATE:CURRENT:%2d", g_Game.gameState);
        jo_nbg0_printf(20, 2, "LAST:%2d", g_Game.lastState);
        jo_nbg0_printf(29, 2, "NEXT:%2d", g_Game.nextState);
        jo_nbg0_printf(2, 3, "FRAME:%i", frame);
        jo_nbg0_printf(2, 4, "VOLUME:%i,%i", g_Audio.masterVolume, g_StartGameFrames);
        // jo_nbg0_printf(2, 5, "TESTCOLLISION:%i", g_GameOptions.testCollision);
        // jo_nbg0_printf(20, 24, "DO_UPDATE_ALL:%i", do_update_All);
        // jo_nbg0_printf(20, 25, "UPDATE_PALETTE_ALL:%i", update_palette_All);
        // jo_nbg0_printf(20, 24, "BACKUP_DEVICE:%i", backup_device);
        // jo_nbg0_printf(20, 25, "MEMORY_ACCESS:%i", memory_access);
        jo_nbg0_printf(20, 3, "GAME MODE:%i", g_Game.gameMode);
        jo_nbg0_printf(20, 4, "DIFFICULTY:%i", g_Game.gameDifficulty);
        // jo_nbg0_printf(20, 5, "LOADING:%i", g_Game.isLoading);
        // jo_nbg0_printf(20, 6, "SPRITES:%d", jo_sprite_count());
        jo_nbg0_printf(2, 26, "DATE:%02d.%02d.%d", now.day, now.month, now.year);
        jo_nbg0_printf(2, 27, "TIME:%02d:%02d:%02d", now.hour, now.minute, now.second);
        // jo_nbg0_printf(2, 26, "CURRENT_BACKGROUND:%i", current_background);
        // jo_nbg0_printf(20, 2, "VDP1 MEMORY:%d%%", jo_sprite_usage_percent());
        // jo_nbg0_printf(20, 3, "RAM USAGE:%d%%", jo_memory_usage_percent());
        
        // transition debug
        // if (g_GameOptions.mosaic_display) {
        // jo_nbg0_printf(20, 2, "MOSAIC_X/Y:%i,%i", mosaic_x, mosaic_y);
        // }
        // jo_nbg0_printf(2, 25, "PLAYING CD:%i", g_Audio.cdIsPlaying);
        // jo_nbg0_printf(2, 27, "VOLUME:%i", g_Audio.masterVolume);
        jo_nbg0_printf(20, 6, "TRANS_IN/OUT:%i, %i", transition_in, transition_out);
        jo_nbg0_printf(20, 5, "FADE_IN/OUT:%i, %i", fade_in, fade_out);
        jo_nbg0_printf(20, 7, "MOSAIC_IN/OUT:%i, %i", mosaic_in, mosaic_out);
        
        switch (g_Game.gameState) {
            case GAME_STATE_PPP_LOGO: // logo
                jo_nbg0_printf(2, 5, "LOGOTIMER:%i", g_LogoTimer/60);
                break;
                
            case GAME_STATE_TITLE_SCREEN: // title
                jo_nbg0_printf(2, 5, "TITLETIMER:%i", g_TitleTimer/60);
                
                // jo_nbg0_printf(2, 7, "LOGO1_POS:%i", logo1_pos);
                // jo_nbg0_printf(20, 7, "LOGO1:%i", logo1.pos.y);
                // jo_nbg0_printf(2, 8, "LOGO2_POS:%i", logo2_pos);
                // jo_nbg0_printf(20, 8, "LOGO2:%i", logo2.pos.y);
                // jo_nbg0_printf(20, 10, "VELOCITY:%i", logo_velocity);
                // jo_nbg0_printf(20, 11, "LOGO_FALLING:%i", logo_falling);
                // jo_nbg0_printf(20, 12, "LOGO_BOUNCE:%i", logo_bounce);
                break;
                
            case GAME_STATE_TITLE_MENU: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE:%i", g_Game.gameMode);
                jo_nbg0_printf(2, 6, "GAMEDIFFICULTY:%i", g_Game.gameDifficulty);
                jo_nbg0_printf(2, 7, "TITLE CHOICE:%i", g_TitleScreenChoice);
                // jo_nbg0_printf(2, 7, "LOGO1_POS:%i", logo1_pos);
                // jo_nbg0_printf(20, 7, "LOGO1:%i", logo1.pos.y);
                // jo_nbg0_printf(2, 8, "LOGO2_POS:%i", logo2_pos);
                // jo_nbg0_printf(20, 8, "LOGO2:%i", logo2.pos.y);
                // jo_nbg0_printf(20, 9, "VELOCITY:%i", logo_velocity);
                // jo_nbg0_printf(20, 11, "LOGO_FALLING:%i", logo_falling);
                // jo_nbg0_printf(20, 12, "LOGO_BOUNCE:%i", logo_bounce);
                // jo_nbg0_printf(2, 18, "NUM.PLAYERS:%i", g_Game.numPlayers);
                // jo_nbg0_printf(2, 19, "MIN.PLAYERS:%i", g_Game.minPlayers);
                // jo_nbg0_printf(2, 20, "MAX.PLAYERS:%i", g_Game.maxPlayers);
                // jo_nbg0_printf(20, 18, "NUM.TEAMS:%i", g_Team.numTeams);
                // jo_nbg0_printf(20, 19, "MIN.TEAMS:%i", g_Team.minTeams);
                // jo_nbg0_printf(20, 20, "MAX.TEAMS:%i", g_Team.maxTeams);
                break;
                
            case GAME_STATE_TITLE_OPTIONS: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE:%i", g_Game.gameMode);
                jo_nbg0_printf(2, 6, "OPTION CHOICE:%i", g_OptionScreenChoice);
                break;
                
            case GAME_STATE_TEAM_SELECT: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE:%i", g_Game.gameMode);
                
                jo_nbg0_printf(20, 14, "NUMPLAYERS:%i", g_Game.currentNumPlayers);
                jo_nbg0_printf(20, 15, "NUMTEAMS:%i", g_Team.numTeams);
                
                // for (int i = 0; i < TOTAL_CHARACTERS; i++) {
                    // jo_nbg0_printf(2, (8+i), "UNLOCKED:%i", characterUnlocked[i]);
                // }
                // for (int i = 0; i < TOTAL_CHARACTERS; i++) {
                    // jo_nbg0_printf(20, (8+i), "AVAILABLE:%i", characterAvailable[i]);
                // }
                // jo_nbg0_printf(2, 7, "P1_INPUT:%i", g_Players[0].input->id);
                // jo_nbg0_printf(20, 7, "IS SELECTED:%i", g_Players[0].input->isSelected);
                // jo_nbg0_printf(2, 8, "P2_INPUT:%i", g_Players[1].input->id);
                // jo_nbg0_printf(20, 8, "IS SELECTED:%i", g_Players[1].input->isSelected);
                
                jo_nbg0_printf(2, 10, "TEAM 1 AVAILABLE:%i", g_Team.isAvailable[TEAM_1]);
                jo_nbg0_printf(2, 11, "TEAM 2 AVAILABLE:%i", g_Team.isAvailable[TEAM_2]);
                jo_nbg0_printf(2, 12, "TEAM 3 AVAILABLE:%i", g_Team.isAvailable[TEAM_3]);
                jo_nbg0_printf(2, 13, "TEAM 4 AVAILABLE:%i", g_Team.isAvailable[TEAM_4]);
                
                // jo_nbg0_printf(2, 7, "TOTAL_CHARACTERS:%i",  TOTAL_CHARACTERS);
                // jo_nbg0_printf(2, 8, "P1 CHAR CHOICE:%i", g_Players[0].character.choice);
                
                // jo_nbg0_printf(20, 7, "P1 STARTSELECT:%i",  g_Players[0].startSelection);
                // jo_nbg0_printf(20, 8, "P1 CHAR SELECT:%i", g_Players[0].character.selected);
                // jo_nbg0_printf(20, 9, "P1 TEAM SELECT:%i", g_Players[0].teamSelected);
                // jo_nbg0_printf(20, 10, "P1 TEAM CHOICE:%i", g_Players[0].teamChoice);
                // jo_nbg0_printf(20, 11, "P1 READY:%i", g_Players[0].isReady);
                
                // jo_nbg0_printf(2, 13, "P2 STARTSELECT:%i",  g_Players[1].startSelection);
                // jo_nbg0_printf(2, 14, "P2 CHAR SELECT:%i", g_Players[1].character.selected);
                // jo_nbg0_printf(2, 15, "P2 TEAM SELECT:%i", g_Players[1].teamSelected);
                // jo_nbg0_printf(2, 16, "P2 TEAM CHOICE:%i", g_Players[1].teamChoice);
                // jo_nbg0_printf(2, 17, "P2 READY:%i", g_Players[1].isReady);
                
                // jo_nbg0_printf(20, 7, "P3 STARTSELECT:%i",  g_Players[2].startSelection);
                // jo_nbg0_printf(20, 8, "P3 CHAR SELECT:%i", g_Players[2].character.selected);
                // jo_nbg0_printf(20, 9, "P3 TEAM SELECT:%i", g_Players[2].teamSelected);
                // jo_nbg0_printf(20, 10, "P3 TEAM CHOICE:%i", g_Players[2].teamChoice);
                // jo_nbg0_printf(20, 11, "P3 READY:%i", g_Players[2].isReady);
                
                // jo_nbg0_printf(20, 13, "P4 STARTSELECT:%i",  g_Players[3].startSelection);
                // jo_nbg0_printf(20, 14, "P4 CHAR SELECT:%i", g_Players[3].character.selected);
                // jo_nbg0_printf(20, 15, "P4 TEAM SELECT:%i", g_Players[3].teamSelected);
                // jo_nbg0_printf(20, 16, "P4 TEAM CHOICE:%i", g_Players[3].teamChoice);
                // jo_nbg0_printf(20, 17, "P4 READY:%i", g_Players[3].isReady);
                
                
                jo_nbg0_printf(2, 18, "NUM.TEAMS %i:", g_Team.numTeams);
                jo_nbg0_printf(2, 19, "MIN.TEAMS %i:", g_Team.minTeams);
                jo_nbg0_printf(2, 20, "MAX.TEAMS %i:", g_Team.maxTeams);
                // jo_nbg0_printf(2, 19, "ALL READY:%i", all_players_ready);
                // jo_nbg0_printf(2, 20, "PRESSEDSTART:%i",  g_TeamSelectPressedStart);
                
                break;
                
            case GAME_STATE_DEMO_LOOP:
                jo_nbg0_printf(2, 5, "GAMEACTIVE:%i", g_Game.isActive);
                jo_nbg0_printf(2, 6, "BALLACTIVE:%i", g_Game.isBallActive);
                // jo_nbg0_printf(2, 5, "DEMOTIMER:%i", g_DemoTimer/60);
                // jo_nbg0_printf(2, 8, "MACCHI.X:%i", macchi.pos.x);
                // jo_nbg0_printf(2, 9, "MACCHI.Y:%i", macchi.pos.y);
                // jo_nbg0_printf(2, 10, "COLLIDING:%i", macchi.isColliding);
                // jo_nbg0_printf(2, 11, "JELLY.X:%i", jelly.pos.x);
                // jo_nbg0_printf(2, 12, "JELLY.Y:%i", jelly.pos.y);
                // jo_nbg0_printf(2, 13, "COLLIDING:%i", jelly.isColliding);
                // jo_nbg0_printf(20, 8, "POPPY.X:%i", pixel_poppy.pos.x);
                // jo_nbg0_printf(20, 9, "POPPY.Y:%i", pixel_poppy.pos.y);
                // jo_nbg0_printf(20, 10, "COLLIDING:%i", pixel_poppy.isColliding);
                
                // jo_nbg0_printf(2, 12, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[0]._sprite->pos.r));
                // jo_nbg0_printf(2, 13, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[1]._sprite->pos.r));
                // jo_nbg0_printf(2, 14, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[2]._sprite->pos.r));
                // jo_nbg0_printf(2, 15, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[3]._sprite->pos.r));       
                
                jo_nbg0_printf(20, 10, "TEAM 1 ACTIVE:%i", g_Team.objectState[TEAM_1]);
                jo_nbg0_printf(20, 11, "TEAM 2 ACTIVE:%i", g_Team.objectState[TEAM_2]);
                jo_nbg0_printf(20, 12, "TEAM 3 ACTIVE:%i", g_Team.objectState[TEAM_3]);
                jo_nbg0_printf(20, 13, "TEAM 4 ACTIVE:%i", g_Team.objectState[TEAM_4]);   
                
                // jo_nbg0_printf(2, 12, "TOUCHEDBY 1:%i", touchedBy[0].hasTouched);
                // jo_nbg0_printf(2, 13, "TOUCHEDBY 2:%i", touchedBy[1].hasTouched);
                // jo_nbg0_printf(2, 14, "TOUCHEDBY 3:%i", touchedBy[2].hasTouched);
                // jo_nbg0_printf(2, 15, "TOUCHEDBY 4:%i", touchedBy[3].hasTouched);
                                                                 
                // if (g_Game.numPlayers == ONE_PLAYER)
                    // jo_nbg0_printf(2, 21, "ONE_PLAYER");
                // else if (g_Game.numPlayers == TWO_PLAYER)
                    // jo_nbg0_printf(2, 21, "TWO_PLAYER");
                // else if (g_Game.numPlayers == THREE_PLAYER)
                    // jo_nbg0_printf(2, 21, "THREE_PLAYER");
                // else
                    // jo_nbg0_printf(2, 21, "FOUR_PLAYER");
                // jo_nbg0_printf(2, 22, "NUMTEAMS:%i", g_Team.numTeams);
                break;
                
            case GAME_STATE_GAMEPLAY:
                // if (g_Game.isPaused == true) {
                    // jo_nbg0_printf(2, 5, "PAUSECHOICE:%i", g_PauseChoice);
                // }
                jo_nbg0_printf(2, 5, "GAMEACTIVE:%i", g_Game.isActive);
                jo_nbg0_printf(2, 6, "BALLACTIVE:%i", g_Game.isBallActive);
                jo_nbg0_printf(2, 8, "CURPOS.DX:%i", g_Players[0].curPos.dx);
                jo_nbg0_printf(2, 9, "CURPOS.DY:%i", g_Players[0].curPos.dy);
                
                // jo_nbg0_printf(20, 10, "NUMPLAYERS:%i", g_Game.currentNumPlayers);
                // jo_nbg0_printf(20, 11, "ROUNDOVER:%i", g_Game.isRoundOver);
                // jo_nbg0_printf(20, 12, "WINNER:%i", g_Game.winner);
                // jo_nbg0_printf(20, 13, "DELAYTIMER:%i", g_Game.endDelayTimer);
                jo_nbg0_printf(20, 14, "NUMTEAMS:%i", g_Team.numTeams+1);
                
                // jo_nbg0_printf(22, 10, "POPPY.VEL.X:%i", JO_FIXED_TO_INT(pixel_poppy.vel.x));
                // jo_nbg0_printf(22, 11, "POPPY.VEL.Y:%i", JO_FIXED_TO_INT(pixel_poppy.vel.y));
                // jo_nbg0_printf(22, 12, "POPPY.VEL.Z:%i", pixel_poppy.vel.z);
                // jo_nbg0_printf(22, 13, "POPPY.POS.X:%i", JO_FIXED_TO_INT(pixel_poppy.pos.x));
                // jo_nbg0_printf(22, 14, "POPPY.POS.Y:%i", JO_FIXED_TO_INT(pixel_poppy.pos.y));
                // jo_nbg0_printf(22, 15, "BALL.COLLIDING:%i", pixel_poppy.isColliding);
                // jo_nbg0_printf(2,  16, "P1.COLLIDING:%i", g_Players[0]._sprite->isColliding);
                // jo_nbg0_printf(2,  17, "P1.LEFTSIDE:%i", g_Players[0].onLeftSide);
                // jo_nbg0_printf(22, 16, "P2.COLLIDING:%i", g_Players[1]._sprite->isColliding);
                // jo_nbg0_printf(22, 17, "P2.LEFTSIDE:%i", g_Players[1].onLeftSide);
                
                
                // jo_nbg0_printf(2,  16, "P1.COLLIDING:%i", g_Players[0]._sprite->isColliding);
                // jo_nbg0_printf(2,  17, "ITEM.COLLIDE:%i", g_item._sprite->isColliding);
                // jo_nbg0_printf(2,  18, "ITEM.ID:%i", g_item.id);
                // jo_nbg0_printf(2,  19, "ITEM.TIMER:%i", g_item.timer[0]);
                
                // jo_nbg0_printf(22, 16, "EXPLODE_BOMB:%i", g_Game.explodeBomb);
                // jo_nbg0_printf(22, 17, "SHROOM_ANGLE:%i", hslSprites.color[p_rangeShroom.lower].h);
                // jo_nbg0_printf(22, 18, "VISIBLE:%i", g_item._sprite->visible);
                // jo_nbg0_printf(22, 19, "TOTALLIVES:%i", (g_Players[0].totalLives * 2));
                // jo_nbg0_printf(22, 20, "NUMLIVES:%i", g_Players[0].numLives);
                         
                // if (g_Players[0].isPlaying && g_GameOptions.testCollision) {
                    // jo_nbg0_printf(2, 11, "ISCOLLIDING:%i", g_Players[0]._sprite->isColliding);
                // }
                // if (g_Players[1].isPlaying && g_GameOptions.testCollision) {
                    // jo_nbg0_printf(22, 11, "ISCOLLIDING:%i", g_Players[1]._sprite->isColliding);
                // } 
                                         
                // if (g_Players[0].isPlaying) {
                    // jo_nbg0_printf(2, 16, "POWER:%i, %d", JO_FIXED_TO_INT(g_Players[0].power), g_Players[0].power);
                // }
                // if (g_Players[1].isPlaying) {
                    // jo_nbg0_printf(20, 16, "POWER:%i, %d", JO_FIXED_TO_INT(g_Players[1].power), g_Players[1].power);
                // }  
                      
                if (g_Players[0].isPlaying) {
                    jo_nbg0_printf(2, 12, "P1 SUBSTATE:%i", g_Players[0].subState);
                }
                if (g_Players[1].isPlaying) {
                    jo_nbg0_printf(2, 13, "P2 SUBSTATE:%i", g_Players[1].subState);
                }
                if (g_Players[2].isPlaying) {
                    jo_nbg0_printf(2, 14, "P3 SUBSTATE:%i", g_Players[2].subState);
                }
                if (g_Players[3].isPlaying) {
                    jo_nbg0_printf(2, 15, "P4 SUBSTATE:%i", g_Players[3].subState);
                }
                
                if (g_Players[0].isPlaying) {
                    jo_nbg0_printf(2, 17, "P1 OBJECTSTATE:%i", g_Players[0].objectState);
                }
                if (g_Players[1].isPlaying) {
                    jo_nbg0_printf(2, 18, "P2 OBJECTSTATE:%i", g_Players[1].objectState);
                }
                if (g_Players[2].isPlaying) {
                    jo_nbg0_printf(2, 19, "P3 OBJECTSTATE:%i", g_Players[2].objectState);
                }
                if (g_Players[3].isPlaying) {
                    jo_nbg0_printf(2, 20, "P4 OBJECTSTATE:%i", g_Players[3].objectState);
                }
                // if (g_Game.numPlayers == ONE_PLAYER)
                    // jo_nbg0_printf(2, 21, "NUMPLAYERS:%i", 1);
                // else if (g_Game.numPlayers == TWO_PLAYER)
                    // jo_nbg0_printf(2, 21, "NUMPLAYERS:%i", 2);
                // else if (g_Game.numPlayers == THREE_PLAYER)
                    // jo_nbg0_printf(2, 21, "NUMPLAYERS:%i", 3);
                // else
                // jo_nbg0_printf(2, 21, "NUMPLAYERS:%i", g_Game.currentNumPlayers);
                // jo_nbg0_printf(2, 22, "NUMTEAMS:%i", g_Team.numTeams);
                // jo_nbg0_printf(2, 23, "PLAYERID:%i", g_Players[0].playerID);
                                
                // jo_nbg0_printf(20, 10, "TEAM 1 AVAILABLE:%i", g_Team.isAvailable[TEAM_1]);
                // jo_nbg0_printf(20, 11, "TEAM 2 AVAILABLE:%i", g_Team.isAvailable[TEAM_2]);
                // jo_nbg0_printf(20, 12, "TEAM 3 AVAILABLE:%i", g_Team.isAvailable[TEAM_3]);
                // jo_nbg0_printf(20, 13, "TEAM 4 AVAILABLE:%i", g_Team.isAvailable[TEAM_4]);   
                
                jo_nbg0_printf(20, 10, "TEAM 1 ACTIVE:%i", g_Team.objectState[TEAM_1]);
                jo_nbg0_printf(20, 11, "TEAM 2 ACTIVE:%i", g_Team.objectState[TEAM_2]);
                jo_nbg0_printf(20, 12, "TEAM 3 ACTIVE:%i", g_Team.objectState[TEAM_3]);
                jo_nbg0_printf(20, 13, "TEAM 4 ACTIVE:%i", g_Team.objectState[TEAM_4]);   
                
                // jo_nbg0_printf(20, 10, "GAMEISACTIVE:%i", g_Game.isActive);
                // jo_nbg0_printf(20, 11, "BEGINTIMER:%i", g_Game.BeginTimer);
                // jo_nbg0_printf(20, 12, "ROUNDS:%i", g_Game.countofRounds);
                // jo_nbg0_printf(20, 13, "STARTTIMER:%i", start_gameplay_timer);
                // jo_nbg0_printf(20, 14, "ISBALLACTIVE:%i", g_Game.isBallActive);
                // jo_nbg0_printf(20, 15, "ROUND_START:%i", round_start);
                
                // jo_nbg0_printf(2, 12, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[0]._sprite->pos.r));
                // jo_nbg0_printf(2, 13, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[1]._sprite->pos.r));
                // jo_nbg0_printf(2, 14, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[2]._sprite->pos.r));
                // jo_nbg0_printf(2, 15, "PLAYER1 RAD:%i", JO_FIXED_TO_INT(g_Players[3]._sprite->pos.r));
                
                // jo_nbg0_printf(2, 12, "TOUCHEDBY 1:%i", touchedBy[0].hasTouched);
                // jo_nbg0_printf(2, 13, "TOUCHEDBY 2:%i", touchedBy[1].hasTouched);
                // jo_nbg0_printf(2, 14, "TOUCHEDBY 3:%i", touchedBy[2].hasTouched);
                // jo_nbg0_printf(2, 15, "TOUCHEDBY 4:%i", touchedBy[3].hasTouched);
                                             
                // jo_nbg0_printf(20, 12, "TOUCHCOUNT 1:%i", touchedBy[0].touchCount);
                // jo_nbg0_printf(20, 13, "TOUCHCOUNT 2:%i", touchedBy[1].touchCount);
                // jo_nbg0_printf(20, 14, "TOUCHCOUNT 3:%i", touchedBy[2].touchCount);
                // jo_nbg0_printf(20, 15, "TOUCHCOUNT 4:%i", touchedBy[3].touchCount);
                                             
                
                break;
                
            case GAME_STATE_CREDITS: // transition
                jo_nbg0_printf(2, 5, "CREDITSTIMER:%i", g_CreditsTimer/60);
                break;
                
            case GAME_STATE_TRANSITION: // transition
                jo_nbg0_printf(2, 5, "TRANSITIONTIMER:%i", g_TransitionTimer/60);
                break;
                
            default:
                return;
        }
}