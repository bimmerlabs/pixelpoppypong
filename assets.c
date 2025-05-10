#include <jo/jo.h>
#include "assets.h"
#include "sprites.h"
#include "util.h"

// NOTE: Palette is loaded with the font and shared with all other sprites.

Uint8 palette_transparent_index = 2;
int paw_blank_id = 0;

ASSETS g_Assets = {
    .GameplaySoundsLoaded = false,
    .NameEntrySoundsLoaded = false,
};

// tilesets
jo_tile paw_tileset[NUM_PAW_SPRITES] = {0};

jo_tile pixel_poppy1_tileset[NUM_POPPY_SPRITES] = {0};

jo_tile goal_tileset[NUM_GOAL_SPRITES] = {0};

jo_tile font_char_tileset[NUM_FONT_CHARS] = {0};

jo_tile menu_text_tileset[NUM_TITLE_MENU_TEXT] = {0};

jo_tile timer_tileset[NUM_TIMER_SPRITES] = {0};

jo_tile heart_tileset[NUM_HEART_SPRITES] = {0};
jo_tile shield_tileset[NUM_SHIELD_SPRITES] = {0};

jo_tile menubg_tileset[NUM_MENUBG_SPRITES] = {0};

jo_tile character_tileset[NUM_CHARACTER_SPRITES] = {0};
jo_tile pcursor_tileset[NUM_PCURSOR_SPRITES] = {0};

jo_tile bomb_tileset[NUM_BOMB_SPRITES] = {0};
jo_tile explode_tileset[NUM_EXPLOD_SPRITES] = {0};
jo_tile fishtank_tileset[NUM_FISH_SPRITES] = {0};
jo_tile shroom_tileset[NUM_SHROOM_SPRITES] = {0};

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

void loadSprite(Sprite *sprite, int *asset, const char *file, jo_tile *tileset, unsigned int frames, int w, int h, bool asset1or2) {
    initTileset(tileset, frames, 1, w, h);
    asset[0] = jo_sprite_add_tga_tileset(NULL, file, palette_transparent_index, tileset, frames);
        for(unsigned int i = 0; i < frames; i++)
        {
            asset[i] = asset[0] + i;
        }
        if (asset1or2) { // use asset1
            sprite->anim1.asset = asset;
            sprite->anim1.max = frames-1;
            sprite->spr_id = sprite->anim1.asset[sprite->anim1.frame];
        }
        else { // use asset2
            sprite->anim2.asset = asset;
            sprite->anim2.max = frames-1;
        }
        sprite->pos.r = h; // is this needed?
}

void loadCoreSoundAssets(void)
{
    // load_8bit_pcm returns the ID of the sound loaded
    // use pcm_reset, giving the id of the last sound you want to keep, to unload extra sounds
    
    // CORE / MENU SOUNDS
    g_Assets.cancelPcm8 = load_8bit_pcm((Sint8 *)"CANCEL.PCM", 15360);
    g_Assets.cursorPcm8 = load_8bit_pcm((Sint8 *)"CURSOR.PCM", 15360);
    g_Assets.nextPcm8 = load_8bit_pcm((Sint8 *)"NEXT.PCM", 15360);
    g_Assets.startPcm8 = load_8bit_pcm((Sint8 *)"START.PCM", 15360);
    g_Assets.tickPcm8 = load_8bit_pcm((Sint8 *)"TICK.PCM", 15360);
}

bool loadGameplaySoundAssets(void)
{
    // GAMEPLAY SOUNDS
    g_Assets.scoreTotalPcm8 = load_8bit_pcm((Sint8 *)"SCORET.PCM", 15360);
    g_Assets.scoreAddPcm8 = load_8bit_pcm((Sint8 *)"SCOREA.PCM", 15360);
    g_Assets.chain0Pcm8 = load_8bit_pcm((Sint8 *)"CHAIN0.PCM", 15360);
    g_Assets.chain1Pcm8 = load_8bit_pcm((Sint8 *)"CHAIN1.PCM", 15360);
    g_Assets.chain2Pcm8 = load_8bit_pcm((Sint8 *)"CHAIN2.PCM", 15360);
    g_Assets.chain3Pcm8 = load_8bit_pcm((Sint8 *)"CHAIN3.PCM", 15360);
    g_Assets.chain5Pcm8 = load_8bit_pcm((Sint8 *)"CHAIN5.PCM", 15360);
    g_Assets.explod1Pcm8 = load_8bit_pcm((Sint8 *)"EXPLOD1.PCM", 15360);
    g_Assets.growPcm8 = load_8bit_pcm((Sint8 *)"GROW.PCM", 15360);
    g_Assets.shrinkPcm8 = load_8bit_pcm((Sint8 *)"SHRINK.PCM", 15360);
    g_Assets.bloopPcm8 = load_8bit_pcm((Sint8 *)"BLOOP.PCM", 15360);
    g_Assets.stadlerPcm8 = load_8bit_pcm((Sint8 *)"STADLER.PCM", 15360);
    g_Assets.dropPcm8 = load_8bit_pcm((Sint8 *)"DROP.PCM", 15360);
    g_Assets.bouncePcm8 = load_8bit_pcm((Sint8 *)"BOUNCE.PCM", 15360);
    g_Assets.shieldPcm8 = load_8bit_pcm((Sint8 *)"SHIELD.PCM", 15360);
    // g_Assets.rechargePcm8 = load_8bit_pcm((Sint8 *)"RECHARGE.PCM", 15360);
    g_Assets.countdownPcm8 = load_8bit_pcm((Sint8 *)"C_DOWN.PCM", 15360);
    g_Assets.bumpPcm16 = load_16bit_pcm((Sint8 *)"BUMP.PCM", 15360);
    g_Assets.gameOverPcm8 = load_8bit_pcm((Sint8 *)"GMOVR8.PCM", 15360);
    g_Assets.winPcm8 = load_8bit_pcm((Sint8 *)"WIN.PCM", 15360);
    
    // CAT SOUNDS
    g_Assets.meowPcm8[0] = load_8bit_pcm((Sint8 *)"MEOW1.PCM", 15360);
    g_Assets.meowPcm8[1] = load_8bit_pcm((Sint8 *)"MEOW5.PCM", 15360);
    g_Assets.meowPcm8[2] = load_8bit_pcm((Sint8 *)"MEOW2.PCM", 15360);
    g_Assets.meowPcm8[3] = load_8bit_pcm((Sint8 *)"MEOW6.PCM", 15360);
    g_Assets.meowPcm8[4] = load_8bit_pcm((Sint8 *)"MEOW3.PCM", 15360);
    g_Assets.meowPcm8[5] = load_8bit_pcm((Sint8 *)"MEOW7.PCM", 15360);
    g_Assets.meowPcm8[6] = load_8bit_pcm((Sint8 *)"MEOW4.PCM", 15360);
    g_Assets.meowPcm8[7] = load_8bit_pcm((Sint8 *)"MEOW8.PCM", 15360);
    g_Assets.meowPcm8[8] = load_8bit_pcm((Sint8 *)"MEOW9.PCM", 15360);
    g_Assets.meowID = MEOW1;
    return true;
}

bool loadNameEntrySoundAssets(void)
{   
    // NAME ENTRY SOUNDS
    g_Assets.name_ketPcm8 = load_8bit_pcm((Sint8 *)"NAME_KET.PCM", 15360);
    g_Assets.name_curPcm8 = load_8bit_pcm((Sint8 *)"NAME_CUR.PCM", 15360);
    g_Assets.name_canPcm8 = load_8bit_pcm((Sint8 *)"NAME_CAN.PCM", 15360);
    g_Assets.name_brkPcm8 = load_8bit_pcm((Sint8 *)"NAME_BRK.PCM", 15360);
    return true;
}

void loadCommonAssets(void)
{
    jo_sprite_free_all();
    // pixel poppy
    loadSprite(&pixel_poppy, g_Assets.pixel_poppy1, "POPPY.TGA", pixel_poppy1_tileset, NUM_POPPY_SPRITES, 64, 50, true);
    // menu cursor
    cursor.spr_id = jo_sprite_add_tga(NULL, "CURSOR.TGA", palette_transparent_index);
    
    // background / UI layers
    loadSprite(&menu_bg1, g_Assets.menubg, "MENUBG.TGA", menubg_tileset, NUM_MENUBG_SPRITES, 2, 2, true);
    menu_bg1.spr_id = menu_bg1.anim1.asset[6];
    menu_bg2.anim1.asset = g_Assets.menubg;
    menu_bg2.spr_id = menu_bg2.anim1.asset[5];
    player_bg.anim1.asset = g_Assets.menubg;
    meter.anim1.asset = g_Assets.menubg;
    meter.spr_id = meter.anim1.asset[7];
    
    ppplogo.spr_id = jo_sprite_add_tga(NULL, "PPPLOGO.TGA", palette_transparent_index);
    pppshadow.spr_id = jo_sprite_add_tga(NULL, "PPPSHDW.TGA", palette_transparent_index); 
}

void loadPPPLogoAssets(void)
{        
    ppplogo.spr_id = jo_sprite_add_tga(NULL, "PPPLOGO.TGA", palette_transparent_index);
    pppshadow.spr_id = jo_sprite_add_tga(NULL, "PPPSHDW.TGA", palette_transparent_index);  
    
    g_Game.isLoading = false;
}

void unloadPPPLogoAssets(void)
{
    g_Game.isLoading = true;
    // unloads everything after this point
    jo_sprite_free_from(ppplogo.spr_id);
    
}

void loadNameEntryAssets(void)
{
    loadSprite(&font, g_Assets.font, "FONT2.TGA", font_char_tileset, NUM_FONT_CHARS, 24, 24, true);
    if (!g_Assets.NameEntrySoundsLoaded) {
            g_Game.isSoundLoading = true;
            g_Assets.NameEntrySoundsLoaded = loadNameEntrySoundAssets();
            g_Game.isSoundLoading = false;            
    }
    g_Game.isLoading = false;
}

void unloadNameEntryAssets(void)
{
    // unloads everything after this point
    jo_sprite_free_from(font.spr_id);
    
}

void loadTitleScreenAssets(void)
{        
    logo1.spr_id = jo_sprite_add_tga(NULL, "LOGO1.TGA", palette_transparent_index);
    logo2.spr_id = jo_sprite_add_tga(NULL, "LOGO2.TGA", palette_transparent_index);
    loadSprite(&menu_text, g_Assets.menu, "TMENU.TGA", menu_text_tileset, NUM_TITLE_MENU_TEXT, 240, 22, true);    
    
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
    loadSprite(&macchi, g_Assets.paw1, "PAW1.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    loadSprite(&jelly, g_Assets.paw2, "PAW2.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    
    loadSprite(&penny, g_Assets.paw3, "PAW3.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    loadSprite(&potter, g_Assets.paw4, "PAW4.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    
    loadSprite(&sparta, g_Assets.paw5, "PAW5.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    
    loadSprite(&poppy, g_Assets.paw6, "PAW6.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    loadSprite(&tj, g_Assets.paw7, "PAW7.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    loadSprite(&george, g_Assets.paw8, "PAW8.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    loadSprite(&wuppy, g_Assets.paw9, "PAW9.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
         
    loadSprite(&stadler, g_Assets.paw10, "PAW10.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);
    loadSprite(&garfield, g_Assets.paw11, "PAW11.TGA", paw_tileset, NUM_PAW_SPRITES, 32, 32, true);


    paw_blank_id = jo_sprite_add_tga(NULL, "PAW0.TGA", palette_transparent_index);
    paw_blank.spr_id = paw_blank_id;
    
    loadSprite(&character_portrait, g_Assets.characters, "PORTRAIT.TGA", character_tileset, NUM_CHARACTER_SPRITES, 48, 48, true);
    
    loadSprite(&player_cursor, g_Assets.pcursor, "SELECT.TGA", pcursor_tileset, NUM_PCURSOR_SPRITES, 24, 24, true);
        
    g_Game.isLoading = false;
}


void loadGameAssets(void)
{
    g_Game.isLoading = true;
    // UI elements
    loadSprite(&timer_num1, g_Assets.timer, "NUM1X1.TGA", timer_tileset, NUM_TIMER_SPRITES, 16, 16, true);
    timer_num10.anim1.asset = g_Assets.timer;
    timer_num100.anim1.asset = g_Assets.timer;

    loadSprite(&goal[0], g_Assets.goal1, "GOAL1.TGA", goal_tileset, NUM_GOAL_SPRITES, 32, 12, true);
    loadSprite(&goal[1], g_Assets.goal2, "GOAL2.TGA", goal_tileset, NUM_GOAL_SPRITES, 32, 12, true);
    loadSprite(&goal[2], g_Assets.goal3, "GOAL3.TGA", goal_tileset, NUM_GOAL_SPRITES, 32, 12, true);
    loadSprite(&goal[3], g_Assets.goal4, "GOAL4.TGA", goal_tileset, NUM_GOAL_SPRITES, 32, 12, true);
    
    loadSprite(&heart, g_Assets.heart, "HEARTS.TGA", heart_tileset, NUM_HEART_SPRITES, 8, 8, true);
    star.anim1.asset = g_Assets.heart;
    star.spr_id = star.anim1.asset[3];
    
    loadSprite(&shield[0], g_Assets.shield, "SHIELD.TGA", shield_tileset, NUM_SHIELD_SPRITES, 32, 64, true);
    shield[1].anim1.asset = g_Assets.shield,
    shield[1].anim1.max = 6,
    shield[1].spr_id = shield[1].anim1.asset[6];
    shield[2].anim1.asset = g_Assets.shield,
    shield[2].anim1.max = 6,
    shield[2].spr_id = shield[2].anim1.asset[6];
    shield[3].anim1.asset = g_Assets.shield,
    shield[3].anim1.max = 6,
    shield[3].spr_id = shield[3].anim1.asset[6];
    
    
    // Items
    loadSprite(&bomb_item, g_Assets.bomb, "BOMB.TGA", bomb_tileset, NUM_BOMB_SPRITES, 24, 24, true);
    loadSprite(&bomb_item, g_Assets.explosion, "EXPLOD.TGA", explode_tileset, NUM_EXPLOD_SPRITES, 72, 72, false);
    loadSprite(&fishtank_item, g_Assets.fishtank, "FISH.TGA", fishtank_tileset, NUM_FISH_SPRITES, 24, 24, true);
    loadSprite(&shroom_item, g_Assets.shroom, "SHROOM.TGA", shroom_tileset, NUM_SHROOM_SPRITES, 24, 24, true);
    
    pixel_poppy.anim2.asset = g_Assets.explosion;
    pixel_poppy.anim2.max = bomb_item.anim2.max;
    
    craig_item.spr_id = jo_sprite_add_tga(NULL, "CRAIG.TGA", palette_transparent_index);
    garfield_item.spr_id = jo_sprite_add_tga(NULL, "GARFIELD.TGA", palette_transparent_index);
    
    if (!g_Assets.GameplaySoundsLoaded) {
            g_Game.isSoundLoading = true;
            g_Assets.GameplaySoundsLoaded = loadGameplaySoundAssets();
            g_Game.isSoundLoading = false;            
    }
    g_Game.isLoading = false;
}

void unloadGameAssets(void)
{
    g_Game.isLoading = true;
    // unloads everything after this point
    jo_sprite_free_from(macchi.anim1.asset[0]);
    
}
