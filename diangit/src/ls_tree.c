#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <unistd.h>
#include "../include/include.h"

/**
 * @brief 解压tree对象
 * @param object_path 对象路径
 * @param output 输出数据
 * @param output_len 输出数据长度
 * @return int
 */
void ls_Tree(unsigned char *hash_str) {
    char object_path[1024];
    snprintf(object_path, sizeof(object_path), ".git/objects/%.2s/%s", hash_str, hash_str + 2);
    unsigned char *decompressed_data = NULL;
    size_t decompressed_size = 0;

    if (decompress_object(object_path, &decompressed_data, &decompressed_size) == 0) {
        // 确保 decompressed_data 非空并且 size 正确
        if (decompressed_data) {
            printf("解压后数据大小: %zu\n", decompressed_size);
        //将解压后的数据转化为原来内容并打印输出
        fwrite(decompressed_data, 1, decompressed_size, stdout);
        printf("\n");
        }
    }

    free(decompressed_data);
}

            
 
