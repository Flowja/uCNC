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
    // static volatile uint8_t t_turn = 0;
    // static volatile uint32_t timestamp = 0;
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

                /*Error*/
                // cnc_call_rt_command(CMD_CODE_FEED_HOLD);
                // serial_clear();
                // protocol_send_error(STATUS_LASERTUBE_NOTFLOOD);
                // // serial_print_str("error:61");
                // // serial_putc('\n');
                // tool_set_speed(0);
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
    // }
    // if (cnt < 12)
    // {
    //     T1_valueBuffer[cnt] = io_get_pinvalue(SENSORPIN_T1);
    //     T2_valueBuffer[cnt] = io_get_pinvalue(SENSORPIN_T2);
    //     cnt++;
    //     return EVENT_CONTINUE;
    // }
    // else
    // {
    //     value = mdAVFilter(T1_valueBuffer);
    //     value = value << 6;
    //     t1 = 0.1f * (float)calc_temperature(value);
    //     value = mdAVFilter(T2_valueBuffer);
    //     value = value << 6;
    //     t2 = 0.1f * (float)calc_temperature(value);
    //     cnt = 0;
    // }

    // if (mcu_millis() - timestamp > 1000)
    // {
    //     if (!t_turn)
    //     {
    //         if (cnt1 < 12)
    //         {
    //             T1_valueBuffer[cnt1] = io_get_pinvalue(SENSORPIN_T1);
    //             // serial_print_int(T1_valueBuffer[cnt]);
    //             // serial_print_str("/");
    //             cnt1++;
    //             return EVENT_CONTINUE;
    //         }
    //         else
    //         {
    //             // cnt = 0; // 先清0否则求平均值有干扰，会多次进入此else
    //             // __ATOMIC__
    //             // {
    //             serial_print_str("T1=");
    //             for (uint8_t ia = 0; ia < 12; ia++)
    //             {

    //                 serial_print_int(T1_valueBuffer[ia]);
    //                 serial_print_str(",");
    //             }
    //             serial_print_str("\n");
    //             value = mdAVFilter(T1_valueBuffer);
    //             // value= T1_valueBuffer[6];
    //             // }
    //             // serial_print_str("|");
    //             // serial_print_int(value);
    //             value = value << 6;
    //             t1 = 0.1f * (float)calc_temperature(value);
    //             // serial_print_flt(t1);
    //             cnt1 = 0;
    //             serial_print_str("cntt1=");
    //             serial_print_int(cnt1);
    //             serial_print_str("\n");
    //             t_turn = 1;
    //             timestamp = mcu_millis();
    //         }
    //     }
    //     else
    //     {
    //         serial_print_str("cntt2=");
    //         serial_print_int(cnt2);
    //         serial_print_str("\n");
    //         if (cnt2 < 12)
    //         {
    //             T2_valueBuffer[cnt2] = io_get_pinvalue(SENSORPIN_T2);
    //             cnt2++;
    //             return EVENT_CONTINUE;
    //         }
    //         else
    //         {
    //             // cnt = 0; // 先清0否则求平均值有干扰，会多次进入此else
    //             // __ATOMIC__
    //             // {
    //                 // serial_print_str("T2=");
    //                 // for (uint8_t ia = 0; ia < 12; ia++)
    //                 // {

    //                 //     serial_print_int(T2_valueBuffer[ia]);
    //                 //     serial_print_str(",");
    //                 // }
    //                 // serial_print_str("\n");
    //                 value = mdAVFilter(T2_valueBuffer);
    //                 // }
    //                 // value = T2_valueBuffer[6];
    //                 value = value << 6;
    //                 t2 = 0.1f * (float)calc_temperature(value);
    //                 cnt2 = 0;
    //                 // serial_print_str("cnt2=");
    //                 // serial_print_int(cnt2);
    //                 t_turn = 0;
    //                 timestamp = mcu_millis();
    //             // }
    //         }
    //     }
    // }
    // // else if (cnt < 24)
    // // {
    // //     T2_valueBuffer[cnt - 12] = io_get_pinvalue(SENSORPIN_T2);
    // //     if (cnt == 12)
    // //     {
    // //         value = mdAVFilter(T1_valueBuffer);
    // //         value = value << 6;
    // //         t1 = 0.1f * (float)calc_temperature(value);
    // //     }
    // //     cnt++;
    // //     return EVENT_CONTINUE;
    // // }
    // else
    // {        return EVENT_CONTINUE;
    // }
    // cnt = 0;
    // value1 = mdAVFilter(T1_valueBuffer);
    // value1 = value1 << 6;
    // t1 = 0.1f * (float)calc_temperature(value1);
    // value = mdAVFilter(T1_valueBuffer);
    //     value = value << 6;
    //     t1 = 0.1f * (float)calc_temperature(value);
    // value = mdAVFilter(T2_valueBuffer);
    // value = value << 6;
    // t2 = 0.1f * (float)calc_temperature(value);
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

            /*Error*/
            // cnc_call_rt_command(CMD_CODE_FEED_HOLD);
            // serial_clear();
            // protocol_send_error(STATUS_LASER_OVER_TEMPERATURE);
            // // serial_print_str("error:60");
            // // serial_putc('\n');
            // tool_set_speed(0);
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
