#ifndef PHOB_SETTINGS_UTIL_H
#define PHOB_SETTINGS_UTIL_H

#include "phob_types.h"

typedef struct
{
    jumpConfig_t jump;
    int l;
    int r;
    int lOffset;
    int rOffset;
    int cxOffset;
    int cyOffset;
    int xSnapback;
    int ySnapback;
    int xSmoothing;
    int ySmoothing;
    int cxSmoothing;
    int cySmoothing;
    int rumble;
    int autoInit;
    int axWaveshaping;
    int ayWaveshaping;
    int cxWaveshaping;
    int cyWaveshaping;
    float axPoints[32];
    float ayPoints[32];
    float cxPoints[32];
    float cyPoints[32];
    float aAngles[16];
    float cAngles[16];
    intFloat_s uExtras[4];
    intFloat_s dExtras[4];
    intFloat_s lExtras[4];
    intFloat_s rExtras[4];
    int schema;
    int AstickCardinalSnapping;
    int CstickCardinalSnapping;
    int AstickAnalogScaler;
    int CstickAnalogScaler;
    int interlaceOffset;
}  __attribute__((packed)) phobSettings_s;

// Public defined settings utility functions

// Commit settings to flash storage
bool commitSettings(bool noLock);

// Load all settings to struct at pointer location
bool loadAllSettings(phobSettings_s *settings);

/* TRIGGER CONFIG OPTIONS */
/* ---------------------- */

typedef enum
{
    TRIGGER_L,
    TRIGGER_R,
} trigger_t;

typedef enum
{
    TRIGGER_SETTING_PRIMARY,
    TRIGGER_SETTING_OFFSET,
} triggerSetting_t;

int getTriggerSetting(trigger_t trigger, triggerSetting_t setting);
void setTriggerSetting(trigger_t trigger, triggerSetting_t setting, int value);

/* ---------------------- */
/* ---------------------- */

/* ANALOG CONFIG OPTIONS */
/* ---------------------- */

typedef enum
{
    ANALOG_STICK_L,
    ANALOG_STICK_R,
} analogStick_t;

typedef enum
{
    ANALOG_AXIS_X,
    ANALOG_AXIS_Y,
} analogAxis_t;

typedef enum
{
    ANALOG_SETTING_OFFSET,
    ANALOG_SETTING_SMOOTH,
    ANALOG_SETTING_WAVESHAPE,
    ANALOG_SETTING_SNAPBACK,
} analogSetting_t;

int getAnalogSetting(analogStick_t stick, analogAxis_t axis, analogSetting_t setting);
void setAnalogSetting(analogStick_t stick, analogAxis_t axis, analogSetting_t setting, int value);

void getNotchAnglesSetting(float *angles, analogStick_t stick);
void setNotchAnglesSetting(float *angles, analogStick_t stick);

void getPointsSetting(float *points, analogStick_t stick, analogAxis_t axis);
void setPointsSetting(float *points, analogStick_t stick, analogAxis_t axis);

int getCardinalSnappingSetting(analogStick_t stick);
void setCardinalSnappingSetting(analogStick_t stick, int value);

int getAnalogScalerSetting(analogStick_t stick);
void setAnalogScalerSetting(int value);

/* ---------------------- */
/* ---------------------- */

/* JUMP REMAP CONFIG OPTIONS */
/* ---------------------- */

jumpConfig_t getJumpConfig();
void setJumpConfig(jumpConfig_t jumpConfig);

/* ---------------------- */
/* ---------------------- */

#endif