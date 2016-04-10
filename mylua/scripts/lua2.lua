
function l_ff(a, b)  
    local c = c_add(a, b) + 1  
    print ("in lua1: ", c)  
    local d = c_step(c)  
    print ("in lua2: ", d)  
    return d  
end  

