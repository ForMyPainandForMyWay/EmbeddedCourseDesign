//
// Created by 冬榆 on 25-6-6.
//

// const struct fb_var_screeninfo info, unsigned char* framebuffer, size_t bytes_per_pixel
#include "l_aux.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


// 初始化屏幕以及全局变量
bool init_screen() {
    // 打开帧缓冲设备
    lcdfd = open("/dev/fb0", O_RDWR);
    if (lcdfd < 0) {
        perror("Error opening framebuffer");
        return false;
    }

    // 获取屏幕信息
    if (ioctl(lcdfd, FBIOGET_VSCREENINFO, &info)) {
        perror("Error reading screen info");
        close(lcdfd);
        return false;
    }

    printf("Resolution: %dx%d, %d bits/pixel\n",
           info.xres, info.yres, info.bits_per_pixel);

    // 计算显存大小
    bytes_per_pixel = info.bits_per_pixel / 8;
    screensize = info.xres * info.yres * bytes_per_pixel;

    // 映射显存
    framebuffer = mmap(0, screensize,
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED, lcdfd, 0);
    if (framebuffer == MAP_FAILED) {
        perror("Error mapping framebuffer");
        close(lcdfd);
        return false;
    }

    // 分配并初始化双缓冲
    framebuffer_back = (unsigned char *) malloc(screensize);
    memset(framebuffer_back, 0x00, screensize);
    return true;
}


// 清理资源
bool clean() {
    free(framebuffer_back); // 清空双缓冲
    bool success = true;
    success &= !munmap(framebuffer, screensize);
    return success & close(lcdfd);
}


// 坐标转换函数，左上角是0,0 右下角是l,w
unsigned char *tf(const unsigned int x, const unsigned int y) {
    return framebuffer_back + (y * info.xres + x) * bytes_per_pixel;
}
