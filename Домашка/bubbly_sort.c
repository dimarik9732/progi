#include <stdio.h>

int bubble_sort(int A[],int n){
	int i;
	int j;
	_Bool f;
	f = 1;
	for(i=0;i<n-1 && f ;i++){
		f = 0;
		for(j=0;j<n-1;j++){
			if(A[j] > A[j+1]){
				A[j+1] = A[j]^A[j+1];
				A[j] = A[j]^A[j+1];
				A[j+1] = A[j+1]^A[j];
				f = 1;
			}
		}
	}
	return 0;
}

int main (void) {
	
	printf("Input count of Array Elements\n");
	int n;
	scanf("%d",&n);

	int mas[n];
	printf("Input %d Array Elements\n",n);
	int i;
	for (i = 0; i < n; i++){
		scanf("%d",&mas[i]);
	}
	
	bubble_sort(mas,n);
	
	printf("Sorted 	Array: ");
	for (i = 0; i < n; i++){
		printf("%d ",mas[i]);
	}
	printf("\n");
 	
	return 0;
}

