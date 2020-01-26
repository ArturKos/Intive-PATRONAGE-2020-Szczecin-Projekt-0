#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void mul64(uint64_t *p, uint64_t a, uint64_t b)
{
    /*varibles declaration */
    uint32_t Ahigh, Bhigh, Alow, Blow;
    uint64_t AlowmulBlow, AlowmulBhigh, AhighmulBlow, AhighmulBhigh, middle;

    /*setting varibles */
    Ahigh   = a >> 32;
    Bhigh   = b >> 32;
    Alow    = (uint32_t)a;
    Blow    = (uint32_t)b;
    AhighmulBhigh = Ahigh * Bhigh;
    AlowmulBlow   = Alow * Blow;
    AlowmulBhigh  = Alow * Bhigh;
    AhighmulBlow  = Ahigh * Blow;
    middle = AhighmulBlow + (AlowmulBlow >> 32) + (uint32_t)AlowmulBhigh;
    p[0]    = (middle << 32) | (uint32_t)AlowmulBlow;
    p[1]    = AhighmulBhigh + (middle >> 32) + (AlowmulBhigh >> 32);



}
void print_big(char *str, uint64_t *p, size_t n)
{
    uint64_t *p32 = malloc(n * 2 * sizeof(*p32)), m;
    char *tmp = malloc(20 * n), *ptr = tmp;
    ssize_t i, j;

    for (i = 0; i < n; i++) {
        p32[2*i] = p[i] & 0xffffffffUL;
        p32[2*i+1] = p[i] >> 32;
    }

    for (i = n*2-1; i >= 0; i--) {
        while (p32[i]) {
            m = 0UL;

            for (j = i; j >= 0; j--) {
                p32[j] += m << 32;
                m = p32[j] % 10;
                p32[j] /= 10;
            }

            *ptr++ = (char)m + '0';
        }
    }

    if (ptr == tmp)
        *str++ = '0';

    else
        while (ptr > tmp)
            *str++ = *--ptr;

    *str = '\0';
    free(p32);
    free(tmp);
}

int main(int argc, char *argv[])
{
    uint64_t a, b, p[2];
    char str[40], *end;

    if (argc != 3)
        return 1;

    a = strtoul(argv[1], &end, 0);

    if (*end)
        return 1;

    b = strtoul(argv[2], &end, 0);

    if (*end)
        return 1;

    mul64(p, a, b);
    print_big(str, p, 2);
    printf("0x%016lx * 0x%016lx = 0x%016lx%016lx\n",
            a, b, p[1], p[0]);
    printf("%ld * %ld = %s\n", a, b, str);
    return 0;
}
