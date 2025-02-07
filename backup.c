#include <jo/jo.h>
#include "main.h"

int memory_access = 0; // for debug use only
Uint8 backup_device = 0;

void    save_game_backup(void)
{
    if (!jo_backup_mount(backup_device)) {
        memory_access = 5;
        return;
    }
    
    jo_backup_save_file_contents(
        backup_device,
        "PPPONG25",              // Save name (must be <=  8 chars)
        "PIXELPOPPY",          // Comment   (must be <= 10 chars)
        &game_options,
        sizeof(GameOptions));
        
    jo_backup_unmount(backup_device);
    memory_access = 6;
}

bool   load_game_backup(void)
{
    backup_device = JoCartridgeMemoryBackup;
    
    GameOptions *loaded_options;
    
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
        loaded_options = (GameOptions *)jo_backup_load_file_contents(backup_device, "PPPONG25", JO_NULL);
        game_options = *loaded_options;
        jo_free(loaded_options);
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