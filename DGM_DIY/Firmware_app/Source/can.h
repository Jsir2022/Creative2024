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
	// ֹͣ������������ģʽ
    CAN_CMD_MOTOR_DISABLE = 0,
	// ������������빤��ģʽ
    CAN_CMD_MOTOR_ENABLE,
    
	// ���õ��Ŀ��ת��
    CAN_CMD_SET_TORQUE,
	// ���õ��Ŀ��ת��
    CAN_CMD_SET_VELOCITY,
	// ���õ��Ŀ��λ��
    CAN_CMD_SET_POSITION,
	// �˶�����ָ��ͬ���ź�
    CAN_CMD_SYNC,

	
    CAN_CMD_CALIB_START,
    CAN_CMD_CALIB_REPORT,
    CAN_CMD_CALIB_ABORT,
    
    CAN_CMD_ANTICOGGING_START,
    CAN_CMD_ANTICOGGING_REPORT,
    CAN_CMD_ANTICOGGING_ABORT,

	// ���õ�ǰλ��Ϊ���
    CAN_CMD_SET_HOME,
	// ����������
    CAN_CMD_ERROR_RESET,
	// ��ȡ��ǰ״̬�ʹ�����Ϣ
    CAN_CMD_GET_STATUSWORD,
	// ����������ڲ�״̬�ʹ�����Ϣ�����ϱ�
    CAN_CMD_STATUSWORD_REPORT,
    
	// ��ȡ��ǰ���ʵ��ת��
    CAN_CMD_GET_TORQUE,
	// ��ȡ��ǰ���ʵ��ת��
    CAN_CMD_GET_VELOCITY,
	// ��ȡ��ǰ���ʵ��λ��
    CAN_CMD_GET_POSITION,
	// ��ȡ��ǰ�������
    CAN_CMD_GET_I_Q,
	// ��ȡ��ǰĸ�ߵ�ѹ
    CAN_CMD_GET_VBUS,
	// ��ȡ��ǰĸ�ߵ���
    CAN_CMD_GET_IBUS,
	// ��ȡ��ǰ�������
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

	// ��������������û�Ҳ�����ǵ�����������ͣ�
	// ����ͨѶ���Ӽ�⣬���շ��յ������践��
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
