#include <jo/jo.h>
#include "assets.h"
#include "sprites.h"
#include "util.h"

// I haven't made any of my assets so this file is a mess
// NOTE: Palette is loaded with the font and shared with all other sprites.

Uint8 palette_transparent_index = 2;
int paw_blank_id = 0;

// tilesets
jo_tile paw1_tileset[NUM_PAW_SPRITES] = {0};
jo_tile paw2_tileset[NUM_PAW_SPRITES] = {0};
jo_tile pixel_poppy1_tileset[NUM_POPPY_SPRITES] = {0};
// jo_tile pixel_poppy2_tileset[NUM_POPPY_SPRITES] = {0};

jo_tile menu_text_tileset[NUM_TITLE_MENU_TEXT] = {0};

jo_tile timer_tileset[NUM_TIMER_SPRITES] = {0};

jo_tile menubg_tileset[NUM_MENUBG_SPRITES] = {0};

jo_tile character_tileset[NUM_CHARACTER_SPRITES] = {0};
jo_tile pcursor_tileset[NUM_PCURSOR_SPRITES] = {0};

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
        col = i % spritesPerRow;
        row = i / spritesPerRow;

        tileset[i].x = col * width;
        tileset[i].y = row * height;
        tileset[i].height = height;
        tileset[i].width = width;
    }
}

void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int num_tilesets, int frames, int w, int h, unsigned int spritesPerRow) {
    initTileset(tileset, num_tilesets, spritesPerRow, w, h);
    asset[0] = jo_sprite_add_tga_tileset(NULL, file, palette_transparent_index, tileset, num_tilesets);
        for(unsigned int i = 0; i < num_tilesets; i++)
        {
            asset[i] = asset[0] + i;
        }
        sprite->anim1.asset = asset;
        sprite->anim1.max = frames-1;
        sprite->spr_id = sprite->anim1.asset[sprite->anim1.frame];
}

void loadCommonAssets(void)
{
    // pixel poppy
    loadSprite(&pixel_poppy, g_Assets.pixel_poppy1, "POPPY.TGA", pixel_poppy1_tileset, COUNTOF(pixel_poppy1_tileset), NUM_POPPY_SPRITES, 64, 50, 1);
    
    // menu cursor
    cursor.spr_id = jo_sprite_add_tga(NULL, "CURSOR.TGA", palette_transparent_index);
    
    // background / UI layers
    loadSprite(&menu_bg1, g_Assets.menubg, "MENU_BG.TGA", menubg_tileset, COUNTOF(menubg_tileset), NUM_MENUBG_SPRITES, 2, 2, 9);
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
    loadSprite(&menu_text, g_Assets.menu, "MENU4.TGA", menu_text_tileset, COUNTOF(menu_text_tileset), NUM_TITLE_MENU_TEXT, 240, 22, 1);    
    
    g_Game.isLoading = false;
}

void unloadTitleScreenAssets(void)
{
    g_Game.isLoading = true;    
    // unloads everything after this point
    // jo_sprite_free_all();
    jo_sprite_free_from(logo1.spr_id);
}

void loadGameAssets(void)
{
    loadSprite(&macchi, g_Assets.paw1, "PAW1.TGA", paw1_tileset, COUNTOF(paw1_tileset), NUM_PAW_SPRITES, 32, 32, NUM_PAW_SPRITES);
    loadSprite(&jelly, g_Assets.paw2, "PAW2.TGA", paw2_tileset, COUNTOF(paw2_tileset), NUM_PAW_SPRITES, 32, 32, NUM_PAW_SPRITES);
        penny.anim1.asset = g_Assets.paw1;
        penny.anim1.max = NUM_PAW_SPRITES-1;
        potter.anim1.asset = g_Assets.paw2;
        potter.anim1.max = NUM_PAW_SPRITES-1;
        
        sparta.anim1.asset = g_Assets.paw1;
        sparta.anim1.max = NUM_PAW_SPRITES-1;
        poppy.anim1.asset = g_Assets.paw2;
        poppy.anim1.max = NUM_PAW_SPRITES-1;
        
        boss1.anim1.asset = g_Assets.paw1;
        boss1.anim1.max = NUM_PAW_SPRITES-1;
        boss2.anim1.asset = g_Assets.paw2;
        boss2.anim1.max = NUM_PAW_SPRITES-1;


    paw_blank_id = jo_sprite_add_tga(NULL, "PAW0.TGA", palette_transparent_index);
    paw_blank.spr_id = paw_blank_id;
      
    loadSprite(&timer_num1, g_Assets.timer, "NUM1X1.TGA", timer_tileset, COUNTOF(timer_tileset), NUM_TIMER_SPRITES, 16, 16, 1);
    timer_num10.anim1.asset = g_Assets.timer;
    
    loadSprite(&character_portrait, g_Assets.characters, "PORTRAIT.TGA", character_tileset, COUNTOF(character_tileset), NUM_CHARACTER_SPRITES, 48, 48, 1);
    
    loadSprite(&player_cursor, g_Assets.pcursor, "PCURSOR.TGA", pcursor_tileset, COUNTOF(pcursor_tileset), NUM_PCURSOR_SPRITES, 24, 24, 2);
        
    g_Game.isLoading = false;
}

void unloadGameAssets(void)
{
    g_Game.isLoading = true;
    // unloads everything after this point
    // jo_sprite_free_all();
    jo_sprite_free_from(macchi.anim1.asset[0]);
    
}
