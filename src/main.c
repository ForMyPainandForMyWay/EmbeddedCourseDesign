//
// Created by 冬榆 on 25-6-6.
//
#include <unistd.h>

#include "UARTControl.h"
#include "LCDControl.h"
#include "I2CControl.h"

// 温度显示
int main() {
    lcd_start(50);
    u_start();
    i_start();
    test_lcd();
    while (true) {
        const float temp = getFloat_i();
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