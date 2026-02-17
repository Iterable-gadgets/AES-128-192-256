#SHARES 3
#IN a b
#RANDOMS r0 r1 r2 r3 r4 r5 r6 r7 r8 r9
#OUT d

tmp0 = r0 + r1
u0 = a0 + tmp0
w0 = a0 + u0
tmp1 = r3 + r4
v0 = b0 + tmp1

var0 = u0 * v0
var1 = w0 * v0
e0 = var0 + r6
e1 = var1 + r7
d0 = e0 + e1


tmpb0 = r1 + r2
u1 = a1 + tmpb0
w1 = a1 + u1
tmpb1 = r4 + r5
v1 = b1 + tmpb1

varb0 = u1 * v1
varb1 = w1 * v1
eb0 = varb0 + r6
eb1 = varb1 + r7
d1 = eb0 + eb1


tmpc0 = r2 + r0
u2 = a2 + tmpc0
w2 = a2 + u2
tmpc1 = r5 + r3
v2 = b2 + tmpc1

varc0 = u2 * v2
varc1 = w2 * v2
ec0 = varc0 + r6
ec1 = varc1 + r7
d2 = ec0 + ec1
