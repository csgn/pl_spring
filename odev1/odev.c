/*
Sergen Cepoglu
02190201200
1.Ogretim 3.Sinif
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE INT_MAX

int parseInt(char n)
{
    return n - '0';
}

int parseChar(int n)
{
    return n + '0';
}

void Reverse(char* str)
{
    int len = strlen(str);
    int i = 0;
    while (len > i)
    {
        char tmp = str[--len];
        str[len] = str[i];
        str[i++] = tmp;
    }
}


char* Addition(char* fn, char* sn)
{
    int c = 0, r = 0;
    int i = strlen(fn)-1, j = strlen(sn)-1, k = 0;
    int fni, sni;
    char* res = (char*) malloc(sizeof(char)*SIZE);

    while (i >= 0 || j >= 0 )
    {
        sni = j < 0 ? 0 : parseInt(sn[j--]);
        fni = i < 0 ? 0 : parseInt(fn[i--]);
        c = fni + sni + r;
        r = c / 10;
        res[k++] = parseChar(c%10);
    }

    Reverse(res);
    return res;
}

char* Multiplication(char* fn, char* sn)
{

}

char* Extraction(char* fn, char* sn)
{

}

void Show(char *fn, char* sn, char* res)
{
    int fnl, snl, resl;
    int x, i;
    fnl = strlen(fn);
    snl = strlen(sn);
    resl = strlen(res);

    for (i = 0; i < fnl; i++)
        printf("%c ", fn[i]);
    printf("\n");

    x = (fnl - snl)*2;
    while (x-- > 0)
        printf(" ");

    for (i = 0; i < snl; i++)
        printf("%c ", sn[i]);

    printf("\n");
    for (i = 0; i < resl; i++)
        printf("%c ", res[i]);

}


int main()
{
    int fns, sns;
    char* fn = (char*) malloc(sizeof(char)*SIZE);
    char* sn = (char*) malloc(sizeof(char)*SIZE);
    char* res = (char*) malloc(sizeof(char)*SIZE);

    scanf("%s%s", fn, sn);

    res = Addition(fn, sn);

    Show(fn, sn, res);

    return 0;
}
