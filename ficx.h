#include <stdlib.h>
#include <inttypes.h>
//fixed point arithmetic library
typedef int16_t fixed; //choose your width, and specify the double width type
typedef int32_t dfixed; 
#define bpoint 6 //binary point places
#define SWAP(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))


//multiply and divide
#define mult(x, y) ((fixed)((dfixed)x*(dfixed)y>>bpoint)) //a/x*b/x in terms of x: (ab/x)/x
#define quot(x, y) ((fixed)(((dfixed)x<<bpoint)/(dfixed)y)) //a/x / b/x in terms of x: (ax/b)/x

//convert number to fixed point
#define fix(x) ((fixed)x<<bpoint)

//output routines
//output first the whole number, then the point, and then the fractional point
#define whole(x) (x>>bpoint)
#define pointmask ((1<<bpoint)-1)
#define frac(x) (x&pointmask)

fixed fxsqrt(fixed n) {
	fixed x = n >> 1;
	fixed y = 0;
	for(;;) {
		y = (x + n/x) >> 1;
		if( y >= x) {
			return x << (bpoint >> 1);
		}
		x = y;
	}
}

char *fxtoa(fixed f, char *out){
	fixed i = whole(f);
	div_t d;
	uint8_t j = 0;
	char *head = out;
	if(f < 0 && !*out) {
		*(out++) = '-';
		f = ~f + 1;
	}
	while(d.quot != 0 && !*out) {
		d = div(i, 10);
		*(out++) = d.rem + '0';
		j++;
		i = d.quot;
	}
	char *foot = out;
	j /= 2;
	for(; j; j--) {
		SWAP(*(head++), *(foot--)); 
	};
	
	if(*out) {
		return out;
	}
	
	*(out++) = '.';
	fixed fr = frac(f);
	uint8_t dig; 
	for(; fr > 0 && !*out; fr -= fix(dig)) {
		fr *= 10;
		dig = whole(fr);
		*(out++) = dig + '0';
	}
	
	*out = '\0';
	return out;
}
