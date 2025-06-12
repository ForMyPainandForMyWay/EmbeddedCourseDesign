//
// Created by 冬榆 on 25-6-6.
//

#include "paint.h"

#include <stdlib.h>
#include <string.h>
// #include <pthread.h>
#include <linux/fb.h>


// 双缓冲拷贝,实现屏幕显示
void show() {
    memcpy(framebuffer, framebuffer_back, screensize);
}


// 初始化参数结构体
void init_args(args* arg,
    const unsigned char red,
    const unsigned char green,
    const unsigned char blue,
    const bool Isleft){
    arg->red = red;
    arg->green = green;
    arg->blue = blue;
    arg->Islfet = Isleft;
}


// 多线程版本粉刷函数
int brush_half(void* arg) {
    const args* params = (args*) arg;
    unsigned int start_x, end_x;
    if (params->Islfet == true) {
        start_x = 0;
        end_x = info.xres/2;
    }
    else {
        start_x = info.xres/2;
        end_x = info.xres;
    }
    for (unsigned int y = 0; y < info.yres; y++) {
        for (unsigned int x = start_x; x < end_x; x++) {
            paint_pixel_rect(x,y,params->red, params->green, params->blue, 0, 0);
        }
    }
    return 0;
}


// 使用某种颜色粉刷整个屏幕,该函数会将缓冲区覆盖
bool brush_screen(
    const unsigned char red,
    const unsigned char green,
    const unsigned char blue) {
    if (red == green == blue) memset(framebuffer_back, red, screensize);
    // if (red == 0 && green == 0 && blue == 0) memset(framebuffer_back, 0x00, screensize);
    // else if (red == 0xff && green == 0xff && blue == 0xff) memset(framebuffer_back, 0xff, screensize);
    else {
        // 全屏填充色
        for (unsigned int y = 0; y < info.yres; y++) {
            for (unsigned int x = 0; x < info.xres; x++) {
                // 计算像素位置
                // unsigned char *pixel = tf(x, y);
                // BGR
                paint_pixel_rect(x,y,red, green, blue, 0,0);
                // if (bytes_per_pixel >= 3) {
                //     // 假设默认RGB顺序
                //     pixel[2] = red; // 红色
                //     pixel[1] = green; // 绿色
                //     pixel[0] = blue; // 蓝色
                //     // 如果存在alpha通道（32bpp），设为0xFF（不透明）
                //     if (bytes_per_pixel == 4) pixel[3] = 0xFF;
                // }
            }
        }
    }
    // // 多线程版本
    //
    // args arg_left, aeg_right;
    // int ret = 0;
    // init_args(&arg_left, red, green, blue, true);
    // init_args(&aeg_right, red, green, blue, false);
    //
    // // 创建并启动函数
    // thrd_t left_thread, right_thread;
    // ret = thrd_create(&left_thread, brush_half, &arg_left);
    // if (ret != thrd_success) return false;
    // ret = thrd_create(&right_thread, brush_half, &aeg_right);
    // if (ret != thrd_success) return false;
    // // 同步
    // ret = thrd_join(left_thread, NULL);
    // if (ret != thrd_success) return false;
    // ret = thrd_join(right_thread, NULL);
    // if (ret != thrd_success) return false;
    // test_lcd();
    return true;
}


// 以某个位置的像素为中心根据era绘制 矩形
void paint_pixel_rect(
    const unsigned int x,
    const unsigned int y,
    const unsigned char red,
    const unsigned char green,
    const unsigned char blue,
    const unsigned int era_x,
    const unsigned int era_y) {
    for (unsigned int x_ = x - era_x; x_ <= x + era_x; x_++) {
        for (unsigned int y_ = y - era_y; y_ <= y + era_y; y_++) {
            unsigned char *pixel = tf(x_, y_);
            pixel[2] = red;
            pixel[1] = green;
            pixel[0] = blue;
        }
    }
}


// 绘制白色十字线
void paint_cross() {
    // 中心作坐标
    const unsigned int x_mid = info.xres / 2;
    const unsigned int y_mid = info.yres / 2;

    for (unsigned int y = 0; y < info.yres; y++)
        paint_pixel_rect(x_mid, y, 0xff, 0xff, 0xff, 0, 0);
    for (unsigned int x = 0; x < info.xres; x++)
        paint_pixel_rect(x, y_mid, 0xff, 0xff, 0xff, 0, 0);

}


// 处理函数，根据温度值与中线位置计算纵坐标
unsigned process(const int temp, const unsigned mid_y) {
    unsigned heigh = temp < 0 ? mid_y - (unsigned) temp : mid_y + temp; // 计算温度位置
    heigh = heigh > info.yres ? info.yres : heigh; // 处理边界条件，防止温度过高超出屏幕
    heigh = temp > 0 ? info.yres - heigh : heigh;
    return heigh;
}


// 光栅化
void Bresenham(unsigned int x0, unsigned int y0, const unsigned int x1, const unsigned int y1) {
    // 使用有符号整数进行运算，支持负方向
    const int dx = abs((int) x1 - (int) x0);
    const int dy = abs((int) y1 - (int) y0);
    const int sx = (x0 < x1) ? 1 : -1; // x方向步进
    const int sy = (y0 < y1) ? 1 : -1; // y方向步进
    int err = (dx > dy ? dx : -dy) / 2; // 误差项初始化
    while (1) {
        // 绘制当前像素
        paint_pixel_rect(y0, x0, x0, x0, x0, 1, 1);
        if (x0 == x1 && y0 == y1) break; // 到达终点则退出
        const int e2 = err; // 计算下一个误差
        if (e2 > -dx) {
            // x方向误差过大
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            // y方向误差过大
            err += dx;
            y0 += sy;
        }
    }
}


// 刷新屏幕,即绘制函数，根据queue的元素绘制内容
// 采样保证温度范围为-20～100，就不做异常值处理了
void refresh_screen(const queue *q) {
    brush_screen(0x00, 0x00, 0x00); // 刷黑
    paint_cross(); // 十字线
    unsigned int y_index = line.left; // 采样点索引
    const unsigned int mid_y = info.yres / 2; // 中间坐标
    for (unsigned int index = 0; index < q->size; index++) {
        // 当前点
        const int temp = q->arr[y_index];
        const unsigned x0 = process(temp, mid_y);
        const unsigned y0 = index * line.groupsize;
        // 绘制数据点标记
        paint_pixel_rect(y0, x0, 0xff, 0x00, 0x00, 3, 3); // 红色
        if (index < q->size - 1) {
            // 下一个点
            const int next_temp = q->arr[(y_index + 1) % q->size];
            const unsigned x1 = process(next_temp, mid_y);
            const unsigned y1 = (index + 1) * line.groupsize;
            // 绘制两点之间的线段
            Bresenham(x0, y0, x1, y1);
        }
        y_index = (y_index + 1) % q->size;
    }
}
