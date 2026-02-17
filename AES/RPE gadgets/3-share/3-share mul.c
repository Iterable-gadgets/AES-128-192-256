void mult_gadget_function(uint8_t * a, uint8_t * b, uint8_t * c){
	uint8_t r0 = get_rand();
	uint8_t r1 = get_rand();
	uint8_t r2 = get_rand();
	uint8_t r3 = get_rand();
	uint8_t r4 = get_rand();
	uint8_t r5 = get_rand();
	uint8_t r6 = get_rand();
	uint8_t r7 = get_rand();
	uint8_t r8 = get_rand();
	uint8_t r9 = get_rand();
	uint8_t r10 = get_rand();



	uint8_t var0 = Add(a[0], r0) ;
	uint8_t var1 = Add(var0, r1) ;
	uint8_t var2 = Add(b[0], r2) ;
	uint8_t var3 = Add(var2, r3) ;
	uint8_t var4 = Add(a[1], r1) ;
	uint8_t var5 = Add(var4, r4) ;
	uint8_t var6 = Add(b[1], r3) ;
	uint8_t var7 = Add(var6, r5) ;
	uint8_t var8 = Add(a[2], r4) ;
	uint8_t var9 = Add(var8, r0) ;
	uint8_t var10 = Add(b[2], r5) ;
	uint8_t var11 = Add(var10, r2) ;


	uint8_t var12 = Multiply(var1, var3) ;
	uint8_t var13 = Add(var12, r6) ;
	uint8_t var14 = Multiply(var1, var7) ;
	uint8_t var15 = Add(var14, r7) ;
	uint8_t var16 = Multiply(var1, var11) ;
	uint8_t var17 = Add(var16, r8) ;
	uint8_t var18 = Add(var13, var15) ;
	c[0] = Add(var18, var17) ;


	uint8_t var19 = Multiply(var5, var3) ;
	uint8_t var20 = Add(var19, r7) ;
	uint8_t var21 = Multiply(var5, var7) ;
	uint8_t var22 = Add(var21, r9) ;
	uint8_t var23 = Multiply(var5, var11) ;
	uint8_t var24 = Add(var23, r10) ;
	uint8_t var25 = Add(var20, var22) ;
	c[1] = Add(var25, var24) ;


	uint8_t var26 = Multiply(var9, var3) ;
	uint8_t var27 = Add(var26, r8) ;
	uint8_t var28 = Multiply(var9, var7) ;
	uint8_t var29 = Add(var28, r10) ;
	uint8_t var30 = Multiply(var9, var11) ;
	uint8_t var31 = Add(var30, r6) ;
	uint8_t var32 = Add(var27, var29) ;
	uint8_t var33 = Add(var32, var31) ;
	c[2] = Add(r9, var33) ;


}
