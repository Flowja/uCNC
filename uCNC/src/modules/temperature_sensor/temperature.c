//
// Created by flow on 2023/8/11.
//
#include "../../cnc.h"
#include "../AMPS/median_average.h"
#include "ntc_table.h"

typedef enum
{
    STATE_READING,
    STATE_PROCESSING
} State;
uint16_t T1_valueBuffer[12];
uint16_t T2_valueBuffer[12];
static uint32_t debounce = 0;
float t1 = 0;
float t2 = 0;
static volatile uint8_t t_turn = 0;
// uint32_t timestamp ;
volatile uint32_t nextsample = 0;
static volatile uint8_t coolant_flood_started;
static State currentState = STATE_READING;
bool getTemp(void *args)
{
    static volatile uint8_t cnt;
    uint32_t value;
    uint32_t current_time = mcu_millis();

    if (tool_get_speed())
    {
        if (io_get_input(WATER_FLOW_SWITCHSENSOR))
        {
            if (mcu_millis() - debounce > 100)
            {
                debounce = mcu_millis();
                /*Alarm*/
                cnc_alarm(EXEC_ALARM_LASERTUBE_NOTFLOOD);
            }
        }
    }

    if (current_time < nextsample)
    {
        return EVENT_CONTINUE;
    }

    if (currentState == STATE_READING)
    {
        if (!t_turn)
        {
            if (cnt < 12)
            {
                T1_valueBuffer[cnt] = io_get_pinvalue(SENSORPIN_T1);
                cnt++;
                return EVENT_CONTINUE; // 提前返回，避免进入处理状态
            }
            else
            {
                currentState = STATE_PROCESSING; // 切换到数据处理状态
            }
        }
        else
        {
            if (cnt < 12)
            {
                T2_valueBuffer[cnt] = io_get_pinvalue(SENSORPIN_T2);
                cnt++;
                return EVENT_CONTINUE; // 提前返回，避免进入处理状态
            }
            else
            {
                currentState = STATE_PROCESSING; // 切换到数据处理状态
            }
        }
    }
    if (currentState == STATE_PROCESSING)
    {
        if (!t_turn)
        {
            // 处理传感器 T1 的数据
            value = mdAVFilter(T1_valueBuffer);
            value = value << 6;
            t1 = 0.1f * calc_temperature(value);
            // cnt = 0;
            t_turn = 1; // 切换到传感器 T2
        }
        else
        {
            // 处理传感器 T2 的数据
            value = mdAVFilter(T2_valueBuffer);
            value = value << 6;
            t2 = 0.1f * calc_temperature(value);
            t_turn = 0; // 切换到传感器 T1
        }
        cnt = 0; // 在所有处理完成后重置 cnt
                 // timestamp = mcu_millis();
        nextsample = mcu_millis() + 1000UL;
        currentState = STATE_READING; // 切换回数据读取状态
    }

    if (t2 >= 35)
    {
        if (!io_get_output(LASER_COOLANT_FLOOD))
        {
            io_set_output(LASER_COOLANT_FLOOD);
            coolant_flood_started = 1;
        }
    }
    if (coolant_flood_started && t2 <= 30)
    {
        if (io_get_output(LASER_COOLANT_FLOOD))
        {
            io_clear_output(LASER_COOLANT_FLOOD);
            coolant_flood_started = 0;
        }
    }
    if (t2 >= 38)
    {
        if (tool_get_setpoint() != 0)
        {
            /*Alarm*/
            cnc_alarm(EXEC_ALARM_LASER_OVER_TEMPERATURE);
        }
    }

    return EVENT_CONTINUE;
}
CREATE_EVENT_LISTENER(cnc_dotasks, getTemp);

bool grblhal_temp_send_status(void *args)
{
    protocol_send_string("T1:");
    serial_print_flt(t1);
    protocol_send_string("T2:");
    serial_print_flt(t2);

    return EVENT_CONTINUE;
}
CREATE_EVENT_LISTENER(protocol_send_status, grblhal_temp_send_status);

DECL_MODULE(sensor_temperature)
{
#ifdef ENABLE_MAIN_LOOP_MODULES
    ADD_EVENT_LISTENER(cnc_dotasks, getTemp);
    ADD_EVENT_LISTENER(protocol_send_status, grblhal_temp_send_status);
#else
#warning "Main loop extensions are not enabled. ."
#endif
}
