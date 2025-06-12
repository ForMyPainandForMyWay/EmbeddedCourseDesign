//
// Created by 冬榆 on 25-6-7.
//

#include "u_aux.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


// 初始化串口通信
bool init_uart(const char *filename) {
    // 打开文件
    uartfd = open(filename, O_RDWR);
    if (uartfd < 0) {
        perror("Error opening framebuffer");
        return false;
    }

    // 通讯设置
    tcgetattr(uartfd, &option); //获取当前的串口设置
    cfsetispeed(&option, B9600); // 设置输入波特率为 9600
    option.c_cflag &= ~CSIZE;
    option.c_cflag |= CS8; // 数据位为8位
    option.c_cflag &= ~PARENB; // 无奇偶校验
    option.c_cflag &= ~CSTOPB; // 停止位为1位
    option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 设置串口为Raw 模式
    option.c_oflag &= ~OPOST;
    tcflush(uartfd, TCIOFLUSH); // 清除输入输出缓冲区
    tcsetattr(uartfd, TCSANOW, &option); // 写入配置

    // 初始化缓冲区
    uart_buffer_read = malloc(MAX_BUFFER_SIZE * sizeof(unsigned char));
    uart_buffer_write = malloc(MAX_BUFFER_SIZE * sizeof(unsigned char));


    memset(uart_buffer_read, 0, MAX_BUFFER_SIZE * sizeof(unsigned char));
    memset(uart_buffer_write, '\n', MAX_BUFFER_SIZE * sizeof(unsigned char));
    nums = 0;
    return true;
}


// 清理
bool clean_uart() {
    free(uart_buffer_read);
    free(uart_buffer_read);
    return close(uartfd);
}


// 从串口读取数据,持续执行直到获取到数据为止
unsigned read_uart(const unsigned char end_char) {
    size_t index = 0;

    // // show();
    // brush_screen(0x00, 0x00, 0x00);
    // int x, y;
    // x = 250;y=200;
    // unsigned char* pix = testxxx(x, y);
    // pix[0] = 0x00;
    // pix[1] = 0x00;
    // pix[2] = 0xff;
    // show();

    while (index < MAX_BUFFER_SIZE) {
        // pix[0] = 0x00;
        // pix[1] = 0xff;
        // pix[2] = 0x00;
        // show();

        unsigned char c;
        const ssize_t n = read(uartfd, &c, 1); // 逐字节读取
        // paint_cross();
        // show();

        // nums_ ++;
        if (n < 0) {
            return false;
        }
        if (n > 0) {
            uart_buffer_read[index++] = c;
            if (c == end_char || c == '\n' || c == '\r') {
            // pix = testxxx(x+30, y+30);
            // pix[0] = 0xff;
            // pix[1] = 0x00;
            // pix[2] = 0;
            // show();
                nums = index;
                return (unsigned) index; // 遇到结束符
            }
        }
    }
    return false; // 缓冲区溢出
}


// 逐字节发送数据
bool write_uart(const unsigned char end_char) {
    size_t i = 0;
    while (i < MAX_BUFFER_SIZE) {
        unsigned char byte = uart_buffer_write[i];
        const ssize_t result = write(uartfd, &byte, 1); // 发送当前字节
        // 错误处理
        if (result != 1) {
            if (result < 0 && errno == EINTR) continue; // 中断重试
            return false; // 其他错误
        }
        if (byte == end_char) break; // 当遇到终止符时停止发送
        i++;
        usleep(20); // 延迟
    }
    return true;
}


// 展示数据，将读缓冲区的内容输出
void show_msg() {
    for (size_t i = 0; i < nums; i++) printf("%c", uart_buffer_read[i]);
}


// 快速幂,不考虑溢出
float pow_(float fractor, int const power) {
    float result = 1.0f;
    unsigned int uy = power >= 0 ? (unsigned) power : (unsigned) -power;
    while (uy) {
        if (uy & 1) result *= fractor;
        uy >>= 1;
        fractor *= fractor;
    }
    return power >= 0 ? result : 1.0f / result;
}


// 转换 读缓冲区数据到 浮点数
float ft2float(const int totalLen, const int pointPlace) {
    float fractor = 10;
    if (nums < totalLen) return 0;
    float result = 0;
    int tff = 1;
    for (int i = 1; i < totalLen; i++) {
        if (i != pointPlace - 1)
            result += pow_(fractor, abs(i - (pointPlace - 1))-tff) * (uart_buffer_read[i]-48);
        else {
            fractor = 0.1f;
            tff = 0;
        };
    }
    return uart_buffer_read[0] == '+' ? result : -result;
}
