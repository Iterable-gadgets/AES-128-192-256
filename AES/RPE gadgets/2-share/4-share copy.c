void copy_gadget_function(uint8_t * a, uint8_t * d, uint8_t * e){
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
	uint8_t r11 = get_rand();
	uint8_t r12 = get_rand();
	uint8_t r13 = get_rand();
	uint8_t r14 = get_rand();
	uint8_t r15 = get_rand();
	uint8_t r16 = get_rand();
	uint8_t r17 = get_rand();
	uint8_t r18 = get_rand();
	uint8_t r19 = get_rand();
	uint8_t r20 = get_rand();
	uint8_t r21 = get_rand();
	uint8_t r22 = get_rand();
	uint8_t r23 = get_rand();
	uint8_t r24 = get_rand();
	uint8_t r25 = get_rand();
	uint8_t r26 = get_rand();
	uint8_t r27 = get_rand();





	uint8_t var0 = Add(a[0], r0) ;
	uint8_t var1 = Add(a[0], r1) ;




	uint8_t var2 = Add(a[1], r0) ;
	uint8_t var3 = Add(a[1], r1) ;






	uint8_t var4 = Add(r2, r3) ;
	uint8_t var5 = Add(r2, r4) ;




	uint8_t var6 = Add(r5, r3) ;
	uint8_t var7 = Add(r5, r4) ;






	uint8_t var8 = Add(r6, r7) ;
	uint8_t var9 = Add(r6, r8) ;




	uint8_t var10 = Add(r9, r7) ;
	uint8_t var11 = Add(r9, r8) ;






	uint8_t var12 = Add(a[2], r10) ;
	uint8_t var13 = Add(a[2], r11) ;




	uint8_t var14 = Add(a[3], r10) ;
	uint8_t var15 = Add(a[3], r11) ;








	uint8_t var16 = Add(var0, r12) ;
	uint8_t var17 = Add(var16, r13) ;
	uint8_t var18 = Add(var4, r14) ;
	uint8_t var19 = Add(var18, r15) ;
	d[0] = Add(var17, var19) ;


	uint8_t var20 = Add(var2, r14) ;
	uint8_t var21 = Add(var20, r13) ;
	uint8_t var22 = Add(var6, r12) ;
	uint8_t var23 = Add(var22, r15) ;
	d[1] = Add(var21, var23) ;


	uint8_t var24 = Add(var1, r16) ;
	uint8_t var25 = Add(var24, r17) ;
	uint8_t var26 = Add(var8, r18) ;
	uint8_t var27 = Add(var26, r19) ;
	e[0] = Add(var25, var27) ;


	uint8_t var28 = Add(var3, r18) ;
	uint8_t var29 = Add(var28, r17) ;
	uint8_t var30 = Add(var10, r16) ;
	uint8_t var31 = Add(var30, r19) ;
	e[1] = Add(var29, var31) ;






	uint8_t var32 = Add(var12, r20) ;
	uint8_t var33 = Add(var32, r21) ;
	uint8_t var34 = Add(var5, r22) ;
	uint8_t var35 = Add(var34, r23) ;
	d[2] = Add(var33, var35) ;


	uint8_t var36 = Add(var14, r22) ;
	uint8_t var37 = Add(var36, r21) ;
	uint8_t var38 = Add(var7, r20) ;
	uint8_t var39 = Add(var38, r23) ;
	d[3] = Add(var37, var39) ;


	uint8_t var40 = Add(var13, r24) ;
	uint8_t var41 = Add(var40, r25) ;
	uint8_t var42 = Add(var9, r26) ;
	uint8_t var43 = Add(var42, r27) ;
	e[2] = Add(var41, var43) ;


	uint8_t var44 = Add(var15, r26) ;
	uint8_t var45 = Add(var44, r25) ;
	uint8_t var46 = Add(var11, r24) ;
	uint8_t var47 = Add(var46, r27) ;
	e[3] = Add(var45, var47) ;
}
