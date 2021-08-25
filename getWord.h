#ifndef GETWORD_H
#define GETWORD_H

#include <stdio.h>

/* Handy "new" type definition for this function */
typedef unsigned char Byte;

typedef struct
{
   Byte *bytes;
   unsigned length;
} Word;

int getWord(FILE *file, Byte **word, unsigned *wordLength, int *hasPrintable);

#endif
