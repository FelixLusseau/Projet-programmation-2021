/**
* Small librairy to perform unit tests to C programs
* Author: Quentin Bramas <bramas@unistra.fr>
*
* usage:
* - create functions for each feature you
*   want to test, and call them in a main()
*   using the macro TEST(test_function)
* - in a function you can use tps_assert(cond)
*   to check the condition.
* - you can change the value of the global var
*   __remaining_alloc in order to make allocations
*   fail when you want.
*   - if __remaining_alloc is 0, all the remaining allocations
*     will fail (malloc, realloc and calloc).
*   - if __remaining_alloc is < 0 (default),
*     all the remaining allocations will work as expected.
*   - otherwise the next __remaining_alloc will work
*     as expected and then remaining allocations will fail
* - global variable must be declared with the macro TEST_INIT_GLOBAL
* Example:

#include "tps_unit_test.h"

TEST_INIT_GLOBAL

void test1()
{
  __remaining_alloc = 0;
  int * a = malloc(sizeof(int));
  tps_assert(a == NULL);
}
void test2()
{
  __remaining_alloc = 1;
  int * a = malloc(sizeof(int));
  int * b = malloc(sizeof(int));
  tps_assert(a != NULL);
  tps_assert(b == NULL);
}

int main(void)
{
  TEST(test1);
  TEST(test2);
}

*/

#include <stdio.h>
#include <stdlib.h>

#ifndef TPS_UNIT_TEST_H
#define TPS_UNIT_TEST_H

extern int __remaining_alloc;
extern char * __current_test_name;

#define TEST_INIT_GLOBAL \
  int __remaining_alloc = -1; \
  char * __current_test_name;

#define calloc(a,b) \
  (__remaining_alloc \
    ? __remaining_alloc--, calloc(a,b) \
    : NULL)
#define malloc(a) \
  (__remaining_alloc \
    ? __remaining_alloc--, malloc(a) \
    : NULL)
#define realloc(a,b) \
  (__remaining_alloc \
    ? __remaining_alloc--, realloc(a,b) \
    : NULL)


#define tps_test_error(msg)  printf("Error %s [ FAILED ]\n", msg)

#define tps_assert(expr) \
  do { \
    if (!(expr)) { \
      puts(__current_test_name); \
      tps_test_error("\nassertion failed: " #expr); \
      exit(1); \
      return; \
    } \
  } while (0)

#define TEST(func) do \
{ \
  __current_test_name = #func; \
  func(); \
  puts(__current_test_name); \
  puts("Done"); \
} while(0)


#endif
