# 学习过程
# (一) Git学习

## git常用基础命令
本地仓库初始化为git仓库：git init

克隆远程仓库到本地：git clone "repository-url"

工作区加载到缓存区：git add .

缓存区提交到本地分支：git commit -m "message"

本地分支推送到远程仓库：git push

查看提交记录：git log

查看本地分支：git branch

创建新的本地分支：git branch "branch_name"

删除本地分支：git branch -d "branch_name"

切换本地分支：git checkout branch_name

合并指定本地分支到当前所在本地分支：git merge
 "branch_name"
将一个分支的更改应用到另一个分支的顶部:git rebase "branch_name"

查看工作区、暂存区、本地分支的状态：git status

标记特定提交（commit）的命令:git tag <tagname>（轻量标签）git tag -a <tagname> -m "Tag message"（附注标签）

# （二）准备
### Linux环境配置



### main函数参数
int main(int argc, char *argv[])
argc记录命令行总共字符串个数 argv为字符串数组
例如：./diangit init dir
此时argc = 3，argv[] = {"./diangit","init","dir" }
### Linux环境
wsl2 Ubuntu22.04
# （三）
snprintf()
fopen()
mkdir()
perror()
fprintf()
SHA()
fseek()
fread()
创建blob格式
compress()
uncompress()
fwrite()
time()
localtime()



# diangit
编译命令gcc -g ./include/* ./src/* -o diangit -lssl -lcrypto -lz 
Hello Dian 哈希值80c1cc25690de21ae43b675fabee9bbe81151c15