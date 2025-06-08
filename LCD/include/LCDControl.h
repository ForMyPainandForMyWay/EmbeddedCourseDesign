//
// Created by 冬榆 on 25-6-6.
//

#ifndef LCDCONTROL_H
#define LCDCONTROL_H
#include <linux/fb.h>
#include "l_aux.h"
#include "array.h"
#include "LCDControl.h"


unsigned char *framebuffer_back; // 缓冲区
unsigned char *framebuffer;

struct fb_var_screeninfo info; // 屏幕信息
size_t bytes_per_pixel = 0; // 每像素字节
size_t screensize = 0; // 像素数
int lcdfd = 0; // 帧缓冲设备
queue line; // 像素队列

// 启动接口
void start();


// 结束接口
void stop();


// 温度队列接口
void push(int value);


// 屏幕刷新绘制接口
void refresh();
#endif //LCDCONTROL_H
