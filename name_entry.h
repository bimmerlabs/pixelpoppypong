#pragma once

extern unsigned int g_NameEntryTimer;
#define NAME_ENTRY_TIMER (99 * 60)

void pppLogo_init(void);
void pppLogo_input(void);
void pppLogo_update(void);
void pppLogo_draw(void);
void update_light_position(void);