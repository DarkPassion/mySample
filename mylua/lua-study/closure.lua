#!/usr/local/bin/lua

print("hello lua")

function func1()
    local index=0
    local inner=function()
        print(index)
        index=index+1
    end

    inner() -- 0
    inner() -- 1
    print(index) -- 2
end


func1()


function func2()
    local index=0
    print("hello")
    return function()
        print(index)
        index=index+1
    end
end

local inner=func2()
print(inner) -- function: 0xaa
inner() -- 0
inner() -- 1

local other=func2()
other() -- 0
other() -- 1




