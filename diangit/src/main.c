#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> // 包含 errno 和 strerr
#include <sys/stat.h> // 包含 stat 的头文件
#include "../include/include.h"
/*
@brief 定义命令枚举类型
*/ 


typedef enum {
    init,
    hash_object,
    cat_file,
    commit,
    logs,
    checkout,
    branch,
    ls_tree,
    tag,
    show_refs,
    add,
    rm,
    status,
    lsfiles,
    checkignore,

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
    } else if (strcmp(argv[1], "commit") == 0) {
        return commit;
    } else if (strcmp(argv[1], "log") == 0) {
        return logs;
    } else if (strcmp(argv[1], "checkout") == 0) {
        return checkout;
    } else if (strcmp(argv[1], "branch") == 0) {
        return branch;
    } else if (strcmp(argv[1], "ls-tree") == 0) {
        return ls_tree;
    } else if (strcmp(argv[1], "tag") == 0) {
        return tag;
    } else if (strcmp(argv[1], "show-ref") == 0) {
        return show_refs;
    } else if (strcmp(argv[1], "add") == 0) {
        return add;
    } else if (strcmp(argv[1], "rm") == 0) {
        return rm;
    } else if (strcmp(argv[1], "status") == 0) {
        return status;
    } else if (strcmp(argv[1], "ls-files") == 0) {
        return lsfiles;
    } else if (strcmp(argv[1], "check-ignore") == 0) {
        return checkignore;
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
        case commit:
             if (argc < 4 || strcmp(argv[2], "-m") != 0) {
                fprintf(stderr, "错误: 需要提供提交信息\n");
                return EXIT_FAILURE;
            }
            commit_(argv[3], "./"); // 提交所有文件
            Hash_object("test.txt");
            break;
        case logs:
            log_history("./"); // 默认为当前目录
            break;
        case checkout:
            if (argc < 3) {
                fprintf(stderr, "错误: 请提供提交哈希值\n");
                return EXIT_FAILURE;
            }
            Checkout(argv[2]);
            break;
        case branch:    
            if (argc < 3) {
                fprintf(stderr, "错误: 请提供分支名\n");
                return EXIT_FAILURE;
            }
            create_branch(argv[2]);
            break;
        case ls_tree:
            if (argc < 3) {
                fprintf(stderr, "错误: 请提供对象哈希值\n");
                return EXIT_FAILURE;
            }
            ls_Tree(argv[2]);
            break;
        case tag:
            
            create_tag(argv[2], argv[3]);
            break;
        case show_refs:
            Show_refs();
            break;
        case add:
            Add(argv[2]);
            break;
        case rm:
            remove_file_from_index(argv[2]);
            break;
        case status:
            show_status();
            break;
        case lsfiles:
        int verbose = 0;
        if (argc == 2) {
            ls_files(verbose);
            break;
        } else if (argc == 3 && strcmp(argv[2], "--verbose") == 0) {
            verbose = 1;
        
        ls_files(verbose);
        break;
        } else {
            fprintf(stderr, "错误: 无效参数\n");
            return EXIT_FAILURE;
        }
        break;
        case checkignore:
            if (argc < 3) {
                fprintf(stderr, "错误: 请提供文件名\n");
                return EXIT_FAILURE;
            }
            if (check_ignore(argv[2])) {
                printf("文件被忽略\n");
            } else {
                printf("文件未被忽略\n");
            }
            break;

        case unknown:
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}