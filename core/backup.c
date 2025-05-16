#include <jo/jo.h>
#include "../main.h"
#include "backup.h"
#include "../game/gameplay.h"

static Uint8 backup_device = 0;
SaveGame save_game;

void init_save_game() {
    save_game.g_GameOptions = g_GameOptions;
    for (int i = 0; i < TOTAL_CHARACTERS; i++) {
        save_game.characterUnlocked[i] = characterUnlocked[i];
    }
    for (int i = 0; i < SCORE_ENTRIES; i++) {
        save_game.highScores[i] = highScores[i];
    }
    for (int i = 0; i < MAX_INPUTS; i++) {
        save_game.inputSettings[i].sensitivity = g_Inputs[i].sensitivity;
    }
}

void save_game_backup(void) {
    if (!jo_backup_mount(backup_device)) {
        return;
    }
    init_save_game();
    jo_backup_save_file_contents(
        backup_device,
        "PPPONG25",
        "PIXELPOPPY",
        &save_game, 
        sizeof (SaveGame));
        
    jo_backup_unmount(backup_device);
}

bool load_game_backup(void) {
    backup_device = JoCartridgeMemoryBackup;
    SaveGame *loaded_save;
    // try cart first
    if (!jo_backup_mount(backup_device)) {
        backup_device = JoInternalMemoryBackup;
    }
    // fall back to internal memory
    if (backup_device == JoInternalMemoryBackup && !jo_backup_mount(backup_device)) {
        return false;
    }
    // load save if it exists
    if (jo_backup_file_exists(backup_device, "PPPONG25")) {
        loaded_save =  (SaveGame *)jo_backup_load_file_contents(backup_device, "PPPONG25", JO_NULL);
        if (!loaded_save) {
            jo_backup_unmount(backup_device);
            return false;
        }
        g_GameOptions = loaded_save->g_GameOptions; 
        for (int i = 0; i < TOTAL_CHARACTERS; i++) {
            characterUnlocked[i] = loaded_save->characterUnlocked[i];
        }
        for (int i = 0; i < SCORE_ENTRIES; i++) {
            highScores[i] = loaded_save->highScores[i];
        }
        for (int i = 0; i < MAX_INPUTS; i++) {
            g_Inputs[i].sensitivity = loaded_save->inputSettings[i].sensitivity;
        }
        
        jo_free(loaded_save);
        jo_backup_unmount(backup_device);
        return true;
    }
    // create default save
    else {
        jo_backup_unmount(backup_device); 
        save_game_backup();
        return false;
    }
}

bool   is_cart_mem_available(void)
{
    if (jo_backup_mount(JoCartridgeMemoryBackup)) {
        jo_backup_unmount(JoCartridgeMemoryBackup);
        return true;    
    }
    else {
        return false;
    }
}

bool   is_internal_mem_available(void)
{
    if (jo_backup_mount(JoInternalMemoryBackup)) {
        jo_backup_unmount(JoInternalMemoryBackup);
        return true;    
    }
    else {
        return false;
    }
}