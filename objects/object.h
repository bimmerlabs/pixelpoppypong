#pragma once

typedef enum _OBJECT_STATE
{
    OBJECT_STATE_INACTIVE = 0,
    OBJECT_STATE_ACTIVE,
} OBJECT_STATE;

typedef struct _FIXED_POSITION
{
    FIXED x;
    FIXED y;
    FIXED dx;
    FIXED dy;

} FIXED_POSITION, *PFIXED_POSITION;

typedef struct _GAME_OBJECT
{
    OBJECT_STATE objectState;
    FIXED_POSITION curPos;
} GAME_OBJECT, *PGAME_OBJECT;
