#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum{
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

	// ��������������û�Ҳ�����ǵ�����������ͣ�����ͨѶ���Ӽ�⣬���շ��յ������践��
    CAN_CMD_HEARTBEAT = 63,
}tCanCmd;

typedef struct {
    uint32_t id:24;
    uint32_t dlc:8;
    uint8_t  data[8];
} CanFrame;

#endif