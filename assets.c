#include <jo/jo.h>
#include "assets.h"
#include "sprites.h"
#include "util.h"
#include "pcmsys.h"

// I haven't made any of my assets so this file is a mess
// NOTE: Palette is loaded with the font and shared with all other sprites.

Uint8 palette_transparent_index = 2;
int paw_blank_id = 0;

// tilesets
jo_tile paw1_tileset[NUM_PAW_SPRITES] = {0};
jo_tile paw2_tileset[NUM_PAW_SPRITES] = {0};
jo_tile paw5_tileset[NUM_PAW_SPRITES] = {0};
jo_tile pixel_poppy1_tileset[NUM_POPPY_SPRITES] = {0};
// jo_tile pixel_poppy2_tileset[NUM_POPPY_SPRITES] = {0};

jo_tile menu_text_tileset[NUM_TITLE_MENU_TEXT] = {0};

jo_tile timer_tileset[NUM_TIMER_SPRITES] = {0};

jo_tile menubg_tileset[NUM_MENUBG_SPRITES] = {0};

jo_tile character_tileset[NUM_CHARACTER_SPRITES] = {0};
jo_tile pcursor_tileset[NUM_PCURSOR_SPRITES] = {0};

jo_tile bomb_tileset[NUM_BOMB_SPRITES] = {0};
jo_tile explode_tileset[NUM_EXPLOD_SPRITES] = {0};
jo_tile fishtank_tileset[NUM_FISH_SPRITES] = {0};

// // loads PCMs from the CD to the g_Assets struct
// void loadPCMAssets(void)
// {
    // jo_audio_load_pcm("CRACK.PCM", JoSoundMono16Bit, &g_Assets.crackPCM);
    // g_Assets.crackPCM.sample_rate = 27086;

    // jo_audio_load_pcm("EXPLODE.PCM", JoSoundMono16Bit, &g_Assets.explodePCM);
    // g_Assets.explodePCM.sample_rate = 27086;

    // return;
// }

static void initTileset(jo_tile* tileset, unsigned int numSprites, unsigned int spritesPerRow, int width, int height)
{
    unsigned int col = 0;
    unsigned int row = 0;

    for(unsigned int i = 0; i < numSprites; i++)
    {
        col = i % spritesPerRow; // modulus
        row = i / spritesPerRow;

        tileset[i].x = col * width;
        tileset[i].y = row * height;
        tileset[i].height = height;
        tileset[i].width = width;
    }
}

void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int num_tilesets, int frames, int w, int h, unsigned int spritesPerRow, bool animation1or2) {
    initTileset(tileset, num_tilesets, spritesPerRow, w, h);
    asset[0] = jo_sprite_add_tga_tileset(NULL, file, palette_transparent_index, tileset, num_tilesets);
        for(unsigned int i = 0; i < num_tilesets; i++)
        {
            asset[i] = asset[0] + i;
        }
        if (animation1or2) { // use asset1
            sprite->anim1.asset = asset;
            sprite->anim1.max = frames-1; // frames should be the same as num_tilesets?
            sprite->spr_id = sprite->anim1.asset[sprite->anim1.frame];
        }
        else { // use asset2
            sprite->anim2.asset = asset;
            sprite->anim2.max = frames-1; // frames should be the same as num_tilesets?
            // sprite->spr_id = sprite->anim2.asset[sprite->anim2.frame]; // don't set here
        }
        sprite->pos.r = h; // is this needed?
        // sprite->bbox.width = toFIXED(w*1.5);
        // sprite->bbox.height = toFIXED(h*1.5);
}

void loadSoundAssets(void)
{
    #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
    g_Assets.gameOverPcm8 = load_8bit_pcm((Sint8 *)"GMOVR8.PCM", 15360);
    g_Assets.bumpPcm16 = load_16bit_pcm((Sint8 *)"A.PCM", 15360);
    #else
    jo_audio_load_pcm("GMOVR8.PCM", JoSoundStereo8Bit, &g_Assets.gameOverPcm8);
    g_Assets.gameOverPcm8.sample_rate = 32000;
    jo_audio_load_pcm("A.PCM", JoSoundMono16Bit, &g_Assets.bumpPcm16);
    g_Assets.bumpPcm16.sample_rate = 32000;
    #endif
}

void loadCommonAssets(void)
{
    jo_sprite_free_all();
    // pixel poppy
    loadSprite(&pixel_poppy, g_Assets.pixel_poppy1, "POPPY.TGA", pixel_poppy1_tileset, COUNTOF(pixel_poppy1_tileset), NUM_POPPY_SPRITES, 64, 50, 1, true);
    
    // menu cursor
    cursor.spr_id = jo_sprite_add_tga(NULL, "CURSOR.TGA", palette_transparent_index);
    
    // background / UI layers
    loadSprite(&menu_bg1, g_Assets.menubg, "MENU_BG.TGA", menubg_tileset, COUNTOF(menubg_tileset), NUM_MENUBG_SPRITES, 2, 2, 9, true);
    menu_bg1.spr_id = menu_bg1.anim1.asset[6];
    menu_bg2.anim1.asset = g_Assets.menubg;
    menu_bg2.spr_id = menu_bg2.anim1.asset[5];
    player_bg.anim1.asset = g_Assets.menubg;
    meter.anim1.asset = g_Assets.menubg;
    meter.spr_id = meter.anim1.asset[7];
}

void loadTitleScreenAssets(void)
{        
    logo1.spr_id = jo_sprite_add_tga(NULL, "LOGO1.TGA", palette_transparent_index);
    logo2.spr_id = jo_sprite_add_tga(NULL, "LOGO2.TGA", palette_transparent_index);
    loadSprite(&menu_text, g_Assets.menu, "TMENU.TGA", menu_text_tileset, COUNTOF(menu_text_tileset), NUM_TITLE_MENU_TEXT, 240, 22, 1, true);    
    
    g_Game.isLoading = false;
}

void unloadTitleScreenAssets(void)
{
    g_Game.isLoading = true;    
    // unloads everything after this point
    jo_sprite_free_from(logo1.spr_id);
}

void loadCharacterAssets(void)
{
    loadSprite(&macchi, g_Assets.paw1, "PAW1.TGA", paw1_tileset, COUNTOF(paw1_tileset), NUM_PAW_SPRITES, 32, 32, NUM_PAW_SPRITES, true);
    loadSprite(&jelly, g_Assets.paw2, "PAW2.TGA", paw2_tileset, COUNTOF(paw2_tileset), NUM_PAW_SPRITES, 32, 32, NUM_PAW_SPRITES, true);
    loadSprite(&sparta, g_Assets.paw5, "PAW4.TGA", paw2_tileset, COUNTOF(paw2_tileset), NUM_PAW_SPRITES, 32, 32, NUM_PAW_SPRITES, true);
        penny.anim1.asset = g_Assets.paw1;
        penny.anim1.max = NUM_PAW_SPRITES-1;
        potter.anim1.asset = g_Assets.paw2;
        potter.anim1.max = NUM_PAW_SPRITES-1;
        poppy.anim1.asset = g_Assets.paw2;
        poppy.anim1.max = NUM_PAW_SPRITES-1;
        
        boss1.anim1.asset = g_Assets.paw1;
        boss1.anim1.max = NUM_PAW_SPRITES-1;
        boss2.anim1.asset = g_Assets.paw2;
        boss2.anim1.max = NUM_PAW_SPRITES-1;


    paw_blank_id = jo_sprite_add_tga(NULL, "PAW0.TGA", palette_transparent_index);
    paw_blank.spr_id = paw_blank_id;
    
    loadSprite(&character_portrait, g_Assets.characters, "PORTRAIT.TGA", character_tileset, COUNTOF(character_tileset), NUM_CHARACTER_SPRITES, 48, 48, 1, true);
    
    loadSprite(&player_cursor, g_Assets.pcursor, "SELECT.TGA", pcursor_tileset, COUNTOF(pcursor_tileset), NUM_PCURSOR_SPRITES, 24, 24, 2, true);
        
    g_Game.isLoading = false;
}


void loadGameAssets(void)
{
    g_Game.isLoading = true;
    loadSprite(&timer_num1, g_Assets.timer, "NUM1X1.TGA", timer_tileset, COUNTOF(timer_tileset), NUM_TIMER_SPRITES, 16, 16, 1, true);
    timer_num10.anim1.asset = g_Assets.timer;

    goal1.spr_id = jo_sprite_add_tga(NULL, "GOAL.TGA", palette_transparent_index);
    goal2.spr_id = goal1.spr_id;
    
    loadSprite(&bomb, g_Assets.bomb, "BOMB.TGA", bomb_tileset, COUNTOF(bomb_tileset), NUM_BOMB_SPRITES, 24, 24, 1, true);
    loadSprite(&bomb, g_Assets.explosion, "EXPLOD.TGA", explode_tileset, COUNTOF(explode_tileset), NUM_EXPLOD_SPRITES, 72, 72, 1, false);
    
    loadSprite(&fishtank, g_Assets.fishtank, "FISH.TGA", fishtank_tileset, COUNTOF(fishtank_tileset), NUM_FISH_SPRITES, 24, 24, 1, true);
        
    g_Game.isLoading = false;
}

void unloadGameAssets(void)
{
    g_Game.isLoading = true;
    // unloads everything after this point
    jo_sprite_free_from(macchi.anim1.asset[0]);
    
}
