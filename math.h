// math.h
#ifndef MATH_H
#define MATH_H

#include <jo/jo.h>

extern FIXED FIXED_510;
extern FIXED FIXED_255;
extern FIXED FIXED_127;

static  __jo_force_inline void      my_fixed_3d_translate_matrix(jo_pos3D_fixed *p)
{
	slTranslate(p->x, p->y, p->z);
}

static  __jo_force_inline void      my_fixed_3d_rotate_matrix_rad(jo_rot3D_fixed *r)
{
	slRotX(RADtoANG(jo_fixed2float(r->rx)));
	slRotY(RADtoANG(jo_fixed2float(r->ry)));
	slRotZ(RADtoANG(jo_fixed2float(r->rz)));
}

// from danny duarte
Uint32 ApproximateIntegerSqrt(Uint32 value);

Uint8 RandomInRange(Uint8 from, Uint8 to);

Uint8 FastRandom();

#endif // MATH_H