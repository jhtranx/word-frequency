#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "getWord.h"

#define IN 1
#define OUT 0

#define AVG_ENGLISH_WORD_LENGTH 17

void checkAlloc(Byte *c)
{
   if (NULL == (c)) {
      fprintf(stderr, "Cannot allocate memory\n");
      exit(EXIT_FAILURE);
   }
}

void allocWord(int c, Byte **word, unsigned *wordLength, 
   int *firstWord, int *size)
{
   if (!isspace(c)) {
      *firstWord = IN;
      (*wordLength)++;

      if (*wordLength >= *size)
         checkAlloc(*word = realloc(*word, *size *= 2));
      (*word)[(*wordLength) - 1] = tolower(c);
   }
}

int getWord(FILE *file, Byte **word, unsigned *wordLength, int *hasPrintable)
{
   /* init all varaibles */
   int c, firstWord;
   int size = AVG_ENGLISH_WORD_LENGTH;
   checkAlloc(*word = malloc(size));

   *wordLength = 0;
   *hasPrintable = 0;
   firstWord = OUT;

   /* iterate through the whitespace until you hit a word */
   while ((c = fgetc(file)) != EOF)
   {
      if (isspace(c) && (firstWord == IN))
      {
         checkAlloc(*word = realloc(*word, *wordLength));
         return 0;
      }
      allocWord(c, word, wordLength, &firstWord, &size);
      if ((firstWord == IN) && isprint(c))
         *hasPrintable = 1;
   }
   if (*wordLength > 0)
   {
      checkAlloc(*word = realloc(*word, *wordLength));
      return EOF;
   }
   free(*word);
   *word = NULL;
   return EOF;
}
