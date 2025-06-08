//
// Created by 冬榆 on 25-6-7.
//

#ifndef UARTCONFIG_H
#define UARTCONFIG_H

#include "u_aux.h"
#include <termios.h>


int uartfd; // 设备
struct termios option; // 设置串口配置
unsigned char *uart_buffer_read; // 读缓冲区
unsigned char *uart_buffer_write; // 写缓冲区
unsigned nums; // 记录读取缓冲区有效字节数

void u_start();

void u_stop();

bool send(const unsigned char *msg, unsigned char end_char);

unsigned recieve(unsigned char end_char);

float getFloat(int totalLen, int pointPlace);
#endif //UARTCONFIG_H
