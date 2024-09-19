#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "../include/include.h"

/**
 * @brief 移除暂存区文件
 */
void remove_file_from_index(const char *filename) {
    FILE *index_file = fopen(".git/index", "rb");
    if (!index_file) {
        perror("无法打开索引文件");
        return;
    }

    FILE *temp_file = fopen(".git/index.temp", "wb");
    if (!temp_file) {
        perror("无法创建临时索引文件");
        fclose(index_file);
        return;
    }

    char line[1024];
    char file[1024];
    char hash_str[SHA_DIGEST_LENGTH * 2 + 1];
    int found = 0;
    while (fgets(line, sizeof(line), index_file)) {
        sscanf(line, "%s %s", file, hash_str);
        if (strcmp(file, filename) == 0) {
            found = 1;
            continue;
        }

        fprintf(temp_file, "%s %s\n", file, hash_str);
    }

    fclose(index_file);
    fclose(temp_file);

    if (!found) {
        fprintf(stderr, "错误: 文件 %s 不在暂存区\n", filename);
        remove(".git/index.temp");
        return;
    }

    remove(".git/index");
    rename(".git/index.temp", ".git/index");
    printf("已移除文件 %s\n", filename);
}  
