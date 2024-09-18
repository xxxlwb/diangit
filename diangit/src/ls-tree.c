#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <zlib.h>
#include "include.h"
#include <stddef.h>

// diangit ls-tree
void ls_tree(const char *hash_str, const char *repo_dir) {
    char tree_path[1024];
    snprintf(tree_path, sizeof(tree_path), "%s/.git/objects/%.2s/%s", repo_dir, hash_str, hash_str + 2);

    unsigned char *tree_data = NULL;
    size_t tree_data_len = 0;
    if (decompress_object(tree_path, &tree_data, &tree_data_len) != 0) {
        perror("解压tree对象失败");
        return;
    }

    Object *tree_obj = parse_object(tree_data);
    if (!tree_obj) {
        perror("解析tree对象失败");
        free(tree_data);
        return;
    }

    // 遍历tree对象的内容
    char *content_ptr = tree_obj->content;
    while (content_ptr < tree_obj->content + tree_obj->size) {
        char mode[7];
        char name[256];
        char hash[SHA_DIGEST_LENGTH * 2 + 1];
        size_t size;

        sscanf(content_ptr, "%6s %255s %20s %zu", mode, name, hash, &size);

        printf("%s %s %s %zu\n", mode, name, hash, size);
        content_ptr += strlen(content_ptr) + 1; // 移动到下一个条目
    }

    free(tree_data);
    free_object(tree_obj);
}
