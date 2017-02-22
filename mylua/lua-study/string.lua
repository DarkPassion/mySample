#!/usr/local/bin/lua
print ("hello lua")

local s1="lua"
print("\"字符串 s1 = \"",s1)

local s2='lua'
print("\"字符串 s2 = \"",s2)

local s3=[["lua"]]
print("\"字符串 s3 = \"",s3)



if (s1=="lua")then
    print("s1==\"lua\"")
end

print("stirng.upper: "..string.upper(s1))
-- LUA

print("string.lower:"..string.lower("LUA"))
-- lua

print("string.gsub:"..string.gsub("lua pppppp", "p","s"))
-- lua ssssss

print("string.find:", string.find("hello Lua user","Lua", 1))
-- 7 9

print("string.reverse",string.reverse("Lua"))
-- auL

print("string.format", string.format("hello %s","lua"))
-- hello lua

print("string.len", string.len("hello lua"))
-- 9
print("string.rep", string.rep("Lua", 2))
-- LuaLua



