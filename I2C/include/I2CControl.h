//
// Created by 冬榆 on 25-6-12.
//

#ifndef I2CCONTROL_H
#define I2CCONTROL_H

#include "I2CControl.h"

int fd;  // 设备

void i_start();

void i_stop();

float getFloat_i();
#endif //I2CCONTROL_H
