#!/usr/local/bin/lua

print("hello lua")

local function func_test1(a,b)
    local c = a + b
    return c
end

print("a+b : 1+2=", func_test1(1,2));


function func_test2(a,b)
    return a*b
end

print("a*b : 2*4=", func_test2(2,4));


function func_test3(a,b,f1,f2)
    print("a type", type(a))
    print("b type", type(b))
    print("f1 type", type(f1))
    print("f2 type", type(f2))

    if (type(a) == "number")then
        print("a type is number")
    else
        print("a type is not number")
    end

    return f1(a,b)+f2(a,b)
end

print("(a*b)+(a+b) :(1*2)+(1+2)=", func_test3(1,2,func_test1,func_test2))


function func_test4(a,b)
    local c = func_test1(a,b)
    local d = func_test2(a,b)
    return c,d
end

print("c,d : ", func_test4(2,4))

if(func_test4(1,1)==2)then
    print("func_test4(1,1)==2 true")
else
    print("func_test4(1,1)==2 flase")
end

local t1,t2 = func_test4(1,1)
print("func_test4(1,1) : t1="..t1.." t2="..t2)


local function func_test5(...)
    local result=0;
    local arg={...}
    for i,v in ipairs(arg) do
        result=result+v
    end
    print("arg num : "..#arg)
    return result/#arg
end

print("func_test5(1,2,3,4,5) = "..func_test5(1,2,3,4,5))


