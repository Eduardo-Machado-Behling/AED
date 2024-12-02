#include "unity.h"

#include "solution.h"
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

void Test1(void) {
  const char *str = "i love eating burger";
  const char *search = "burg";

  const int expected = 4;
  const int res = isPrefixOfWord(str, search);

  TEST_ASSERT_EQUAL_INT(expected, res);
}

void Test2(void) {
  const char *str = "this problem is an easy problem";
  const char *search = "pro";

  const int expected = 2;
  const int res = isPrefixOfWord(str, search);

  TEST_ASSERT_EQUAL_INT(expected, res);
}

void Test3(void) {
  const char *str = "i am tired";
  const char *search = "you";

  const int expected = -1;
  const int res = isPrefixOfWord(str, search);

  TEST_ASSERT_EQUAL_INT(expected, res);
}

void Test4(void) {
  const char *str = "hello from the other side";
  const char *search = "they";

  const int expected = -1;
  const int res = isPrefixOfWord(str, search);

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
