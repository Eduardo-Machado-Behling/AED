#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

void Test1(void) {
  const char* str = "LeetcodeHelpsMeLearn";
  int a[] = {8,13,15};
  int sa = sizeof(a) / sizeof(int);

  const char* expected = "Leetcode Helps Me Learn";
  const char* res = addSpaces(str, a, sa);

  TEST_ASSERT_EQUAL_STRING(expected, res);

  free(res);
}

void Test2(void) {
  const char* str = "icodeinpython";
  int a[] = {1,5,7,9};
  int sa = sizeof(a) / sizeof(int);

  const char* expected = "i code in py thon";
  const char* res = addSpaces(str, a, sa);

  TEST_ASSERT_EQUAL_STRING(expected, res);

  free(res);
}

void Test3(void) {
  const char* str = "spacing";
  int a[] = {0,1,2,3,4,5,6};
  int sa = sizeof(a) / sizeof(int);

  const char* expected = " s p a c i n g";
  const char* res = addSpaces(str, a, sa);

  TEST_ASSERT_EQUAL_STRING(expected, res);

  free(res);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(Test1);
  RUN_TEST(Test2);
  RUN_TEST(Test3);

  return UNITY_END();
}
