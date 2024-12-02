#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

void Test1(void) {
  int a[] = {2, 3, 1, 1, 4};
  int sa = sizeof(a) / sizeof(int);

  const bool expected = true;
  const bool res = canJump(a, sa);

  TEST_ASSERT_EQUAL_INT(expected, res);
}

void Test2(void) {
  int a[] = {3, 2, 1, 0, 4};
  int sa = sizeof(a) / sizeof(int);

  const bool expected = false;
  const bool res = canJump(a, sa);

  TEST_ASSERT_EQUAL_INT(expected, res);
}

void Test3(void) {
  int a[] = {2, 0, 0};
  int sa = sizeof(a) / sizeof(int);

  const bool expected = true;
  const bool res = canJump(a, sa);

  TEST_ASSERT_EQUAL_INT(expected, res);
}

void Test4(void) {
  int a[] = {0};
  int sa = sizeof(a) / sizeof(int);

  const bool expected = true;
  const bool res = canJump(a, sa);

  TEST_ASSERT_EQUAL_INT(expected, res);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(Test1);
  RUN_TEST(Test2);
  RUN_TEST(Test3);
  RUN_TEST(Test4);

  return UNITY_END();
}
