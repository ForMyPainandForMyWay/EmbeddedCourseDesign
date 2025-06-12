//
// Created by 冬榆 on 25-6-7.
//

#include "UARTControl.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// 启动函数
void u_start() {
    init_uart("/dev/s3c2410_serial0");
}


// 关闭函数
void u_stop() {
    close(uartfd);
    free(uart_buffer_read);
    free(uart_buffer_write);
}


// 发送函数,每次调用发送信息并清空写缓冲区
bool send(const unsigned char *msg, const unsigned char end_char) {
    memcpy(uart_buffer_write, msg, strlen((const char *) msg) * sizeof(char));
    const bool success = write_uart(end_char) ? true : false; // 发送数据
    memset(uart_buffer_read, end_char, MAX_BUFFER_SIZE * sizeof(unsigned char)); // 清空写缓冲
    return success;
}


// 接收函数,每次调用写入缓冲区而(不是直接返回)
unsigned recieve(const unsigned char end_char) {
    read_uart(end_char);
    return nums;
}


// 根据给定的代码,格式为[符号][百位][十位][个位][.][小数1][小数2][小数3]
// 参数分别应该是8, 5
float getFloat(const int totalLen, const int pointPlace) {
    return ft2float(totalLen, pointPlace);
}
