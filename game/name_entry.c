#include <jo/jo.h>
#include "highscores.h"
#include "name_entry.h"
#include "../main.h"
#include "../core/assets.h"
#include "../core/state.h"
#include "../core/backup.h"
#include "../core/screen_transition.h"
#include "../palettefx/nbg1.h"
#include "../palettefx/lighting.h"
#include "../palettefx/sprite_colors.h"

const char letters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!.< ";

static NAME_ENTRY nameEntry = {0};

void initNameEntryStruct(void) {
    nameEntry.timer = NAME_ENTRY_TIMER;
    nameEntry.selection = FIRST_INITIAL;
    nameEntry.rotateLeft = false;
    nameEntry.rotateRight = false;
    nameEntry.initials[0] = 0;  // 3 letters + null terminator
    nameEntry.initials[1] = 0;
    nameEntry.initials[2] = 0;
    nameEntry.initials[3] = 0;
    nameEntry.char_id[0] = 10; // letters[] index
    nameEntry.char_id[1] = 39;
    nameEntry.char_id[2] = 39;
    nameEntry.xRadius = NAME_X_RADIUS;
    nameEntry.yRadius = NAME_Y_RADIUS;
    nameEntry.yPos = NAME_Y_POS;
    nameEntry.angle = 360;
    nameEntry.gapAngle = 0;
    nameEntry.pressedLeft = false;
    nameEntry.pressedRight = false;
    nameEntry.isAngleSnapped = true;
    nameEntry.finished = false;
    nameEntry.end = false;
}

void nameEntryInit(void)
{
    loadNameEntryAssets();
    initNameEntryColors();
    initNameEntryStruct();
     
    set_spr_scale(&pixel_poppy, 2.0, 2.0);
    pixel_poppy.rot.z = 0;
    set_spr_position(&pixel_poppy, 0, 0, 100);
    // set to "smile"
    pixel_poppy.spr_id = pixel_poppy.anim1.asset[1];
        
    convertNumberToDigits(nameEntry.timer);
    font.spr_id = font.anim1.asset[tens];
    font.spr_id = font.anim1.asset[ones];
    
    g_Game.lastState = GAME_STATE_NAME_ENTRY;
    
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN); // use the default here    
    g_Transition.mosaic_in = true;
    g_Transition.music_in = true;
    g_Transition.fade_in = true;
    g_Transition.all_in = true;
    playCDTrack(NAME_ENTRY_TRACK, true);
}

void nameEntryInput(void)
{
    if (nameEntry.end) {
        return;
    }
    PPLAYER player = &g_Players[g_Game.winner];
    if(jo_is_input_key_pressed(player->input->id, JO_KEY_UP))
    {
        if (nameEntry.yRadius < FIXED_0) {
            nameEntry.xRadius -= FIXED_HALF;
            nameEntry.yRadius += FIXED_2;
        }
        else if (nameEntry.yRadius < FIXED_100) {
            nameEntry.xRadius += FIXED_HALF;
            nameEntry.yRadius += FIXED_2;
        }
    }
    if(jo_is_input_key_pressed(player->input->id, JO_KEY_DOWN))
    {
        if (nameEntry.yRadius > FIXED_0) {
            nameEntry.xRadius -= FIXED_HALF;
            nameEntry.yRadius -= FIXED_2;
        }
        else if (nameEntry.yRadius > -FIXED_100) {
            nameEntry.xRadius += FIXED_HALF;
            nameEntry.yRadius -= FIXED_2;
        }
    }
    if (!nameEntry.isAngleSnapped) {
         return;
    }
    if(jo_is_input_key_pressed(player->input->id, JO_KEY_LEFT) && !nameEntry.finished)
    {
        nameEntry.pressedLeft = true;
        nameEntry.pressedRight = false;
        nameEntry.isAngleSnapped = false;
        nameEntry.angle += 1;
        if (nameEntry.angle > 360)
            nameEntry.angle -= 360;
    }
    else if(jo_is_input_key_pressed(player->input->id, JO_KEY_RIGHT) && !nameEntry.finished)
    {
        nameEntry.pressedLeft = false;
        nameEntry.pressedRight = true;
        nameEntry.isAngleSnapped = false;
        nameEntry.angle -= 1;
        if (nameEntry.angle < 0)
            nameEntry.angle += 360;
    }
    else if(jo_is_input_key_down(player->input->id, JO_KEY_A) || jo_is_input_key_down(player->input->id, JO_KEY_C))
    {
        if (nameEntry.finished && nameEntry.angle == 12) {
            pcm_play(g_Assets.name_brkPcm8, PCM_VOLATILE, 6);
            nameEntry.end = true;
            return;
        }
        if (!nameEntry.finished && nameEntry.angle != 12) {
            pcm_play(g_Assets.name_ketPcm8, PCM_PROTECTED, 7);
            nameEntry.selection++;
        }
        if (nameEntry.selection == MAX_INITIAL) {
            pcm_play(g_Assets.name_brkPcm8, PCM_VOLATILE, 6);
            nameEntry.finished = true;
            nameEntry.isAngleSnapped = false;
        }
    }
    else if(jo_is_input_key_down(player->input->id, JO_KEY_B))
    {
        pcm_play(g_Assets.name_canPcm8, PCM_PROTECTED, 7);
        if (nameEntry.selection == MAX_INITIAL) {
            nameEntry.angle = 360;
            nameEntry.selection--;
            nameEntry.char_id[nameEntry.selection] = 39;
            initNameEntryColors();
            nameEntry.finished = false;
            nameEntry.isAngleSnapped = true;
            nameEntry.rotateRight = false;
            nameEntry.rotateLeft = false;
            pixel_poppy.spr_id = pixel_poppy.anim1.asset[2];
        }
        else if (nameEntry.selection > FIRST_INITIAL) {
            nameEntry.selection--;
        }
    }
    if (nameEntry.isAngleSnapped == false) {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[0];
    }
    else {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[1];
    }
}


void nameEntryUpdate(void)
{
    if(g_Game.gameState != GAME_STATE_NAME_ENTRY)
    {
        return;
    }
    if (nameEntry.timer > 0 && g_Game.frame % 60 == 0) { // modulus
        nameEntry.timer--;
    }
    
    jo_nbg0_printf(17, 2, "NAME ENTRY");
    
    if (nameEntry.timer > 0 && !nameEntry.end) {
        // make an inline sub for this
        convertNumberToDigits(nameEntry.timer);
        set_spr_position(&font, 0, -170, 50);
        set_spr_scale(&font, 1.1, 1.1);
        font.spr_id = font.anim1.asset[tens];
        font.zmode = _ZmRC;
        my_sprite_draw(&font);
        font.spr_id = font.anim1.asset[ones];
        font.zmode = _ZmLC;
        my_sprite_draw(&font);
    }
    
    // make an inline sub for this
    if (nameEntry.char_id[FIRST_INITIAL] != 39) {
        set_spr_position(&font, -24, nameEntry.yPos, 50);
        set_spr_scale(&font, 2, 2);
        font.spr_id = font.anim1.asset[nameEntry.char_id[FIRST_INITIAL]];
        font.zmode = _ZmRC;
        my_sprite_draw(&font);
    }
    if (nameEntry.selection >= 1 && nameEntry.char_id[SECOND_INITIAL] != 39) {
        set_spr_position(&font, 0, nameEntry.yPos, 50);
        set_spr_scale(&font, 2, 2);
        font.spr_id = font.anim1.asset[nameEntry.char_id[SECOND_INITIAL]];
        font.zmode = _ZmCC;
        my_sprite_draw(&font);
    }
    if (nameEntry.selection >= 2 && nameEntry.char_id[THIRD_INITIAL] != 39) {
        set_spr_position(&font, 24, nameEntry.yPos, 50);
        set_spr_scale(&font, 2, 2);
        font.spr_id = font.anim1.asset[nameEntry.char_id[THIRD_INITIAL]];
        font.zmode = _ZmLC;
        my_sprite_draw(&font);
    }
    if (nameEntry.end && nameEntry.xRadius > END_RADIUS) {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[3];
        nameEntry.xRadius -= toFIXED(6);
        nameEntry.yRadius -= toFIXED(4);
        nameEntry.angle += 3;
        if (nameEntry.angle > 360)
            nameEntry.angle -= 360;
        if (nameEntry.yPos > 0)
            nameEntry.yPos -= 2;
    }
    
    font.zmode = _ZmCC;
    nameEntryDraw();
    
    
    nameEntry.initials[0] = letters[nameEntry.char_id[FIRST_INITIAL]];
    nameEntry.initials[1] = letters[nameEntry.char_id[SECOND_INITIAL]];
    nameEntry.initials[2] = letters[nameEntry.char_id[THIRD_INITIAL]];
    nameEntry.initials[3] = '\0';
    
    if(nameEntry.timer == 0 || nameEntry.xRadius <= END_RADIUS)
    {
        addHighScore(g_Players[g_Game.winner].score.points, nameEntry.initials);
        save_game_backup();
        g_Game.lastState = GAME_STATE_NAME_ENTRY;
        transitionState(GAME_STATE_HIGHSCORES);
        return;
    }

}

void nameEntryDraw(void)
{
    if (!nameEntry.finished) {
        nameEntry.angle = snap_to_nearest_12(nameEntry.angle);
    }
    if (nameEntry.finished) {
        nameEntry.angle = snap_to_end(nameEntry.angle);
    }
    for (nameEntry.gapAngle = 0; nameEntry.gapAngle <= 360; nameEntry.gapAngle += GAP_ANGLE) {
            nameEntryPositionUpdate((nameEntry.angle+ 6 + nameEntry.gapAngle), (nameEntry.gapAngle + 102));
    }
    do_update_font2All = calculate_sprites_color(&p_rangeFont2All);
    
    set_spr_scale(&pixel_poppy, 6.0, 6.0);
    int poppyAngle = nameEntry.angle;
    if (poppyAngle > 360)
        poppyAngle -= 360;
    pixel_poppy.rot.z = -1*poppyAngle;
    set_spr_position(&pixel_poppy, 0, 0, 100);
    my_sprite_draw(&pixel_poppy);
    
    #if ENABLE_DEBUG_MODE == 1
    if (g_GameOptions.debug_display) {
        jo_nbg0_printf(2, 24, "RAD:X=%3d,Y=%3d", JO_FIXED_TO_INT(nameEntry.xRadius), JO_FIXED_TO_INT(nameEntry.yRadius));
        jo_nbg0_printf(2, 25, "POS:X=%3d,Y=%3d", JO_FIXED_TO_INT(light.x), JO_FIXED_TO_INT(light.y));
        jo_nbg0_printf(2, 26, "ANGLE:%3d", nameEntry.angle);
        jo_nbg0_printf(2, 27, "ENDNAMEENTRY:%i", nameEntry.end);
        jo_nbg0_printf(20, 24, "ISANGLESNAPPED:%i", nameEntry.isAngleSnapped);
        jo_nbg0_printf(20, 25, "ISNAMEENTRYFINISHED:%i", nameEntry.finished);
    }
    #endif
}

void nameEntryPositionUpdate(int angle, int sprAngle) {
    if (angle > 360)
        angle -= 360;
    
    if (sprAngle > 360)
        sprAngle -= 360;
    int spr_id = ((sprAngle / 12) % 30) + 10; // because letter A starts at 10
        
    int scaleAngle = angle + 90;
    if (scaleAngle > 360)
        scaleAngle -= 360;
    
    light.x = jo_fixed_mult(jo_cos(angle), nameEntry.xRadius);
    light.y = jo_fixed_mult(my_fixed_sine(angle), nameEntry.yRadius);
    font.pos.x = 2*(light.x);
    font.pos.y = -2*(light.y);
    
    if (angle > 262 && angle < 278) {
        if (!nameEntry.finished) {
            nameEntry.char_id[nameEntry.selection] = spr_id;
        }
        font.scl.x = toFIXED(2.7);
    }
    else {
        font.scl.x = FIXED_1 + (FIXED_127 + jo_fixed_mult(jo_cos(scaleAngle), NAME_X_RADIUS))/200;
    }
    font.scl.y = font.scl.x;
    font.rot.z = 0;
    
    
    int zAngle = angle + 91;
    int zPos = 0;
    if (zAngle > 360)
        zAngle -= 360;
    if (zAngle > 258 && zAngle < 282) {
        zPos = 90;  // Keep zPos fixed at 90 within the hysteresis range
    }
    else {
        zPos = 180 - ABS(zAngle - 180);
    }
    font.pos.z = toFIXED(zPos+50);
    
    int colorid = spr_id + 54; // + 64 degrees -10 (start of "A")
    if (!nameEntry.finished) {
        hslSprites.color[colorid].s = 255 - zPos;
        hslSprites.color[colorid].l = 255 - zPos;
    }
    else {
        jo_getdate(&g_Game.now);
        int hue = g_Game.now.minute*6;
        hslSprites.color[colorid].h = hue;
        hslSprites.color[colorid].s = 240;
        hslSprites.color[colorid].l = 240;
    }
    
    font.spr_id = font.anim1.asset[spr_id];
    my_sprite_draw(&font);
}

int snap_to_nearest_12(int angle) {
    // If already snapped, do nothing
    if (nameEntry.isAngleSnapped) {
        return angle;
    }

    // Adjust angle only when a button is pressed
    if (nameEntry.pressedRight) {
        angle--;  // Move counterclockwise
        if (angle < 0)
            angle += 360;
    } else if (nameEntry.pressedLeft) {
        angle++;  // Move clockwise
        if (angle > 360)
            angle -= 360;
    }

    // If angle is now a multiple of 12, stop adjusting
    if (angle % 12 == 0) {
        pcm_play(g_Assets.name_curPcm8, PCM_VOLATILE, 7);
        nameEntry.isAngleSnapped = true;  // Mark snapping complete
    }

    return angle;
}

int snap_to_end(int angle) {
    // If already snapped, do nothing
    if (nameEntry.isAngleSnapped) {
        return angle;
    }
    if (angle == 0)
        angle = 360;
    if (!nameEntry.rotateLeft && !nameEntry.rotateRight) {
        if (angle - 180 < 0) {
            nameEntry.rotateRight = true;
        }
        else {
            nameEntry.rotateLeft = true;
        }
    }
    // Adjust angle only when a button is pressed
    if (nameEntry.rotateRight) { // need to compare to original angle value and set a boolean?
        angle -= 6;  // Move counterclockwise
        pcm_play(g_Assets.name_curPcm8, PCM_VOLATILE, 7);
        if (angle < 0)
            angle += 360;
    }
    if (nameEntry.rotateLeft) {
        angle += 6;  // Move clockwise
        pcm_play(g_Assets.name_curPcm8, PCM_VOLATILE, 7);
        if (angle > 360)
            angle -= 360;
    }
    // If angle is now a multiple of 12, stop adjusting
    if (angle == 12) {
        nameEntry.isAngleSnapped = true;  // Mark snapping complete
    }

    return angle;
}