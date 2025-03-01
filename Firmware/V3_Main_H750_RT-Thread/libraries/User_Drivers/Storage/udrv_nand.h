#ifndef __UDRV_NAND_H__
#define __UDRV_NAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// 定义W25N01GV NAND Flash基本信息，四线SPI驱动
#define FLASH_JEDEC_ID 		(0xEFAA21)
#define TOTAL_SIZE 			(128 * 1024 * 1024)

#define PAGE_SIZE			(2048)
#define PAGE_PER_BLOCK		(64)
#define TOTAL_BLOCK			(1024)
#define OOB_SIZE			(64)
#define OOB_FREE			(64)
#define BUFF_SIZE			(PAGE_SIZE + OOB_SIZE)
#define PAGE_ADDR_START		(13)			/* 代表Block0的前12页是有特殊功能的 */


#define NAND_CMD_DEV_RST 		0xFF		/* 复位器件命令 */
#define NAND_CMD_READ_ID		0x9F		/* 读器件ID命令 */

#define NAND_CMD_READ_PAGE		0x13  		/* 读页数据到缓存命令 */
#define NAND_CMD_READ_BUFF		0x03  		/* 连续读数据缓存命令 */

#define NAND_CMD_WRITE_EN      	0x06		/* 写使能命令 */
#define NAND_CMD_WRITE_DISAB	0x04		/* 写失能命令 */

#define NAND_CMD_WRITE_BUFF		0x02  		/* 写数据到缓存命令 */
#define NAND_CMD_WRITE_PAGE		0x10  		/* 写缓存到页面命令，执行后自动写失能，连续写需要重复写使能 */
#define NAND_CMD_ERASE_BLOCK    0xD8		/* 单个块擦除命令，等于64页128KiB */

#define NAND_CMD_READ_SR     	0x0F		/* 读状态寄存器命令 */
#define NAND_CMD_WRITE_SR     	0x1F  		/* 写状态寄存器命令 */

#define PROTECTION_REG 			0xA0		// 保护寄存器，默认0x7C
#define PR_TURN_OFF_ALL_PROTECT	0x00		// 关闭所有的保护

#define CONFIGURATION_REG 		0xB0		// 配置寄存器，默认0x18
#define CR_OTP_LOCK_MASK		0x80		// 1锁定OTP区域，一般用不上
#define CR_OTP_ACCESS_MASK		0x40		// 1才能读写OTP区域
#define CR_SR1_LOCK_MASK		0x20		// 1锁定保护某些区域
#define CR_ECC_ENABLE_MASK		0x10		// 1启用ECC保护，具体码看手册
#define CR_BUF_OPERATE_MASK		0x08		// 1启用2048+64模式，0只有2048

#define STATUS_REG 				0xC0		// 状态寄存器，默认0x00
#define SR_BUSY_BIT_MASK		0x01		// 1为忙碌于某项进程，只接受NAND_CMD_READ_SR和NAND_CMD_READ_ID指令
#define SR_WEL_BIT_MASK			0x02		// 1为写使能，0为写失能
#define SR_EFAIL_BIT_MASK		0x04		// 1为擦除超时，0为成功
#define SR_PFAIL_BIT_MASK		0x08		// 1为写页面超时，0为成功

#ifdef __cplusplus
}
#endif

#endif

