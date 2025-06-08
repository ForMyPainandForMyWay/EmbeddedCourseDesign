//
// Created by 冬榆 on 25-6-7.
//


#include <stdio.h>

#include "UARTControl.h"

int main() {
    u_start();
    while (true) {
        recieve('n');
        show_msg();
        if (nums == 1 && uart_buffer_read[0] == 1) break;

        // printf("\n %.4f \n", getFloat(8, 5));  // 测试温度板,注意给出的代码中使用的波特率是9600,8位数据位、1位停止位、无校验位
        send("hello world!\n", '\n');
    }
    u_stop();
    return 0;
}
