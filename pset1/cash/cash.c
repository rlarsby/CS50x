#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    //cash owed
    float x;
    //coins back
    int qa = 0;
    int da = 0;
    int na = 0;
    int pa = 0;
    //remainder
    int r;
    //control variable
    int w;
    //coins
    int q = 25; //quarter
    int d = 10; //dime
    int n = 5; //nickel
    int p = 1; //penny

    do
    {
        //ask how much change is owed
        x = get_float("change owed?: ");
    }
    while (x < 0);

    int c = round(x * 100); //new cash owed in cents!!

    if (c >= q)
    {
        r = c % q;
        w = c - r;
        qa =qa + (w / q);
        c = r;
        printf("%i quarters\n", qa);
    }
    if (c >= d)
    {
        r = c % d;
        w = c - r;
        da = da + (w / d);
        c = r;
        printf("%i dimes\n", da);
    }
    if (c >= n)
    {
        r = c % n;
        w = c - r;
        na = na + (w / n);
        c = r;
        printf("%i nickels\n", na);
    }
    if (c >= p)
    {
        r = c % p;
        w = c - r;
        pa = pa + (w / p);
        c = r;
        printf("%i pennies\n", pa);
    }
}