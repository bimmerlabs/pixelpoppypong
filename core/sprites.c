#include "sprites.h"
#include "assets.h"
#include "../palettefx/sprite_colors.h"
    
Sprite font = {
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,                                       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite ppplogo = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(100)},   // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,                                       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite pppshadow = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(110)},   // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,                                       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHon,
    .zmode = _ZmCC
};

// UI ELEMENTS
Sprite logo1 = {
    .pos = {toFIXED(0), toFIXED(-200), toFIXED(97)},   // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.3), toFIXED(1.3)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,                                       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCT
};

Sprite logo2 = {
    .pos = {toFIXED(0), toFIXED(200), toFIXED(97)},   // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.3), toFIXED(1.3)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCB
};

Sprite cursor = {
    .pos = {toFIXED(-147), toFIXED(0), toFIXED(90)},   // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};

Sprite menu_text = {
    .pos = {toFIXED(0), toFIXED(90), toFIXED(90)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1), toFIXED(1)},                 // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};

Sprite menu_choices = {
    .pos = {toFIXED(0), toFIXED(90), toFIXED(90)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1), toFIXED(1)},                 // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmLC
};

Sprite menu_bg1 = {
    .pos = {toFIXED(0), toFIXED(130), toFIXED(95)},    // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(156), toFIXED(50)},                // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHon,
    .zmode = _ZmCC
};

Sprite character_portrait = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(90)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite dead = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(85)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite menu_bg2 = {
    .pos = {toFIXED(0), toFIXED(130), toFIXED(95)},    // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(156), toFIXED(50)},                // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHon,
    .zmode = _ZmCC
};

Sprite player_bg = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(80)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(90), toFIXED(52)},                 // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHon,
    .zmode = _ZmLC
};

Sprite player_cursor = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(91)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(4), toFIXED(4)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite timer = {
    .pos = {toFIXED(0), toFIXED(-210), toFIXED(80)},   // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCT
};

Sprite meter = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(80)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(20), toFIXED(4)},                  // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmLC
};

Sprite heart = {
    .pos = {toFIXED(-100), toFIXED(-150), toFIXED(80)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                  // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite star = {
    .pos = {toFIXED(100), toFIXED(-150), toFIXED(80)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                  // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

// PLAYER SHIELDS
Sprite shield[MAX_PLAYERS] = {
    {
        .scl = {toFIXED(2), toFIXED(2)},                     // Scale x, y
        .rot = {0, 0, 0},                                    // Rotation x, y, z
        .spr_id = 0,                                         // Sprite ID
        .visible = true,                                     
        .mesh = MESHoff,
        .zmode = _ZmCC,
    }
};

Sprite goal[MAX_PLAYERS] = {
    {
        .scl = {toFIXED(2), toFIXED(2)},                     // Scale x, y
        .rot = {0, 0, 0},                                    // Rotation x, y, z
        .spr_id = 0,                                         // Sprite ID
        .visible = true,                                     
        .mesh = MESHoff,
    }
};

// CHARACTER ELEMENTS
Sprite pixel_poppy = {
    .isColliding = false,
    .pos = {toFIXED(0), toFIXED(0), toFIXED(100)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.0), toFIXED(1.1)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

// CHARACTER ELEMENTS
Sprite pixel_poppy_shadow = {
    .isColliding = false,
    .pos = {toFIXED(0), toFIXED(0), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.0), toFIXED(1.0)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite macchi = {
    .isColliding = false,
    .pos = {toFIXED(-346), toFIXED(0), toFIXED(110)},  // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite jelly = {
    .isColliding = false,
    .pos = {toFIXED(-346), toFIXED(0), toFIXED(110)},   // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite penny = {
    .isColliding = false,
    .pos = {toFIXED(-346), toFIXED(100), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite potter = {
    .isColliding = false,
    .pos = {toFIXED(346), toFIXED(100), toFIXED(110)}, // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite sparta = {
    .isColliding = false,
    .pos = {toFIXED(-346), toFIXED(-100), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                    // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                   // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                   // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                    // Velocity (x and y initialized to 0)
    .spr_id = 0,                                        // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 2,                                        // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite poppy = {
    .isColliding = false,
    .pos = {toFIXED(346), toFIXED(-100), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 2,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite tj = {
    .isColliding = false,
    .pos = {toFIXED(346), toFIXED(-100), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 2,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite george = {
    .isColliding = false,
    .pos = {toFIXED(346), toFIXED(-100), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 2,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite wuppy = {
    .isColliding = false,
    .pos = {toFIXED(346), toFIXED(-100), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 3,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite stadler = {
    .pos = {toFIXED(-346), toFIXED(200), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2.0), toFIXED(2.0)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 3,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite garfield = {
    .pos = {toFIXED(346), toFIXED(-200), toFIXED(110)},// Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2.0), toFIXED(2.0)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 3,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite paw_blank = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(95)},      // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)},                   // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

// CHARACTER ELEMENTS
Sprite bomb_item = {
    .isColliding = false,
    .pos = {toFIXED(-100), toFIXED(-100), toFIXED(100)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2.0), toFIXED(2.0)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

// CHARACTER ELEMENTS
Sprite fishtank_item = {
    .isColliding = false,
    .pos = {toFIXED(-100), toFIXED(100), toFIXED(100)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2.0), toFIXED(2.0)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 2,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

// CHARACTER ELEMENTS
Sprite shroom_item = {
    .isColliding = false,
    .pos = {toFIXED(100), toFIXED(100), toFIXED(100)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2.0), toFIXED(2.0)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 2,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

// CHARACTER ELEMENTS
Sprite craig_item = {
    .isColliding = false,
    .pos = {toFIXED(100), toFIXED(-100), toFIXED(100)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.6), toFIXED(1.5)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 3,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

// CHARACTER ELEMENTS
Sprite garfield_item = {
    .isColliding = false,
    .pos = {toFIXED(-175), toFIXED(0), toFIXED(100)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.1), toFIXED(1.0)},               // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .visible = true,       
    .pal_id = 3,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

void ball_animation_reset(Sprite *ball) {
    Uint8 spin = ABS(ball->vel.z);
    if (spin > 50) {
        ball->spr_id = ball->anim1.asset[6];
    }
    else if (spin > 40 && spin <= 50) {
        ball->spr_id = ball->anim1.asset[5];
    }
    else if (spin > 30 && spin <= 40) {
        ball->spr_id = ball->anim1.asset[4];
    }
    else if (spin > 20 && spin <= 30) {
        ball->spr_id = ball->anim1.asset[3];
    }
    else if (spin > 10 && spin <= 20) {
        ball->spr_id = ball->anim1.asset[1];
    }
    else if (spin > 0 && spin <= 10) {
        ball->spr_id = ball->anim1.asset[0];
    }
    ball->anim2.frame = 0;
}

void drawGoalSprites(Sprite *sprite, int sprite_id, int shadow_id, int zmode, int flip, int x, int y, int scale_y)
{
    sprite->spr_id = sprite->anim1.asset[sprite_id];
    sprite->zmode = zmode;
    sprite->flip = flip;
    set_spr_position(sprite, x, y, 120);
    set_spr_scale(sprite, 2, scale_y);
    my_sprite_draw(sprite);

    sprite->spr_id = sprite->anim1.asset[shadow_id];
    set_spr_position(sprite, x + 2, y + 2, 125);
    my_sprite_draw(sprite);
}

void resetSpriteColors(void) {
    reset_sprites();
    do_update_All = true;
    updateAllColors();
    updateAllPalette();
}