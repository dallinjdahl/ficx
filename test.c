#include "ficx.h"

int main(int argc, char **argv) {
	char strbuf[15];
	strbuf[14] = -1;
	fixed x = fix(169);
	fixed y = fxsqrt(x);
	printf("sqrt(%s)", fxtoa(x,strbuf));
	printf(": %s\n", fxtoa(y,strbuf));
	return 0;
};

