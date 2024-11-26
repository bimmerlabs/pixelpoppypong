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
    if (debug_display) {
        jo_nbg0_printf(2, 2, "GAME_STATE: %i", g_Game.gameState);
        jo_nbg0_printf(2, 3, "FRAME: %i", frame);
        jo_nbg0_printf(2, 4, "VOLUME: %i,%i", volume, g_StartGameFrames);
        jo_nbg0_printf(20, 2, "TRANSITION: %i", transition_complete);
        jo_nbg0_printf(20, 3, "FADE RATE: %i,%i", nbg0_rate, nbg1_rate);
        jo_nbg0_printf(20, 4, "PLAYING CD: %i", cd_is_playing);
        
        switch (g_Game.gameState) {
            case GAME_STATE_PPP_LOGO: // logo
                jo_nbg0_printf(2, 5, "LOGOTIMER: %i", g_LogoTimer/60);
                break;
                
            case GAME_STATE_TITLE_SCREEN: // title
                jo_nbg0_printf(2, 5, "TITLETIMER: %i", g_TitleTimer/60);
                
                break;
                
            case GAME_STATE_TITLE_MENU: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE: %i", g_Game.gameMode);
                jo_nbg0_printf(2, 6, "GAMEDIFFICULTY: %i", g_Game.gameDifficulty);
                jo_nbg0_printf(2, 7, "TITLESCREENCHOICE: %i", g_TitleScreenChoice);
                break;
                
            case GAME_STATE_TEAM_SELECT: // menu
                jo_nbg0_printf(2, 5, "GAMEMODE: %i", g_Game.gameMode);
                jo_nbg0_printf(2, 7, "STARTSELECTION: %i",  g_Players[0].character.startSelection);
                jo_nbg0_printf(2, 8, "TEAMSELECTED: %i",  g_Players[0].team.selected);
                jo_nbg0_printf(2, 9, "PRESSEDSTART: %i",  g_TeamSelectPressedStart);
                jo_nbg0_printf(2, 10, "TRANSITION_COMPLETE: %i",  transition_complete);
                jo_nbg0_printf(2, 11, "STARTGAMEFRAMES: %i",  g_StartGameFrames);
                break;
                
            case GAME_STATE_DEMO_LOOP: // demo
                jo_nbg0_printf(2, 5, "DEMOTIMER: %i", g_DemoTimer/60);
                break;
                
            case GAME_STATE_GAMEPLAY: // demo
                if (g_Game.isPaused == true) {
                    jo_nbg0_printf(2, 5, "PAUSECHOICE: %i", g_PauseChoice);
                }
                jo_nbg0_printf(2, 6, "CURPOS.DX: %i", g_Players[0].curPos.dx);
                jo_nbg0_printf(2, 7, "CURPOS.DY: %i", g_Players[0].curPos.dy);
                jo_nbg0_printf(2, 8, "POS.X: %i", g_Players[0]._sprite->pos.x);
                jo_nbg0_printf(2, 9, "POS.Y: %i", g_Players[0]._sprite->pos.y);
                jo_nbg0_printf(2, 10, "PLAYER 1 ACTIVE: %i",  g_Players[0].isPlaying);
                break;
                
            case GAME_STATE_TRANSITION: // transition
                jo_nbg0_printf(2, 5, "TRANSITIONTIMER: %i", g_TransitionTimer/60);
                break;
                
            default:
                return;
        }
    }
}