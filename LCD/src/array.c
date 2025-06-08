//
// Created by 冬榆 on 25-6-6.
//
#include "array.h"

#include <stdlib.h>


// 初始化数组结构体内容为全0
void init_array(
    queue *array,
    const unsigned int size,
    const unsigned int start,
    const unsigned int groupsize) {
    array->size = size;
    array->arr = malloc(size * sizeof(int));
    array->left = start;
    // array->right = end;
    for (int i = 0; i < size; i++) array->arr[i] = 0;
    array->groupsize = groupsize;
}


// 处理循环队列
void push_queue(queue *array, const int value) {
    array->arr[array->left++] = value;
    array->left %= array->size;
}
