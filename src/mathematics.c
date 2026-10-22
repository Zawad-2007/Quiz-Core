#include"../include/mathematics.h"

int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modulo_inv(int a, int m)
{
    if (gcd(a, m) != 1)
    {
        printf("a and m are required to be coprime(gcd(a,m)=1) for inverse of a modulo m to exist\n");
        return -1;
    }
    else
    {
        for (int i = 0; i < m; i++) // Range of multiplicative inverse in modular arithmatic : 0 to m-1
        {
            if ((a * i) % m == 1) // a*i=1(mod m) -> here i is the inverse of a modulo m
            {
                return i;
            }
        }
    }
    return -1;
}