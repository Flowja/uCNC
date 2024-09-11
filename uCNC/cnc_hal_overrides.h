#ifndef CNC_HAL_OVERRIDES_H
#define CNC_HAL_OVERRIDES_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "cnc_hal_reset.h"
#define ENABLE_COOLANT
#define S_CURVE_ACCELERATION_LEVEL 0
#define BRESENHAM_16BIT
#define ESTOP_PULLUP_ENABLE
#define SAFETY_DOOR_PULLUP_ENABLE
#define FHOLD_PULLUP_ENABLE
#define CS_RES_PULLUP_ENABLE
#define LIMIT_X_PULLUP_ENABLE
#define LIMIT_Y_PULLUP_ENABLE
#define LIMIT_Z_PULLUP_ENABLE
#define LIMIT_X2_PULLUP_ENABLE
#define LIMIT_Y2_PULLUP_ENABLE
#define LIMIT_Z2_PULLUP_ENABLE
#define DISABLE_PROBE
#define TOOL1 laser_pwm
#define LASER_PWM PWM0
#define LASER_FREQ 8000
// #define ENABLE_LASER_PPI
// #define LASER_PPI PWM0
#define ENCODERS 0
#define ENABLE_EXTRA_SYSTEM_CMDS
#define ENABLE_MAIN_LOOP_MODULES
    // #define ENABLE_MULTISTREAM_GUARD
    // #define ENABLE_IO_MODULES
    // #define ENABLE_TONE_SPEAKER
#define ENABLE_PARSER_MODULES
    // Custom configurations

#ifdef __cplusplus
}
#endif
#endif
