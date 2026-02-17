#ORDER 1
#SHARES 2
#IN a b 
#RANDOMS r0 r1 r2 r3 
#OUT c 


var0 = a0 + r0
var1 = a0 + var0
var2 = b0 + r1
var3 = b1 + r1


var4 = var0 * var2
var5 = var4 + r2
var6 = var0 * var3
var7 = var6 + r3
c0 = var5 + var7


var8 = var1 * var2
var9 = var8 + r2
var10 = var1 * var3
var11 = var10 + r3
c1 = var9 + var11
