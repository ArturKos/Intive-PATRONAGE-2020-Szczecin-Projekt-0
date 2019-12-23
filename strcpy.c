#include <stdio.h>
#include <stdlib.h>

void str_copy(char *to, const char *from)
{
    while ( *to++ = *from ++);
}
int main(int argc, char *argv[])
{
 char zrodlo[]= "to jest napis";
 char dest[25];

 str_copy(dest, zrodlo);
 printf("%s", dest);
 return 0;
}
