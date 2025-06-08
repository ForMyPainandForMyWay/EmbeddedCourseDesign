//
// Created by 冬榆 on 25-6-6.
//
#include <linux/fb.h>
#include "l_aux.h"
#include "array.h"
#include "paint.h"
#include "LCDControl.h"


// 启动接口,初始化屏幕并show
void start() {
    init_array(&line, info.xres, 0, info.xres - 1);
    init_screen();
    brush_screen(0x00, 0X00, 0x00); // 黑屏并将缓冲区置0
    paint_cross();
    show(); // 缓冲区拷贝展示
}


// 结束接口
void stop() {
    clean();
}


// 温度环形缓冲区接口,不绘制
void push(const int value) {
    push_queue(&line, value);
}


// 屏幕刷新绘制接口,调用show绘制
void refresh() {
    refresh_screen(&line);
    show(); // 缓冲区拷贝
}


// int main() {
//     // array_init(&line);  // 初始化数组
//     // if (!init_screen()) {
//     //     perror("Error initializing screen");
//     //     return 1;
//     // }
//     start();
//
//     brush_screen(0xff, 0Xff, 0xff);  // 白屏并将缓冲区置0
//     // while (1) {
//     //     const int high = 120;
//     //     const int low = -20;
//     //
//     //     int tempture =  rand() % (high - low + 1) + low;
//     //
//     //
//     //     sleep(1);
//     // }
//
//
//     // clean();  // 清空资源
//     stop();
//     return 0;
// }
