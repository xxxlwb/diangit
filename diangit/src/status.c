#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/**
 * @brief 检查文件是否在暂存区
 * @param filename 文件名
 * @param fgets()函数从流中读取n-1个字符，保存到line指向的字符串中，遇到换行符或文件结束符时结束。
 * @param strncmp()函数用于比较两个字符串的前n个字符
 * @return 1: 在暂存区 0: 不在暂存区
 */
int is_file_in_index(const char *filename) {
    FILE *index_file = fopen(".git/index", "r");
    if (index_file == NULL) {
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), index_file)) {
        if (strncmp(line, filename, strlen(filename)) == 0) {
            fclose(index_file);
            return 1;  // 文件已在暂存区
        }
    }

    fclose(index_file);
    return 0;  // 文件不在暂存区
}
/**
 * @brief 检查文件是否被提交
 */

/**
 * @brief 显示文件状态
 * @param opendir()函数打开一个目录流，返回一个指向DIR结构的指针 DIR结构是一个系统定义的结构体
 * @param readdir()函数返回一个指向dirent结构的指针，该结构包含了目录流的下一个目录项的信息
 * @param stat()函数用来获取指定文件的文件状态，把文件的状态信息保存在结构体stat中
 * @param S_ISREG()函数用来判断指定的文件是否为普通文件
 */
void show_status() {
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("无法打开当前目录");
        return;
    }

    struct dirent *entry;
    printf("文件状态:\n");

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;  // 跳过隐藏文件

        

         
            if (is_file_in_index(entry->d_name)) {
                printf("暂存: %s\n", entry->d_name);
            } else {
                printf("未暂存: %s\n", entry->d_name);
            }
        
    }

    closedir(dir);
}