#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void mul64(uint64_t *p, uint64_t a, uint64_t b)
{
    /*varibles declaration */
    uint32_t Ahigh, Bhigh, Alow, Blow;
    uint64_t AlowmulBlow, AlowmulBhigh, AhighmulBlow, AhighmulBhigh, aux, aux2, aux3;
    register int16_t i;

    /*setting varibles to 0 */
    Ahigh   = 0;
    Bhigh   = 0;
    Alow    = 0;
    Blow    = 0;
    aux     = 0;
    aux3    = 0;
    p[0]    = 0;
    p[1]    = 0;


    /* gething and setting higher and lower bits */
    for(i = 0; i<64; i++)
    {
        /* set Alow, Blow bits from a anb b varibles */
        if( i<32 )
        {
            if( (a >> i) & 1UL) Alow |= 1UL << i;
            if( (b >> i) & 1UL) Blow |= 1UL << i;
        }else{
            if( (a >> i) & 1UL) Ahigh |= 1UL << (i-32);
            if( (b >> i) & 1UL) Bhigh |= 1UL << (i-32);
        }
    }

    /* Alow mul Blow */
    AlowmulBlow = Alow * Blow;

    if(Alow!=0)
    if(AlowmulBlow/Alow!=Blow) /*checking overflow of multiplication */
    printf("AlowmulBlow overflow");

    /* Alow mul Bhigh */
    AlowmulBhigh = Alow * Bhigh;

    if(Alow!=0)
    if( AlowmulBhigh /Alow!=Bhigh) /*checking overflow of multiplication */
    printf("AlowmulBhigh overflow");

    /* Ahigh mul Blow */
    AhighmulBlow = Ahigh * Blow;

    if(Ahigh!=0)
    if( AhighmulBlow /Ahigh!=Blow) /*checking overflow of multiplication */
    printf("AhighmulBlow overflow");

    /* Ahigh mul Bhigh */
    AhighmulBhigh = Ahigh * Bhigh;

    if(Ahigh!=0)
    if( AhighmulBhigh /Ahigh!=Bhigh) /*checking overflow of multiplication */
    printf("AhighmulBlow overflow");


    /* a mul b
       first - copy bits into p, aux, aux3 */
for(i = 0; i<64;i++)
    {
        if( (AlowmulBlow >> i) & 1UL) p[0] |= 1UL << i;
        if( (AhighmulBhigh>> i) & 1UL) p[1] |= 1UL << i;
        if(i<32){
         if ( (AhighmulBhigh >> i) & 1UL)  aux |= 1UL << (i+32);
         if ( (AlowmulBlow >> (i+32)) & 1UL) aux3 |= 1UL << i;
         }else{
         if ( (AlowmulBlow >> i) & 1UL)  aux |= 1UL << (i-32);
         if ( (AhighmulBhigh>> (i-32)) & 1UL) aux3 |= 1UL << i;
         }
    }

aux2 = aux;
aux = aux + AlowmulBhigh + AhighmulBlow + aux3;
if( aux - AlowmulBhigh - AhighmulBlow - aux3 !=aux2 ) /*checking overflow of addition */
printf("Addition overflow");

for(i = 0; i<64;i++)
    if(i<32)
    {
     if( (aux >> i) & 1UL)
      p[0] |= 1UL << (i+32);else
      p[0] &= ~(1UL << (i+32));
    }else
    {
     if( (aux >> i) & 1UL)
      p[1] |= 1UL << (i-32);else
      p[1] &= ~(1UL << (i-32));
    }

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
