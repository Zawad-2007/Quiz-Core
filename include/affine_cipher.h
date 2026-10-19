#ifndef AFFINE_CIPHER_H
#define AFFINE_CIPHER_H

#include"mathematics.h"

void encrypt_affine(unsigned char *string, int length, int a, int c);

void decrypt_affine(unsigned char *string, int length, int a, int c);

#endif