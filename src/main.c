//
// Created by 冬榆 on 25-6-6.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "UARTControl.h"
#include "LCDControl.h"


// 测试LCD
int main() {
    start(); // 启动接口
    for (int _ = 0; _ < 10000; _++) {
        const int low = -20;
        const int high = 100;
        const int temp = rand() % (high - low + 1) + low;
        push(temp); // 温度队列接口
        refresh(); // 绘制刷新
        usleep(33333); // 延时
    }
    stop(); // 释放接口
    return 0;
}


// 测试串口通信
// int main() {
//     u_start();
//     while (true) {
//         recieve('n');
//         show_msg();
//         if (nums==1 && uart_buffer_read[0]==1) break;
//         send("hello world!\n", '\n');
//     }
//     u_stop();
//     return 0;
// }


// 温度显示
// int main() {
//     start();
//     u_start();
//     while (true) {
//         // 串口通信
//         recieve('\n');
//         show_msg();
//         if (nums==1 && uart_buffer_read[0]==1) break;
//         // 读取温度
//         const float temp = getFloat(8, 5);
//         printf("\n %.4f \n", temp);
//
//         // 屏幕刷新
//         push((int)temp);
//         refresh();
//         usleep(1000000);
//     }
// }
