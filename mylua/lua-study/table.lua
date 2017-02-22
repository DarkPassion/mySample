#!/usr/local/bin/lua
print ("hello lua")

local t1={}
print("t1 type ".. type(t1))


t1[1]="Lua"
t1["wow"]="Hello"

print("t1[1] = ".. t1[1])
print("t1[\"wow\"] = "..t1["wow"])

print()

local t2=t1
print("t2[1] = ".. t2[1])
print("t2[\"wow\"] = "..t2["wow"])

t2=nil
print("t2=nil")
print("t1[1] = ".. t1[1])
print("t1[\"wow\"] = "..t1["wow"])



fruits={"banana","orange","apple"}
-- table.concat(table [,sep [,start [,end]]])
-- sep 元素分割符号
-- start 从table起始位置
-- end table结束为止

print("table.concat(fruits) ".. table.concat(fruits))

print("table.concat(fruits, \",\") "..table.concat(fruits, ","))

print("table.concat(fruits, \",\", 2, 3) "..table.concat(fruits, ",",2,3))


-- table.insert(table,[pos,] vale)
-- pos 指定位置插入
-- value 插入值

print("table.insert(fruits, \"lua\")", table.insert(fruits, "lua"))
print("fruits[4] :", fruits[4])

print("table.insert(fruits, 4, \"4lua\")", table.insert(fruits, 4, "4lua"))
print("fruits[4] :", fruits[4])

for k,v in ipairs(fruits) do
    print("fruits k="..k.." v="..v)
end


-- table.sort
-- 排序
--
local function table_print(a)
    print("table print function")
    for k,v in ipairs(a) do
        print("fruits k="..k.." v="..v)
    end
end

print()

print("table sort before")
table_print(fruits)
table.sort(fruits)
print("table sort after")
table_print(fruits)


print()
print("table len ".. #fruits)




