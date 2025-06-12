//
// Created by 冬榆 on 25-6-12.
//

#include "I2CControl.h"
#include "i_aux.h"

void i_start() {
    init_i2c();
}

void i_stop() {
    clean();
}

float getFloat_i() {
    return read_temperature();
}