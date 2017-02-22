#!/usr/local/bin/lua

print("hello lua")


local function table_print(a)
    print("table print function "..type(a))
    for k,v in ipairs(a) do
        print("k="..k.." v="..v)
    end
end

-- setmetatable(table,metatable)
-- 设置table元表(metatable) 
-- getmetatable(table)
-- 返回设置的元表

local table1={"hello"}
local table2={"1", "2", "3"}
setmetatable(table1,table2)
table_print(getmetatable(table1))


-- __index 元方法
-- 如果A是B的元表，如果访问A中不存在的成员，就会在B中进行查找，如果索引失败的情况会调用__index函数
print()
other={foo=3}
t=setmetatable({},{__index=other})
print(t.foo)

print()
local table3={key1="value1"}
print(table3.key1, table3.key2)
mytable=setmetatable(table3, {
    __index=function(table3, key)
        print("call metatable __index method "..key)
        if key=="key2"then
            return "metable-value"
        else
            return nil
        end
    end
        
})

print(table3.key1, table3.key2, table3.key3)
-- table3.key1 存在key1键不会调用到__index函数
-- call metatable __index method key2
-- call metatable __index method key3

-- __newindex 元方法
--

print()
local table4={}
local t4=setmetatable({key1="value1"}, {__newindex=table4})
print(t4.key1)
t4.newkey="new-val"
print(t4.newkey, table4.newkey)
t4.key1="update-value"
print(t4.key1, table4.key1)


print()
t4=setmetatable({key1="value1"},{
    __newindex=function(t4,key,value)
        rawset(mytable,key, "\"..value..\"")
    end
})

t4.key1="new-value"
t4.key2="key-value"
print(t4.key1, t4.key2)












