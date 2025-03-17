#include "unity.h"

#include "solution.h"

void setUp( void ) {
}

void tearDown( void ) {
}

void Test1( void ) {
    const int difficulty[]   = { 2, 4, 6, 8, 10 };
    const int difficultySize = sizeof( difficulty ) / sizeof( *difficulty );

    const int profit[]   = { 10, 20, 30, 40, 50 };
    const int profitSize = sizeof( profit ) / sizeof( *profit );

    const int worker[]   = { 4, 5, 6, 7 };
    const int workerSize = sizeof( worker ) / sizeof( *worker );

    const int expected = 100;
    const int res      = maxProfitAssignment( difficulty, difficultySize, profit, profitSize, worker, workerSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test2( void ) {
    const int difficulty[]   = { 85, 47, 57 };
    const int difficultySize = sizeof( difficulty ) / sizeof( *difficulty );

    const int profit[]   = { 24, 66, 99 };
    const int profitSize = sizeof( profit ) / sizeof( *profit );

    const int worker[]   = { 40, 25, 25 };
    const int workerSize = sizeof( worker ) / sizeof( *worker );

    const int expected = 0;
    const int res      = maxProfitAssignment( difficulty, difficultySize, profit, profitSize, worker, workerSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

void Test3( void ) {
    const int difficulty[]   = { 66, 1, 28, 73, 53, 35, 45, 60, 100, 44, 59, 94, 27, 88, 7, 18, 83, 18, 72, 63 };
    const int difficultySize = sizeof( difficulty ) / sizeof( *difficulty );

    const int profit[]   = { 66, 20, 84, 81, 56, 40, 37, 82, 53, 45, 43, 96, 67, 27, 12, 54, 98, 19, 47, 77 };
    const int profitSize = sizeof( profit ) / sizeof( *profit );

    const int worker[]   = { 61, 33, 68, 38, 63, 45, 1, 10, 53, 23, 66, 70, 14, 51, 94, 18, 28, 78, 100, 16 };
    const int workerSize = sizeof( worker ) / sizeof( *worker );

    const int expected = 1392;
    const int res      = maxProfitAssignment( difficulty, difficultySize, profit, profitSize, worker, workerSize );

    TEST_ASSERT_EQUAL_INT( expected, res );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );
    RUN_TEST( Test3 );

    return UNITY_END();
}
