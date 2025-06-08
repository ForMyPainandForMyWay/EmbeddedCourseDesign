//
// Created by 冬榆 on 25-6-6.
//

#include "paint.h"

#include <stdlib.h>
#include <string.h>
#include <linux/fb.h>


// 双缓冲拷贝
void show() {
    memcpy(framebuffer_back, framebuffer, screensize);
}


// 使用某种颜色粉刷整个屏幕,该函数会将缓冲区覆盖
bool brush_screen(
    const unsigned char red,
    const unsigned char green,
    const unsigned char blue) {
    if (red == 0 && green == 0 && blue == 0) memset(framebuffer_back, 0x00, screensize);
    else if (red == 0xff && green == 0xff && blue == 0xff) memset(framebuffer_back, 0xff, screensize);
    else {
        // 全屏填充色
        for (unsigned int y = 0; y < info.yres; y++) {
            for (unsigned int x = 0; x < info.xres; x++) {
                // 计算像素位置
                // unsigned char* pixel = framebuffer +
                // (y * info.xres + x) * bytes_per_pixel;
                unsigned char *pixel = tf(x, y);
                // 设置RGB分量
                // 注意：具体顺序可能为RGB或BGR，这里先处理最常见情况
                if (bytes_per_pixel >= 3) {
                    // 假设默认RGB顺序
                    pixel[0] = red; // 红色分量
                    pixel[1] = green; // 绿色分量
                    pixel[2] = blue; // 蓝色分量

                    // 如果存在alpha通道（32bpp），设为0xFF（不透明）
                    if (bytes_per_pixel == 4) pixel[3] = 0xFF;
                }
            }
        }
    }
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
            if (bytes_per_pixel >= 3) {
                pixel[0] = red;
                pixel[1] = green;
                pixel[2] = blue;
            }
            if (bytes_per_pixel == 4) pixel[3] = 0xFF;
        }
    }
}


// 绘制白色十字线
void paint_cross() {
    // 中心作坐标
    const unsigned int x_mid = info.xres / 2;
    const unsigned int y_mid = info.yres / 2;

    for (unsigned int y = 0; y < info.yres; y++) {
        // unsigned char *pixel = tf(x_mid, y);
        // if (bytes_per_pixel >= 3) {
        //     pixel[0] = 0x00;
        //     pixel[1] = 0x00;
        //     pixel[2] = 0x00;
        //     if (bytes_per_pixel == 4) pixel[3] = 0xFF;
        // }
        paint_pixel_rect(x_mid, y, 0xff, 0xff, 0xff, 0, 0);
    }
    for (unsigned int x = 0; x < info.xres; x++) {
        // unsigned char* pixel = tf(x, y_mid);
        // if (bytes_per_pixel >= 3) {
        //     pixel[0] = 0x00;
        //     pixel[1] = 0x00;
        //     pixel[2] = 0x00;
        //     if (bytes_per_pixel == 4) pixel[3] = 0xFF;
        // }
        paint_pixel_rect(x, y_mid, 0xff, 0xff, 0xff, 0, 0);
    }
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
        paint_pixel_rect(y0, x0, y0, y0, y0, 0, 0);
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

    // 循环绘图
    // unsigned int y = line.left;
    // const unsigned int mid_y = info.yres / 2;  // 中间坐标
    // for (unsigned int index = 0; index < q->size ; index++) {
    //     const int temp = q->arr[y];
    //     // const unsigned heigh = unsigned(temp + (int)mid_y);
    //     unsigned heigh = temp<0 ? mid_y - (unsigned)temp : mid_y + temp;  // 计算温度位置
    //     heigh = heigh>info.yres ? info.yres : heigh;  // 处理边界条件，防止温度过高超出屏幕
    //     // 最后反转一下温度位置
    //     paint_pixel_rect(info.yres- heigh,index, temp, temp, temp, 0, 0);
    //     y = (y+1)%q->size;
    // }
    unsigned int y_index = line.left; // 采样点索引
    const unsigned int mid_y = info.yres / 2; // 中间坐标
    for (unsigned int index = 0; index < q->size; index++) {
        // 当前点
        const int temp = q->arr[y_index];
        const unsigned y0 = process(temp, mid_y);
        const unsigned x0 = index * line.groupsize;
        // 绘制数据点标记
        paint_pixel_rect(y0, x0, y0, y0, y0, 0, 0); // 红色
        if (index < q->size - 1) {
            // 下一个点
            const int next_temp = q->arr[(y_index + 1) % q->size];
            const unsigned y1 = process(next_temp, mid_y);
            const unsigned x1 = (index + 1) * line.groupsize;
            // 绘制两点之间的线段
            Bresenham(x0, y0, x1, y1);
        }
        y_index = (y_index + 1) % q->size;
    }
}
