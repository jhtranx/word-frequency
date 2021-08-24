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

static void printWord(const Byte *word, unsigned wordLength)
{
   int i;

   /* For readability, this program limits each line to 30 characters max */
   for (i = 0; i < wordLength && i < 30; i++)
   {
      if (isprint(word[i]))
         printf("%c", word[i]);
      else
         printf("%c", '.');
   }
   
   /* Indicate the word is longer that was actually displayed */
   if (wordLength > 30)
      printf("...");

   printf("\n");
}

int findPrintNum(int uniqueCount, int printNum)
{
   return (uniqueCount > printNum ? printNum : uniqueCount);
}

void printResults(void *ht, int printNum, int uniqueCount, HTEntry *entries)
{
   int i;

   printNum = findPrintNum(uniqueCount, printNum);

   printf("%d unique words found in %d total words\n", 
      uniqueCount, htTotalEntries(ht));
   for (i = 0; i < printNum; i++)
   {
      printf("%10u - ", (entries[i]).frequency);
      printWord(((Word*)(entries[i]).data)->bytes, 
         ((Word*)(entries[i]).data)->length);
   }
}
