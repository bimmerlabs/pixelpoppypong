#include <jo/jo.h>
#include "main.h"
#include "screen_transition.h"
#include "ppp_logo.h"
#include "title_screen.h"
#include "team_select.h"
#include "gameplay.h"
#include "pause.h"
#include "objects/player.h"

extern PLAYER g_Players[MAX_PLAYERS];

void debux_text(void)
{    
    if (game_options.debug_display) {
        jo_nbg0_printf(2, 2, "GAME_STATE:%i", g_Game.gameState);
        jo_nbg0_printf(2, 3, "FRAME:%i", frame);
        jo_nbg0_printf(2, 4, "VOLUME:%i,%i", volume, g_StartGameFrames);
        jo_nbg0_printf(20, 2, "GAME MODE:%i", g_Game.gameMode);
        jo_nbg0_printf(20, 3, "PLAYERS:%i", g_Game.numPlayers);
        jo_nbg0_printf(20, 4, "DIFFICULTY:%i", g_Game.gameDifficulty);
        
        // jo_nbg0_printf(20, 2, "VDP1 MEMORY:%d%%", jo_sprite_usage_percent());
        // jo_nbg0_printf(20, 3, "RAM USAGE:%d%%", jo_memory_usage_percent());
        
        // transition debug
        // if (game_options.mosaic_display) {
        // jo_nbg0_printf(20, 2, "MOSAIC_X/Y:%i,%i", mosaic_x, mosaic_y);
        // }
        // jo_nbg0_printf(20, 3, "PLAYING CD:%i", cd_is_playing);
        // jo_nbg0_printf(20, 4, "TRANS_IN/OUT:%i, %i", transition_in, transition_out);
        // jo_nbg0_printf(20, 5, "FADE_IN/OUT:%i, %i", fade_in, fade_out);
        // jo_nbg0_printf(20, 6, "MOSAIC_IN/OUT:%i, %i", mosaic_in, mosaic_out);
        
        switch (g_Game.gameState) {
            case GAME_STATE_PPP_LOGO: // logo
                jo_nbg0_printf(2, 5, "LOGOTIMER:%i", g_LogoTimer/60);
                break;
                
            case GAME_STATE_TITLE_SCREEN: // title
                jo_nbg0_printf(2, 5, "TITLETIMER:%i", g_TitleTimer/60);
                jo_nbg0_printf(2, 7, "LOGO1_POS:%i", logo1_pos);
                jo_nbg0_printf(20, 7, "LOGO1:%i", logo1.pos.y);
                jo_nbg0_printf(2, 8, "LOGO2_POS:%i", logo2_pos);
                jo_nbg0_printf(20, 8, "LOGO2:%i", logo2.pos.y);
                jo_nbg0_printf(20, 10, "VELOCITY:%i", logo_velocity);
                jo_nbg0_printf(20, 11, "LOGO_FALLING:%i", logo_falling);
                jo_nbg0_printf(20, 12, "LOGO_BOUNCE:%i", logo_bounce);
                break;
                
            case GAME_STATE_TITLE_MENU: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE:%i", g_Game.gameMode);
                jo_nbg0_printf(2, 7, "LOGO1_POS:%i", logo1_pos);
                jo_nbg0_printf(20, 7, "LOGO1:%i", logo1.pos.y);
                jo_nbg0_printf(2, 8, "LOGO2_POS:%i", logo2_pos);
                jo_nbg0_printf(20, 8, "LOGO2:%i", logo2.pos.y);
                jo_nbg0_printf(20, 9, "VELOCITY:%i", logo_velocity);
                jo_nbg0_printf(2, 10, "GAMEDIFFICULTY:%i", g_Game.gameDifficulty);
                jo_nbg0_printf(2, 10, "TITLE CHOICE:%i", g_TitleScreenChoice);
                jo_nbg0_printf(20, 11, "LOGO_FALLING:%i", logo_falling);
                jo_nbg0_printf(20, 12, "LOGO_BOUNCE:%i", logo_bounce);
                break;
                
            case GAME_STATE_TITLE_OPTIONS: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE:%i", g_Game.gameMode);
                jo_nbg0_printf(2, 6, "OPTION CHOICE:%i", g_OptionScreenChoice);
                break;
                
            case GAME_STATE_TEAM_SELECT: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE:%i", g_Game.gameMode);
                
                jo_nbg0_printf(2, 7, "TOTAL_CHARACTERS:%i",  TOTAL_CHARACTERS);
                jo_nbg0_printf(2, 8, "P1 CHAR CHOICE:%i", g_Players[0].character.choice);
                
                // jo_nbg0_printf(2, 7, "P1 STARTSELECT:%i",  g_Players[0].startSelection);
                // jo_nbg0_printf(2, 8, "P1 CHAR SELECT:%i", g_Players[0].character.selected);
                // jo_nbg0_printf(2, 9, "P1 TEAM SELECT:%i", g_Players[0].team.selected);
                // jo_nbg0_printf(2, 10, "P1 TEAM CHOICE:%i", g_Players[0].team.choice);
                // jo_nbg0_printf(2, 11, "P1 READY:%i", g_Players[0].isReady);
                
                // jo_nbg0_printf(2, 13, "P2 STARTSELECT:%i",  g_Players[1].startSelection);
                // jo_nbg0_printf(2, 14, "P2 CHAR SELECT:%i", g_Players[1].character.selected);
                // jo_nbg0_printf(2, 15, "P2 TEAM SELECT:%i", g_Players[1].team.selected);
                // jo_nbg0_printf(2, 16, "P2 TEAM CHOICE:%i", g_Players[1].team.choice);
                // jo_nbg0_printf(2, 17, "P2 READY:%i", g_Players[1].isReady);
                
                // jo_nbg0_printf(20, 7, "P3 STARTSELECT:%i",  g_Players[2].startSelection);
                // jo_nbg0_printf(20, 8, "P3 CHAR SELECT:%i", g_Players[2].character.selected);
                // jo_nbg0_printf(20, 9, "P3 TEAM SELECT:%i", g_Players[2].team.selected);
                // jo_nbg0_printf(20, 10, "P3 TEAM CHOICE:%i", g_Players[2].team.choice);
                // jo_nbg0_printf(20, 11, "P3 READY:%i", g_Players[2].isReady);
                
                // jo_nbg0_printf(20, 13, "P4 STARTSELECT:%i",  g_Players[3].startSelection);
                // jo_nbg0_printf(20, 14, "P4 CHAR SELECT:%i", g_Players[3].character.selected);
                // jo_nbg0_printf(20, 15, "P4 TEAM SELECT:%i", g_Players[3].team.selected);
                // jo_nbg0_printf(20, 16, "P4 TEAM CHOICE:%i", g_Players[3].team.choice);
                // jo_nbg0_printf(20, 17, "P4 READY:%i", g_Players[3].isReady);
                
                
                jo_nbg0_printf(2, 18, "NUM.TEAMS %i:", numTeams);
                jo_nbg0_printf(2, 19, "ALL READY:%i", all_players_ready);
                jo_nbg0_printf(2, 20, "PRESSEDSTART:%i",  g_TeamSelectPressedStart);
                
                break;
                
            case GAME_STATE_DEMO_LOOP: // demo
                jo_nbg0_printf(2, 5, "DEMOTIMER:%i", g_DemoTimer/60);
                break;
                
            case GAME_STATE_GAMEPLAY: // demo
                if (g_Game.isPaused == true) {
                    jo_nbg0_printf(2, 5, "PAUSECHOICE:%i", g_PauseChoice);
                }
                jo_nbg0_printf(2, 6, "CURPOS.DX:%i", g_Players[0].curPos.dx);
                jo_nbg0_printf(2, 7, "CURPOS.DY:%i", g_Players[0].curPos.dy);
                jo_nbg0_printf(2, 8, "POS.X:%i", g_Players[0]._sprite->pos.x);
                jo_nbg0_printf(2, 9, "POS.Y:%i", g_Players[0]._sprite->pos.y);
                jo_nbg0_printf(2, 10, "PLAYER 1 ACTIVE:%i",  g_Players[0].isPlaying);
                break;
                
            case GAME_STATE_TRANSITION: // transition
                jo_nbg0_printf(2, 5, "TRANSITIONTIMER:%i", g_TransitionTimer/60);
                break;
                
            default:
                return;
        }
    }
}