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

// 创建标签 (refs/tags)
void create_tag(const char *tag_name, const char *commit_hash) {
    char tag_path[1024];
    snprintf(tag_path, sizeof(tag_path), "%s%s", ".git/refs/tags/", tag_name);

    FILE *tag_file = fopen(tag_path, "w");
    if (tag_file == NULL) {
        perror("无法创建标签文件");
        return;
    }

    fprintf(tag_file, "%s\n", commit_hash);  // 写入提交哈希值
    fclose(tag_file);
    printf("成功创建标签 '%s' 指向提交 %s\n", tag_name, commit_hash);
}

// 列出所有分支 (refs/heads)
void list_branches() {
    printf("现有分支:\n");
    system("ls .git/refs/heads/");
}

// 列出所有标签 (refs/tags)
void list_tags() {
    printf("现有标签:\n");
    system("ls .git/refs/tags/");
}

// 显示所有 refs (show-ref)
void Show_refs() {
    printf("现有 refs:\n");
    printf("分支:\n");
    system("ls .git/refs/heads/");
    printf("标签:\n");
    system("ls .git/refs/tags/");
}