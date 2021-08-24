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

/*
 * This is the fundamental type of a hash table using separate
 * chaining as its collision strategy. It will only be used by the
 * hash table so it can be defined in your hash table source file so
 * that it is private/file local.
 */
typedef struct node
{
   /* Other unspecified fields you deem necessary here... */
   int freq;
   void *data;

   /* The quintisential "next" pointer */
   struct node *next;
} HashNode;

/*
 * This is the structure representing a hash table. It will be defined
 * in your hash table source file so that it is private/file local. And,
 * you will be allocating dynamic memory for one so that you can return
 * a void* to one from the htCreate "constructor-like" function.
 *
 * Notice that theArray is a double-pointer. That is because it is an
 * array (a pointer) of HashNode pointers!
 */
typedef struct
{
   /* Other unspecified fields you deem necessary here... */
   HTFunctions *functions;
   unsigned *sizesArray;
   int numSizes;
   int currSizeIdx;
   unsigned capacity;
   unsigned uniqueEntries;
   unsigned totalEntries;
   float rehashLoadFactor;

   /* The quintisential "hash table", a.k.a., an array of node pointers */
   HashNode **theArray;
} HashTable;

int htCurrSizeIdx(void *hashTable);

#endif
