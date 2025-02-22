#include <jo/jo.h>
#include "main.h"
#include "backup.h"
#include "gameplay.h"

int memory_access = 0; // for debug use only
Uint8 backup_device = 0;
SaveGame save_game;

void init_save_game() {
    save_game.game_options = game_options;
    save_game.characterAvailable = characterAvailable;
    for (int i = 0; i < SCORE_ENTRIES; i++) {
        save_game.highScores[i] = highScores[i];
    }
    for (int i = 0; i < MAX_INPUTS; i++) {
        save_game.inputSettings[i].sensitivity = g_Inputs[i].sensitivity;
    }
}

void save_game_backup(void) {
    if (!jo_backup_mount(backup_device)) {
        memory_access = 5;
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
    memory_access = 6;
}

bool load_game_backup(void) {
    backup_device = JoCartridgeMemoryBackup;
    SaveGame *loaded_save;
    memory_access = 1;
    // try cart first
    if (!jo_backup_mount(backup_device)) {
        backup_device = JoInternalMemoryBackup;
    }
    // fall back to internal memory
    if (backup_device == JoInternalMemoryBackup && !jo_backup_mount(backup_device)) {
        memory_access = 2;
        return false;
    }
    // load save if it exists
    if (jo_backup_file_exists(backup_device, "PPPONG25")) {
        loaded_save =  (SaveGame *)jo_backup_load_file_contents(backup_device, "PPPONG25", JO_NULL);
        if (!loaded_save) {
            jo_backup_unmount(backup_device);
            memory_access = 0;
            return false;
        }
        game_options = loaded_save->game_options; 
        for (int i = 0; i < TOTAL_CHARACTERS; i++) {
            characterAvailable[i] = loaded_save->characterAvailable[i];
        }
        for (int i = 0; i < SCORE_ENTRIES; i++) {
            highScores[i] = loaded_save->highScores[i];
        }
        for (int i = 0; i < MAX_INPUTS; i++) {
            g_Inputs[i].sensitivity = loaded_save->inputSettings[i].sensitivity;
        }
        
        jo_free(loaded_save);
        jo_backup_unmount(backup_device);
        memory_access = 3;
        return true;
    }
    // create default save
    else {
        jo_backup_unmount(backup_device); 
        save_game_backup();
        memory_access = 4;
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