#pragma once

extern int memory_access;

void    save_game_backup(void);
bool    load_game_backup(void);
bool    is_cart_mem_available(void);
bool    is_internal_mem_available(void);