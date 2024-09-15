#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> // 包含 errno 和 strerr
#include <sys/stat.h> // 包含 stat 的头文件
#include <stddef.h>//包含size_t
#include "include.h"
/**
 * @brief 初始化一个Git仓库
 * @param repo_dir 仓库目录
 */

void init_repository(const char *repo_dir) {
    // 检查目标目录是否已经是Git仓库
    char dot_git_path[1024]; // 扩大缓冲区大小
    snprintf(dot_git_path, sizeof(dot_git_path), "%s/.git", repo_dir);

    FILE *check = fopen(dot_git_path, "r");
    if (check != NULL) {
        fclose(check);
        fprintf(stderr, "警告: 目标目录 %s 已经是一个Git仓库。\n", repo_dir);
        return;
    }

    // 创建 .git 目录
    if (mkdir(dot_git_path, 0777) == -1) {
        perror("无法创建 .git 目录");
        return;
    }

    /*
    @brief创建 .git/config 文件,动态分配config_path内存
    */

    size_t config_path_len = strlen(dot_git_path) + strlen("/config") + 1;
    char *config_path = (char *)malloc(config_path_len);

    if (config_path == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return;
    }

    snprintf(config_path, config_path_len, "%s/config", dot_git_path);
    FILE *config_file = fopen(config_path, "w");
    if (config_file == NULL) {
        perror("无法创建 config 文件");
        free(config_path); // 记得释放动态分配的内存
        return;
    }

    // 写入基本配置信息
    fprintf(config_file, "[core]\n");
    fprintf(config_file, "    repositoryformatversion = 0\n");
    fprintf(config_file, "    filemode = true\n");

    fclose(config_file);
    free(config_path); // 释放动态分配的内存
    printf("初始化空的Diangit仓库在 '%s'\n", repo_dir);
}
