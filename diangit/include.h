#include <stddef.h>
void init_repository(const char *repo_dir);

void calculate_sha1(const unsigned char *data, size_t length, unsigned char *hash) ;//计算文件的hash值

void Hash_object(const char *filename);//hash-object 命令

void Cat_file(const char *hash_str);//cat-file 命令

size_t create_git_object(const unsigned char *data, size_t length, unsigned char **output);//创建Git对象格式

int compress_and_store(const char *hash_str, const unsigned char *data, size_t length) ;//压缩并存储对象到.git/objects

int decompress_object(const char *object_path, unsigned char **output, size_t *output_len);//解压缩对象

