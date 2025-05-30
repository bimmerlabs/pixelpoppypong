#include <jo/jo.h>
#include "../main.h"
#include "highscores.h"
#include "character_select.h"
#include "../core/assets.h"
#include "../core/state.h"
#include "../core/backup.h"
#include "../core/screen_transition.h"
#include "../palettefx/nbg1.h"
#include "../palettefx/lighting.h"
#include "../palettefx/sprite_colors.h"

unsigned int g_CharacterEntryTimer = 0;
unsigned int g_CharacterEntrySelection = 0;
// const char letters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!.< ";
// static char initials[4];  // 3 letters + null terminator
// static int char_id[3];

#define NAME_X_RADIUS toFIXED(140)
#define NAME_Y_RADIUS toFIXED(50)
#define NAME_Y_POS 170
#define END_RADIUS toFIXED(-1400)
#define GAP_ANGLE 12
static FIXED xRadius = NAME_X_RADIUS;
static FIXED yRadius = NAME_Y_RADIUS;
static FIXED yPos = NAME_Y_POS;
static int characterEntryAngle = 360;
static int gapAngle = 0;
static bool pressedLeft = false;
static bool pressedRight = false;
static bool isAngleSnapped = true;
static bool isCharacterEntryFinished = false;
static bool endCharacterEntry = false;

void characterSelectInit(void)
{
    // g_Game.isLoading = true;
    // todo: implement or use same assets as team select
    // loadCharacterEntryAssets();
    // initCharacterEntryColors();
    
    g_CharacterEntrySelection = 0;
    xRadius = NAME_X_RADIUS;
    yRadius = NAME_Y_RADIUS;
    yPos = NAME_Y_POS;
    characterEntryAngle = 360;
    isAngleSnapped = true;
    pressedLeft = false;
    pressedRight = false;
    rotateRight = false;
    rotateLeft = false;
    isCharacterEntryFinished = false;
    endCharacterEntry = false;
    set_spr_scale(&pixel_poppy, 2.0, 2.0);
    pixel_poppy.rot.z = 0;
    set_spr_position(&pixel_poppy, 0, 0, 100);
    // set to "smile"
    pixel_poppy.spr_id = pixel_poppy.anim1.asset[1];
        
    g_CharacterEntryTimer = CHARACTER_ENTRY_TIMER;    
    convertNumberToDigits(g_CharacterEntryTimer);
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

void characterEntryInput(void)
{
     if (endCharacterEntry) {
         return;
     }
    // if(jo_is_pad1_key_down(JO_KEY_START))
    // {
        // // turn off display first?
        // transitionState(GAME_STATE_HIGHSCORES);
        // // unloadCharacterEntryAssets(); // this needs to happen before the next state is loaded
        // return;
    // }
    if(jo_is_pad1_key_pressed(JO_KEY_UP))
    {
        if (yRadius < FIXED_0) {
            xRadius -= FIXED_HALF;
            yRadius += FIXED_2;
        }
        else if (yRadius < FIXED_100) {
            xRadius += FIXED_HALF;
            yRadius += FIXED_2;
        }
    }
    if(jo_is_pad1_key_pressed(JO_KEY_DOWN))
    {
        if (yRadius > FIXED_0) {
            xRadius -= FIXED_HALF;
            yRadius -= FIXED_2;
        }
        else if (yRadius > -FIXED_100) {
            xRadius += FIXED_HALF;
            yRadius -= FIXED_2;
        }
    }
    if (!isAngleSnapped) {
         return;
    }
    if(jo_is_pad1_key_pressed(JO_KEY_LEFT) && !isCharacterEntryFinished)
    {
        pressedLeft = true;
        pressedRight = false;
        isAngleSnapped = false;
        characterEntryAngle += 1;
        if (characterEntryAngle > 360)
            characterEntryAngle -= 360;
    }
    else if(jo_is_pad1_key_pressed(JO_KEY_RIGHT) && !isCharacterEntryFinished)
    {
        pressedLeft = false;
        pressedRight = true;
        isAngleSnapped = false;
        characterEntryAngle -= 1;
        if (characterEntryAngle < 0)
            characterEntryAngle += 360;
    }
    else if(jo_is_pad1_key_down(JO_KEY_A) || jo_is_pad1_key_down(JO_KEY_C))
    {
        if (isCharacterEntryFinished && characterEntryAngle == 12) {
            endCharacterEntry = true;
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
            return;
        }
        if (!isCharacterEntryFinished && characterEntryAngle != 12) {
            g_CharacterEntrySelection++;
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        }
        if (g_CharacterEntrySelection == 8) { // MAX_CHARACTERS (count of available/unlocked)
            isCharacterEntryFinished = true;
            isAngleSnapped = false;
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        }
    }
    else if(jo_is_pad1_key_down(JO_KEY_B))
    {
        if (g_CharacterEntrySelection == 8) { // MAX_CHARACTERS (count of available/unlocked)
            characterEntryAngle = 360;
            g_CharacterEntrySelection--;
            // char_id[g_CharacterEntrySelection] = 39; // character selection
            // TODO: implement here
            // player->_sprite = &macchi;
            // player->character.selected = true;
            // player->character.choice = CHARACTER_MACCHI;
            // characterAvailable[player->character.choice] = false;
            // player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];
            // assignCharacterStats(player);
            
            // initNameEntryColors();
            isCharacterEntryFinished = false;
            isAngleSnapped = true;
            rotateRight = false;
            rotateLeft = false;
            pixel_poppy.spr_id = pixel_poppy.anim1.asset[2];
            pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        }
        // else if (g_CharacterEntrySelection > FIRST_INITIAL) { // TODO: unselect character
            // g_CharacterEntrySelection--;
            // pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
        // }
    }
    if (isAngleSnapped == false) {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[0];
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 7);
    }
    else {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[1];
    }
}


void characterEntryUpdate(void)
{
    if(g_Game.gameState != GAME_STATE_CHARACTER_SELECT)
    {
        return;
    }
    if (g_CharacterEntryTimer > 0 && g_Game.frame % 60 == 0) { // modulus
        g_CharacterEntryTimer--;
    }
    
    jo_nbg0_printf(11, 2, "CHOOSE YOUR CHARACTER");
    
    if (g_CharacterEntryTimer > 0 && !endCharacterEntry) {
        // make an inline sub for this
        convertNumberToDigits(g_CharacterEntryTimer);
        set_spr_position(&font, 0, -170, 50);
        set_spr_scale(&font, 1.1, 1.1);
        font.spr_id = font.anim1.asset[tens];
        font.zmode = _ZmRC;
        my_sprite_draw(&font);
        font.spr_id = font.anim1.asset[ones];
        font.zmode = _ZmLC;
        my_sprite_draw(&font);
    }
    
    // draw paw & stats instead
    set_spr_position(&character_portrait, 0, yPos, 50);
    set_spr_scale(&character_portrait, 2, 2);
    character_portrait.spr_id = 0;
    character_portrait.zmode = _ZmCC;
    my_sprite_draw(&character_portrait);
        
    if (endCharacterEntry && xRadius > END_RADIUS) {
        pixel_poppy.spr_id = pixel_poppy.anim1.asset[3];
        xRadius -= toFIXED(6);
        yRadius -= toFIXED(4);
        characterEntryAngle += 3;
        if (characterEntryAngle > 360)
            characterEntryAngle -= 360;
        if (yPos > 0)
            yPos -= 2;
    }
    
    character_portrait.zmode = _ZmCC;
    characterEntryDraw();
    
    
    // initials[0] = letters[char_id[FIRST_INITIAL]];
    // initials[1] = letters[char_id[SECOND_INITIAL]];
    // initials[2] = letters[char_id[THIRD_INITIAL]];
    // initials[3] = '\0';
    // jo_nbg0_printf(17, 27, "TEST:%s", initials); // FOR DEBUG
    
    if(g_CharacterEntryTimer == 0 || xRadius <= END_RADIUS)
    {
        // TODO: SAVE HIGH SCORE
        // addHighScore(g_Players[g_Game.winner].score.points, initials);
        // save_game_backup();
        g_Game.lastState = GAME_STATE_CHARACTER_SELECT;
        transitionState(GAME_STATE_GAMEPLAY);
        return;
    }

}

void characterEntryDraw(void)
{
    if (!isCharacterEntryFinished) {
        characterEntryAngle = snap_to_nearest_angle(characterEntryAngle, pressedLeft, pressedRight, &isAngleSnapped);
    }
    if (isCharacterEntryFinished) {
        // characterEntryAngle = snap_to_end(characterEntryAngle, &isAngleSnapped);
    }
    for (gapAngle = 0; gapAngle <= 360; gapAngle += GAP_ANGLE) {
        // if (!isAngleSnapped)
            characterEntryPositionUpdate((characterEntryAngle+ 6 + gapAngle), (gapAngle + 102));
    }
    // do_update_font2All = calculate_sprites_color(&p_rangeFont2All);
    
    set_spr_scale(&pixel_poppy, 6.0, 6.0);
    int poppyAngle = characterEntryAngle;
    if (poppyAngle > 360)
        poppyAngle -= 360;
    pixel_poppy.rot.z = -1*poppyAngle;
    set_spr_position(&pixel_poppy, 0, 0, 100);
    my_sprite_draw(&pixel_poppy);
    
    #if ENABLE_DEBUG_MODE == 1
    if (g_GameOptions.debug_display) {
        jo_nbg0_printf(2, 24, "RAD:X=%3d,Y=%3d", JO_FIXED_TO_INT(xRadius), JO_FIXED_TO_INT(yRadius));
        jo_nbg0_printf(2, 25, "POS:X=%3d,Y=%3d", JO_FIXED_TO_INT(light.x), JO_FIXED_TO_INT(light.y));
        jo_nbg0_printf(2, 26, "ANGLE:%3d", characterEntryAngle);
        jo_nbg0_printf(2, 27, "ENDNAMEENTRY:%i", endCharacterEntry);
        jo_nbg0_printf(20, 24, "ISANGLESNAPPED:%i", isAngleSnapped);
        jo_nbg0_printf(20, 25, "ISNAMEENTRYFINISHED:%i", isCharacterEntryFinished);
        // jo_nbg0_printf(2, 14, "DO_UPDATE_FONT2ALL:%i", do_update_font2All);
        // jo_nbg0_printf(2, 15, "HUE:%d", hslSprites.color[74].h);
        // jo_nbg0_printf(2, 16, "SAT:%d", hslSprites.color[74].s);
        // jo_nbg0_printf(2, 17, "LUM:%d", hslSprites.color[74].l);
        // jo_nbg0_printf(20, 15, "RGB:%d", bufferSprites.color[74]);
    }
    #endif
}

void characterEntryPositionUpdate(int angle, int sprAngle) {
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
        if (!isCharacterEntryFinished) {
            // char_id[g_CharacterEntrySelection] = spr_id; // todo: portrait id?
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
    
    // int colorid = spr_id + 54; // + 64 degrees -10 (start of "A")
    // if (!isCharacterEntryFinished) {
        // hslSprites.color[colorid].s = 255 - zPos;
        // hslSprites.color[colorid].l = 255 - zPos;
    // }
    // else {
        // jo_getdate(&g_Game.now);
        // int hue = g_Game.minute*6;
        // hslSprites.color[colorid].h = hue;
        // hslSprites.color[colorid].s = 240;
        // hslSprites.color[colorid].l = 240;
    // }
    
    character_portrait.spr_id = character_portrait.anim1.asset[spr_id];
    my_sprite_draw(&font);
}
