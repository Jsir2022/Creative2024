#include "OpticalFlow.h"
#include "bsp_uart.h"

#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"

//#include "EventRecorder.h"
#include <stdio.h>

uint8_t TxData[2];
uint8_t Motion;
uint8_t Prodect_ID;
uint8_t Revision_ID;
int8_t Delta_X;
int8_t Delta_Y;
uint8_t Squal;
bool clear=false;
uint32_t timus=0;

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim==&htim7)
  {
	  if(timus!=0)
	  {
		timus--;
	  }
  }
}

void TIM_delayus(uint32_t _us)
{
	timus=_us;
	while(timus);
}

void OF_initDevice()
{
//	EventRecorderInitialize(EventRecordAll, 1U);
//	EventRecorderStart();
	HAL_TIM_Base_Start_IT(&htim7);
	
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(OF_NPD_GPIO_Port,OF_NPD_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(OF_RST_GPIO_Port,OF_RST_Pin,GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(OF_RST_GPIO_Port,OF_RST_Pin,GPIO_PIN_RESET);
	HAL_Delay(10);
	
	TxData[0]=0x00;
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,TxData,1,1);
	TIM_delayus(100);
	HAL_SPI_Receive(&hspi1,&Prodect_ID,1,1);
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
	
	TxData[0]=0x01;
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,TxData,1,1);
	TIM_delayus(100);
	HAL_SPI_Receive(&hspi1,&Revision_ID,1,1);
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
}

void OF_readDeltaXY()
{
	TxData[0]=0x02;
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,TxData,1,1);
	TIM_delayus(100);
	HAL_SPI_Receive(&hspi1,&Motion,1,1);
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
	
	TxData[0]=0x03;
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,TxData,1,1);
	TIM_delayus(100);
	HAL_SPI_Receive(&hspi1,&Delta_X,1,1);
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
	
	TxData[0]=0x04;
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,TxData,1,1);
	TIM_delayus(100);
	HAL_SPI_Receive(&hspi1,&Delta_Y,1,1);
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
	
	TxData[0]=0x05;
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,TxData,1,1);
	TIM_delayus(100);
	HAL_SPI_Receive(&hspi1,&Squal,1,1);
	HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
	
	if(Squal>30)
		DMA_Printf("%d,%d,%d\n",(int16_t)Delta_X,(int16_t)Delta_Y,(int16_t)Squal);
	
	if(clear==true)
	{
		clear=false;
		TxData[0]=0x12 & 0x80;
		HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi1,TxData,2,1);
		HAL_GPIO_WritePin(OF_NCS_GPIO_Port,OF_NCS_Pin,GPIO_PIN_SET);
		TIM_delayus(100);
	}
}

