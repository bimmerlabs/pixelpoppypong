#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "state.h"
#include "name_entry.h"
#include "screen_transition.h"
#include "BG_DEF/nbg1.h"
#include "lighting.h"
#include "BG_DEF/sprite_colors.h"

unsigned int g_NameEntryTimer = 0;
unsigned int g_NameEntrySelection = 0;
const char letters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!.  ";
static char initials[4];  // 3 letters + null terminator
static int char_id[3];

#define NAME_X_RADIUS toFIXED(140)
#define NAME_Y_RADIUS toFIXED(50)
#define NAME_Y_POS 170
#define END_RADIUS toFIXED(-1400)
#define GAP_ANGLE 12
static FIXED xRadius = NAME_X_RADIUS;
static FIXED yRadius = NAME_Y_RADIUS;
static FIXED yPos = NAME_Y_POS;
static int nameEntryAngle = 360;
static int gapAngle = 0;
static bool pressedLeft = false;
static bool pressedRight = false;
static bool isAngleSnapped = true;
static bool isNameEntryFinished = false;
static bool endNameEntry = false;

void nameEntryInit(void)
{
    // g_Game.isLoading = true; // don't show loading, it's too short and looks bad
    loadNameEntryAssets();
    initNameEntryColors();
    
    g_NameEntrySelection = FIRST_INITIAL;
    
    char_id[0] = 10; // A
    char_id[1] = 39; // blank
    char_id[2] = 39; // blank
    xRadius = NAME_X_RADIUS;
    yRadius = NAME_Y_RADIUS;
    yPos = NAME_Y_POS;
    nameEntryAngle = 360;
    isAngleSnapped = true;
    pressedLeft = false;
    pressedRight = false;
    rotateRight = false;
    rotateLeft = false;
    isNameEntryFinished = false;
    endNameEntry = false;
    set_spr_scale(&pixel_poppy, 2.0, 2.0);
    pixel_poppy.rot.z = 0;
    set_spr_position(&pixel_poppy, 0, 0, 100);
    // set to "smile"
    pixel_poppy.spr_id = pixel_poppy.anim1.asset[1];
        
    g_NameEntryTimer = NAME_ENTRY_TIMER;    
    convertNumberToDigits(g_NameEntryTimer);
    font.spr_id = font.anim1.asset[tens];
    font.spr_id = font.anim1.asset[ones];
    
    g_Game.lastState = GAME_STATE_NAME_ENTRY;
    
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN); // use the default here    
    mosaic_in = true;
    music_in = true;
    fade_in = true;
    transition_in = true;
    playCDTrack(NAME_ENTRY_TRACK, true);
}

void nameEntryInput(void)
{
     if (endNameEntry) {
         return;
     }
    // if(jo_is_pad1_key_down(JO_KEY_START))
    // {
        // // turn off display first?
        // transitionState(GAME_STATE_HIGHSCORES);
        // // unloadNameEntryAssets(); // this needs to happen before the next state is loaded
        // return;
    // }
    if(jo_is_pad1_key_pressed(JO_KEY_UP))
    {
        if (yRadius < toFIXED(0)) {
            xRadius -= toFIXED(0.5);
            yRadius += toFIXED(2);
        }
        else if (yRadius < toFIXED(100)) {
            xRadius += toFIXED(0.5);
            yRadius += toFIXED(2);
        }
    }
    if(jo_is_pad1_key_pressed(JO_KEY_DOWN))
    {
        if (yRadius > toFIXED(0)) {
            xRadius -= toFIXED(0.5);
            yRadius -= toFIXED(2);
        }
        else if (yRadius > toFIXED(-100)) {
            xRadius += toFIXED(0.5);
            yRadius -= toFIXED(2);
        }
    }
    if (!isAngleSnapped) {
         return;
    }
    if(jo_is_pad1_key_pressed(JO_KEY_LEFT) && !isNameEntryFinished)
    {
        pressedLeft = true;
        pressedRight = false;
        isAngleSnapped = false;
        nameEntryAngle += 1;
        if (nameEntryAngle > 360)
            nameEntryAngle -= 360;
    }
    else if(jo_is_pad1_key_pressed(JO_KEY_RIGHT) && !isNameEntryFinished)
    {
        pressedLeft = false;
        pressedRight = true;
        isAngleSnapped = false;
        nameEntryAngle -= 1;
        if (nameEntryAngle < 0)
            nameEntryAngle += 360;
    }
    else if(jo_is_pad1_key_down(JO_KEY_A) || jo_is_pad1_key_down(JO_KEY_C))
    {
        if (isNameEntryFinished && nameEntryAngle == 12) {
            endNameEntry = true;
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
            return;
        }
        if (!isNameEntryFinished && nameEntryAngle != 12) {
            g_NameEntrySelection++;
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        }
        if (g_NameEntrySelection == MAX_INITIAL) {
            isNameEntryFinished = true;
            isAngleSnapped = false;
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        }
    }
    else if(jo_is_pad1_key_down(JO_KEY_B))
    {
        if (g_NameEntrySelection == MAX_INITIAL) {
            nameEntryAngle = 360;
            g_NameEntrySelection--;
            char_id[g_NameEntrySelection] = 39;
            initNameEntryColors();
            isNameEntryFinished = false;
            isAngleSnapped = true;
            rotateRight = false;
            rotateLeft = false;
            pixel_poppy.spr_id = pixel_poppy.anim1.asset[2];
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        }
        else if (g_NameEntrySelection > FIRST_INITIAL) {
            g_NameEntrySelection--;
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        }
    }
    if (isAngleSnapped == false) {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[0];
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
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
    if (g_NameEntryTimer > 0 && frame % 60 == 0) { // modulus
        g_NameEntryTimer--;
    }
    
    jo_nbg0_printf(17, 2, "NAME ENTRY");
    
    if (g_NameEntryTimer > 0 && !endNameEntry) {
        // make an inline sub for this
        convertNumberToDigits(g_NameEntryTimer);
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
    if (char_id[FIRST_INITIAL] != 39) {
        set_spr_position(&font, -24, yPos, 50);
        set_spr_scale(&font, 2, 2);
        font.spr_id = font.anim1.asset[char_id[FIRST_INITIAL]];
        font.zmode = _ZmRC;
        my_sprite_draw(&font);
    }
    if (g_NameEntrySelection >= 1 && char_id[SECOND_INITIAL] != 39) {
        set_spr_position(&font, 0, yPos, 50);
        set_spr_scale(&font, 2, 2);
        font.spr_id = font.anim1.asset[char_id[SECOND_INITIAL]];
        font.zmode = _ZmCC;
        my_sprite_draw(&font);
    }
    if (g_NameEntrySelection >= 2 && char_id[THIRD_INITIAL] != 39) {
        set_spr_position(&font, 24, yPos, 50);
        set_spr_scale(&font, 2, 2);
        font.spr_id = font.anim1.asset[char_id[THIRD_INITIAL]];
        font.zmode = _ZmLC;
        my_sprite_draw(&font);
    }
    if (endNameEntry && xRadius > END_RADIUS) {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[3];
        xRadius -= toFIXED(6);
        yRadius -= toFIXED(4);
        nameEntryAngle += 3;
        if (nameEntryAngle > 360)
            nameEntryAngle -= 360;
        if (yPos > 0)
            yPos -= 2;
    }
    
    font.zmode = _ZmCC;
    nameEntryDraw();
    
    
    initials[0] = letters[char_id[FIRST_INITIAL]];
    initials[1] = letters[char_id[SECOND_INITIAL]];
    initials[2] = letters[char_id[THIRD_INITIAL]];
    initials[3] = '\0';
    // jo_nbg0_printf(17, 27, "TEST:%s", initials); // FOR DEBUG
    
    if(g_NameEntryTimer == 0 || xRadius <= END_RADIUS)
    {
        transitionState(GAME_STATE_HIGHSCORES);
        return;
    }

}

void nameEntryDraw(void)
{
    if (!isNameEntryFinished) {
        nameEntryAngle = snap_to_nearest_12(nameEntryAngle, pressedLeft, pressedRight, &isAngleSnapped);
    }
    if (isNameEntryFinished) {
        nameEntryAngle = snap_to_end(nameEntryAngle, &isAngleSnapped);
    }
    for (gapAngle = 0; gapAngle <= 360; gapAngle += GAP_ANGLE) {
        // if (!isAngleSnapped)
            nameEntryPositionUpdate((nameEntryAngle+ 6 + gapAngle), (gapAngle + 102));
    }
    do_update_font2All = calculate_sprites_color(&p_rangeFont2All);
    
    set_spr_scale(&pixel_poppy, 6.0, 6.0);
    int poppyAngle = nameEntryAngle;
    if (poppyAngle > 360)
        poppyAngle -= 360;
    pixel_poppy.rot.z = -1*poppyAngle;
    set_spr_position(&pixel_poppy, 0, 0, 100);
    my_sprite_draw(&pixel_poppy);
    
    if (g_GameOptions.debug_display) {
        jo_nbg0_printf(2, 24, "RAD:X=%3d,Y=%3d", JO_FIXED_TO_INT(xRadius), JO_FIXED_TO_INT(yRadius));
        jo_nbg0_printf(2, 25, "POS:X=%3d,Y=%3d", JO_FIXED_TO_INT(light.x), JO_FIXED_TO_INT(light.y));
        jo_nbg0_printf(2, 26, "ANGLE:%3d", nameEntryAngle);
        jo_nbg0_printf(2, 27, "ENDNAMEENTRY:%i", endNameEntry);
        jo_nbg0_printf(20, 24, "ISANGLESNAPPED:%i", isAngleSnapped);
        jo_nbg0_printf(20, 25, "ISNAMEENTRYFINISHED:%i", isNameEntryFinished);
        // jo_nbg0_printf(2, 14, "DO_UPDATE_FONT2ALL:%i", do_update_font2All);
        // jo_nbg0_printf(2, 15, "HUE:%d", hslSprites.color[74].h);
        // jo_nbg0_printf(2, 16, "SAT:%d", hslSprites.color[74].s);
        // jo_nbg0_printf(2, 17, "LUM:%d", hslSprites.color[74].l);
        // jo_nbg0_printf(20, 15, "RGB:%d", bufferSprites.color[74]);
    }
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
    
    light.x = jo_fixed_mult(jo_cos(angle), xRadius);
    light.y = jo_fixed_mult(my_fixed_sine(angle), yRadius);
    font.pos.x = 2*(light.x);
    font.pos.y = -2*(light.y);
    
    if (angle > 262 && angle < 278) {
        if (!isNameEntryFinished) {
            char_id[g_NameEntrySelection] = spr_id;
        }
        font.scl.x = toFIXED(2.7);
    }
    else {
        font.scl.x = toFIXED(1) + (FIXED_127 + jo_fixed_mult(jo_cos(scaleAngle), NAME_X_RADIUS))/200;
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
    if (!isNameEntryFinished) {
        hslSprites.color[colorid].s = 255 - zPos;
        hslSprites.color[colorid].l = 255 - zPos;
    }
    else {
        jo_getdate(&now);
        int hue = now.minute*6;
        hslSprites.color[colorid].h = hue;
        hslSprites.color[colorid].s = 240;
        hslSprites.color[colorid].l = 240;
    }
    
    font.spr_id = font.anim1.asset[spr_id];
    my_sprite_draw(&font);
}

// original (secret mode?)
// void nameEntryPositionUpdate(int angle) {
    // if (angle > 360)
        // angle -= 360;
    
    // light.x = jo_fixed_mult(jo_cos(angle), xRadius);
    // light.y = jo_fixed_mult(my_fixed_sine(angle), yRadius);
    // pixel_poppy.pos.x = 2*(light.x);
    // pixel_poppy.pos.y = -2*(light.y);
    
    // int scaleAngle = angle + 90;
    // if (scaleAngle > 360)
        // scaleAngle -= 360;
    // pixel_poppy.scl.x = toFIXED(1) + (FIXED_127 + jo_fixed_mult(jo_cos(scaleAngle), NAME_X_RADIUS))/300;
    // pixel_poppy.scl.y = pixel_poppy.scl.x;
    // pixel_poppy.rot.z = 0;
    
    // int zAngle = angle + 91;
    // int zPos = 0;
    // if (zAngle > 360)
        // zAngle -= 360;
    // if (zAngle > 258 && zAngle < 282) {
        // zPos = 90;  // Keep zPos fixed at 90 within the hysteresis range
    // }
    // else {
        // zPos = 180 - ABS(zAngle - 180);
    // }
    // pixel_poppy.pos.z = toFIXED(zPos+50);

    // pixel_poppy.spr_id = pixel_poppy.anim1.asset[0];
    // my_sprite_draw(&pixel_poppy);
// }