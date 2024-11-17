#include "unity.h"

#define ARRAY_EXPOSE_CLASS
#define NAME_MAX_LEN 64
#include "array.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

StringArray_t* sarr;

void setUp( void ) {
    sarr = create_string_array( 20 );
}

void tearDown( void ) {
    destroy_string_array( &sarr );
}

struct CompareArgs_t {
    bool result;
    const char** expected;
    size_t i;
};

void compare( void* args, const char* name ) {
    struct CompareArgs_t* arg = args;

    if ( !arg->result )
        return;

    if ( strncmp( arg->expected[arg->i++], name, NAME_MAX_LEN ) != 0 )
        arg->result = false;
}

void test_proper_usage( void ) {
    string_array_add( sarr, "Eduardo", NAME_MAX_LEN );
    string_array_add( sarr, "Erick", NAME_MAX_LEN );
    string_array_add( sarr, "Leonardo", NAME_MAX_LEN );

    const char* expected = "EduardoErickLeonardo";
    TEST_ASSERT_EQUAL_STRING_MESSAGE( sarr->array, expected, "array->buffer isn't properly formatted" );

    string_array_rmv( sarr, "Erick" );

    expected = "EduardoLeonardo";
    TEST_ASSERT_EQUAL_STRING_MESSAGE( sarr->array, expected, "array->buffer isn't properly formatted" );

    string_array_add( sarr, "Roberto", NAME_MAX_LEN );
    string_array_add( sarr, "Roger", NAME_MAX_LEN );

    expected = "EduardoLeonardoRobertoRoger";
    TEST_ASSERT_EQUAL_STRING_MESSAGE( sarr->array, expected, "array->buffer isn't properly formatted" );

    const char* ex[] = { "Eduardo",
                         "Leonardo",
                         "Roberto",
                         "Roger" };

    struct CompareArgs_t args = { .i = 0, .result = true, .expected = ex };
    string_array_for_each( sarr, compare, &args );
    TEST_ASSERT_TRUE_MESSAGE( args.result, "For each didn't work" );
}

void test_tricky_usage( void ) {
    string_array_add( sarr, "Edu;;;ç~]]]ardo", NAME_MAX_LEN );
    string_array_add( sarr, "eR;I___CK", NAME_MAX_LEN );
    string_array_add( sarr, "Leo__NARD~~o", NAME_MAX_LEN );

    const char* expected = "EduardoErickLeonardo";
    TEST_ASSERT_EQUAL_STRING_MESSAGE( sarr->array, expected, "array->buffer isn't properly formatted" );

    string_array_rmv( sarr, "Erick" );
    string_array_rmv( sarr, "Edu" );
    string_array_rmv( sarr, "Leo;;]´[ç[]]" );

    expected = "EduardoLeonardo";
    TEST_ASSERT_EQUAL_STRING_MESSAGE( sarr->array, expected, "array->buffer isn't properly formatted" );

    string_array_add( sarr, "Roger", NAME_MAX_LEN );
    string_array_add( sarr, "Roberto", NAME_MAX_LEN );
    string_array_add( sarr, "Roger", NAME_MAX_LEN );
    string_array_rmv( sarr, "Roger" );

    expected = "EduardoLeonardoRobertoRoger";
    TEST_ASSERT_EQUAL_STRING_MESSAGE( sarr->array, expected, "array->buffer isn't properly formatted" );

    const char* ex[] = { "Eduardo",
                         "Leonardo",
                         "Roberto",
                         "Roger" };

    struct CompareArgs_t args = { .i = 0, .result = true, .expected = ex };
    string_array_for_each( sarr, compare, &args );
    TEST_ASSERT_TRUE_MESSAGE( args.result, "For each didn't work" );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( test_proper_usage );
    RUN_TEST( test_tricky_usage );

    return UNITY_END();
}
