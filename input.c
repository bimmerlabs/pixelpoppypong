#include "input.h"
#include "main.h"
#include "state.h"
#include "gameplay.h"

#define MOVEMENT_SPEED toFIXED(3)

void    demo_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        transitionState(GAME_STATE_TITLE_SCREEN);
        g_DemoTimer = 0;
    }
}

void    my_input(void)	{
    if(g_Game.isPaused == true)
    {
        return;
    }
    if (start_timer) {
        if (jo_is_pad1_key_pressed(JO_KEY_UP)) {
            macchi.pos.y -= MOVEMENT_SPEED;
        }
        else if (jo_is_pad1_key_pressed(JO_KEY_DOWN)) {
            macchi.pos.y += MOVEMENT_SPEED;
        }	
        if (jo_is_pad1_key_pressed(JO_KEY_LEFT)) {
            macchi.pos.x -= MOVEMENT_SPEED;
        }
        else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT)) {
            macchi.pos.x += MOVEMENT_SPEED;
        }
    }
    // if (jo_is_pad1_key_pressed(JO_KEY_L) && _light1->z > JO_FIXED_0) {
        // _light1->z -= MOVEMENT_SPEED;
        // move_light = true;
        // do_update = true;
    // }
    // if (jo_is_pad1_key_pressed(JO_KEY_R) && _light1->z < FIXED_255) {
        // _light1->z += MOVEMENT_SPEED;
        // move_light = true;
        // do_update = true;
    // }
    // if (jo_is_pad1_key_down(JO_KEY_START)) {
        // g_Game.isPaused = !g_Game.isPaused;
    // }
    // // hue
    if (jo_is_pad1_key_down(JO_KEY_A)) {
        start_timer = !start_timer;
    }
    // else if (jo_is_pad1_key_pressed(JO_KEY_X)) {
        // _increment->h += 1;
    // do_update = true;
    // }
    // // saturation
    // if (jo_is_pad1_key_pressed(JO_KEY_B) && _hslPal->color[_attr->max_sat_id].s > 0) {
        // _increment->s -= 1;
    // do_update = true;
    // }
    // else if (jo_is_pad1_key_pressed(JO_KEY_Y) && _hslPal->color[_attr->min_sat_id].s < 255) {
        // _increment->s += 1;
    // do_update = true;
    // }
    // // other
    // if (jo_is_pad1_key_pressed(JO_KEY_Z)) {
        // ball_rotation += 1;
    // }
    // // other
    // if (jo_is_pad1_key_down(JO_KEY_C)) {
        // ball_rotation += 45;
    // }
}