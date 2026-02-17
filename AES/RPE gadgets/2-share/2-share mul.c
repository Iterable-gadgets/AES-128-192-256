void mult_gadget_function(uint8_t * a, uint8_t * b, uint8_t * c){
	uint8_t r0 = get_rand();
	uint8_t r1 = get_rand();
	uint8_t r2 = get_rand();
	uint8_t r3 = get_rand();



	uint8_t var0 = Add(a[0], r0) ;
	uint8_t var1 = Add(a[1], r0) ;
	uint8_t var2 = Add(b[0], r1) ;
	uint8_t var3 = Add(b[1], r1) ;




	uint8_t var4 = Multiply(var0, var2) ;
	uint8_t var5 = Add(var4, r2) ;
	uint8_t var6 = Multiply(var0, var3) ;
	uint8_t var7 = Add(var6, r3) ;
	c[0] = Add(var5, var7) ;


	uint8_t var8 = Multiply(var1, var2) ;
	uint8_t var9 = Add(var8, r2) ;
	uint8_t var10 = Multiply(var1, var3) ;
	uint8_t var11 = Add(var10, r3) ;
	c[1] = Add(var9, var11) ;
}
