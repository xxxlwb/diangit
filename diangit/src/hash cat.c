#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <zlib.h>
#define SHA_DIGEST_LENGTH 20

/**
 * @brief 接收文件名，获取文件数据的指针
 * 
 */

unsigned char *get_file_data(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("无法打开文件");
        return NULL;
    }
    fseek(file, 0, SEEK_END);//将文件指针移动到文件末尾
    size_t length =(size_t )ftell(file);//获取文件大小
    fseek(file, 0, SEEK_SET);//将文件指针移动到文件开头

    unsigned char *data = (unsigned char *)malloc(length);
    if (fread(data, 1, length, file) != length) {//读取文件数据
        perror("无法读取文件");
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return data;
}

/**
 * @brief 接收文件名，计算文件的SHA1哈希值
 * 
 */

void calculate_sha1(const unsigned char *data, unsigned char *hash) {
     SHA1(data, strlen((char *)data), hash);//计算哈希值
}

// 创建 Git 对象格式
size_t create_git_object(const unsigned char *data, unsigned char **output) {
    size_t length = strlen((char *)data);
    const char *type = "blob";
    size_t total_len = strlen(type) + 1 + snprintf(NULL, 0, "%zu", length) + 1 + length;//计算总长度

    *output = (unsigned char *)malloc(total_len);//分配内存
    unsigned char *p = *output;
    p += sprintf((char *)p, "%s %zu:", type, length);//格式化输出
    memcpy(p, data, length);//拷贝数据

    return total_len;
}

// 压缩数据并存储到 .git/objects 目录

int compress_and_store(const char *hash_str, const unsigned char *data) {
    size_t length = strlen((char *)data);
    char dir_path[1024];
    snprintf(dir_path, sizeof(dir_path), ".git/objects/%.2s", hash_str);//创建目录路径

    mkdir(dir_path, 0777);//创建目录
       
    char file_path[2048];
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, hash_str + 2);//创建文件路径

    FILE *file = fopen(file_path, "wb");//打开文件
   
    uLongf compressed_length = compressBound(length);//计算压缩后的长度,compressBound()函数用于计算压缩后的长度
    unsigned char *compressed_data = (unsigned char *)malloc(compressed_length);//分配内存

    compress(compressed_data, &compressed_length, data, length);//压缩数据,compress()函数用于压缩数据,
       
    fwrite(compressed_data, 1, compressed_length, file);
    fclose(file);
    free(compressed_data);
    return 0;
}

//hash-object命令

void Hash_object(const char *filename) {
   
    unsigned char *data = get_file_data(filename);//获取文件数据
    unsigned char hash[SHA_DIGEST_LENGTH];
    calculate_sha1(data, hash);//计算哈希值

    char hash_str[SHA_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hash_str + i * 2, "%02x", hash[i]);//格式化输出
    }

    printf("文件哈希值: %s\n", hash_str);

    // 创建 Git 对象格式
    unsigned char *git_object_data;
    size_t total_len = create_git_object(data, &git_object_data);//创建Git对象格式
    if (compress_and_store(hash_str, git_object_data) == -1) {//压缩并存储对象
        perror("压缩和存储失败");
    }

    free(git_object_data);
    free(data);
}

//解压缩对象

int decompress_object(const char *object_path, unsigned char **output, size_t *output_len) {
    FILE *file = fopen(object_path, "rb");//打开文件
    
    fseek(file, 0, SEEK_END);
    long compressed_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("压缩文件大小: %ld\n", compressed_size);

    unsigned char *compressed_data = (unsigned char *)malloc(compressed_size);//分配内存
    

    fread(compressed_data, 1, compressed_size, file);
    fclose(file);

    uLongf decompressed_size = 1024 * 1024; // 初始分配解压后大小
    *output = (unsigned char *)malloc(decompressed_size);

    if (uncompress(*output, &decompressed_size, compressed_data, compressed_size) != Z_OK) {
        fprintf(stderr, "数据解压失败\n");
        free(compressed_data);
        return -1;
    }

    printf("解压后数据大小: %zu\n", decompressed_size);
    printf("解压后数据前100字节: ");
    for (size_t i = 0; i < decompressed_size && i < 100; ++i) {
        printf("%02X ", (*output)[i]);
    }
    printf("\n");

    *output_len = decompressed_size;
    free(compressed_data);
    return 0;
}

//cat-file命令

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
            printf("解压后数据大小: %zu\n", decompressed_size);
            unsigned char *content = (unsigned char *)memchr(decompressed_data, ':', decompressed_size);
            if (content) {
                printf("头部信息: %.*s\n", (int)(content - decompressed_data), (const char *)decompressed_data);

                content++; // 跳过冒号
                size_t content_size = decompressed_size - (content - decompressed_data);
                fwrite(content, 1, content_size, stdout);
                printf("\n");
            } 
        } 
    } 

    free(decompressed_data);
}

