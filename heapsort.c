#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Functions prototypes
*/
typedef int (*cmp_t)(const void *a, const void *b);
int cmp_int(const void *a, const void *b);
int cmp_str(const void *a, const void *b);

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
    int   *baseint = (int*)array;
    char **basestr = (char **)array;

	if(array == NULL) return;

        if(cmp == cmp_int)
        {
            if(leftChild < heapSize && cmp(&baseint[leftChild], &baseint[maxIndex])==1){
                maxIndex = leftChild;
            }
            if(rightChild < heapSize && cmp(&baseint[rightChild], &baseint[maxIndex])==1){
                maxIndex = rightChild;
            }
            if(maxIndex != parentIndex){
                swap(&baseint[maxIndex], &baseint[parentIndex], size);
                validateMaxHeap(array, heapSize, size, maxIndex, cmp);
            }
        }else
        {
            if(leftChild < heapSize && cmp(&basestr[leftChild], &basestr[maxIndex])>0){
                maxIndex = leftChild;
            }
            if(rightChild < heapSize && cmp(&basestr[rightChild], &basestr[maxIndex])>0){
                maxIndex = rightChild;
            }
            if(maxIndex != parentIndex){
                swap(&basestr[maxIndex], &basestr[parentIndex], size);
                validateMaxHeap(array, heapSize, size, maxIndex, cmp);
            }
        }
}
void heapsort(void *base, size_t nmemb, size_t size, cmp_t cmp)
{
    /* Declaring local function varibles */
    int   *baseint = (int*)base;
    char **basestr = (char **)base;
    int n = nmemb;
    register int i;

		if(base == NULL || nmemb == 0) return;

		if(cmp == cmp_int) /* Are we comparing int or string values? */
		{ /*comparing int values */
            for(i = n / 2 - 1; i >= 0; i--){
                validateMaxHeap(base, n, size, i, cmp);
            }
            for(i = n - 1; i > 0; i--){
                swap(&baseint[0], &baseint[i], size);
                validateMaxHeap(base, --n, size, 0, cmp);
            }
		}else
		{ /*comparing strings values */
            for(i = n / 2 - 1; i >= 0; i--){
                validateMaxHeap(base, n, size, i, cmp);
            }
            for(i = n - 1; i > 0; i--){
                swap(&basestr[0], &basestr[i], size);
                validateMaxHeap(base, --n, size, 0, cmp);
            }
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
