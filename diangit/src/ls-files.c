#include <stdio.h>
#include <string.h>

/**
 * @brief 列出所有文件
 * @param verbose 是否输出详细信息
 * @param fopen()函数打开一个文件，返回一个指向FILE对象的指针
 * @param fgets()函数从流中读取n-1个字符，保存到line指向的字符串中，遇到换行符或文件结束符时结束。
 * @param strtok()函数用于分割字符串
 * @param verbose 是否输出详细信息（哈希值）
 */
void ls_files(int verbose) {
    FILE *index_file = fopen(".git/index", "r");
    if (index_file == NULL) {
        perror("无法打开索引文件");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), index_file)) {
        // 文件名和哈希值之间用空格分隔, strtok()函数用于分割字符串
        char *file_name = strtok(line, " ");
        char *file_hash = strtok(NULL, " \n");//NULL表示从上一次分割的位置继续分割

        // 输出文件名
        printf("%s", file_name);
        printf("\n");

        // 如果使用 --verbose 参数，则输出更多信息,比如哈希值
        if (verbose == 1) {
            printf("哈希值: %s", file_hash);
            printf("\n");
           
        } 
    }

    fclose(index_file);
}