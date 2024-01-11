@echo off
::设置解压缩密码
set pass=123


:: 跳到bat所在的文件夹
cd /d %~dp0
:: 如果不存在unpack文件夹,则创建一个
if not exist %~dp0/unpack md unpack
::设置解压文件的路径，保存在unpack文件夹里面
set out_pth=%~dp0/unpack

::遍历解压文件夹
:: /d 只遍历当前目录的文件夹  /r遍历当前目录的文件夹（含子目录）
for /r . %%a in (*) do (
    7z.exe x "%%a"  -p"%pass%" -o"%out_pth%" -x"!*.bat"
)
