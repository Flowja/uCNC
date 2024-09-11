//
// Created by flow on 2023/8/12.
//
#include "../../cnc.h"
#include "median_average.h"
#include "math.h"

uint16_t sense_valueBuffer[12];
uint16_t aa;
bool getmAMP(void *args)
{
    static volatile uint8_t cnt;
    if (cnt < 12)
    {
        sense_valueBuffer[cnt] = io_get_pinvalue(SENSORPIN_AMPS);
        cnt++;
        return EVENT_CONTINUE;
    }
    else
    {
        cnt = 0;
        uint16_t value;

        value = mdAVFilter(sense_valueBuffer);
        // 采样电阻0.1欧姆，ina199a3放大倍数200，vin=(adcvalue/4095)*vref,0.1*200*I=(adcvalue/4095)*3.3,I=adcvalue*0.165/4095安=adcvalue*165/4095毫安
        aa = (uint16_t)round((double)value * 165 / 4095);
    }
    return EVENT_CONTINUE;
}

CREATE_EVENT_LISTENER(cnc_dotasks, getmAMP);

bool grblhal_sensor_send_status(void *args)
{
    // protocol_send_string(__romstr__("MSG:"));
    protocol_send_string(__romstr__("AMP:"));
    serial_print_int(aa);

    return EVENT_CONTINUE;
}
CREATE_EVENT_LISTENER(protocol_send_status, grblhal_sensor_send_status);

DECL_MODULE(sensor_amps)
{
#ifdef ENABLE_MAIN_LOOP_MODULES
    ADD_EVENT_LISTENER(cnc_dotasks, getmAMP);
    ADD_EVENT_LISTENER(protocol_send_status, grblhal_sensor_send_status);
#else
#warning "Main loop extensions are not enabled. ."
#endif
}