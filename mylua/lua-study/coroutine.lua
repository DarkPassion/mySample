#!/usr/local/bin/lua
print ("hello lua")


-- coroutine.create
-- coroutine.resume
-- coroutine.yield
-- coroutine.status
-- coroutine.wrap
-- coroutine.running
--


co = coroutine.create(
    function(i)
        print(i)
    end
)

coroutine.resume(co, 1) 
-- 1

print(coroutine.status(co))
-- dead

print()


co2 = coroutine.create(
    function()
        for i=1,10 do
            print(i)
            if i==3 then
                print("coroutine.status(co2)",coroutine.status(co2))
                print("coroutine.running(co2)",coroutine.running(co2))
            end
            coroutine.yield()
        end
    end
)

coroutine.resume(co2) -- 1
coroutine.resume(co2) -- 2
coroutine.resume(co2) -- 3

print("coroutine.status(co2)",coroutine.status(co2))
-- suspended

print("coroutine.running(co2)",coroutine.running(co2))
print()




-- producer and consumer
-- 利用coroutine.yield(), coroutine.resume()进行数据交换

local lProducer

function producer()
    local i = 0
    while true do
        i = i + 1
        send(i)
        if i>10 then
            break
        end
    end
end


function consumer()
    while true do
        local i = receive()
        print(i)
        if i>10 then
            break
        end
    end

end


function receive()
    local status,value=coroutine.resume(lProducer)
    return value
end

function send(x)
    coroutine.yield(x)
end


lProducer=coroutine.create(producer)
consumer()




