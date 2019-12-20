#include <stdio.h>
#include <stdlib.h>

void snail(long w, long h, void (*ret)(long, void*), void *data)
{
    /* uzupełnić - wartość zwracamy przez wywołanie ret(x, data)
       ret - wskaźnik na funkcję zwracającą dane (tzw. callback)
       data - wskaźnik na dane kontekstowe wywoływanej funkcji
       Przykład użycia - zwrócenie po kolei liczb od 1 do 5:
           int i;
           for (i = 1; i <= 5; i++)
               ret(i, data);
     */
    const int incdec = w, rozmiar = w * h;
    int i , licznik, add;
    add = 0;
    licznik = 0;
    while(licznik <  rozmiar)
    {
      for (i = 0; i < w; i++)
      {
          licznik++;
          ret(++add, data);
      }
      h--;
      if(licznik >=  rozmiar) break;
      for (i = 0; i < h; i++)
      {
          add += incdec;
          licznik++;
          ret(add, data);
      }
      w--;
      if(licznik >=  rozmiar) break;
      for (i = 0; i < w; i++)
      {
          add--;
          licznik++;
          ret(add, data);
      }
      h--;
      if(licznik >=  rozmiar) break;
      for (i = 0; i < h; i++)
      {
          add -= incdec;
          licznik++;
          ret(add, data);
      }
      w--;
    }

}

struct print_data
{
    long w;
    int d, c;
};

static int parse(long *w, long *h, int argc, char *argv[])
{
    char *err;

    if (argc != 3)
        return -1;

    *w = strtol(argv[1], &err, 0);

    if (err && *err)
        return -1;

    *h = strtol(argv[2], &err, 0);

    if (err && *err)
        return -1;

    return 0;
}

static void print_init(struct print_data *p, long w, long h)
{
    long n = w * h;
    int d = 1;

    while (n > 9)
        d++, n /= 10;

    p->w = w;
    p->d = d;
    p->c = 0;
}

static void print(long v, void *data)
{
    struct print_data *p = (struct print_data *) data;

    printf("%*ld%c", p->d, v, (p->c++, p->c %= p->w) ? ' ' : '\n');
}

int main(int argc, char *argv[])
{
    long w, h, i, j;
    struct print_data pd;

    if (parse(&w, &h, argc, argv) < 0)
        return 1;

    printf("%ld %ld\n\n", w, h);

    print_init(&pd, w, h);

    for (i = 0; i < h; i++)
        for (j = 0; j < w; j++)
            print(i * w + j + 1, &pd);

    printf("\n");
    snail(w, h, print, &pd);

    return 0;
}
