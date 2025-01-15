/*
    Copyright 2021 codenocold codenocold@qq.com
    Address : https://github.com/codenocold/dgm
    This file is part of the dgm firmware.
    The dgm firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    The dgm firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __CAN_H__
#define __CAN_H__

#include "main.h"
#include "mc_task.h"

// nodeID | CMD
// 5 bit  | 6 bit
#define NODE_ID_BIT     0x7C0U
#define CMD_BIT         0x03FU

typedef enum eCanCmd{
	// 停止电机，进入待机模式
    CAN_CMD_MOTOR_DISABLE = 0,
	// 启动电机，进入工作模式
    CAN_CMD_MOTOR_ENABLE,
    
	// 设置电机目标转矩
    CAN_CMD_SET_TORQUE,
	// 设置电机目标转速
    CAN_CMD_SET_VELOCITY,
	// 设置电机目标位置
    CAN_CMD_SET_POSITION,
	// 运动控制指令同步信号
    CAN_CMD_SYNC,

	
    CAN_CMD_CALIB_START,
    CAN_CMD_CALIB_REPORT,
    CAN_CMD_CALIB_ABORT,
    
    CAN_CMD_ANTICOGGING_START,
    CAN_CMD_ANTICOGGING_REPORT,
    CAN_CMD_ANTICOGGING_ABORT,

	// 设置当前位置为零点
    CAN_CMD_SET_HOME,
	// 清除错误代码
    CAN_CMD_ERROR_RESET,
	// 获取当前状态和错误信息
    CAN_CMD_GET_STATUSWORD,
	// 电机驱动器内部状态和错误信息主动上报
    CAN_CMD_STATUSWORD_REPORT,
    
	// 获取当前电机实际转矩
    CAN_CMD_GET_TORQUE,
	// 获取当前电机实际转速
    CAN_CMD_GET_VELOCITY,
	// 获取当前电机实际位置
    CAN_CMD_GET_POSITION,
	// 获取当前电机电流
    CAN_CMD_GET_I_Q,
	// 获取当前母线电压
    CAN_CMD_GET_VBUS,
	// 获取当前母线电流
    CAN_CMD_GET_IBUS,
	// 获取当前电机功率
    CAN_CMD_GET_POWER,
    
    CAN_CMD_SET_CONFIG,
    CAN_CMD_GET_CONFIG,
    CAN_CMD_SAVE_ALL_CONFIG,
    CAN_CMD_RESET_ALL_CONFIG,

    CAN_CMD_GET_FW_VERSION = 50,
    CAN_CMD_WRITE_APP_BACK_START,
    CAN_CMD_WRITE_APP_BACK,
    CAN_CMD_CHECK_APP_BACK,
    CAN_CMD_DFU_START,

	// 心跳命令，可以是用户也可以是电机驱动器发送，
	// 用于通讯连接监测，接收方收到后无需返回
    CAN_CMD_HEARTBEAT = 63,
}tCanCmd;

typedef struct {
    uint32_t id:24;
    uint32_t dlc:8;
    uint8_t  data[8];
} CanFrame;

void CAN_set_node_id(uint8_t nodeID);
void CAN_comm_loop(void);
void CAN_reset_rx_timeout(void);
void CAN_reset_tx_timeout(void);

void CAN_receive_callback(void);

void CAN_tx_statusword(tMCStatusword statusword);

// only used in one NVIC
void CAN_calib_report(int32_t step, uint8_t *data);
void CAN_anticogging_report(int32_t step, int32_t value);

#endif /* __CAN_H__ */
