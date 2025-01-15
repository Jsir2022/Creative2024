#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include <stdint.h>
#include <stdbool.h>

void DMA_Printf(const char *format, ...);
void DMA_JustFloat(float _NumArgs[], uint8_t _len);

#endif