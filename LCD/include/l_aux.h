//
// Created by 冬榆 on 25-6-6.
//

#ifndef AUX_H
#define AUX_H
#include <stdbool.h>
#include <stddef.h>

extern unsigned char *framebuffer_back; // 缓冲区
extern unsigned char *framebuffer;

extern struct fb_var_screeninfo info; // 屏幕信息
extern size_t bytes_per_pixel; // 每像素字节
extern size_t screensize; // 像素数
extern int lcdfd; // 帧缓冲设备

bool init_screen();

bool clean();

unsigned char *tf(unsigned int x, unsigned int y);


#endif //AUX_H
