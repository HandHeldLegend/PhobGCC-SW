#ifndef VARIABLES_H
#define VARIABLES_H

#include "structsAndEnums.h"

extern Buttons _btn;

extern Buttons _hardware;

extern RawStick _raw;

extern StickParams _aStickParams;

extern StickParams _cStickParams;

//ADC reference values for Phob 1.0
extern float _ADCScale;
extern float _ADCScaleFactor;

extern int _rumblePower;

extern float _dT;

#endif //VARIABLES_H
