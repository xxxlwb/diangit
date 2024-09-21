#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/include.h"

/**
 * @brief 检查文件是否被 .gitignore 忽略
 * @param strcspn()函数用于计算字符串中连续有几个字符不在指定字符串中
 * 
 */

// 检查文件是否被 .gitignore 忽略
int check_ignore(const char *filename) {
    FILE *gitignore_file = fopen(".gitignore", "r");
    if (gitignore_file == NULL) {
        return 0;  // 没有 .gitignore 文件
    }

    char line[256];
    while (fgets(line, sizeof(line), gitignore_file)) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;
//strcspn()函数用于计算字符串中连续有几个字符不在指定字符串中，目的就是为了去除换行符
        if (strcmp(line, filename) == 0) {
            fclose(gitignore_file);
            return 1;  // 文件被忽略
        }
    }

    fclose(gitignore_file);
    return 0;  // 文件未被忽略
}