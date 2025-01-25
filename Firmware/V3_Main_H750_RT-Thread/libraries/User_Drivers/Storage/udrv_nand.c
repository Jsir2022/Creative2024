/**
 * @file        udrv_nand.c
 * @brief       SPI驱动NAND Flash (W25N01GVZEIG)
 * @version     V1.0
 * @author      Jsir2022 (jsir2022@outlook.com)
 * @date        2025-01-15 16:20:53
 * LastEditors  Jsir2022
 * LastEditTime 2025-01-15 18:31:27
 * history		版本	日期		作者		说明
 * 				V1.0	2025-1-17	Jsir2022	正式发布
 * Copyright (c) 2025 by Jsir2022, All Rights Reserved. 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 */

#include <rtconfig.h>

#ifdef JSIR_CARD_USING_NAND

#include <board.h>
#include "udrv_nand.h"
#include "drv_spi.h"

#define NAND_DEBUG rt_kprintf

struct stm32h7_nand
{
    uint8_t id[3];
    struct rt_mutex lock;
    struct rt_completion comp;
}_device;

static struct rt_mtd_nand_device nand_flash;
static struct rt_spi_device *spi_device = RT_NULL;

static rt_bool_t nand_readstatus(void)
{
    uint8_t cmd_read_sr[2] = {NAND_CMD_READ_SR,STATUS_REG};
    uint8_t status = 0;
	rt_spi_send_then_recv(spi_device,cmd_read_sr,2,&status,1);

    // 返回 BUSY 位的状态
    return (status & SR_BUSY_BIT_MASK);
}

static void nand_writeenable(void)
{
    uint8_t cmd_write_en[1] = {NAND_CMD_WRITE_EN};
	rt_spi_send(spi_device,cmd_write_en,1);
}

static void nand_writedisable(void)
{
    uint8_t cmd_write_disen[1] = {NAND_CMD_WRITE_DISAB};
    rt_spi_send(spi_device,cmd_write_disen,1);
}

static rt_err_t nandflash_readid(struct rt_mtd_nand_device *mtd)
{
	uint8_t cmd_read_id[2]={NAND_CMD_READ_ID,NULL};
	uint8_t recv[3];
	rt_spi_send_then_recv(spi_device,cmd_read_id,2,recv,3);
	
	uint32_t jedec_id = ((uint32_t)recv[0] << 16) | ((uint32_t)recv[1] << 8) | recv[2];
	NAND_DEBUG("NAND_JEDEC_ID[0x%X]\n",jedec_id);
	if (jedec_id == FLASH_JEDEC_ID)
    {
        return (RT_EOK);
    }
	
	return (RT_ERROR);
}

static rt_err_t nandflash_readpage(struct rt_mtd_nand_device* device, rt_off_t page,
                                   rt_uint8_t *data, rt_uint32_t data_len,
                                   rt_uint8_t *spare, rt_uint32_t spare_len)
{
    rt_uint32_t index;
    rt_uint32_t gecc, recc;
    rt_uint8_t tmp[4];
    rt_err_t result;

    page = page + device->block_start * device->pages_per_block;
    if (page/device->pages_per_block > device->block_end)
    {
        return -RT_MTD_EIO;
    }

    result = RT_MTD_EOK;
    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

    if (data && data_len)
    {
		//读页数据到缓存中
		uint8_t cmd_read_page[4]={NAND_CMD_READ_PAGE,NULL,((page & 0xFF00) >> 8),(page & 0xFF)};
		rt_spi_send(spi_device,cmd_read_page,4);
		// 等待数据加载完成，检查 BUSY 位
		while (nand_readstatus() != 0)
		{
			// 等待 BUSY 位变为 0，表示数据加载完成
			rt_thread_mdelay(1);  // 可根据需要调整等待时间
		}
		rt_thread_mdelay(10);
		//读缓存中的数据
		uint8_t cmd_read_data[4]={NAND_CMD_READ_BUFF,NULL,NULL,NULL};
		rt_spi_send_then_recv(spi_device,cmd_read_data,4,data,data_len);

//        FSMC_NANDECCCmd(FSMC_Bank3_NAND,ENABLE);
//        dmaRead(data, data_len);
//        gecc = FSMC_GetECC(FSMC_Bank3_NAND);
//        FSMC_NANDECCCmd(FSMC_Bank3_NAND,DISABLE);

//        if (data_len == PAGE_DATA_SIZE)
//        {
//            for (index = 0; index < ECC_SIZE; index ++)
//                tmp[index] = nand_read8();
//            if (spare && spare_len)
//            {
//                dmaRead(&spare[ECC_SIZE], spare_len - ECC_SIZE);
//                rt_memcpy(spare, tmp , ECC_SIZE);
//            }

//            recc   = (tmp[3] << 24) | (tmp[2] << 16) | (tmp[1] << 8) | tmp[0];

//            if (recc != 0xFFFFFFFF && gecc != 0)
//                result = nand_datacorrect(gecc, recc, data);

//            if (result != RT_MTD_EOK)
//                NAND_DEBUG("page: %d, gecc %X, recc %X>",page, gecc, recc);

//            goto _exit;
//        }
//    }

//    if (spare && spare_len)
//    {
//        nand_cmd(NAND_CMD_READ_1);
//        nand_addr(0);
//        nand_addr(8);
//        nand_addr(page);
//        nand_addr(page >> 8);
//        nand_addr(page >> 16);
//        nand_cmd(NAND_CMD_READ_TRUE);

//        nand_waitready();

//        dmaRead(spare, spare_len);
    }
_exit:
    rt_mutex_release(&_device.lock);

    return (result);
}

static rt_err_t nandflash_writepage(struct rt_mtd_nand_device* device, rt_off_t page,
                                    const rt_uint8_t *data, rt_uint32_t data_len,
                                    const rt_uint8_t *spare, rt_uint32_t spare_len)
{
    rt_err_t result;
    rt_uint32_t gecc;

    page = page + device->block_start * device->pages_per_block;
    if (page/device->pages_per_block > device->block_end)
    {
        return -RT_MTD_EIO;
    }

    result = RT_MTD_EOK;
    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

    if (data && data_len)
    {
		rt_uint8_t* pdata = (rt_uint8_t*)data;
		//写使能
		nand_writeenable();
		
		uint8_t cmd_write_buff[3]={NAND_CMD_WRITE_BUFF,0,0};
		
		for(uint32_t round = data_len/PAGE_SIZE; round != 0; round--)
		{
			//写缓存
			rt_spi_send(spi_device,cmd_write_buff,3);
			rt_spi_send(spi_device,pdata,PAGE_SIZE);
			pdata=pdata+PAGE_SIZE;
			//把缓存中的数据写到页中
			uint8_t cmd_write_page[4]={NAND_CMD_WRITE_PAGE,NULL,((page & 0xFF00) >> 8),(page & 0xFF)};
			rt_spi_send(spi_device,cmd_write_page,4);
			page++;
			// 等待数据加载完成，检查 BUSY 位
			while (nand_readstatus() != 0)
			{
				// 等待 BUSY 位变为 0，表示数据加载完成
				rt_thread_mdelay(1);  // 可根据需要调整等待时间
			}
		}
		
		uint16_t remain = data_len%PAGE_SIZE;
		if(remain != 0)
		{
			//写缓存
			rt_spi_send(spi_device,cmd_write_buff,3);
			rt_spi_send(spi_device,pdata,remain);
			//把缓存中的数据写到页中
			uint8_t cmd_write_page[4]={NAND_CMD_WRITE_PAGE,NULL,((page & 0xFF00) >> 8),(page & 0xFF)};
			rt_spi_send(spi_device,cmd_write_page,4);
			// 等待数据加载完成，检查 BUSY 位
			while (nand_readstatus() != 0)
			{
				// 等待 BUSY 位变为 0，表示数据加载完成
				rt_thread_mdelay(1);  // 可根据需要调整等待时间
			}
		}
		
//        if (data_len == PAGE_DATA_SIZE)
//        {
//            nand_write8((uint8_t)gecc);
//            nand_write8((uint8_t)(gecc >> 8));
//            nand_write8((uint8_t)(gecc >> 16));
//            nand_write8((uint8_t)(gecc >> 24));

//            if (spare && spare_len)
//                dmaWrite(&spare[ECC_SIZE], spare_len - ECC_SIZE);
//        }

//        nand_cmd(NAND_CMD_PAGEPROGRAM_TRUE);

//        nand_waitready();

//        if (nand_readstatus() & 0x01 == 1)
//            result = -RT_MTD_EIO;
//        goto _exit;
//    }

//    if (spare && spare_len)
//    {
//        nand_cmd(NAND_CMD_PAGEPROGRAM);

//        nand_addr(ECC_SIZE);
//        nand_addr(0x08);
//        nand_addr(page);
//        nand_addr(page >> 8);
//        nand_addr(page >> 16);

//        dmaWrite(&spare[ECC_SIZE], spare_len - ECC_SIZE);

//        nand_cmd(NAND_CMD_PAGEPROGRAM_TRUE);
//        nand_waitready();

//        if (nand_readstatus() & 0x01 == 1)
//            result = -RT_MTD_EIO;
    }

_exit:
    rt_mutex_release(&_device.lock);

    return (result);
}

rt_err_t nandflash_eraseblock(struct rt_mtd_nand_device* device, rt_uint32_t block)
{
//    rt_uint32_t page;
//    rt_err_t result;

//    block = block + device->block_start;

//    result = RT_MTD_EOK;
//    page = block * 64;

//    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

//    nand_cmd(NAND_CMD_ERASE0);

//    nand_addr(page);
//    nand_addr(page >> 8);
//    nand_addr(page >> 16);

//    nand_cmd(NAND_CMD_ERASE1);

//    nand_waitready();

//    if (nand_readstatus() & 0x01 == 1)
//        result = -RT_MTD_EIO;
//    rt_mutex_release(&_device.lock);

//    return (result);
}

static rt_err_t nandflash_pagecopy(struct rt_mtd_nand_device *device, rt_off_t src_page, rt_off_t dst_page)
{
//    rt_err_t result;

//    src_page = src_page + device->block_start * device->pages_per_block;
//    dst_page = dst_page + device->block_start * device->pages_per_block;

//    result = RT_MTD_EOK;
//    rt_mutex_take(&_device.lock, RT_WAITING_FOREVER);

//    nand_cmd(NAND_CMD_RDCOPYBACK);

//    nand_addr(0);
//    nand_addr(0);
//    nand_addr(src_page);
//    nand_addr(src_page >> 8);
//    nand_addr(src_page >> 16);

//    nand_cmd(NAND_CMD_RDCOPYBACK_TRUE);

//    nand_waitready();

//    nand_cmd(NAND_CMD_COPYBACKPGM);

//    nand_addr(0);
//    nand_addr(0);
//    nand_addr(dst_page);
//    nand_addr(dst_page >> 8);
//    nand_addr(dst_page >> 16);

//    nand_cmd(NAND_CMD_COPYBACKPGM_TRUE);

//    nand_waitready();
//    if ((nand_readstatus() & 0x01) == 0x01)
//        result = -RT_MTD_EIO;

//    rt_mutex_release(&_device.lock);

//    return (result);
}

static rt_err_t nandflash_checkblock(struct rt_mtd_nand_device* device, rt_uint32_t block)
{
    return (RT_MTD_EOK);
}

static rt_err_t nandflash_markbad(struct rt_mtd_nand_device* device, rt_uint32_t block)
{
    return (RT_MTD_EOK);
}

static struct rt_mtd_nand_driver_ops ops =
{
    nandflash_readid,
    nandflash_readpage,
    nandflash_writepage,
    nandflash_pagecopy,
    nandflash_eraseblock,
#if defined(RT_USING_DFS_UFFS) && !defined(RT_UFFS_USE_CHECK_MARK_FUNCITON)
    RT_NULL,
    RT_NULL,
#else
    nandflash_checkblock,
    nandflash_markbad
#endif
};

static int rt_hw_mtd_nand_init(void)
{
	spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
	if(RT_NULL == spi_device)
    {
        NAND_DEBUG("Failed to malloc the spi device.");
        return -RT_ENOMEM;
    }
	if (RT_EOK != rt_spi_bus_attach_device_cspin(spi_device, "spi40", "spi4", rt_pin_get("PE.4"), RT_NULL))
    {
        NAND_DEBUG("Failed to attach the spi device.");
        return -RT_ERROR;
    }
	
	struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz = 2 * 1000 *1000; //稳定起见，使用2MHz。测试过最高30MHz
    rt_spi_configure(spi_device, &cfg);

    nand_flash.page_size   = PAGE_SIZE;
    nand_flash.pages_per_block = PAGE_PER_BLOCK;
    nand_flash.block_total = TOTAL_BLOCK;
    nand_flash.oob_size    = OOB_SIZE;
    nand_flash.oob_free    = OOB_FREE;
    nand_flash.block_start = 0;
    nand_flash.block_end   = TOTAL_BLOCK-1;
    nand_flash.ops         = &ops;
	if (RT_EOK != rt_mtd_nand_register_device("nand0", &nand_flash))
    {
        NAND_DEBUG("Failed to probe the W25N01GV.");
        return -RT_ERROR;
    };
	
	rt_mutex_init(&_device.lock, "nand", RT_IPC_FLAG_FIFO);
	
	return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_mtd_nand_init);

#endif
