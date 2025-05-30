// math.h
#ifndef MATH_H
#define MATH_H

#include <jo/jo.h>

#define FIXED_HALF toFIXED(0.5)
#define FIXED_0 toFIXED(0)
#define FIXED_1 toFIXED(1)
#define FIXED_2 toFIXED(2)
#define FIXED_4 toFIXED(4)
#define FIXED_8 toFIXED(8)
#define FIXED_100 toFIXED(100)
#define FIXED_127 toFIXED(127)
#define FIXED_255 toFIXED(255)

// these could be useful, but they aren't currently used
// static  __jo_force_inline void      my_fixed_3d_translate_matrix(jo_pos3D_fixed *p)
// {
	// slTranslate(p->x, p->y, p->z);
// }

// static  __jo_force_inline void      my_fixed_3d_rotate_matrix_rad(jo_rot3D_fixed *r)
// {
	// slRotX(RADtoANG(jo_fixed2float(r->rx)));
	// slRotY(RADtoANG(jo_fixed2float(r->ry)));
	// slRotZ(RADtoANG(jo_fixed2float(r->rz)));
// }

static inline FIXED my_fixed_clamp(FIXED value, FIXED min, FIXED max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// from danny duarte
Uint32 ApproximateIntegerSqrt(Uint32 value);

int my_random_range(int min, int max);

FIXED my_fixed_sine(int angle);
#endif // MATH_H