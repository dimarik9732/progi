#include <stdlib.h>
#include <stdio.h>
#define OFFSET_OF(field_name, struct_name) (unsigned long)(&(((struct_name *)NULL)->field_name)) 
#define swap(a,b) do if (sizeof(a) == sizeof(b)) { long long _a = (long long) a; long long _b = (long long) b; \
_a ^= _b; _b ^= _a; _a ^= _b; a = _a; b = _b; } while(0)

struct circle{


	double xc;
	int r;
	char yc;
	char *name;
};

int main(){
//	unsigned long a, b, c, d;
//	a = OFFSET_OF(xc,struct circle);
//	b = OFFSET_OF(yc, struct circle);
//	c = OFFSET_OF(r, struct circle);
//	d = OFFSET_OF(name, struct circle);
	double a = 1.5, b = 3.5;
	printf("%f %f\n", a, b); 
	swap(a,b);
	printf("%f %f \n", a, b);
	return 0;
}