#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"
#include "command.h"
#include "wordFreq.h"

int main(int argc, char **argv)
{
   int printNum = DEFAULT_PRINT_NUM;
   int totalFlags = 0;

   processArgs(argc, argv, &printNum, &totalFlags);

   wordFreq(argc, argv, printNum, totalFlags);
   return 0;
}
