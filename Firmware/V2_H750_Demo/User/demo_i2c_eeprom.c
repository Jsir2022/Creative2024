#include "demo_i2c_eeprom.h"
#include "bsp.h"

#define TEST_ADDR		0		/* ��д���Ե�ַ */
#define TEST_SIZE		1024	/* ��д�������ݴ�С */

/* �������ļ��ڵ��õĺ������� */
//static void sfDispMenu(void);

/*
*********************************************************************************************************
*	�� �� ��: DemoSpiFlash
*	����˵��: QSPI��д����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DemoI2CEEPROM(void)
{
	int32_t iTime1, iTime2;
	
	printf("\r\n*******************************************\r\n");
	
	/* ��⴮��Flash OK */
	if(ee_CheckOk()==0)
	{
		printf("\r\n��ⲻ��EEPROM!\r\n");
		return;
	}
	
	printf("\r\n��ʼ��дEEPROM.\r\n");
	uint8_t buff[1024];
	for(uint16_t i = 0; i<1024; i++)
	{
		buff[i]=0xC6;
	}
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	ee_WriteBytes(buff,0x0,1024);
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */
	printf("���ݳ���: %d�ֽ�, д��ʱ: %dms, д�ٶ�: %dKB/s\r\n", 1024, iTime2 - iTime1, (10 * 1024) / (iTime2 - iTime1));
	
	for(uint16_t i = 0; i<1024; i++)
	{
		buff[i]=0x0;
	}
	
	iTime1 = bsp_GetRunTime();	/* ���¿�ʼʱ�� */
	ee_ReadBytes(buff,0x0,1024);
	iTime2 = bsp_GetRunTime();	/* ���½���ʱ�� */
	printf("���ݳ���: %d�ֽ�, ����ʱ: %dms, ���ٶ�: %dKB/s\r\n", 1024, iTime2 - iTime1, (10 * 1024) / (iTime2 - iTime1));
	
	for(uint16_t i = 0; i<1024; i++)
	{
		if(buff[i] != 0xC6)
		{
			printf("EEPROM����������!\r\n");
			break;
		}
	}
	printf("EEPROM�����ϣ���д1Kbyte������.\r\n");
}
