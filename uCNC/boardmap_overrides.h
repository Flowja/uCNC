#ifndef BOADMAP_OVERRIDES_H
#define BOADMAP_OVERRIDES_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "boardmap_reset.h"
#define MCU MCU_STM32F1X
// #define MCU MCU_VIRTUAL_WIN
#define KINEMATIC KINEMATIC_CARTESIAN
#define AXIS_COUNT 3
#define TOOL_COUNT 1
#define BAUDRATE 115200
#define BOARD BOARD_BLUEPILL
#define BOARD_NAME "STM32 Bluepill"
// #define BOARD BOARD_VIRTUAL
// #define BOARD_NAME "BOARD_VIRTUAL"
// #define UART_PORT 1
// #define UART_PORT 2
#define ITP_TIMER 1
#define SERVO_TIMER 4
#define ONESHOT_TIMER 3
#define STEP0_BIT 0
#define STEP0_PORT B
#define STEP1_BIT 1
#define STEP1_PORT B
#define STEP2_BIT 2
#define STEP2_PORT B
#define DIR0_BIT 5
#define DIR0_PORT A
#define DIR1_BIT 6
#define DIR1_PORT A
#define DIR2_BIT 7
#define DIR2_PORT A
#define STEP0_EN_BIT 12
#define STEP0_EN_PORT B
#define PWM0_BIT 10
#define PWM0_PORT B
#define PWM0_CHANNEL 3
#define PWM0_TIMER 2
#define PWM1_BIT 11
#define PWM1_PORT B
#define PWM1_CHANNEL 4
#define PWM1_TIMER 2
#define DOUT0_BIT 13
#define DOUT0_PORT B
#define DOUT1_BIT 8
#define DOUT1_PORT A
#define DOUT2_BIT 14
#define DOUT2_PORT B
#define DOUT3_BIT 15
#define DOUT3_PORT B
#define LIMIT_X_BIT 15
#define LIMIT_X_PORT A
#define LIMIT_X_ISR
#define LIMIT_Y_BIT 4
#define LIMIT_Y_PORT B
#define LIMIT_Y_ISR
#define LIMIT_Z_BIT 6
#define LIMIT_Z_PORT B
#define LIMIT_Z_ISR
#define LIMIT_X2_BIT 3
#define LIMIT_X2_PORT B
#define LIMIT_X2_ISR
#define LIMIT_Y2_BIT 5
#define LIMIT_Y2_PORT B
#define LIMIT_Y2_ISR
#define LIMIT_Z2_BIT 7
#define LIMIT_Z2_PORT B
#define LIMIT_Z2_ISR
#define ESTOP_BIT 14
#define ESTOP_PORT A
#define ESTOP_ISR
#define SAFETY_DOOR_BIT 9
#define SAFETY_DOOR_PORT A
#define SAFETY_DOOR_ISR
#define FHOLD_BIT 13
#define FHOLD_PORT A
#define FHOLD_ISR
#define CS_RES_BIT 10
#define CS_RES_PORT A
#define CS_RES_ISR
#define ANALOG0_BIT 0
#define ANALOG0_PORT A
#define ANALOG0_CHANNEL 0
#define ANALOG1_BIT 1
#define ANALOG1_PORT A
#define ANALOG1_CHANNEL 1
#define ANALOG2_BIT 2
#define ANALOG2_PORT A
#define ANALOG2_CHANNEL 2
#define ANALOG3_BIT 3
#define ANALOG3_PORT A
#define ANALOG3_CHANNEL 3
#define ANALOG4_BIT 4
#define ANALOG4_PORT A
#define ANALOG4_CHANNEL 4
#define ANALOG6_CHANNEL -1
#define ANALOG7_CHANNEL -1
#define ANALOG8_CHANNEL -1
#define ANALOG9_CHANNEL -1
#define ANALOG10_CHANNEL -1
#define ANALOG11_CHANNEL -1
#define ANALOG12_CHANNEL -1
#define ANALOG13_CHANNEL -1
#define ANALOG14_CHANNEL -1
#define ANALOG15_CHANNEL -1
#define DIN0_BIT 15
#define DIN0_PORT C
#define DIN1_BIT 13
#define DIN1_PORT C
#define DIN2_BIT 14
#define DIN2_PORT C
#define DIN3_BIT 8
#define DIN3_PORT B
#define DIN4_BIT 9
#define DIN4_PORT B
// #define TX_BIT 9
// #define TX_PORT A
// #define RX_BIT 10
// #define RX_PORT A
// #define TX_BIT 2
// #define TX_PORT A
// #define RX_BIT 3
// #define RX_PORT A
#define USB_DM_BIT 11
#define USB_DM_PORT A
#define USB_DP_BIT 12
#define USB_DP_PORT A
#define IC74HC595_COUNT 0

#define SENSORPIN_AMPS ANALOG0
#define JOYSTICK_INPUT_0 ANALOG1
#define JOYSTICK_INPUT_1 ANALOG2
#define SENSORPIN_T1 ANALOG3
#define SENSORPIN_T2 ANALOG4

#define LASER_PWM PWM0
#define LASER_PPI PWM1

#define LASER_FIREONCE DIN0
#define JOYSTICK_Z_UP DIN1
#define JOYSTICK_Z_DOWN DIN2
#define WATER_FLOW_SWITCHSENSOR DIN3
#define JOYSTICK_EN DIN4
    // #define LASER_FIREONCE_MASK 1
    // #define JOYSTICK_Z_UP_MASK 2
    // #define JOYSTICK_Z_DOWN_MASK 4

#define LASER_COOLANT_FLOOD DOUT0
#define RUN_LIGHT DOUT1
#define STOP_LIGHT DOUT3
    // #define LASER_AIR_ASSIT DOUT2          /*defined in laser_pwm.c and laser_ppi.c*/
    // #define SPEAKER_PWM PWM0
    // Custom configurations

#ifdef __cplusplus
}
#endif
#endif
