#ifndef WORDFREQ_H
#define WORDFREQ_H

void createWords(FILE* file, void *ht);

void openFiles(char *arg, void *ht);

void wordFreq(int argc, char *argv[], int printNum, int totalFlags);

#endif
