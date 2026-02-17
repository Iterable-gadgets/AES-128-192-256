void copy_gadget_function(uint8_t * a, uint8_t * d, uint8_t * e){
	uint8_t r0 = get_rand();
	uint8_t r1 = get_rand();
	uint8_t r2 = get_rand();
	uint8_t r3 = get_rand();
	uint8_t r4 = get_rand();
	uint8_t r5 = get_rand();



	uint8_t var0 = Add(r0, r1) ;
	d[0] = Add(var0, a[0]) ;
	uint8_t var1 = Add(r2, r3) ;
	e[0] = Add(var1, a[0]) ;


	uint8_t var2 = Add(r4, r0) ;
	d[1] = Add(var2, a[1]) ;
	uint8_t var3 = Add(r5, r2) ;
	e[1] = Add(var3, a[1]) ;


	uint8_t var4 = Add(r1, r4) ;
	d[2] = Add(var4, a[2]) ;
	uint8_t var5 = Add(r3, r5) ;
	e[2] = Add(var5, a[2]) ;
}
