#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "command.h"

void exitFailFlags()
{
   fprintf(stderr, "Usage: wf [-nX] [file...]\n");
   exit(EXIT_FAILURE);
}

void setNum(char *arg, int *printNum, int *totalFlags)
{
   if (0 == sscanf(&arg[2], "%d", printNum)) 
      exitFailFlags();
   *totalFlags += 1;
}

void checkNum(char *arg, int *printNum, int *totalFlags)
{
   if (!strncmp(arg, "-n", 2))
      setNum(arg, printNum, totalFlags);
   else if (arg[0] == '-')
      exitFailFlags();
}

void checkFlags(int argc, char *argv[], int *printNum, int *totalFlags)
{
   int i;
   for (i = 1; i < argc; i++)
      if (strlen(argv[i]) >= 3)
         checkNum(argv[i], printNum, totalFlags);
}

void processArgs(int argc, char *argv[], int *printNum, int *totalFlags)
{
   checkFlags(argc, argv, printNum, totalFlags);
}
