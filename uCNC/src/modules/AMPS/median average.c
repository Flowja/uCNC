//
// Created by flow on 2023/8/8.
#include "median_average.h"
// 中位值平均滤波法（防脉冲干扰平均滤波法）
#define ARRAY_SIZE 12
uint16_t mdAVFilter(uint16_t *value_buf)
{
    volatile uint8_t i, j, count;
    // volatile uint8_t N = ARRAY_SIZE;
    volatile uint32_t sum = 0;
    volatile uint16_t temp, result;
    volatile uint16_t arrary_temp[ARRAY_SIZE];

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        arrary_temp[i] = value_buf[i]; // 把数组中的值给临时数组。对临时数组进行排序
    }

    /*问题分析，这里如果对原始数据进行排序的话，因为数组是先入先出。所以新来的数据会把最小的值级覆盖掉。
    这里的排序算法是没有问题的。也就是说每取一个数都要进行排序，计算量比较大。
    */
    for (j = 0; j < ARRAY_SIZE - 1; j++)
    {
        for (i = 0; i < ARRAY_SIZE - j - 1; i++)
        {
            if (arrary_temp[i] > arrary_temp[i + 1])
            {
                temp = arrary_temp[i];
                arrary_temp[i] = arrary_temp[i + 1];
                arrary_temp[i + 1] = temp;
            }
        }
    }

    for (count = 1; count < ARRAY_SIZE - 1; count++) // 这里做了修改
    {
        sum += arrary_temp[count]; // 去掉最大值和最小值
    }
    result = sum / (ARRAY_SIZE - 2);
    return result;
}
//
