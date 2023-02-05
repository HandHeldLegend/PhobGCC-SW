#ifndef PHOB_TYPES_H
#define PHOB_TYPES_H

typedef struct
{   
    union
    {
        int intValue;
        float floatValue;
    } val;
} intFloat_s;

typedef enum {
	CM_NULL,        //do nothing
	CM_REACTION,    //record immediately
	CM_STICK_RISING,//starting when a stick coord exceeds a threshold distance from center
	CM_STICK_FALLING,//starting when a stick coord falls below a threshold
	CM_STICK_PIVOT, //starting when a stick coord falls below a threshold
	CM_TRIG,        //increasing threshold on triggers
	CM_JUMP,        //x, y, or melee tap jump threshold
	CM_PRESS,       //any button press
    CM_MAXVAL,      // Max value placeholder
} captureMode_t;

typedef enum 
{
	JUMP_CONFIG_DEFAULT,
	JUMP_CONFIG_SWAP_XZ,
	JUMP_CONFIG_SWAP_YZ,
	JUMP_CONFIG_SWAP_XL,
	JUMP_CONFIG_SWAP_YL,
	JUMP_CONFIG_SWAP_XR,
	JUMP_CONFIG_SWAP_YR,
} jumpConfig_t;

#endif