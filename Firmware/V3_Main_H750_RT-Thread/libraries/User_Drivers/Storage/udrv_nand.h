#ifndef __UDRV_NAND_H__
#define __UDRV_NAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//����W25N01GV NAND Flash������Ϣ������SPI����
#define FLASH_JEDEC_ID 		(0xEFAA21)
#define TOTAL_SIZE 			(128 * 1024 * 1024)

#define PAGE_SIZE			(2048)
#define PAGE_PER_BLOCK		(64)
#define TOTAL_BLOCK			(1024)
#define OOB_SIZE			(64)
#define OOB_FREE			(60)
#define BUFF_SIZE			(PAGE_SIZE + PAGE_PER_BLOCK)
#define PAGE_ADDR_START		(0x0C)			/* ����Block0��ǰ12ҳ�������⹦�ܵ� */


#define NAND_CMD_DEV_RST 		0xFF		/* ��λ�������� */
#define NAND_CMD_READ_ID		0x9F		/* ������ID���� */

#define NAND_CMD_READ_PAGE		0x13  		/* ��ҳ���ݵ��������� */
#define NAND_CMD_READ_BUFF		0x03  		/* ���������ݻ������� */

#define NAND_CMD_WRITE_EN      	0x06		/* дʹ������ */
#define NAND_CMD_WRITE_DISAB	0x04		/* дʧ������ */

#define NAND_CMD_WRITE_BUFF		0x02  		/* д���ݵ��������� */
#define NAND_CMD_WRITE_PAGE		0x10  		/* д���浽ҳ�����ִ�к��Զ�дʧ�ܣ�����д��Ҫ�ظ�дʹ�� */
#define NAND_CMD_ERASE_BLOCK    0xD8		/* ����������������64ҳ128KiB */

#define NAND_CMD_READ_SR     	0x0F		/* ��״̬�Ĵ������� */
#define NAND_CMD_WRITE_SR     	0x1F  		/* д״̬�Ĵ������� */

#define PROTECTION_REG 			0xA0

#define CONFIGURATION_REG 		0xB0

#define STATUS_REG 				0xC0		//״̬�Ĵ����ĵ�ַ
#define SR_BUSY_BIT_MASK		0x01		//1Ϊæµ��ĳ����̣�ֻ����NAND_CMD_READ_SR��NAND_CMD_READ_IDָ��
#define SR_WEL_BIT_MASK			0x02		//1Ϊдʹ�ܣ�0Ϊдʧ��
#define SR_EFAIL_BIT_MASK		0x04		//1Ϊ������ʱ��0Ϊ�ɹ�
#define SR_PFAIL_BIT_MASK		0x08		//1Ϊдҳ�泬ʱ��0Ϊ�ɹ�

#ifdef __cplusplus
}
#endif

#endif

