#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <zlib.h>
#include "include.h"
#define SHA_DIGEST_LENGTH 20
// 解析提交对象并恢复文件状态
void Checkout(const char *commit_hash) {
    char commit_path[1024];
    snprintf(commit_path, sizeof(commit_path), ".git/objects/%.2s/%s", commit_hash, commit_hash + 2);

    // 解压提交对象
    unsigned char *commit_data=NULL;
    size_t commit_size;
    if (decompress_object(commit_path, &commit_data, &commit_size) != 0) {
        printf("错误: 找不到提交对象 %s\n", commit_hash);
        return;
    }

    // 解析提交对象中的文件哈希值
    unsigned char file_hash[SHA_DIGEST_LENGTH]; 
    calculate_sha1(commit_data, file_hash); 
    if (!file_hash) {
        printf("错误: 提交对象中未找到有效的文件哈希值\n");
        free(commit_data);
        return;
    }
printf("Commit data: %p, Size: %zu\n", commit_data, commit_size); // 根据文件哈希值还原文件

 printf("File hash str: ");
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        printf("%02x", file_hash[i]);
    }
    printf("\n");

    // 还原文件对象
    char file_object_path[1024];
    snprintf(file_object_path, sizeof(file_object_path), ".git/objects/%.2s/%s", file_hash, file_hash + 2);

    unsigned char *file_data;
    size_t file_size;
    if (decompress_object(file_object_path, &file_data, &file_size) == 0) {
        // 将文件数据写回工作区
        FILE *output_file = fopen("work.txt", "w");
        fwrite(file_data, 1, file_size, output_file);
        fclose(output_file);
        printf("成功切换到提交版本 %s\n", commit_hash);
    } else {
        printf("错误: 无法解压文件对象\n");
    }

    free(commit_data);
    free(file_data);
}