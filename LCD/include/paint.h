//
// Created by 冬榆 on 25-6-6.
//

#ifndef PAINT_H
#define PAINT_H
#include <stdbool.h>
#include <stddef.h>

#include "l_aux.h"
#include "array.h"


extern unsigned char *framebuffer_back; // 缓冲区
extern struct fb_var_screeninfo info; // 屏幕信息
extern size_t bytes_per_pixel; // 每像素字节
extern size_t screensize; // 像素数
extern int lcdfd; // 帧缓冲设备
extern queue line;

void show();

bool brush_screen(unsigned char red,
                  unsigned char green,
                  unsigned char blue);

void paint_cross();

void paint_pixel_rect(
    unsigned int x,
    unsigned int y,
    unsigned char red,
    unsigned char green,
    unsigned char blue,
    unsigned int era_x,
    unsigned int era_y);

void refresh_screen(const queue *q);

#endif //PAINT_H
