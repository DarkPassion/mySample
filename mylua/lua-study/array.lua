#!/usr/local/bin/lua
print ("hello lua \n")

local c1={"hello", "lua"}
for i=0,2 do
    print(c1[i])
end

print()

local c2={}
for i=-2,2 do
    c2[i]=i
    print(c2[i])
end

print()

local c3={}
for i=1,3 do
    c3[i]={}
    for j=1,4 do
        c3[i][j]=i*j
    end
end

for i=1,3 do
    for j=1,4 do
        print("c3 i="..i," j="..j ..": "..c3[i][j])
    end
end



