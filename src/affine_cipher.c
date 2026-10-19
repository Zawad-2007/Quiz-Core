#include "../include/mathematics.h"

void encrypt_affine(unsigned char *string, int length, int a, int c)
{
    for (int i = 0; i < length; i++)
    {
        string[i] = (a * string[i] + c) % 256;
        // Modulo 256 prevents byte overflow data loss naturally
    }
}


void decrypt_affine(unsigned char *string, int length, int a, int c)
{
    int inv_a = modulo_inv(a, 256);

    for (int i = 0; i < length; i++)
    {
        string[i] = (inv_a * (string[i] - c + 256)) % 256; // added 256 with (string[i] - c) to get the value positive
                                                           //  x=ā*(y-c)(mod m) -> ā*(y-c)=x(mod m)
                                                           // when ā*(y-c) is divided by m the remainder is x
                                                           // x=ā*(y-c)%m [also, ā*(y-c)%m == ā*(y-c+m)%m]
    }
}