#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <zlib.h>

/**
 * @brief 计算文件的hash值
 * 
 */

void calculate_sha1(const unsigned char *data, size_t length, unsigned char *hash) {
    SHA1(data, length, hash);
}

// 创建 Git 对象格式
size_t create_git_object(const unsigned char *data, size_t length, unsigned char **output) {
    const char *type = "blob";
    size_t total_len = strlen(type) + 1 + snprintf(NULL, 0, "%zu", length) + 1 + length;
    *output = (unsigned char *)malloc(total_len);

    if (*output == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return 0;
    }

    unsigned char *p = *output;
    p += sprintf((char *)p, "%s %zu", type, length);
    memcpy(p, data, length);

    return total_len;
}

// 压缩数据并存储到 .git/objects 目录
int compress_and_store(const char *hash_str, const unsigned char *data, size_t length) {
    char dir_path[1024];
    snprintf(dir_path, sizeof(dir_path), ".git/objects/%.2s", hash_str);

    if (mkdir(dir_path, 0777) == -1 && errno != EEXIST) {
        perror("无法创建对象目录");
        return -1;
    }

    char file_path[2048];
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, hash_str + 2);

    FILE *file = fopen(file_path, "wb");
    if (!file) {
        perror("无法打开对象文件");
        return -1;
    }

    uLongf compressed_length = compressBound(length);
    unsigned char *compressed_data = (unsigned char *)malloc(compressed_length);

    if (compress(compressed_data, &compressed_length, data, length) != Z_OK) {
        fprintf(stderr, "数据压缩失败\n");
        free(compressed_data);
        fclose(file);
        return -1;
    }

    fwrite(compressed_data, 1, compressed_length, file);
    fclose(file);
    free(compressed_data);
    return 0;
}

// hash-object 命令
void Hash_object(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("无法打开文件");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *data = (unsigned char *)malloc(file_size);
    if (!data) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        return;
    }

    fread(data, 1, file_size, file);
    fclose(file);

    unsigned char sha1_hash[SHA_DIGEST_LENGTH];
    calculate_sha1(data, file_size, sha1_hash);

    // 转换哈希为字符串
    char hash_str[41];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hash_str + (i * 2), "%02x", sha1_hash[i]);
    }

    // 创建 Git 对象格式
    unsigned char *git_object_data;
    size_t git_object_size = create_git_object(data, file_size, &git_object_data);

    // 压缩并存储对象
    if (git_object_size > 0 && compress_and_store(hash_str, git_object_data, git_object_size) == 0) {
        printf("%s\n", hash_str);  // 输出对象的哈希值
    }

    free(git_object_data);
    free(data);
}

// 解压缩对象
int decompress_object(const char *object_path, unsigned char **output, size_t *output_len) {
    FILE *file = fopen(object_path, "rb");
    if (!file) {
        perror("无法打开对象文件");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long compressed_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *compressed_data = (unsigned char *)malloc(compressed_size);
    if (!compressed_data) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        return -1;
    }

    fread(compressed_data, 1, compressed_size, file);
    fclose(file);

    uLongf decompressed_size = 1024 * 1024; // 初始分配解压后大小
    *output = (unsigned char *)malloc(decompressed_size);

    if (uncompress(*output, &decompressed_size, compressed_data, compressed_size) != Z_OK) {
        fprintf(stderr, "数据解压失败\n");
        free(compressed_data);
        free(*output);
        return -1;
    }

    free(compressed_data);
    *output_len = decompressed_size;
    return 0;
}

void Cat_file(const char *hash_str) {
    char object_path[1024];
    snprintf(object_path, sizeof(object_path), ".git/objects/%.2s/%s", hash_str, hash_str + 2);

    // 输出调试信息，确保路径正确
    printf("对象文件路径: %s\n", object_path);

    unsigned char *decompressed_data = NULL;
    size_t decompressed_size = 0;

    if (decompress_object(object_path, &decompressed_data, &decompressed_size) == 0) {
        // 确保 decompressed_data 非空并且 size 正确
        if (decompressed_data) {
            unsigned char *content = memchr(decompressed_data, '\0', decompressed_size);
            if (content) {
                content++; // 跳过类型头和大小信息
                size_t content_size = decompressed_size - (content - decompressed_data);
                fwrite(content, 1, content_size, stdout);
                printf("\n");
            } else {
                fprintf(stderr, "未找到内容分隔符\n");
            }
        } else {
            fprintf(stderr, "解压缩数据为空\n");
        }
    } else {
        fprintf(stderr, "无法解压缩对象文件\n");
    }

    free(decompressed_data);
}