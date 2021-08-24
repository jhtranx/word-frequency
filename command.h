#ifndef COMMAND_H
#define COMMAND_H

#define DEFAULT_PRINT_NUM 10

void exitFailFlags();

void setNum(char *arg, int *printNum, int *totalFlags);
void checkNum(char *arg, int *printNum, int *totalFlags);
void checkFlags(int argc, char *argv[], int *printNum, int *totalFlags);

void processArgs(int argc, char *argv[], int *printNum, int *totalFlags);

#endif
