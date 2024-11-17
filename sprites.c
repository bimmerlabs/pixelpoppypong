#include "sprites.h"

Sprite cursor = {
    .pos = {toFIXED(-147), toFIXED(0), toFIXED(90)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};

Sprite menu_text = {
    .pos = {toFIXED(0), toFIXED(90), toFIXED(90)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(.7), toFIXED(.7)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};

Sprite menu_bg = {
    .pos = {toFIXED(0), toFIXED(130), toFIXED(95)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(78), toFIXED(50)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHon,
    .zmode = _ZmCC
};

Sprite timer_num1 = {
    .pos = {toFIXED(0), toFIXED(-215), toFIXED(80)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmLT
};

Sprite timer_num10 = {
    .pos = {toFIXED(0), toFIXED(-215), toFIXED(80)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmRT
};

Sprite poppy = {
    .pos = {toFIXED(0), toFIXED(0), toFIXED(100)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.0), toFIXED(1.1)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmCC
};

Sprite macchi = {
    .pos = {toFIXED(-350), toFIXED(0), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmLC
};

Sprite jelly = {
    .pos = {toFIXED(350), toFIXED(0), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2), toFIXED(2)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 0,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};

Sprite penny = {
    .pos = {toFIXED(-348), toFIXED(100), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.7), toFIXED(1.7)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmLC
};

Sprite potter = {
    .pos = {toFIXED(348), toFIXED(100), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.7), toFIXED(1.7)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 1,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};

Sprite sparta = {
    .pos = {toFIXED(-346), toFIXED(-100), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.7), toFIXED(1.7)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 2,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmLC
};

Sprite player1 = {
    .pos = {toFIXED(346), toFIXED(-100), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(1.7), toFIXED(1.7)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 2,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};

Sprite boss1 = {
    .pos = {toFIXED(-344), toFIXED(200), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2.0), toFIXED(2.0)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 3,                                       // Palette ID initialized to 0
    .flip = sprNoflip,
    .mesh = MESHoff,
    .zmode = _ZmLC
};

Sprite boss2 = {
    .pos = {toFIXED(344), toFIXED(-200), toFIXED(110)},     // Position x, y, z (all initialized to 0)
    .scl = {toFIXED(2.0), toFIXED(2.0)}, // Scale x, y, z (all initialized to 1)
    .rot = {0, 0, 0},                                  // Rotation x, y, z (all initialized to 0 ANGLE)
    .vec2 = {JO_FIXED_0, JO_FIXED_0},                  // 2D Vector (x and y initialized to 0)
    .vel = {JO_FIXED_0, JO_FIXED_0},                   // Velocity (x and y initialized to 0)
    .spr_id = 0,                                       // Sprite ID initialized to 0
    .pal_id = 3,                                       // Palette ID initialized to 0
    .flip = sprHflip,
    .mesh = MESHoff,
    .zmode = _ZmRC
};


//			    SPR_ATTRIBUTE   tex           col            gouraud      mode       dir
//SPR_ATTR attr = SPR_ATTRIBUTE( spr_id, pal_id, No_Gouraud , CL256Bnk , sprNoflip | _ZmCC );

void	my_sprite_draw(Sprite *sprite) {
	FIXED pos[XYZSS] = { sprite->pos.x, sprite->pos.y, sprite->pos.z, sprite->scl.x, sprite->scl.y };
	SPR_ATTR attr = SPR_ATTRIBUTE( sprite->spr_id, JO_MULT_BY_64(sprite->pal_id), No_Gouraud, sprite->mesh | ECdis | CL64Bnk, sprite->flip | sprite->zmode );
	slDispSpriteHV(pos, &attr, DEGtoANG(sprite->rot.z));
}