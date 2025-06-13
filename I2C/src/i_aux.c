//
// Created by 冬榆 on 25-6-12.
//

#include "i_aux.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#define I2C_DEV "/dev/i2c-2"
#define LM75A_ADDR 0x48         // I2C 地址


// 初始化
void init_i2c() {
    fd = open(I2C_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C device");
    }

    if (ioctl(fd, I2C_SLAVE, LM75A_ADDR) < 0) {
        perror("Failed to set I2C address");
        close(fd);
    }
}


// 清理
void clean() {
    close(fd);
}


// 读取总线数据
float read_temperature() {
    // LM75 温度寄存器地址为 0x00
    unsigned char buf[2];

    if (read(fd, buf, 2) != 2) {
        perror("Failed to read temperature");
        return -1000.0f;
    }
    // 将高8位和低8位合并为16进制，LM75是高11位有效，舍弃低5位
    int16_t data = (int16_t)(buf[0] << 8 | buf[1]);
    data >>= 5;  // 数值右移
    return (float)data * 0.125f;
}