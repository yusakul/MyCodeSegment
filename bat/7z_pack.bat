@echo off
::设置压缩密码
set pass=123

for /d %%a in (*) do (
::遍历当前目录的文件夹，并将其输出到pack文件夹里面
::-mhe=on  让文件名也加密
::-mx 压缩等级 0-9之间选择[0仅仅是复制 ]
    7z.exe a -t7z  ".\pack\%%~na" ".\%%a\*" -p"%pass%" -mx=9 -mhe=on  -x"!*.7z"
)
