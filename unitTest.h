/* Provided stubbed test macros for the Unit Test Macros exercise. Your job
 * is to complete them AND develop unit tests for them in the provided
 * partially complete unit test driver (testUnitTestMacros.c).
 */

#ifndef UNITTEST_H
#define UNITTEST_H

#define MAXSTRING_LEN 6

#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * The TEST_SIGNED is given to you in the assigned reading so you should find
 * it and copy it here. Be sure to test your copy to make sure it works as
 * expected!
 *
 * AND, remember that the provided tests are not sufficient - you'll need to
 * write more!
 */
#define TEST_SIGNED(_ACTUAL, _EXPECT)\
{\
   long _actual = _ACTUAL, _expect = _EXPECT;\
   if (_actual != _expect) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value %ld, expected %ld\n",\
         #_ACTUAL, _actual, _expect);\
   }\
}

/*
 * Stubbed macros you must complete.
 */
#define TEST_UNSIGNED(_ACTUAL, _EXPECT)\
{\
   unsigned long _actual = _ACTUAL, _expect = _EXPECT;\
   if (_actual != _expect) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value %lu, expected %lu\n",\
         #_ACTUAL, _actual, _expect);\
   }\
}

#define TEST_REAL(_ACTUAL, _EXPECT, _EPSILON)\
{\
   double _actual = _ACTUAL, _expect = _EXPECT, _epsilon = fabs(_EPSILON);\
   if ((_actual < (_expect - _epsilon)) || (_actual > (_expect + _epsilon))) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr,"   Found substitution %s, value %g, expected %g +/-%g\n"\
         ,#_ACTUAL, _actual, _expect, _epsilon);\
   }\
}

#define TEST_BOOLEAN(_ACTUAL, _EXPECT)\
{\
   double _actual = _ACTUAL, _expect = _EXPECT;\
   if ((_actual == 0) && (_expect != 0))\
   {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr,"   Found substitution %s, value %s, expected %s\n"\
         ,#_ACTUAL, "false", "true");\
   }\
   if ((_actual != 0) && (_expect == 0)) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr,"   Found substitution %s, value %s, expected %s\n"\
         ,#_ACTUAL, "true", "false");\
   }\
}

#define TEST_CHAR(_ACTUAL, _EXPECT)\
{\
   char _actual = _ACTUAL, _expect = _EXPECT;\
   if (_actual != _expect) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value '%c', expected '%c'\n",\
         #_ACTUAL, _actual, _expect);\
   }\
}

#define TEST_STRING(_ACTUAL, _EXPECT)\
{\
   if (strcmp(_ACTUAL, _EXPECT) != 0) {\
      fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
      fprintf(stderr, "   Found substitution %s, value \"%s\""\
         ", expected \"%s\"\n", #_ACTUAL, _ACTUAL, _EXPECT);\
   }\
}

#define TEST_ERROR(_FUNCTION_CALL)\
{\
   _FUNCTION_CALL;\
   fprintf(stderr, "Failed test in %s at line %d:\n", __FILE__, __LINE__);\
   fprintf(stderr, "   Expected error detection did not occur\n");\
}

#endif
