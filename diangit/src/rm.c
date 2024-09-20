#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "../include/include.h"

/**
 * @brief 移除暂存区内指定文件，将暂存区文件内容写入index.temp，利用continue语句跳过指定删除的文件
 * @param fgets()函数从流中读取n-1个字符，保存到line指向的字符串中，遇到换行符或文件结束符时结束。
 * @param sscanf()函数从一个字符串中读进与指定格式相符的数据
 * @param continue语句结束当前循环，开始下一轮循环
 * @param fopen()函数打开一个文件，返回一个指向FILE对象的指针
 * @param remove()函数用于删除一个文件
 * @param rename()函数用于修改文件名或移动文件
 * @param fprintf()函数用于向文件中写入格式化数据
 */
void remove_file_from_index(const char *filename) {
    FILE *index_file = fopen(".git/index", "rb");
 

    FILE *temp_file = fopen(".git/index.temp", "wb");
   

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
