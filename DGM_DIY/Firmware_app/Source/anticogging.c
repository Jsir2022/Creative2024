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

#include "anticogging.h"
#include "usr_config.h"
#include "controller.h"
#include "pwm_curr.h"
#include "mc_task.h"
#include "util.h"
#include "heap.h"
#include "can.h"
#include "foc.h"

bool AnticoggingValid = false;

static int mNumber;
static int mLoopCount;

void ANTICOGGING_start(void)
{
    mNumber = 0;
    mLoopCount = 0;
    AnticoggingValid = false;
    USR_CONFIG_set_default_cogging_map();
}

void ANTICOGGING_end(void)
{
    FOC_disarm();
    
    if(!AnticoggingValid){
        USR_CONFIG_set_default_cogging_map();
    }
}

void ANTICOGGING_loop(void)
{
    static const int gap_number = 100;
    
    // loop contrl 0.025s
    if(++mLoopCount < 500){
        return;
    }
    mLoopCount = 0;
    
    mNumber ++;
    
    if(mNumber <= gap_number){
        // CW
        const float delta = (1.0f / (float)COGGING_MAP_NUM);
        float pos_ref = Controller.input_position + delta;
        if(pos_ref > 1.0f){
            pos_ref -= 1.0f;
        }
        Controller.input_position = pos_ref;
    }else if(mNumber <= (gap_number + COGGING_MAP_NUM)){
        float torque = Foc.i_q_filt * UsrConfig.torque_constant;
        int16_t tmp = (int16_t)(torque * 5000.0f);
        int16_t index = nearbyintf(COGGING_MAP_NUM * Controller.input_position);
        if(index >= COGGING_MAP_NUM){
            index = 0;
        }
        pCoggingMap->map[index] = tmp;
        
        CAN_anticogging_report(index + 1, pCoggingMap->map[index]);
        
        // CW
        const float delta = (1.0f / (float)COGGING_MAP_NUM);
        float pos_ref = Controller.input_position + delta;
        if(pos_ref > 1.0f){
            pos_ref -= 1.0f;
        }
        Controller.input_position = pos_ref;
    }else if(mNumber <= (gap_number + COGGING_MAP_NUM + gap_number)){
        // CCW
        const float delta = -(1.0f / (float)COGGING_MAP_NUM);
        float pos_ref = Controller.input_position + delta;
        if(pos_ref < 0.0f){
            pos_ref += 1.0f;
        }
        Controller.input_position = pos_ref;
    }else if(mNumber <= (gap_number + COGGING_MAP_NUM + gap_number + COGGING_MAP_NUM)){
        float torque = Foc.i_q_filt * UsrConfig.torque_constant;
        int16_t tmp = (int16_t)(torque * 5000.0f);
        int16_t index = nearbyintf(COGGING_MAP_NUM * Controller.input_position);
        if(index >= COGGING_MAP_NUM){
            index = 0;
        }
        pCoggingMap->map[index] = (pCoggingMap->map[index] + tmp) / 2;
        
        CAN_anticogging_report(COGGING_MAP_NUM + index + 1, tmp);
        
        // CCW
        const float delta = -(1.0f / (float)COGGING_MAP_NUM);
        float pos_ref = Controller.input_position + delta;
        if(pos_ref < 0.0f){
            pos_ref += 1.0f;
        }
        Controller.input_position = pos_ref;
    }else{
        // End
        CAN_anticogging_report(0, 0);
        AnticoggingValid = true;
        MCT_set_state(IDLE);
    }
}
