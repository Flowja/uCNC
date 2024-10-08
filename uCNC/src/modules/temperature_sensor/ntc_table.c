#include "ntc_table.h"
/*
https://github.com/Egoruch/NTC-STM32-HAL
https://aterlux.ru/article/ntcresistor-en

/* Table of ADC sum value, corresponding to temperature. Starting from higher value to lower.
   Next parameters had been used to build table:
     R1(T1): 50kOhm(25°С)
     R2(T2): 3.2334kOhm(100°С)
     Scheme: A
     Ra: 10kOhm
     Multiplier ADC result: 64
     U0/Uref: 3.3V/3.3V
*/
const temperature_table_entry_type termo_table[] PROGMEM = {
    257572, 257280, 256972, 256647, 256305, 255945, 255567, 255169,
    254752, 254313, 253853, 253371, 252866, 252336, 251782, 251203,
    250598, 249965, 249305, 248615, 247897, 247148, 246368, 245557,
    244713, 243835, 242924, 241978, 240997, 239980, 238926, 237835,
    236707, 235540, 234335, 233090, 231807, 230483, 229120, 227717,
    226273, 224790, 223266, 221702, 220097, 218453, 216770, 215047,
    213286, 211486, 209649, 207775, 205865, 203919, 201939, 199925,
    197879, 195802, 193694, 191558, 189394, 187204, 184989, 182750,
    180490, 178210, 175911, 173595, 171263, 168918, 166561, 164194,
    161818, 159435, 157047, 154656, 152263, 149871, 147480, 145092,
    142709, 140333, 137965, 135606, 133259, 130924, 128603, 126297,
    124007, 121736, 119483, 117250, 115037, 112847, 110680, 108536,
    106417, 104323, 102255, 100214, 98200, 96213, 94255, 92325,
    90424, 88553, 86710, 84898, 83115, 81362, 79639, 77946,
    76283, 74651, 73048, 71475, 69931, 68418, 66933, 65478,
    64051, 62653, 61284, 59943, 58629, 57343, 56084, 54851,
    53645, 52465, 51310, 50181, 49076, 47996, 46940, 45907,
    44898, 43911, 42946, 42004, 41083, 40183, 39303, 38444,
    37605, 36785};

// This function is calculating temperature in tenth of degree of Celsius
// depending on ADC sum value as input parameter.
int16_t calc_temperature(temperature_table_entry_type adcsum)
{
    temperature_table_index_type l = 0;
    temperature_table_index_type r = (sizeof(termo_table) / sizeof(termo_table[0])) - 1;
    temperature_table_entry_type thigh = TEMPERATURE_TABLE_READ(r);

    // Checking for bound values
    if (adcsum <= thigh)
    {
#ifdef TEMPERATURE_UNDER
        if (adcsum < thigh)
            return TEMPERATURE_UNDER;
#endif
        return TEMPERATURE_TABLE_STEP * r + TEMPERATURE_TABLE_START;
    }
    temperature_table_entry_type tlow = TEMPERATURE_TABLE_READ(0);
    if (adcsum >= tlow)
    {
#ifdef TEMPERATURE_OVER
        if (adcsum > tlow)
            return TEMPERATURE_OVER;
#endif
        return TEMPERATURE_TABLE_START;
    }

    // Table lookup using binary search
    while ((r - l) > 1)
    {
        temperature_table_index_type m = (l + r) >> 1;
        temperature_table_entry_type mid = TEMPERATURE_TABLE_READ(m);
        if (adcsum > mid)
        {
            r = m;
        }
        else
        {
            l = m;
        }
    }
    temperature_table_entry_type vl = TEMPERATURE_TABLE_READ(l);
    if (adcsum >= vl)
    {
        return l * TEMPERATURE_TABLE_STEP + TEMPERATURE_TABLE_START;
    }
    temperature_table_entry_type vr = TEMPERATURE_TABLE_READ(r);
    temperature_table_entry_type vd = vl - vr;
    int16_t res = TEMPERATURE_TABLE_START + r * TEMPERATURE_TABLE_STEP;
    if (vd)
    {
        // Linear interpolation
        res -= ((TEMPERATURE_TABLE_STEP * (int32_t)(adcsum - vr) + (vd >> 1)) / vd);
    }
    return res;
}