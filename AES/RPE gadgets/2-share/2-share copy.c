void copy_gadget_function(uint8_t * a, uint8_t * d, uint8_t * e){
	uint8_t r0 = get_rand();
	uint8_t r1 = get_rand();





	d[0] = Add(a[0], r0) ;
	e[0] = Add(a[0], r1) ;




	d[1] = Add(a[1], r0) ;
	e[1] = Add(a[1], r1) ;
}
