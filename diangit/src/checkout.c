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

 void calculate_sha2(const unsigned char *data, size_t size, unsigned char *hash) {
    SHA1(data, size, hash); // 使用实际的大小
}

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
    
printf("解压后数据: ");
for (size_t i = 0; i < commit_size && i < 100; ++i) {
    printf("%02x ", commit_data[i]);
}
printf("\n");

   // 跳过 'blob <size>:' 前缀部分，找到实际数据
    unsigned char *File_data = commit_data;
    // 找到第一个冒号
    for (size_t i = 0; i < commit_size; i++) {
        if (commit_data[i] == ':') {
            File_data = commit_data + i + 1; // 冒号后面的字节是实际数据
            break;
        }
    }

    

    size_t file_data_size = commit_size - (File_data - commit_data);


    // 解析提交对象中的文件哈希值
    unsigned char file_hash[SHA_DIGEST_LENGTH]; 
    //calculate_sha1(commit_data, file_hash); //这个报错，没有正确计算commit_data的大小？
    calculate_sha2(File_data, file_data_size, file_hash); // 使用实际的大小
  
printf("File data: %p, Size: %zu\n", File_data, file_data_size); // 根据文件哈希值还原文件

 printf("File hash str: ");
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        printf("%02x", file_hash[i]);
    }
    printf("\n");

    // 还原文件对象
    char file_object_path[5096];
    snprintf(file_object_path, sizeof(file_object_path), "work.txt");

    //unsigned char *file_data;
    //size_t file_size;
    //if (decompress_object(file_object_path, &file_data, &file_size) == 0) {
        // 将文件数据写回工作区
        FILE *output_file = fopen(file_object_path, "w");
        fwrite(File_data, 1, file_data_size, output_file);
        fclose(output_file);
        printf("成功切换到提交版本 %s\n", commit_hash);
     

    free(commit_data);
    
}