void add_gadget_function(uint8_t * a, uint8_t * b, uint8_t * c){
	uint8_t r0 = get_rand();
	uint8_t r1 = get_rand();
	uint8_t r2 = get_rand();
	uint8_t r3 = get_rand();
	uint8_t r4 = get_rand();
	uint8_t r5 = get_rand();



	uint8_t var0 = Add(r0, r1) ;
	uint8_t var1 = Add(a[0], var0) ;
	uint8_t var2 = Add(r2, r3) ;
	uint8_t var3 = Add(b[0], var2) ;
	c[0] = Add(var1, var3) ;


	uint8_t var4 = Add(r2, r4) ;
	uint8_t var5 = Add(a[1], var4) ;
	uint8_t var6 = Add(r5, r1) ;
	uint8_t var7 = Add(b[1], var6) ;
	c[1] = Add(var5, var7) ;


	uint8_t var8 = Add(r5, r3) ;
	uint8_t var9 = Add(a[2], var8) ;
	uint8_t var10 = Add(r0, r4) ;
	uint8_t var11 = Add(b[2], var10) ;
	c[2] = Add(var9, var11) ;
}
