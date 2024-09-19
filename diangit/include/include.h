#include <stddef.h>
void init_repository(const char *repo_dir);

unsigned char *get_file_data(const char *filename);//获取文件数据指针

void calculate_sha1(const unsigned char *data, unsigned char *hash);//计算哈希值

void Hash_object(const char *filename);//hash-object 命令

void Cat_file(const char *hash_str);//cat-file 命令

size_t create_git_object(const unsigned char *data, unsigned char **output);//创建Git对象格式

int compress_and_store(const char *hash_str, const unsigned char *data);//压缩并存储对象到.git/objects

int decompress_object(const char *object_path, unsigned char **output, size_t *output_len);//解压缩对象

void log_history(const char *repo_dir);//输出日志文件中的每一行
 
void commit_(const char *message, const char *repo_dir);//生成提交对象并写入对象目录

unsigned char* create_tree_object(const char *filename, const char *hash_str);//生成树对象

void Checkout(const char *commit_hash);//checkout 命令

void ls_Tree(unsigned char *hash_str);//解压tree对象

void create_branch(const char *branch_name);//创建分支 (refs/heads)

void create_tag(const char *tag_name, const char *commit_hash);//创建标签 (refs/tags)

void Show_refs();//显示所有 refs (show-ref)