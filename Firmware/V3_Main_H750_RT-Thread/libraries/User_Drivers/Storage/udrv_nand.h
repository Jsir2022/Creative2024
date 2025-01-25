#ifndef __UDRV_NAND_H__
#define __UDRV_NAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//定义W25N01GV NAND Flash基本信息，四线SPI驱动
#define FLASH_JEDEC_ID 		(0xEFAA21)
#define TOTAL_SIZE 			(128 * 1024 * 1024)

#define PAGE_SIZE			(2048)
#define PAGE_PER_BLOCK		(64)
#define TOTAL_BLOCK			(1024)
#define OOB_SIZE			(64)
#define OOB_FREE			(60)
#define BUFF_SIZE			(PAGE_SIZE + PAGE_PER_BLOCK)


#define PROTECTION_REG 			0xA0

#define CONFIGURATION_REG 		0xB0

#define STATUS_REG 				0xC0
#define SR_BUSY_BIT_MASK		0x01

#define NAND_CMD_READ_ID		0x9F		/* 读器件ID命令 */

#define NAND_CMD_READ_PAGE		0x13  		/* 读页数据到缓存命令 */
#define NAND_CMD_READ_BUFF		0x03  		/* 连续读数据缓存命令 */

#define NAND_CMD_READ_SR     	0x0F		/* 读状态寄存器命令 */
#define NAND_CMD_WRITE_SR     	0x01  		/* 写状态寄存器命令 */

#define NAND_CMD_WRITE_EN      	0x06		/* 写使能命令 */
#define NAND_CMD_WRITE_DISAB	0x04		/* 写失能命令 */

#define NAND_CMD_WRITE_BUFF		0x02  		/* 写数据到缓存命令 */
#define NAND_CMD_WRITE_PAGE		0x10  		/* 写缓存到页面命令 */

#define CMD_BE        0xD8		/* 批量擦除命令 */

#define CMD_EWRSR	  0x50		/* 允许写状态寄存器的命令 */
#define CMD_SE        0x20		/* 擦除扇区命令 */

#define DUMMY_BYTE    0xA5		/* 哑命令，可以为任意值，用于读操作 */

#define WIP_FLAG      0x18		/* 状态寄存器中的正在编程标志（WIP) */

#ifdef __cplusplus
}
#endif

#endif

