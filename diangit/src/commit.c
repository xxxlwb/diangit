#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/include.h"

#define SHA_DIGEST_LENGTH 20

/*
@brief 获取当前的时间戳,并改成字符串格式
@brief 生成提交对象并写入.git/objects目录
@param message 提交信息
@param repo_dir 仓库目录
*/
// 获取当前时间字符串
void get_current_time(char *buffer, size_t buffer_len) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, buffer_len, "%Y-%m-%d %H:%M:%S", t);
}

//计算哈希值，未使用step2中的函数（抽时间优化）
/*void sha1_file(const char *filename, unsigned char *hash) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("无法打开文件");
        return;
    }

    SHA_CTX ctx;
    SHA1_Init(&ctx);

    unsigned char buf[8192];
    size_t len;
    while ((len = fread(buf, 1, sizeof(buf), file)) > 0) {
        SHA1_Update(&ctx, buf, len);
    }

    SHA1_Final(hash, &ctx);
    fclose(file);
}
*/
 /**
    * @brief 创建tree对象文件，记录提交的文件名字，模式，哈希值
    */

    unsigned char* create_tree_object(const char *filename, const char *hash_str) {
    char tree_object_path[2048];
    snprintf(tree_object_path, sizeof(tree_object_path), ".git/objects/tree/tree_%s", hash_str);
    FILE *tree_file = fopen(tree_object_path, "wb");
   

    fprintf(tree_file, "模式:100644 \n");//文件模式
    fprintf(tree_file, "filename: %s\n", filename);
    fprintf(tree_file, "tree %s\n", hash_str);
    fclose(tree_file);
    unsigned char *data = get_file_data(tree_object_path);//获取文件数据
    if (!data) {
        perror("获取文件数据失败");
        return NULL;
    }
    
    return data;
}




// 生成提交对象并写入对象目录
void commit_(const char *message, const char *repo_dir) {
    // 获取文件哈希值 (简单化为针对test文件)
    //unsigned char hash[SHA_DIGEST_LENGTH];
    const char *filename = "test.txt";
    unsigned char *data = get_file_data(filename);//获取文件数据
     if (!data) {
        perror("获取文件数据失败");
        return;
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    calculate_sha1(data, hash);//计算哈希值

    char hash_str[SHA_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hash_str + i * 2, "%02x", hash[i]);//格式化输出
    }
    


    // 创建提交路径(commit提交对象)
    char commit_object_path[2048];
    snprintf(commit_object_path, sizeof(commit_object_path), "%s/.git/objects/commit_%s", repo_dir, hash_str);

    FILE *commit_file = fopen(commit_object_path, "wb");
    if (!commit_file) {
        perror("无法创建提交对象文件");
        return;
    }

    // 写入提交信息
    char time_str[1024];
    get_current_time(time_str, sizeof(time_str));

    fprintf(commit_file, "commit %s\n", message);
    fprintf(commit_file, "Date: %s\n", time_str);
    fprintf(commit_file, "File Hash: ");
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        fprintf(commit_file, "%02x", hash_str[i]);
    }
    fprintf(commit_file, "\n");
    fclose(commit_file);
    


    /**
     * @brief 创建tree对象文件，记录提交的文件名字，模式，哈希值
     */
    unsigned char *tree_data = create_tree_object(filename, hash_str);
    if (!tree_data) {
        perror("创建树对象文件失败");
        return;
    }
    //压缩并存储对象到.git/objects
    unsigned char tree_hash[SHA_DIGEST_LENGTH];
    calculate_sha1(tree_data, tree_hash);
    char tree_hash_str[SHA_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(tree_hash_str + i * 2, "%02x", tree_hash[i]);
    }
    printf("tree哈希值: %s\n", tree_hash_str);
    if (compress_and_store(tree_hash_str, tree_data) == -1) {
        perror("压缩和存储失败");
    }
   
    




    // 记录到日志文件
    char log_file_path[5096];
    snprintf(log_file_path, sizeof(log_file_path), "%s/.git/logs", repo_dir);

    FILE *log_file = fopen(log_file_path, "a");
    if (!log_file) {
        perror("无法打开日志文件");
        return;
    }

    fprintf(log_file, "%s - commit: %s\n", time_str, message);
    fprintf(log_file, "File Hash: ");
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        fprintf(log_file, "%02x", hash_str[i]);//直接输出哈希值，未转化为十六进制
    }
    fprintf(log_file,"\n");
    fclose(log_file);

    printf("提交成功: %s\n", message);
}

