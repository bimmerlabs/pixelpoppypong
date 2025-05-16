#include <jo/jo.h>
#include "items.h"
#include "../main.h"
#include "../game/gameplay.h"
#include "../palettefx/sprite_colors.h"

static bool animate_bomb = false;
static bool explode_bomb = false;
static bool bomb_heating = false;
static Uint16 bomb_timer = 0;
static float item_scale = 0.1;
static float item_velocity = 0.0;

void animateBombColor(bool *_do_update) {
    if (!animate_bomb) {
        return;
    }
    if (JO_MOD_POW2(g_Game.frame, 4) == 0) {
        if (bomb_heating) {
            hsl_incSprites[HSL_BOMB].h = 180;
            bomb_heating = false;
        } 
        else {
            hsl_incSprites[HSL_BOMB].h = -180;
            bomb_heating = true;
        }
        *_do_update = true;
    }
}

void setItemPositions(void) {
    if (g_GameOptions.debug_mode || !g_GameOptions.enableItems) {
        // g_item.id = GAME_ITEM_BOMB;
        // g_item.id = GAME_ITEM_FISH;
        // g_item.id = GAME_ITEM_SHROOM;
        // g_item.id = GAME_ITEM_GARF;
        // g_item.id = GAME_ITEM_CRAIG;
        g_item.id = GAME_ITEM_MAX;
    }
    else {
        int item = jo_random(1000000);
        switch(g_Game.gameDifficulty)
        {
            case GAME_DIFFICULTY_EASY:
                if (item >= 0 && item < 450000) { 
                    g_item.id = GAME_ITEM_FISH;
                }
                else if (item >= 450000 && item < 500000) { 
                    g_item.id = GAME_ITEM_BOMB;
                }
                else if (item >= 500000 && item < 700000) { 
                    g_item.id = GAME_ITEM_SHROOM;
                }
                else if (item >= 700000 && item < 850000) { 
                    g_item.id = GAME_ITEM_GARF;
                }
                else if (item >= 850000 && item < 1000000) { 
                    g_item.id = GAME_ITEM_CRAIG;
                }
                else {
                    g_item.id = GAME_ITEM_MAX;
                }
                break;
            case GAME_DIFFICULTY_MEDIUM:
                if (item >= 0 && item < 200000) { 
                    g_item.id = GAME_ITEM_BOMB;
                }
                else if (item >= 200000 && item < 400000) { 
                    g_item.id = GAME_ITEM_FISH;
                }
                else if (item >= 400000 && item < 600000) { 
                    g_item.id = GAME_ITEM_SHROOM;
                }
                else if (item >= 600000 && item < 800000) { 
                    g_item.id = GAME_ITEM_GARF;
                }
                else if (item >= 800000 && item < 1000000) { 
                    g_item.id = GAME_ITEM_CRAIG;
                }
                else {
                    g_item.id = GAME_ITEM_MAX;
                }
                break;
            case GAME_DIFFICULTY_HARD:
                if (item >= 0 && item < 500000) { 
                    g_item.id = GAME_ITEM_BOMB;
                }
                else if (item >= 500000 && item < 650000) { 
                    g_item.id = GAME_ITEM_FISH;
                }
                else if (item >= 650000 && item < 800000) { 
                    g_item.id = GAME_ITEM_SHROOM;
                }
                else if (item >= 800000 && item < 900000) { 
                    g_item.id = GAME_ITEM_GARF;
                }
                else if (item >= 900000 && item < 1000000) { 
                    g_item.id = GAME_ITEM_CRAIG;
                }
                else {
                    g_item.id = GAME_ITEM_MAX;
                }
                break;
            default:
                g_item.id = GAME_ITEM_MAX;
                break;
        }
    }
    item_scale = 0.0;
    item_velocity = 0.1;
    switch (g_item.id) {
        case GAME_ITEM_BOMB:
            reset_bomb_color();
            animate_bomb = false;
            bomb_heating = false;
            explode_bomb = false;
            bomb_timer = BOMB_TIMER;
            g_item._sprite = &bomb_item;
            g_item._sprite->visible = true;
            sprite_frame_reset(g_item._sprite);
            break;
        case GAME_ITEM_FISH:
            g_item._sprite = &fishtank_item;
            g_item._sprite->visible = true;
            hsl_incSprites[HSL_FISH].h -= FISH_HUE_INCREMENT;
            do_update_fish = true;
            break;
        case GAME_ITEM_SHROOM:
            g_item._sprite = &shroom_item;
            g_item._sprite->visible = true;
            break;
        case GAME_ITEM_GARF:
            g_item._sprite = &garfield_item;
            g_item._sprite->visible = true;
            break;
        case GAME_ITEM_CRAIG:
            g_item._sprite = &craig_item;
            g_item._sprite->visible = true;
            break;
        default:
            g_item._sprite = &fishtank_item;
            g_item._sprite->visible = false;
            break;
    }
    set_item_position(g_item._sprite);
    set_spr_scale(g_item._sprite, item_scale, item_scale);
    g_item._sprite->isColliding = false;
    g_item.isActive = false;
}

void drawGameItems(void) {
    switch (g_item.id) {
        case GAME_ITEM_BOMB:
            if (item_scale < 2.0) {
                item_scale += item_velocity;
                set_spr_scale(&bomb_item, item_scale, item_scale);
                if (item_scale >= 2.0) {
                    g_item.isActive = true;
                }
            }
            if (explode_bomb) {
                if (g_Transition.explosion_flash) {
                    g_Transition.explosion_flash = explosionEffect();
                }
                explode_bomb = explode_animation(&bomb_item);
                if (explode_bomb == false) {
                    bomb_item.visible = false;
                    g_item.isActive = false;
                }
            }
            else {
                looped_animation_pow(&bomb_item, 4);
            }
            break;
        case GAME_ITEM_FISH:
            if (item_scale < 2.0) {
                item_scale += item_velocity;
                set_spr_scale(&fishtank_item, item_scale, item_scale);
                if (item_scale >= 2.0) {
                    g_item.isActive = true;
                }
            }
            looped_animation_pow(&fishtank_item, 8);
            break;
        case GAME_ITEM_SHROOM:
            if (item_scale < 2.0) {
                item_scale += item_velocity;
                set_spr_scale(&shroom_item, item_scale, item_scale);
                if (item_scale >= 2.0) {
                    g_item.isActive = true;
                }
            }
            looped_animation_pow(&shroom_item, 4);
            hsl_incSprites[HSL_SHROOM].h += 2; // TODO: replace with one just for the mushroom?
            do_update_shroom = true;
            break;
        case GAME_ITEM_GARF:
            if (item_scale < 1.0) {
                item_scale += item_velocity;
                set_spr_scale(&garfield_item, item_scale, item_scale);
                if (item_scale >= 1.0) {
                    g_item.isActive = true;
                }
            }
            break;
        case GAME_ITEM_CRAIG:
            if (item_scale < 1.6) {
                item_scale += item_velocity;
                set_spr_scale(&craig_item, item_scale, item_scale);
                if (item_scale >= 1.6) {
                    g_item.isActive = true;
                }
            }
            break;
        default:
            g_item._sprite->visible = false;
            g_item.isActive = false;
            break;
    }
    if (g_item._sprite->visible) {
        my_sprite_draw(g_item._sprite);
    }
}

void handlePlayerItemCollision(PPLAYER player) {
    g_item.isActive = false;
    switch (g_item.id) {
        case GAME_ITEM_BOMB:
            explode_bomb = true;
            bomb_heating = false;
            animate_bomb = false;
            bomb_timer = 0;
            g_Transition.explosion_flash = true;
            if (!player->isExploded) {
                player->isExploded = explodePLayer(player);
            }
            g_GameOptions.bombTouchCounter++;
            break;
        case GAME_ITEM_FISH:
            pcm_play(g_Assets.bloopPcm8, PCM_PROTECTED, 7);
            // int missing = player->totalLives - player->numLives;
            if ((player->totalLives - player->numLives) > 1) {
                player->numLives += (player->totalLives - player->numLives) / 2;
            }
            else if (player->numLives < player->totalLives * 2) {
                player->numLives += 1;
            }
            if (player->numLives > player->totalLives * 2) {
                player->numLives = player->totalLives * 2;
            }
            fishtank_item.visible = false;
            player->score.points += 10000;
            g_GameOptions.fishTouchCounter++;
            break;
        case GAME_ITEM_SHROOM:
            shroom_item.visible = false;
            Uint16 shroom_angle = hslSprites.color[p_rangeShroom.lower].h;
            g_item.timer[player->playerID] = SHROOM_TIMER;
            // affect the player depending on the color of the shroom
            if ((shroom_angle > 0 && shroom_angle <= 90) || (shroom_angle > 270 && shroom_angle <= 360)) {
                pcm_play(g_Assets.growPcm8, PCM_PROTECTED, 7);
                player->_sprite->pos.r = PLAYER_RADIUS_LARGE;
                player->isBig = true;
                player->isSmall = false;
                g_GameOptions.redShroomTouchCounter++;
            }
            if (shroom_angle > 90 && shroom_angle <= 270) {
                pcm_play(g_Assets.shrinkPcm8, PCM_PROTECTED, 7);
                player->_sprite->pos.r = PLAYER_RADIUS_SMALL;
                player->isBig = false;
                player->isSmall = true;
                g_GameOptions.blueShroomTouchCounter++;
            }
            player->score.points += 50000;
            break;
        case GAME_ITEM_GARF:
            garfield_item.visible = false;
            // "i hate mondays" etc
            if (touchedBy[player->playerID].touchCount == 0) {
                touchedBy[player->playerID].touchCount = 20;
            }
            else {
                touchedBy[player->playerID].touchCount *= 2;
                if (touchedBy[player->playerID].touchCount > 99) {
                    touchedBy[player->playerID].touchCount = 99;
                }
                else {
                    player->score.points += 250000;
                }
            }
            g_GameOptions.garfTouchCounter++;
            if (g_GameOptions.craigTouchCounter == 200) {
                pcm_play(g_Assets.stadlerPcm8, PCM_PROTECTED, 7);
            }
            else {
                pcm_play(g_Assets.chain5Pcm8, PCM_PROTECTED, 7);
            }
            break;
        case GAME_ITEM_CRAIG:
            craig_item.visible = false;
            // "nice shot"
            player->score.points += 100000;
            g_GameOptions.craigTouchCounter++;
            if (g_GameOptions.craigTouchCounter == 100) {
                pcm_play(g_Assets.chain5Pcm8, PCM_PROTECTED, 7);
            }
            else {
                pcm_play(g_Assets.stadlerPcm8, PCM_PROTECTED, 7);
            }
            break;
        default:
            g_item._sprite->visible = false;
            break;
    }
}

void bombTimer(void) {
    if (bomb_timer < BOMB_FLASH_TIMER && !animate_bomb) {
        animate_bomb = true;
    }
    if (bomb_timer == 0 && !explode_bomb) {
        explode_bomb = true;
        g_Transition.explosion_flash = true;
        animate_bomb = false;
    }
    else {
        bomb_timer--;
    }
}