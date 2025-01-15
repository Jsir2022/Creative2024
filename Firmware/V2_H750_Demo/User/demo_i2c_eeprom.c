#include "demo_i2c_eeprom.h"
#include "bsp.h"

#define TEST_ADDR		0		/* 读写测试地址 */
#define TEST_SIZE		1024	/* 读写测试数据大小 */

/* 仅允许本文件内调用的函数声明 */
//static void sfDispMenu(void);

/*
*********************************************************************************************************
*	函 数 名: DemoSpiFlash
*	功能说明: QSPI读写例程
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DemoI2CEEPROM(void)
{
	int32_t iTime1, iTime2;
	
	printf("\r\n*******************************************\r\n");
	
	/* 检测串行Flash OK */
	if(ee_CheckOk()==0)
	{
		printf("\r\n检测不到EEPROM!\r\n");
		return;
	}
	
	printf("\r\n开始读写EEPROM.\r\n");
	uint8_t buff[1024];
	for(uint16_t i = 0; i<1024; i++)
	{
		buff[i]=0xC6;
	}
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	ee_WriteBytes(buff,0x0,1024);
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */
	printf("数据长度: %d字节, 写耗时: %dms, 写速度: %dKB/s\r\n", 1024, iTime2 - iTime1, (10 * 1024) / (iTime2 - iTime1));
	
	for(uint16_t i = 0; i<1024; i++)
	{
		buff[i]=0x0;
	}
	
	iTime1 = bsp_GetRunTime();	/* 记下开始时间 */
	ee_ReadBytes(buff,0x0,1024);
	iTime2 = bsp_GetRunTime();	/* 记下结束时间 */
	printf("数据长度: %d字节, 读耗时: %dms, 读速度: %dKB/s\r\n", 1024, iTime2 - iTime1, (10 * 1024) / (iTime2 - iTime1));
	
	for(uint16_t i = 0; i<1024; i++)
	{
		if(buff[i] != 0xC6)
		{
			printf("EEPROM数据有问题!\r\n");
			break;
		}
	}
	printf("EEPROM检测完毕，读写1Kbyte无问题.\r\n");
}
