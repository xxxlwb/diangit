#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <zlib.h>
#include "../include/include.h"
#define SHA_DIGEST_LENGTH 20

extern char *commit_object_path;

// 创建分支 (refs/heads)
void create_branch(const char *branch_name)
{
    char branch_path[1024];
    struct stat st = {0};
    if (stat(".git/refs", &st) == -1)
    {
        int ret = mkdir(".git/refs", 0777); // 创建目录
        if (ret == -1)
        {
            perror("创建目录失败");
            return;
        }
    }
    if (stat(".git/refs/heads/", &st) == -1)
    {
        int ret = mkdir(".git/refs/heads/", 0777); // 创建目录
        if (ret == -1)
        {
            perror("创建目录失败");
            return;
        }
    }
    snprintf(branch_path, sizeof(branch_path), "%s%s", ".git/refs/heads/", branch_name);

    FILE *branch_file = fopen(branch_path, "w");
    if (branch_file == NULL)
    {
        perror("无法创建分支文件");
        return;
    }
    const char *filename = "test.txt";
    unsigned char *data = get_file_data(filename); // 获取文件数据
    if (!data)
    {
        perror("获取文件数据失败");
        return;
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    calculate_sha1(data, hash); // 计算哈希值

    char hash_str[SHA_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(hash_str + i * 2, "%02x", hash[i]); // 格式化输出
    }
    fprintf(branch_file, "%s\n", hash_str); // 写入提交哈希值
    fclose(branch_file);

    printf("成功创建分支 '%s' 指向提交 %s\n", branch_name, hash_str);
}