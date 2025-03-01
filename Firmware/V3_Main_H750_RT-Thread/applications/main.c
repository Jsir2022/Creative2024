/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-04     bkk          first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "dfs_fs.h"
#include "udrv_nand.h"
#include "yaffsfs.h"
#include "dfs_fs.h"

//int ret = 0;

int main(void)
{
	int ret = 0;
	rt_mtd_nand_t nand_dev = (rt_mtd_nand_t)rt_device_find("nand0");
//	ret = yaffs_start_up(nand_dev,"/");
	
//	ret = dfs_mkfs("yaffs", "nand0");
//	if ( ret == 0) {
//        rt_kprintf("Mk success at root\n");
//    }
//	else
//	{
//		rt_kprintf("Mk failue at root\n");
//	}
	
//	ret = dfs_mount("nand0", "/", "yaffs", 0, 0);
//    if (ret == 0) {
//        rt_kprintf("Mount success at root\n");
//    }
//	else
//	{
//		rt_kprintf("Mount failue at root\n");
//	}
	
    for(;;)
    {
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}

/* 设置中断向量表地址到NOR_FLASH */
#include "stm32h7xx.h"
static int vtor_config(void)
{
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);
