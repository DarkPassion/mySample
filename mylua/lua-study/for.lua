#!/usr/local/bin/lua
print ("hello lua")

function f(x)
    print("function ==")
    return x*2
end

print("xx for i=1,f(3) print(i)")
for i=1,f(3) do print(i)
end
-- 1,2,3,4,5,6

print("")
for i=1,10,11 do --print(i)
    print(i)
end
-- 1






