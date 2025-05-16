#include <jo/jo.h>
#include "../main.h"
#include "input.h"
#include "assets.h"
INPUT g_Inputs[MAX_INPUTS] = {0};
void init_inputs(void) {
    PINPUT input = NULL;
    for(unsigned int i = 0; i < COUNTOF(g_Inputs); i++)
    {
        input = &g_Inputs[i];
        input->id = i;
	input->axis_x = JO_FIXED_0;
	input->axis_y = JO_FIXED_0;
	input->left_trigger = JO_FIXED_0;
	input->right_trigger = JO_FIXED_0;
	input->sensitivity = ANALOG_SENSITIVITY;
        input->isSelected = false;
        input->isAnalog = false;
    }
}

void reset_inputs(void) {
    PINPUT input = NULL;
    for(unsigned int i = 0; i < COUNTOF(g_Inputs); i++)
    {
        input = &g_Inputs[i];
        input->id = i;
	input->axis_x = JO_FIXED_0;
	input->axis_y = JO_FIXED_0;
	input->left_trigger = JO_FIXED_0;
	input->right_trigger = JO_FIXED_0;
        input->isSelected = false;
        input->isAnalog = false;
    }
}
// check to see if a controller is plugged in and what type it is
void check_inputs(void) {
    PINPUT input = NULL;
    for(unsigned int i = 0; i < COUNTOF(g_Inputs); i++)
    {
        input = &g_Inputs[i];  
        if (!jo_is_input_available(i)) {
            continue;
        }
        if (jo_get_input_type(i) == JoNightsPad) { // TODO: handle analog inputs for menu screens
                input->isAnalog = true;
            	// X and Y are centered at 128
		char axis_x_raw = jo_get_input_axis(i, JoAxis1) - 0x80;
		char axis_y_raw = jo_get_input_axis(i, JoAxis2) - 0x80;

		// Left and right trigger start at 0 and go to 255
		unsigned char axis_right_trigger_raw = jo_get_input_axis(i, JoAxis3);
		unsigned char axis_left_trigger_raw  = jo_get_input_axis(i, JoAxis4);

		// Scale input to the size of the screen
		input->axis_x = jo_fixed_mult(input->sensitivity, toFIXED(axis_x_raw));
		input->axis_y = jo_fixed_mult(input->sensitivity, toFIXED(axis_y_raw));
		input->left_trigger  = toFIXED(axis_left_trigger_raw);
		input->right_trigger = toFIXED(axis_right_trigger_raw);
        }
        else {
            input->isAnalog = false;
        }
    }
}
void analogAdjustmentScreen_input(void) {
    PINPUT input = NULL;
    for(unsigned int i = 0; i < COUNTOF(g_Inputs); i++)
    {
        input = &g_Inputs[i];  
        if (!jo_is_input_available(i)) {
            continue;
        }
        if (jo_get_input_type(i) == JoNightsPad) {
            if (jo_is_input_key_down(i, JO_KEY_LEFT) && input->sensitivity > ANALOG_MIN) {
                pcm_play(g_Assets.tickPcm8, PCM_PROTECTED, 6); // tick sound
                input->sensitivity -= toFIXED(0.01);
            }
            if (jo_is_input_key_down(i, JO_KEY_RIGHT) && input->sensitivity < ANALOG_MAX) {
                pcm_play(g_Assets.tickPcm8, PCM_PROTECTED, 6); // tick sound
                input->sensitivity += toFIXED(0.01);
            }
        }
    }
}
void analogAdjustmentScreen_draw(int input_x, int input_y) {
    PINPUT input = NULL;
    input_x += 2;
    for(unsigned int i = 0; i < COUNTOF(g_Inputs); i++)
    {
        input = &g_Inputs[i];  
        if (!jo_is_input_available(i)) {
            continue;
        }
        if (jo_get_input_type(i) == JoNightsPad) {
            jo_nbg0_printf(input_x, input_y, "INPUT %i: - %2i +", i+1, toINT(jo_fixed_mult(input->sensitivity, FIXED_100)));
        }
        else {
            jo_nbg0_printf(input_x, input_y, "INPUT %i: DIGITAL", i+1);
        }
        input_y += 1;
    }
}