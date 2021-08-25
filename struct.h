#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include "hashTable.h"

#define CK_ALLOC(_ALLOC)\
{\
   if (NULL == (_ALLOC)) {\
      fprintf(stderr, "wf: system_specific_error_message_here");\
      perror(NULL);\
      exit(EXIT_FAILURE);\
   }\
}

typedef struct node
{
   int freq;
   void *data;

   struct node *next;
} HashNode;

typedef struct
{
   HTFunctions *functions;
   unsigned *sizesArray;
   int numSizes;
   int currSizeIdx;
   unsigned capacity;
   unsigned uniqueEntries;
   unsigned totalEntries;
   float rehashLoadFactor;

   HashNode **theArray;
} HashTable;

int htCurrSizeIdx(void *hashTable);

#endif
