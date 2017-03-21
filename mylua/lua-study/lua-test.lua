#!/usr/local/bin/lua

print("hello lua")

local x = 1
local y = x
local y = 10
assert(x==1)

local function func(x)
    x = 10
end

x=1
func(x)
assert(x==1)


-- table 传递引用
--

x={aa="11", 12}
y = x
x.aa = "xia"
assert(y.aa=="xia")

local function func1(x)
    if(type(x)=="table")then
        x.bb="bb"
    else
        print("x type not table")
    end
end

x={}
func1(x)
assert(x.bb=="bb")





