/*
 * A general purpose C unit testing framework developed for CPE 357 and
 * provided to students to expose them to and encourage their use of unit
 * testing and test driven development (TDD). While it was provided in
 * conjunction with the hash table project and associated exercise(s), students
 * are welcome and encourage to use and/or modify it, as they see fit, on
 * subsequent assignments in CPE 357 or anywhere else it they find it useful.
 *
 * Author: Kurt Mammen
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include "unitTest.h"
#include "hashTable.h"

#define TEST_ALL -1
#define REGULAR -2 
#define SPECIAL -3

#define MAX_RANDOM_STR 72 /* Max length of any random string */

/* Prototype for all test functions. This allows the creation of an array of
 * function pointers which makes the testing code shorter and more clear. It
 * also makes it easier/faster to add new tests - NICE!
 */
typedef void (*TestFunc)();

/* Structure for a test. See initRegularTests and initSpecialTests to see
 * how it is used.
 */
typedef struct
{
   TestFunc fn;
   char *fnName;
} Test;

/* Modified hash algorithm from K&R (page 144). This version does NOT mod the
 * calculated hash value by the table size because it does not know the table
 * size - the caller (the hash table itself) does though!
 *
 * Also note the following:
 *   * This hash only works for C strings.
 *   * Not the best/fastest/most flexible hash - you'll want something better
 *     for the Word Frequency project.
 */
static unsigned hashString(const void *data)
{
   unsigned hash;
   const char *str = data;

   for (hash = 0; *str;  str++)
      hash = *str + 31 * hash;

   /* Return the raw hash value - the modulo must be done by caller */
   return hash;
}

static unsigned newhashString(const void *data)
{
   unsigned hash = 5381;
   const char *str = data;
   int c;

   while ((c = *str++))
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

   return hash;
}

static unsigned stupidHashString(const void *data)
{
   unsigned hash = 5;
   return hash;
}

static int compareString(const void *a, const void *b)
{
   return strcmp(a, b);
}

/* Helper function to make random strings for robust testing. Strings will be
 * a minimum of 3 characters up to MAX_RANDOM_STR characters in length.
 */
static char* randomString()
{
   char *str;
   int i;
   int length = (rand() % MAX_RANDOM_STR - 2) + 3;

   if (NULL == (str = malloc(length + 1)))
   {
      perror("randomString()");
      exit(EXIT_FAILURE);
   }

   /* Put random but visible char values in the string */
   for (i = 0; i < length; i++)
      str[i] = (rand() % (' ' - '~' + 1)) + ' ';

   str[length] = 0; /* nul-terminate it! */

   return str; 
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test01. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */
static void core01()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, -987, 0.73));
}

static void core02()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 0, 0.73));
}

static void core03()
{
   unsigned sizes[] = {0};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 1, 0.73));
}

static void core04()
{
   unsigned sizes[] = {12, 13, 13, 13};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 4, 0.73));
}

static void core05()
{
   unsigned sizes[] = {1, 2, 3, 2, 1};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 5, 0.73));
}

static void core06()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 1, -1));
}

static void core07()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 1, 0));
}

static void core08()
{
   unsigned sizes[] = {7};
   HTFunctions funcs = {hashString, compareString, NULL};

   TEST_ERROR(htCreate(&funcs, sizes, 1, 1.1));
}

static void core09()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   TEST_ERROR(htAdd(ht, NULL));
}

static void core10()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   TEST_ERROR(htLookUp(ht, NULL));
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test11. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */ 
static void core11()
{
   unsigned sizes[] = {31};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 1, 0.73);

   /* Do the test(s) */
   TEST_UNSIGNED(htCapacity(ht), 31);
   TEST_UNSIGNED(htUniqueEntries(ht), 0);
   TEST_UNSIGNED(htTotalEntries(ht), 0);

   htDestroy(ht);
}

/* PROVIDED TEST (do not modify)
 *
 * This is a provided sample test that matches the Evaluation System's English
 * test description for the Hash Table project's core test12. I hope it is
 * obvious to you that this is but one of many many variations that one could
 * write that would satisfy the English test description. Recall that one of
 * the important goals of the Evaluation System is to provide you with guidance
 * on how to test you solution but still allow you the opportunity develop your
 * own tests in the hopes that doing so will create an efficient and fruitful
 * learning experience!
 */ 
static void core12()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   /* Single entry */
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 1);
   
   htDestroy(ht);
}

static void core13()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   /* Single entry */
   freq = htAdd(ht, randomString());
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);
   
   htDestroy(ht);
}

static void core14()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   char *input1 = randomString();
   char *input2 = malloc(sizeof(char) * strlen(input1) + 1);
   strcpy(input2, input1);

   /* Adds two entries that are the same */
   freq = htAdd(ht, input1);
   freq = htAdd(ht, input2);

   TEST_UNSIGNED(freq, 2);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 1);
   TEST_UNSIGNED(htTotalEntries(ht), 2);

   free(input2);
   
   htDestroy(ht);
}

static void core15()
{
   int freq = 0;
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {stupidHashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);

   /* Adds two entries that are NOT the same */
   freq = htAdd(ht, randomString());
   freq = htAdd(ht, randomString());

   TEST_UNSIGNED(freq, 1);
   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 2);
   
   htDestroy(ht);
}

static void core16()
{
   char search[] = "hello";
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTEntry entry;
   entry = htLookUp(ht, search);

   TEST_BOOLEAN(entry.data == NULL, 1);
   TEST_UNSIGNED(entry.frequency, 0);

   htDestroy(ht);
}

static void core17()
{
   char search[] = "hello";
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTEntry entry;

   /* Adds two entries that are NOT the same */
   htAdd(ht, randomString());
   htAdd(ht, randomString());

   entry = htLookUp(ht, search);

   TEST_BOOLEAN(entry.data == NULL, 1);
   TEST_UNSIGNED(entry.frequency, 0);

   htDestroy(ht);
}

static void core18()
{
   unsigned sizes[] = {11, 23};
   HTFunctions funcs = {hashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.73);
   HTEntry entry1, entry2;

   char *input1 = randomString();
   char *input2 = randomString();

   char *deep1 = malloc(sizeof(char) * strlen(input1) + 1);
   char *deep2 = malloc(sizeof(char) * strlen(input2) + 1);

   strcpy(deep1, input1);
   strcpy(deep2, input2);

   htAdd(ht, input1);
   htAdd(ht, input2);

   entry1 = htLookUp(ht, deep1);
   TEST_BOOLEAN(entry1.data == NULL, 0);
   TEST_STRING((char *)entry1.data, input1);
   TEST_UNSIGNED(entry1.frequency, 1);

   entry2 = htLookUp(ht, deep2);
   TEST_BOOLEAN(entry2.data == NULL, 0);
   TEST_STRING((char *)entry2.data, input2);
   TEST_UNSIGNED(entry2.frequency, 1);

   free(deep1);
   free(deep2);

   htDestroy(ht);
}

static void feature02()
{
   HTEntry ret1, ret2, ret3, ret4, ret5, ret6, ret7, ret8, ret9;
   char *str1 = randomString();
   char *str2 = malloc((sizeof(char) * strlen(str1)) + sizeof(char));
   char *str3 = randomString();
   char *str4 = malloc((sizeof(char) * strlen(str1)) + sizeof(char));
   char *str5 = randomString();
   char *str6 = randomString();
   char *str7 = randomString();
   char *str8 = randomString();
   char *str9 = malloc((sizeof(char) * strlen(str1)) + sizeof(char));

   unsigned sizes[] = {5, 11};
   HTFunctions funcs = {newhashString, compareString, NULL};
   void *ht = htCreate(&funcs, sizes, 2, 0.75);

   TEST_UNSIGNED(htCapacity(ht), 5);

   htAdd(ht, str1);
   strcpy(str2, str1);
   htAdd(ht, str2);
   htAdd(ht, str5);

   TEST_UNSIGNED(htUniqueEntries(ht), 2);
   TEST_UNSIGNED(htTotalEntries(ht), 3);
   TEST_UNSIGNED(htCapacity(ht), 5);

   htAdd(ht, str3);

   TEST_UNSIGNED(htUniqueEntries(ht), 3); 
   TEST_UNSIGNED(htTotalEntries(ht), 4);

   strcpy(str4, str3);
   htAdd(ht, str4);
   htAdd(ht, str6);
   htAdd(ht, str7);
   htAdd(ht, str8);
   strcpy(str9, str3);
   htAdd(ht, str9);

   TEST_UNSIGNED(htUniqueEntries(ht), 6);
   TEST_UNSIGNED(htCapacity(ht), 11);

   TEST_UNSIGNED(htTotalEntries(ht), 9);

   ret1 = htLookUp(ht, str1);
   ret2 = htLookUp(ht, str2);
   ret3 = htLookUp(ht, str3);
   ret4 = htLookUp(ht, str4);
   ret5 = htLookUp(ht, str5);
   ret6 = htLookUp(ht, str6);
   ret7= htLookUp(ht, str7);
   ret8 = htLookUp(ht, str8);
   ret9 = htLookUp(ht, str9);

   TEST_UNSIGNED(htCapacity(ht), 11);
   TEST_UNSIGNED(htUniqueEntries(ht), 6);
   TEST_UNSIGNED(htTotalEntries(ht), 9);

   TEST_BOOLEAN((ret1.data) == str1, 1);
   TEST_BOOLEAN((ret2.data) == str1, 1);
   TEST_BOOLEAN((ret3.data) == str3, 1);
   TEST_BOOLEAN((ret4.data) == str3, 1);
   TEST_BOOLEAN((ret5.data) == str5, 1);
   TEST_BOOLEAN((ret6.data) == str6, 1);
   TEST_BOOLEAN((ret7.data) == str7, 1);
   TEST_BOOLEAN((ret8.data) == str8, 1);
   TEST_BOOLEAN((ret9.data) == str3, 1);

   TEST_BOOLEAN((ret1.frequency) == 2, 1);
   TEST_BOOLEAN((ret2.frequency) == 2, 1);
   TEST_BOOLEAN((ret3.frequency) == 3, 1);
   TEST_BOOLEAN((ret4.frequency) == 3, 1); 
   TEST_BOOLEAN((ret5.frequency) == 1, 1); 
   TEST_BOOLEAN((ret6.frequency) == 1, 1); 
   TEST_BOOLEAN((ret7.frequency) == 1, 1); 
   TEST_BOOLEAN((ret8.frequency) == 1, 1); 
   TEST_BOOLEAN((ret9.frequency) == 3, 1); 
 
   TEST_BOOLEAN(ret1.data != NULL, 1);
   TEST_BOOLEAN(ret2.data != NULL, 1);
   TEST_BOOLEAN(ret3.data != NULL, 1);
   TEST_BOOLEAN(ret4.data != NULL, 1);
   TEST_BOOLEAN(ret5.data != NULL, 1);
   TEST_BOOLEAN(ret6.data != NULL, 1); 
   TEST_BOOLEAN(ret7.data != NULL, 1); 
   TEST_BOOLEAN(ret8.data != NULL, 1); 
   TEST_BOOLEAN(ret9.data != NULL, 1);

   printf("MaxChainLen: %d\n", htMetrics(ht).maxChainLength);
   printf("AvgChainLen: %f\n", htMetrics(ht).avgChainLength);
   printf("numChains: %u\n", htMetrics(ht).numberOfChains);

   free(str9);
   free(str2);
   free(str4);
 
   htDestroy(ht);
}

static void testAll(Test* tests)
{
   int i;

   for (i = 0; tests[i].fn != NULL; i++)
   {
      printf("Running regular %s\n", tests[i].fnName);

      /* Call the test function via function pointer in the array */
      tests[i].fn();
   }
}

static void findAndCall(Test* tests, const char *type, char *fnName)
{
   int i;

   for (i = 0; tests[i].fn != NULL; i++)
   {
      if (0 == strcmp(tests[i].fnName, fnName))
      {
         /* Found it, call the function via function pointer... */
         printf("Running %s %s\n", type, fnName);
         tests[i].fn();
         return;
      }
   }

   fprintf(stderr, "ERROR %s %s: Function not found\n", type, fnName);
   exit(EXIT_FAILURE);
}

static void runTests(Test *tests, const char *type, char *fnName)
{
   if (fnName == NULL)
      testAll(tests);
   else
      findAndCall(tests, type, fnName);

   /* Free the tests (allocated in initTests) */
   free(tests);
}

static char* checkArgs(int argc, char *argv[], int *testType)
{
   char *testName;

   if (argc == 1)
   {
      *testType = REGULAR;      
      testName = NULL;
   }
   else if (argc == 2)
   {
      *testType = REGULAR; 
      testName = argv[1];
   }
   else if (argc == 3)
   {
      if (0 != strcmp(argv[1], "-special"))
      {
         fprintf(stderr, "Invalid option '%s'\n", argv[1]);
         exit(EXIT_FAILURE);
      }
      
      *testType = SPECIAL;
      testName = argv[2];
   }
   else
   {
      fprintf(stderr, "Usage: %s [testName | -special testName]\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   return testName;
}

Test* initTests(Test tests[], int size)
{
   Test *dynamicMemory = malloc(size);

   if (dynamicMemory == NULL)
   {
      fprintf(stderr, "FAILURE in %s at %d: ", __FILE__, __LINE__);
      perror(NULL);
      exit(EXIT_FAILURE);
   }

   return memcpy(dynamicMemory, tests, size);
}

/* Allocates, initializes, and returns the array of regular test functions.
 * Regular test functions are those that are expected to pass or report failure
 * BUT NOT terminate the test driver.
 *
 * By default, the test driver runs ALL of the regular tests. Alternatively, you
 * can run one at a time by simply specifying its name when you invoke the
 * test driver.
 *
 * See initSpecialTests for tests that you always want to run individually.
 *
 * NOTE: The last structure in the array must have NULL values as this indicates
 *    the array's end.
 *
 * IMPORTANT SUBTLETY: You can only use sizeof to obtain an array's size in
 *    the scope where the array is declared, otherwise you will just get
 *    the size of the pointer to the array.
 */
Test* initRegularTests()
{
   Test local[] = {
      {core11, "core11"},
      {core12, "core12"},
      {core13, "core13"},
      {core14, "core14"},
      {core15, "core15"},
      {core16, "core16"},
      {core17, "core17"},
      {core18, "core18"},
      {feature02, "feature02"},
      {NULL, NULL}
   };

   /* See IMPORTANT SUBTLETY above regarding the use of sizeof on arrays */
   return initTests(local, sizeof(local));
}

/* Allocates, initializes, and returns the array of special test functions.
 * Special test functions are those that you want to run individually for one
 * reason or another. For example, a test to see if a function asserts failure
 * when it is supposed to. Or, a test intended to measure CPU or memory
 * performance using tools like the Unix time command or Valgrind - for
 * tests like these you only want to run the single test and then observe the
 * results.
 *
 * See initRegularTests for tests that can together.
 *
 * NOTE: The last structure in the array must have NULL values as this indicates
 *    the array's end.
 *
 * IMPORTANT SUBTLETY: You can only use sizeof to obtain an array's size in
 *    the scope where the array is declared, otherwise you will just get
 *    the size of the pointer to the array.
 */
Test* initSpecialTests()
{
   Test local[] = {
      {core01, "core01"},
      {core02, "core02"},
      {core03, "core03"},
      {core04, "core04"},
      {core05, "core05"},
      {core06, "core06"},
      {core07, "core07"},
      {core08, "core08"},
      {core09, "core09"},
      {core10, "core10"},
      {NULL, NULL}
   };

   /* See IMPORTANT SUBTLETY above regarding the use of sizeof on arrays */
   return initTests(local, sizeof(local)); 
}

/* Test driver for Hash Table Project.
 *
 * Usage: testHashTable [functionName | -special functionName]
 *
 * When no option is specified ALL of the regular tests are run.
 * When "functionName" is specified that single regular test is run.
 * When "-special functionName" is specified that single special test is run.
 *
 * What TODO: As provided with the Unit Test Hash Table exercise, this unit test
 * driver provides several working examples of actual Evaluation System tests
 * for the Hash Table project. The exercise asks you to develop a few more so
 * that you can experience how easy it is to develop unit tests BEFORE you even
 * implement any actual code for the solution being tested. The hope is that
 * this will encourage you to develop tests early and often as you develop your
 * Hash Table project solution thereby adding unit testing and Test Driven
 * Development (TDD) to your personal repertoire of programming skills! 
 *    
 * To add a new test you must:
 * 
 *    1) Write a test function.
 *    2) Add its name to the appropriate array of test functions, see
 *       initRegularTests and initSpecialTests for details.
 *    3) Compile and test your test by verifying:
 *          1. It report failure when run on code known to be incorrect.
 *          2. It passes (does NOT report failure) when run on code believed to
 *             be correct.
 *    4) Be sure use Valgrind, where appropriate, to check for memory errors
 *       and/or leaks too!
 */
int main(int argc, char *argv[])
{
   char *testName;
   int testType;

   /* Random numbers used to produce "interesting" strings for testing */
   srand(182955);

   /* Make stdout unbuffered so that test output is synchronous on signals */
   setbuf(stdout, NULL);

   /* Get the test name type */
   testName = checkArgs(argc, argv, &testType);
 
   /* Run the test(s)... */
   if (testType == REGULAR)
      runTests(initRegularTests(), "regular", testName);
   else
      runTests(initSpecialTests(), "special", testName);
   
   return 0;
}
