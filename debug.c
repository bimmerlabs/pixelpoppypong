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
            
        switch (g_Game.gameState) {
            case GAME_STATE_PPP_LOGO: // logo
                jo_nbg0_printf(2, 4, "LOGOTIMER: %i", g_LogoTimer/60);
                // jo_nbg0_printf(2, 5, "NBG0_RATE: %i", nbg0_rate);
                // jo_nbg0_printf(2, 6, "NBG1_RATE: %i", nbg1_rate);
                // jo_nbg0_printf(2, 7, "FADEDIRECTION: %i", fadeDirection);
                break;
                
            case GAME_STATE_TITLE_SCREEN: // title
                jo_nbg0_printf(2, 4, "TITLETIMER: %i", g_TitleTimer/60);
                break;
                
            case GAME_STATE_TITLE_MENU: // menu
                jo_nbg0_printf(2, 4, "GAMEMODE: %i", g_Game.gameMode);
                jo_nbg0_printf(2, 5, "GAMEDIFFICULTY: %i", g_Game.gameDifficulty);
                jo_nbg0_printf(2, 5, "TITLESCREENCHOICE: %i", g_TitleScreenChoice);
                break;
                
            case GAME_STATE_TEAM_SELECT: // menu
                jo_nbg0_printf(2, 4, "GAMEMODE: %i", g_Game.gameMode);
                jo_nbg0_printf(2, 5, "PLAYER 1 CHOICE: %i",  g_Players[0].teamSelectChoice);
                jo_nbg0_printf(2, 6, "PLAYER 2 CHOICE: %i",  g_Players[1].teamSelectChoice);
                jo_nbg0_printf(2, 7, "PLAYER 3 CHOICE: %i",  g_Players[2].teamSelectChoice);
                jo_nbg0_printf(2, 8, "PLAYER 4 CHOICE: %i",  g_Players[3].teamSelectChoice);
                jo_nbg0_printf(2, 9, "PLAYER 5 CHOICE: %i",  g_Players[4].teamSelectChoice);
                jo_nbg0_printf(2, 10, "PLAYER 6 CHOICE: %i",  g_Players[5].teamSelectChoice);
                jo_nbg0_printf(2, 10, "PLAYER 6 CHOICE: %i",  g_Players[5].isPlaying);
                break;
                
            case GAME_STATE_DEMO_LOOP: // demo
                jo_nbg0_printf(2, 4, "DEMOTIMER: %i", g_DemoTimer/60);
                // jo_nbg0_printf(2, 5, "NBG0_RATE: %i", nbg0_rate);
                // jo_nbg0_printf(2, 6, "NBG1_RATE: %i", nbg1_rate);
                // jo_nbg0_printf(2, 7, "FADEDIRECTION: %i", fadeDirection);
                break;
                
            case GAME_STATE_GAMEPLAY: // demo
                if (g_Game.isPaused == true) {
                    jo_nbg0_printf(2, 4, "PAUSECHOICE: %i", g_PauseChoice);
                }
                jo_nbg0_printf(2, 5, "CURPOS.DX: %i", g_Players[0].curPos.dx);
                jo_nbg0_printf(2, 6, "CURPOS.DY: %i", g_Players[0].curPos.dy);
                jo_nbg0_printf(2, 7, "POS.X: %i", g_Players[0]._sprite->pos.x);
                jo_nbg0_printf(2, 8, "POS.Y: %i", g_Players[0]._sprite->pos.y);
                jo_nbg0_printf(2, 9, "PLAYER 1 ACTIVE: %i",  g_Players[0].isPlaying);
                break;
                
            case GAME_STATE_TRANSITION: // transition
                jo_nbg0_printf(2, 4, "TRANSITIONTIMER: %i", g_TransitionTimer/60);
                // jo_nbg0_printf(2, 5, "NBG0_RATE: %i", nbg0_rate);
                // jo_nbg0_printf(2, 6, "FADEDIRECTION: %i", fadeDirection);
                break;
                
            default:
                return;
        }
    }
}