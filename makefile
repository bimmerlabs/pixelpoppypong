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
CCFLAGS += -DENABLE_DEBUG_MODE=0
CCFLAGS += -DENABLE_EXPERIMENTAL_FEATURES=0
SRCS=main.c core/state.c core/screen_transition.c core/assets.c core/sprites.c core/audio.c core/pcmsys.c core/input.c core/pause.c core/debug.c core/math.c core/util.c core/backup.c game/physics.c game/ppp_logo.c game/title_screen.c game/team_select.c game/gameplay.c game/storymode.c game/AI.c game/credits.c game/highscores.c game/name_entry.c objects/player.c objects/goal.c objects/teams.c objects/items.c palettefx/font.c palettefx/ColorHelpers.c palettefx/palettetools.c palettefx/lighting.c palettefx/sprite_colors.c palettefx/nbg1.c
JO_ENGINE_SRC_DIR=../../jo_engine
COMPILER_DIR=../../Compiler
include $(COMPILER_DIR)/COMMON/jo_engine_makefile
