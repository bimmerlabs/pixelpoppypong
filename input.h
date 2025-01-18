#pragma once

#define MAX_INPUTS (12)
#define SENSITIVITY toFIXED(0.1)
#define ANALOG_MIN toFIXED(0.02)
#define ANALOG_MAX toFIXED(0.2)

// Saturn inputs
typedef struct _INPUT
{
	Uint8 id;
	FIXED axis_x;
	FIXED axis_y;
	FIXED left_trigger;
	FIXED right_trigger;
	FIXED sensitivity;
	bool isAnalog;
	bool isSelected;
} INPUT, *PINPUT;

extern INPUT g_Inputs[MAX_INPUTS];

void init_inputs(void);
void reset_inputs(void);
void check_inputs(void);

void analogAdjustmentScreen_input(void);
void analogAdjustmentScreen_draw(int input_x, int options_y);