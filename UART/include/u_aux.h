//
// Created by 冬榆 on 25-6-7.
//

#ifndef AUX_H
#define AUX_H
#include <stdbool.h>
#include <stddef.h>

#define MAX_BUFFER_SIZE 256

extern int uartfd;
extern struct termios option;
extern unsigned char *uart_buffer_read;
extern unsigned char *uart_buffer_write;
extern unsigned nums;


bool init_uart(const char *filename);

bool clean_uart();

unsigned read_uart(unsigned char end_char);

bool write_uart(unsigned char end_char);

void show_msg();

float ft2float(int totalLen, int pointPlace);
#endif //AUX_H
