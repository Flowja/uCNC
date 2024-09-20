//
// Created by flow on 2023/8/12.
//
#include "../../cnc.h"

bool buttonLight(void *args)
{
    static uint32_t debounce = 0;
    static uint8_t key_laserfire;
    if ((cnc_get_exec_state(EXEC_JOG) == EXEC_JOG) || (cnc_get_exec_state(EXEC_ALLACTIVE) == EXEC_IDLE))
    {
        if (!io_get_input(LASER_FIREONCE))
        {
            if (!key_laserfire)
            {
                if (mcu_millis() - debounce > 10)
                {
                    key_laserfire = 1;
                    tool_set_speed(1000);
                    debounce = mcu_millis();
                }
                else
                    key_laserfire = 0;
            }
        }
        else if (key_laserfire)
        {
            tool_set_speed(0);
            key_laserfire = 0;
        }
        else
            debounce = mcu_millis();
    }
    // static uint32_t debounce = 0;
    // if (cnc_get_exec_state(EXEC_ALARM) || cnc_has_alarm())
    // {
    //     if (mcu_millis() - debounce > 1000)
    //     {
    //         debounce = mcu_millis();
    //         io_toggle_output(STOP_LIGHT);
    //     }
    // }
    // else
    if (cnc_get_exec_state(EXEC_RUN | EXEC_JOG | EXEC_HOMING))
    {
        io_set_output(RUN_LIGHT);
        io_clear_output(STOP_LIGHT);
    }
    else
    {
        io_set_output(STOP_LIGHT);
        io_clear_output(RUN_LIGHT);
    }

    return EVENT_CONTINUE;
}
CREATE_EVENT_LISTENER(cnc_dotasks, buttonLight);

DECL_MODULE(button_light)
{
#ifdef ENABLE_MAIN_LOOP_MODULES
    ADD_EVENT_LISTENER(cnc_dotasks, buttonLight);
#else
#warning "Main loop extensions are not enabled. ."
#endif
}