#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/include.h"

/*
@brief 获取当前的时间戳
@param fgets()函数从流中读取n-1个字符，保存到str指向的字符串中，遇到换行符或文件结束符时结束。
*/

void log_history(const char *repo_dir) {
    char log_file_path[1024];
    snprintf(log_file_path, sizeof(log_file_path), "%s/.git/logs", repo_dir);

    FILE *log_file = fopen(log_file_path, "r");
    if (!log_file) {
        perror("无法打开日志文件");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), log_file) != NULL) {
        printf("%s", line);  // 输出日志文件中的每一行
    }

    fclose(log_file);
}