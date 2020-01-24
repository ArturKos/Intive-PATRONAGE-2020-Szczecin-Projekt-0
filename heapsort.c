#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*
Functions prototypes
*/
typedef int (*cmp_t)(const void *a, const void *b);

/*
Auxiliary function which swaps the pointer of two variables
*/
void swap(void *v1, void *v2, size_t size)
{
    void *v3 = malloc(size);
    if (v3 != 0)
    {
        memmove(v3, v1, size);
        memmove(v1, v2, size);
        memmove(v2, v3, size);
        free(v3);
    }
}
void validateMaxHeap(void *array, size_t heapSize, size_t size, int parentIndex, cmp_t cmp){

	/* Declaring local function varibles */
	int maxIndex = parentIndex;
	int leftChild = parentIndex * 2 + 1;
	int rightChild = parentIndex * 2 + 2;
	bool loop = true;

	if(array == NULL) return;
    while(loop)
     {
            loop = false;
            if(leftChild < heapSize && cmp( (uint8_t*)array+leftChild*size, (uint8_t*)array+maxIndex*size)>0){
                maxIndex = leftChild;
            }
            if(rightChild < heapSize && cmp( (uint8_t*)array+rightChild*size, (uint8_t*)array+maxIndex*size)>0){
                maxIndex = rightChild;
            }
            if(maxIndex != parentIndex){
                loop = true;
                swap((uint8_t*)array+maxIndex*size, (uint8_t*)array+parentIndex*size, size);
                parentIndex = maxIndex;
                leftChild = parentIndex * 2 + 1;
                rightChild = parentIndex * 2 + 2;
            }
     }
}
void heapsort(void *base, size_t nmemb, size_t size, cmp_t cmp)
{
    /* Declaring local function varibles */
    int n = nmemb;
    register int i;

		if(base == NULL || nmemb == 0) return;
            for(i = n / 2 - 1; i >= 0; i--){
                validateMaxHeap(base, n, size, i, cmp);
            }
            for(i = n - 1; i > 0; i--){
                swap(base, ((uint8_t*)base+i*size), size);
                validateMaxHeap(base, --n, size, 0, cmp);
            }
}

int cmp_int(const void *a, const void *b)
{
    if (*(int *)a < *(int *)b)
        return -1;

    else if (*(int *)a > *(int *)b)
        return 1;

    else
        return 0;
}

int cmp_str(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(void)
{
    int A[] = {5, 8, 2, 11, 3, 7, 9, 12};
    char *B[] = { "Jacek", "Maria", "Szymon", "Zosia",
        "Adam", "Gosia", "Bonawentura", "Kunegunda" };

    int i;

      heapsort(A, 8, sizeof(*A), cmp_int);
      heapsort(B, 8, sizeof(*B), cmp_str);

    for (i = 0; i < 8; i++)
        printf("%d%c", A[i], (i == 7) ? '\n' : ' ');

    for (i = 0; i < 8; i++)
        printf("%s%c", B[i], (i == 7) ? '\n' : ' ');

    return 0;
}
