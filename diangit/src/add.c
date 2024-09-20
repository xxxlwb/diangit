#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "../include/include.h"

void Add(const char *filename) {
    unsigned char *data = get_file_data(filename);
    if (!data) {
        perror("获取文件数据失败");
        return;
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    calculate_sha1(data, hash);

    char hash_str[SHA_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hash_str + i * 2, "%02x", hash[i]);
    }

    FILE *index_file = fopen(".git/index", "ab");//ab模式，追加写入,b模式，二进制文件,a模式，追加模式
    if (!index_file) {
        perror("无法打开索引文件");
        return;
    }
    fprintf(index_file, "%s ", filename);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        fprintf(index_file, "%02x", hash[i]);
    }
    fprintf(index_file,"\n");
    fclose(index_file);
    printf("已将文件 %s 添加到暂存区\n",filename);
}    
