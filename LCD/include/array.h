//
// Created by 冬榆 on 25-6-6.
//

#ifndef ARRAY_H
#define ARRAY_H

typedef struct array {
    unsigned int size; // 大小
    int *arr; // 数组元素,存储温度
    unsigned int left; // 最左侧
    unsigned int groupsize; // 每组
} queue;

void init_array(queue *array, unsigned int size, unsigned int start, unsigned int groupsize);

void push_queue(queue *array, int value);
#endif //ARRAY_H
