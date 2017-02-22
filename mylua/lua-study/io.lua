#!/usr/local/bin/lua
print ("hello lua \n")

local file=io.open("if.lua", "r")

io.input(file) -- 默认文件输入

print(io.read()) --输出文件第一行

io.close(file) -- 关闭文件

file = io.open("if.lua", "a")

io.output(file) -- 默认文件输出

io.write("-- if.lua 文件末尾注释")

io.close()





