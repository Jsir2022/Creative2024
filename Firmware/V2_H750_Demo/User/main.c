#include "bsp.h"			 /* 底层硬件驱动 */
#include "demo_fmc_sdram.h"
#include "demo_qspi_nor.h"
#include "demo_i2c_eeprom.h"
#include "demo_spi_nand.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"H750-外部SDRAM,NOR,NAND,EEPROM性能测试"
#define EXAMPLE_DATE	"2024-12-22"
#define DEMO_VER		"1.0"

static void PrintfLogo(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参: 无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	bsp_Init();		/* 硬件初始化 */
	PrintfLogo();	/* 打印例程名称和版本等信息 */

	DemoFmcSDRAM(); /* 外部SDRAM读写测试 */
	DemoQSPINOR(); /* QSPI NOR Flash测试 */
	DemoI2CEEPROM();/* I2C EEPROM测试 */
	DemoSPINAND(); /* SPI NAND Flash测试 */
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* 检测CPU ID */
	{
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;
		
		CPU_Sn0 = *(__IO uint32_t*)(0x1FF1E800);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FF1E800 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FF1E800 + 8);

		printf("CPU : STM32H750XBH6, TFBGA265, 主频: %dMHz\r\n", SystemCoreClock / 1000000);
		printf("UID = %08X %08X %08X\n\r", CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST的HAL库版本 */
	printf("* HAL库版本  : V1.3.0 (STM32H7xx HAL Driver)\r\n");
	printf("* Copyright www.armfly.com 安富莱电子\r\n");
	printf("*************************************************************\n\r");
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
