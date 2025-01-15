/**
 * @file        bsp_uart.c
 * @brief
 * @version     V1.0
 * @author      Jsir2022 (jsir2022@outlook.com)
 * @date        2024-02-27 18:57:40
 * LastEditors  Jsir2022
 * LastEditTime 2024-02-27 19:35:18
 * history		版本	日期		作者		说明
 * 				V1.0	202x-x-x	Jsir2022	正式发布
 * Copyright (c) 2024 by Jsir2022, All Rights Reserved.
 * @verbatim
 * ==============================================================================
 *
 * ==============================================================================
 * @endverbatim
 */
#include "bsp_uart.h"
#include "usart.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

uint8_t Uart3_TXBuf[256] = {0};
uint8_t Uart3_RXBuf[256] = {0};
uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    { // 串口3接收中断
        HAL_UART_Receive_DMA(&huart1, (uint8_t *)Uart3_RXBuf, 1);
    }
}

/**
 * @brief   用UART3的DMA发送字符串
 * @note    和printf一个用法
 * @param   {char} *format 格式和参数
 * @return  {*}
 */
void DMA_Printf(const char *format, ...)
{
    uint8_t length;
    va_list args;

    // 等待串口发送完成，注意是串口发送完成，不是DMA传输完成
    while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC))
        ;
    // 如果没有上面这条语句，连续调用printf_DMA时可能输出错误。

    va_start(args, format);
    length = vsnprintf((char *)Uart3_TXBuf, sizeof(Uart3_TXBuf), (char *)format, args);
    va_end(args);

    HAL_UART_Transmit(&huart1, Uart3_TXBuf, length, 1000);
}

/**
 * @brief   用UART3的DMA发送Vofa的JustFloat数据
 * @note    
 * @param   {float} _NumArgs 浮点数数组
 * @param   {uint8_t} _len 发送个数
 * @return  {*}
 */
void DMA_JustFloat(float _NumArgs[], uint8_t _len)
{
    float num;
    uint8_t i, j, temp[4];
    uint8_t *p = Uart3_TXBuf;

    while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC))
        ;

    for (j = 0; j < _len; j++)
    {
        num = _NumArgs[j];
        memcpy(temp, &num, 4);
        for (i = 0; i < 4; i++)
        {
            // *p = temp[i];
            *p = temp[3 - i];
            p++;
        }
    }
    memcpy(p, tail, 4);
    _len = _len * 4 + 4;
    HAL_UART_Transmit_DMA(&huart1, Uart3_TXBuf, _len);
}