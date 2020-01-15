#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
//fixed point arithmetic library
typedef int16_t fixed; //choose your width, and specify the double width type
typedef int32_t dfixed; 
#define bpoint 9 //binary point places
#define SWAP(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))


//multiply and divide
#define fmul(x, y) ((fixed)((dfixed)x*(dfixed)y>>bpoint)) //a/x*b/x in terms of x: (ab/x)/x
#define fdiv(x, y) ((fixed)(((dfixed)x<<bpoint)/(dfixed)y)) //a/x / b/x in terms of x: (ax/b)/x

//convert number to fixed point
#define fix(x) ((fixed)x<<bpoint)

//output routines
#define whole(x) (x>>bpoint)
#define pointmask ((1<<bpoint)-1)
#define frac(x) (x&pointmask)

char buf[15];

fixed fxsqrt(fixed n) {
	if(n < 0) {
		return fix(-1);
	}
	if(n == 0) {
		return 0;
	}
	fixed x = n >> 1;
	fixed y = 0;
	for(;;) {
		y = (x + fdiv(n,x)) >> 1;
		if( y >= x) {
			return x;// << (bpoint >> 1);
		}
		x = y;
	}
}

//output first the whole number, then the point, and then the fractional point
char *fxtoa(fixed f, char *out){
	div_t d;
	char *res = out;
	char *head = out-1;
	uint8_t j = 0;
	if(f < 0 && !*out) {
		*(out++) = '-';
		f = ~f + 1;
		head++;
	}

	fixed i = whole(f);

	if(*out == -1) {
		return res;
	}
	do {
		d = div(i, 10);
		*(out++) = d.rem + '0';
		i = d.quot;
		j++;
	} while(d.quot != 0 && ~*out);

	char *foot = out;
	j /= 2;

	while (j) {
		char c = head[j];
		head[j] = foot[-j];
		foot[-j] = c;
		j--;
	}

	*(out++) = '.';
	
	if(*out == -1) {
		return res;
	}

	f = frac(f) * 10;
	do {
		i = whole(f);
		*(out++) = i + '0';
		f = frac(f) * 10;
	} while(f != 0 && ~*out);

	*out = '\0';
	return res;
}

