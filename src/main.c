//
// Created by 冬榆 on 25-6-6.
//
#include <stdio.h>
#include <unistd.h>

#include "UARTControl.h"
#include "LCDControl.h"
#include "I2CControl.h"
#include "paint.h"

// 温度显示
int main() {
    lcd_start(50);
    u_start();
    i_start();
    test_lcd();
    while (true) {
        // recieve('\n');
        // show_msg();
        // if (nums==1) continue;
        // const float temp = getFloat(8, 5);

        // 读取温度
        // float temp = read_temperature(fd) * 3;
        const float temp = getFloat_i() * 3;
        // printf("%.2f\n", temp);
        // 屏幕刷新
        push((int)temp);
        refresh();
        test_lcd();
        usleep(500000);
    }

    // 测试串口通信
    // while (true) {
    //     // printf("ready for getd data\n");
    //     recieve('\n');
    //     printf("???\n");
    //     show_msg();
    //     printf("show msg done\n");
    //     send("hello world!\n", '\n');
    //     // printf("333\n");
    //     sleep(1);
    //     break;
    // }

}