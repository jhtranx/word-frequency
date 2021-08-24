#include <limits.h>
#include <string.h>
#include <assert.h>
#include "hashTable.h"
#include "struct.h"

void* htCreate(
   HTFunctions *functions,
   unsigned sizes[],
   int numSizes,
   float rehashLoadFactor)
{
   HashTable* ht;
   int i;
   /* ASSERTS */
   assert(numSizes > 0);
   assert(sizes[0] != 0);

   for (i = 1; i < numSizes; i++)
      assert(sizes[i] > sizes[i - 1]);
   assert(rehashLoadFactor > 0 && rehashLoadFactor <= 1);

   /* ALLOCATE MEMORY */
   CK_ALLOC(ht = malloc(sizeof(HashTable)));

   ht->capacity = sizes[0];
   ht->uniqueEntries = 0;
   ht->totalEntries = 0;
   ht->rehashLoadFactor = rehashLoadFactor;
   ht->numSizes = numSizes;
   ht->currSizeIdx = 0;

   CK_ALLOC(ht->theArray = malloc(ht->capacity * sizeof(HashNode*)));
   /* set all HashNode pointers to NULL */
   for (i = 0; i < ht->capacity; i++)
      ht->theArray[i] = NULL;

   CK_ALLOC(ht->sizesArray = calloc(numSizes, sizeof(unsigned)));
   for (i = 0; i < numSizes; i++)
      ht->sizesArray[i] = sizes[i];

   CK_ALLOC(ht->functions = malloc(sizeof(HTFunctions)));
   ht->functions->hash = functions->hash;
   ht->functions->compare = functions->compare;
   ht->functions->destroy = functions->destroy;

   return ht;
} 

void freeSepChainCheck(void *hashTable, HashNode *prev)
{
   if ((*((HashTable*)hashTable)->functions->destroy != NULL))
      (*((HashTable*)hashTable)->functions->destroy)(prev->data); 
   free(prev->data);
}

void freeSepChain(void *hashTable, int index)
{
   HashNode *curr = NULL;
   HashNode *prev = NULL;

   curr = ((HashTable*)hashTable)->theArray[index];

   while (curr != NULL)
   {
      prev = curr;
      curr = curr->next;
      freeSepChainCheck(hashTable, prev);
      free(prev);
   }
}

void htDestroy(void *hashTable)
{
   int i;

   for (i = 0; i < htCapacity(hashTable); i++)
      if (((HashTable*)hashTable)->theArray[i] != NULL)
         freeSepChain(hashTable, i);

   free(((HashTable*)hashTable)->theArray);
   free(((HashTable*)hashTable)->functions);
   free(((HashTable*)hashTable)->sizesArray);
   free((HashTable*)hashTable);
}

void sepChainDupe(
   void *hashTable, 
   HashNode *curr, 
   void *data, 
   unsigned *freqRet,
   int *foundDupe)
{
   if (!((*((HashTable*)hashTable)->functions->compare)(curr->data, data)))
   {
      (curr->freq) += 1;
      *freqRet = curr->freq;
      *foundDupe = 1;
   }
}

void sepChainCollision(
   void *hashTable, 
   HashNode *prev, 
   void *data, 
   unsigned *freqRet,
   int foundDupe)
{
   HashNode *node = NULL;

   if (!foundDupe)
   {
      node = malloc(sizeof(HashNode));
      node->data = data;
      node->freq = 1;
      node->next = NULL;
      *freqRet = 1;
      prev->next = node;
      ((HashTable*)hashTable)->uniqueEntries += 1;
   }
}

void sepChain(void *hashTable, void *data, unsigned index, unsigned *freqRet)
{
   int foundDupe = 0;
   HashNode *curr = NULL;
   HashNode *prev = NULL;

   curr = ((HashTable*)hashTable)->theArray[index];

   while (curr && !foundDupe)
   {
      sepChainDupe(hashTable, curr, data, freqRet, &foundDupe);
      prev = curr;
      curr = curr->next;
   }
   sepChainCollision(hashTable, prev, data, freqRet, foundDupe);
}

void addToNewArrayHelper(
   HashNode *oldNode,   
   HashNode *newCurr)
{
   HashNode *prev = NULL;

   while (newCurr != NULL)
   {
      prev = newCurr;
      newCurr = newCurr->next;
   }
   prev->next = oldNode; 
}

void addToNewArray(
   HashNode *oldNode,
   HashNode **newArray,
   unsigned i)
{
   HashNode *newCurr = newArray[i];
   if (newCurr == NULL)
      newArray[i] = oldNode;
   else
      addToNewArrayHelper(oldNode, newCurr);
}

void rehashSepChain(
   void *hashTable,  
   HashNode **newArray, 
   HashNode *curr)
{
   unsigned newIdx;
   HashNode *temp = NULL;

   while (curr != NULL)
   {
      newIdx = ((*((HashTable*)hashTable)->functions->hash)(curr->data) 
         % htCapacity(hashTable));
      addToNewArray(curr, newArray, newIdx);
      temp = curr;
      curr = curr->next;
      temp->next = NULL;
   }
}

void rehashCheck(void *hashTable, HashNode **newArray, int i)
{
   if (((HashTable*)hashTable)->theArray[i] != NULL)
      rehashSepChain(hashTable, newArray, 
         ((HashTable*)hashTable)->theArray[i]);
}

void rehash(void *hashTable)
{
   int i;
   unsigned oldCapacity;
   HashNode **newArray;

   (((HashTable*)hashTable)->currSizeIdx) += 1;
   oldCapacity = htCapacity(hashTable);
   ((HashTable*)hashTable)->capacity = 
      ((HashTable*)hashTable)->sizesArray[htCurrSizeIdx(hashTable)];
   
   CK_ALLOC(newArray = 
      malloc(((HashTable*)hashTable)->capacity * sizeof(HashNode*)));
   for (i = 0; i < htCapacity(hashTable); i++)
      newArray[i] = NULL;

   for (i = 0; i < oldCapacity; i++)
      rehashCheck(hashTable, newArray, i);
   free(((HashTable*)hashTable)->theArray);

   ((HashTable*)hashTable)->theArray = newArray;
}

void checkRehash(void *hashTable)
{
   if (((HashTable*)hashTable)->rehashLoadFactor == 1)
      return;
   if (htCurrSizeIdx(hashTable) >= 
      (((HashTable*)hashTable)->numSizes) - 1)
      return;
   if (((double)(htUniqueEntries(hashTable)) / 
      (double)(htCapacity(hashTable))) <=
      ((HashTable*)hashTable)->rehashLoadFactor)
      return;
   rehash(hashTable);
}

unsigned htAdd(void *hashTable, void *data)
{
   HashNode* node;
   unsigned index, freqRet;

   assert(data != NULL);
   ((HashTable*)hashTable)->totalEntries += 1;
   checkRehash(hashTable);

   index = ((*((HashTable*)hashTable)->functions->hash)(data) 
      % htCapacity(hashTable));

   if (((HashTable*)hashTable)->theArray[index] == NULL)
   {
      node = malloc(sizeof(HashNode));
      node->data = data;
      node->freq = 1;
      node->next = NULL;
      freqRet = 1;
      ((HashTable*)hashTable)->uniqueEntries += 1;
      ((HashTable*)hashTable)->theArray[index] = node;
   }
   else
      sepChain(hashTable, data, index, &freqRet);
   return freqRet;
}

void htLookUpCompare(
   void *hashTable, 
   HashNode *curr,
   HTEntry *entry, 
   unsigned *notFound,
   void *data)
{
   if (!((*((HashTable*)hashTable)->functions->compare)(curr->data, data)))
   {
      entry->data = curr->data;
      entry->frequency = curr->freq;
      notFound = 0;
   }
}

HTEntry htLookUp(void *hashTable, void *data)
{
   HTEntry entry;
   unsigned index, notFound;
   HashNode *curr = NULL;

   notFound = 1;
   assert(data != NULL);

   index = ((*((HashTable*)hashTable)->functions->hash)(data) 
      % htCapacity(hashTable));

   curr = ((HashTable*)hashTable)->theArray[index];

   entry.data = NULL;
   entry.frequency = 0;

   while (curr && notFound)
   {
      htLookUpCompare(hashTable, curr, &entry, &notFound, data);
      curr = curr->next;
   }
   return entry;
}

void htToArraySepChain(
   void *hashTable, 
   HTEntry* htArray, 
   int *htArrayIdx, 
   HashNode *curr)
{
   while (curr != NULL)
   {
      htArray[*htArrayIdx].data = curr->data;
      htArray[*htArrayIdx].frequency = curr->freq;
      curr = curr->next;
      *htArrayIdx += 1;
   }
}

void htToArrayCheck(void *hashTable, int i, HTEntry *htArray, int *htArrayIdx)
{
   if (((HashTable*)hashTable)->theArray[i] != NULL)
      htToArraySepChain(hashTable, htArray, htArrayIdx, 
         ((HashTable*)hashTable)->theArray[i]);
}

HTEntry* htToArray(void *hashTable, unsigned *size)
{
   int i, htArrayIdx;
   HTEntry* htArray = NULL;

   htArrayIdx = 0;

   *size = htUniqueEntries(hashTable);
   if (htUniqueEntries(hashTable) == 0)
      return NULL;

   CK_ALLOC(htArray = 
      malloc(*size * sizeof(HTEntry)));
     
   for (i = 0; i < htCapacity(hashTable); i++)
      htToArrayCheck(hashTable, i, htArray, &htArrayIdx);

   return htArray;
}

unsigned htCapacity(void *hashTable)
{
   /* Always incorrect - htCreate does does not allow a size of zero. */
   return ((HashTable*)hashTable)->capacity;
}

unsigned htUniqueEntries(void *hashTable)
{
   /* All values possible but UNIT_MAX is unlikely and recognizable! */
   return ((HashTable*)hashTable)->uniqueEntries;
}

unsigned htTotalEntries(void *hashTable)
{
   /* All values possible but UNIT_MAX is unlikely and recognizable! */
   return ((HashTable*)hashTable)->totalEntries;
}

int htCurrSizeIdx(void *hashTable)
{
   /* All values possible but UNIT_MAX is unlikely and recognizable! */
   return ((HashTable*)hashTable)->currSizeIdx;
}

void htMetricsSepChain(void *hashTable, HashNode *curr, int *chain)
{
   while (curr != NULL)
   {
      curr = curr->next;
      *chain += 1;
   }
}

void htMetricsMaxChain(HTMetrics *metrics, int chain)
{
   if (chain > metrics->maxChainLength)
      metrics->maxChainLength = chain;
}

void htMetricsAvgChain(HTMetrics *metrics, int chain)
{
   metrics->avgChainLength = 
      (metrics->avgChainLength * (metrics->numberOfChains - 1) + chain) 
      / metrics->numberOfChains;
}

void htMetricsCheck(void *hashTable, HTMetrics *metrics, int i)
{
   int chain = 0;
   if (((HashTable*)hashTable)->theArray[i] != NULL)
   {
      metrics->numberOfChains += 1;
      htMetricsSepChain(hashTable,
         ((HashTable*)hashTable)->theArray[i], &chain);
      htMetricsAvgChain(metrics, chain);
      htMetricsMaxChain(metrics, chain);
   }
}

HTMetrics htMetrics(void *hashTable)
{
   int i;
   HTMetrics metrics;
   metrics.numberOfChains = 0;
   metrics.maxChainLength = 0;
   metrics.avgChainLength = 0;

   for (i = 0; i < htCapacity(hashTable); i++)
      htMetricsCheck(hashTable, &metrics, i);
   return metrics;
}
