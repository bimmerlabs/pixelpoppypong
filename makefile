JO_COMPILE_WITH_VIDEO_MODULE = 0
JO_PSEUDO_SATURN_KAI_SUPPORT = 0
JO_COMPILE_WITH_BACKUP_MODULE = 1
JO_COMPILE_WITH_RAM_CARD_MODULE = 0
JO_COMPILE_WITH_TGA_MODULE = 1
JO_COMPILE_WITH_AUDIO_MODULE = 0
JO_COMPILE_WITH_3D_MODULE = 0
JO_COMPILE_WITH_PSEUDO_MODE7_MODULE = 0
JO_COMPILE_WITH_EFFECTS_MODULE = 0
JO_COMPILE_WITH_DUAL_CPU_MODULE = 0
JO_COMPILE_WITH_PRINTF_MODULE = 0
JO_DEBUG = 0
JO_NTSC = 1
JO_COMPILE_USING_SGL=1
CCFLAGS += -DMY_CRAM_MODE=1
CCFLAGS += -DMY_CRAM_OFFSET=0
CCFLAGS += -DMY_TV_704x240=1
CCFLAGS += -DENABLE_DEBUG_MODE=1
SRCS=main.c backup.c state.c screen_transition.c assets.c audio.c pcmsys.c ppp_logo.c title_screen.c team_select.c gameplay.c storymode.c AI.c objects/player.c objects/goal.c objects/teams.c input.c pause.c debug.c util.c physics.c math.c font.c ColorHelpers.c palettetools.c sprites.c bg_def/sprite_colors.c bg_def/nbg1.c credits.c highscores.c lighting.c name_entry.c
JO_ENGINE_SRC_DIR=../../jo_engine
COMPILER_DIR=../../Compiler
include $(COMPILER_DIR)/COMMON/jo_engine_makefile
