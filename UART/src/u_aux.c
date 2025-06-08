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
#include <tgmath.h>
#include <unistd.h>
#include <math.h>


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
    cfsetispeed(&option, B115200); // 设置输入波特率为 115200
    cfsetospeed(&option, B115200);
    option.c_cflag &= ~CSIZE;
    option.c_cflag |= CS8; // 数据位为8位
    option.c_cflag &= ~PARENB; // 无奇偶校验
    option.c_cflag &= ~CSTOPB; // 停止位为1位
    option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 设置串口为Raw 模式
    option.c_oflag &= ~OPOST;
    tcflush(uartfd, TCIOFLUSH); // 清除输入输出缓冲区
    tcsetattr(uartfd, TCSANOW, &option); // 写入配置

    // 初始化缓冲区
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
    while (index < MAX_BUFFER_SIZE) {
        unsigned char c;
        const ssize_t n = read(uartfd, &c, 1); // 逐字节读取
        if (n < 0) return false;
        if (n > 0) {
            uart_buffer_read[index++] = c;
            if (c == end_char) {
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
        usleep(20); // 物理层延迟
    }
    return true;
}


// 展示数据，将读缓冲区的内容输出
void show_msg() {
    printf("Message received:\n");
    for (size_t i = 0; i < nums; i++) printf("%c", uart_buffer_read[i]);
    printf("\n");
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
    for (int i = 1; i < totalLen; i++) {
        if (i != pointPlace - 1) result += pow_(fractor, abs(i - (pointPlace - 1))) * (int) uart_buffer_read[i];
        else fractor = 0.1f;
    }
    // result = uart_buffer_read[0]=='+' ? result : -result;
    return uart_buffer_read[0] == '+' ? result : -result;
}
