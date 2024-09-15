#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> // 包含 errno 和 strerr
#include <sys/stat.h> // 包含 stat 的头文件
#include "include.h"
/*
@brief 定义命令枚举类型
*/ 


typedef enum {
    init,
    hash_object,
    cat_file,
    unknown
} Command;

// 解析命令行参数
Command parse_args(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "错误: 需要至少一个命令\n");
        return unknown;
    }

    if (strcmp(argv[1], "init") == 0) {
        return init;
    } else if (strcmp(argv[1], "hash-object") == 0) {
        return hash_object;
    } else if (strcmp(argv[1], "cat-file") == 0) {
        return cat_file;
    }

    fprintf(stderr, "未知命令: %s\n", argv[1]);
    return unknown;
}


int main(int argc, char *argv[]) {
    Command command = parse_args(argc, argv);

    switch (command) {
        case init:
            if (argc < 3) {
                init_repository("./"); // 默认为当前目录
            } else {
                init_repository(argv[2]); // 用户指定的目录
            }
            break;
        case hash_object:
            if (argc < 3) {
                fprintf(stderr, "错误: 请提供文件路径\n");
                return EXIT_FAILURE;
            }
            Hash_object(argv[2]);
            break;
        case cat_file:
            if (argc < 3) {
                fprintf(stderr, "错误: 请提供对象哈希值\n");
                return EXIT_FAILURE;
            }
            Cat_file(argv[2]);
            break;

        case unknown:
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}