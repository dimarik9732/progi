#include <stdio.h>
unsigned int HashRs(char *str)
{

		const unsigned int b = 378551;
		unsigned int a = 63689;
		unsigned int hash = 0;

		for(; *str; str++)
		{
			hash = hash * a + (unsigned char)(*str);
			a *= b;
		}

		return hash;

}
int main (void) {
	
	int a = 0x0bbbaaaa;
	unsigned int b = HashRs("nina_solnce123");
	char c = (char)a;
	printf("%x %hx %x\n",a, b,c);
	return 0;
}
