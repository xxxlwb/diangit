#!/usr/bin/bash

rm -rf .git

echo "Hello, World!" > test.txt

diangit init

diangit add test.txt

diangit ls-files 

diangit ls-files --verbose

diangit commit -m "Initial commit"

diangit log

diangit hash-object test.txt

diangit checkout 60fde9c2310b0d4cad4dab8d126b04387efba289

diangit cat-file 60fde9c2310b0d4cad4dab8d126b04387efba289

echo "你好，世界！" >> test.txt

diangit add test.txt

# 延时1秒

sleep 1

diangit commit -m "Update test.txt"

diangit log

diangit checkout b43a491cb9992bc70a4ce480f8769fa7dcae7a96

diangit checkout 60fde9c2310b0d4cad4dab8d126b04387efba289

diangit status

diangit check-ignore test.txt

diangit check-ignore work.txt

echo "work.txt" > .gitignore