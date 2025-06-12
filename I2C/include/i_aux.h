//
// Created by 冬榆 on 25-6-12.
//

#ifndef I_AUX_H
#define I_AUX_H

extern int fd;

void init_i2c();
void clean();

float read_temperature();
#endif //I_AUX_H
