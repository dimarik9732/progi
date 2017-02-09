#include <stdio.h>

int main(void){
        unsigned first;
        unsigned second;
        unsigned temp;
        printf("Please, input the first number: ");
        scanf("%u", &first);
        printf("Please, input the second number: ");
        scanf("%u", &second);

        while ((first - second) != 0) {

                if (first <  second){
                        temp  = first;
                        first = second;
                        second = temp;
                        }

                first = first - second;
        }

        printf("GCD is ");
        printf("%u", first);
	printf("\n");
	return 0;
}

