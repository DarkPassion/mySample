#!/usr/local/bin/lua
print ("hello lua \n")


function func()
    print(debug.traceback("Stack trace"))
    print(debug.getinfo(1))
    print("Stack trace end")
    return 100
end


func()
print(debug.getinfo(1))


