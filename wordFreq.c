#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"
#include "struct.h"
#include "command.h"
#include "wordFreq.h"
#include "getWord.h"
#include "ui.h"

static unsigned hashWord(const void *data) /* djb2 by Dan Bernstein */
{
   int c;
   unsigned hash = 5381;
   const Byte *bytes = ((Word*)data)->bytes;
   unsigned len = ((Word*)data)->length;

   while (len)
   {
      c = *bytes++;
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
      len -= 1;
   }
      
   return hash;
}

static int compareWord(const void * a, const void * b) {
   int ret, shortLength;

   shortLength = (((Word*)a))->length <=
      ((Word*)b)->length ? 
      ((Word*)a)->length : 
      ((Word*)b)->length;

   ret = memcmp(((Word*)a)->bytes,
      ((Word*)b)->bytes,
      shortLength);

   if (!ret)
      ret = (((Word*)a)->length -
         ((Word*)b)->length);

   return ret;
}

void destroyWord(const void *data)
{
   free(((Word*)data)->bytes);
}

static FILE* openFile(const char *fname)
{
   FILE* file = fopen(fname, "r");

   if (file == NULL)
   {
      fprintf(stderr, "wf: %s: ", fname);
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   return file;
}

void createWords(FILE* file, void *ht)
{
   Byte *bytes = NULL;
   Word *word = NULL;
   unsigned wordLength = 0;
   int hasPrintable = 0;

   /* Call getWord and print the words, until done... */
   while(EOF != getWord(file, &bytes, &wordLength, &hasPrintable))
   {
      CK_ALLOC(word = malloc(sizeof(Word)));
      word->bytes = bytes;
      word->length = wordLength;
      if(!hasPrintable)
      {
         free(bytes);
         free(word);
         continue;
      }
      if (htAdd(ht, word) > 1)
      {
         free(bytes);
         free(word);
      }
   }

   CK_ALLOC(word = malloc(sizeof(Word)));
   word->bytes = bytes;
   word->length = wordLength;
   if(!hasPrintable)
   {
      free(bytes);
      free(word);
   }
   else if (htAdd(ht, word) > 1)
   {
      free(bytes);
      free(word);
   }
   
   fclose(file);
}

void openFiles(char *arg, void *ht)
{
   FILE* file = openFile(arg);
   createWords(file, ht);
}

int cmpfreq (const void * a, const void * b) {
   return ( ((HTEntry*)b)->frequency - ((HTEntry*)a)->frequency);
}

int cmpbytes (const void * a, const void * b) {
   int ret, shortLength;

   shortLength = (((Word*)((HTEntry*)a)->data)->length <=
      ((Word*)((HTEntry*)b)->data)->length) ? 
      ((Word*)((HTEntry*)a)->data)->length : 
      ((Word*)((HTEntry*)b)->data)->length;

   ret = memcmp(((Word*)((HTEntry*)a)->data)->bytes,
      ((Word*)((HTEntry*)b)->data)->bytes,
      shortLength);

   if (!ret)
      ret = (((Word*)((HTEntry*)a)->data)->length -
         ((Word*)((HTEntry*)b)->data)->length);

   return ret;
}

void qsortHTEntries(HTEntry *entries, int numberOfEntries)
{
   qsort(entries, numberOfEntries, sizeof(HTEntry), cmpbytes);
   qsort(entries, numberOfEntries, sizeof(HTEntry), cmpfreq);
}

void wordFreq(int argc, char *argv[], int printNum, int totalFlags)
{
   int i;
   HTEntry *entries;
   unsigned uniqueCount;
   unsigned sizes[] = {
      13, 29, 47, 157, 2063, 9491, 18973, 
      49999, 100003, 500009, 1299989, 61149061
   };

   HTFunctions funcs = {hashWord, compareWord, destroyWord};
   void *ht = htCreate(&funcs, sizes, 12, 0.75);

   if ((argc - totalFlags - 1) == 0)
      createWords(stdin, ht);

   for (i = 1; i < argc; i++)
      if (argv[i][0] != '-')
         openFiles(argv[i], ht);

   uniqueCount = htUniqueEntries(ht);

   entries = htToArray(ht, &uniqueCount);
   qsortHTEntries(entries, uniqueCount);

   printResults(ht, printNum, uniqueCount, entries);

   free(entries);
   htDestroy(ht);
}
