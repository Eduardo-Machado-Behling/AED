#include <benchmark/benchmark.h>
#include <cstdlib>
#include <cstring>

int extraStudents;
int classesSize;
int classesColSize = 2;
int** matrix;

#include "linkedlist.h"
extern "C" {
#include "solution.h"
}

static void arraySolution( benchmark::State& state ) {
    // Code inside this loop is measured repeatedly
    for ( auto _ : state ) {
        maxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    }
}
// Register the function as a benchmark

static void listSolution( benchmark::State& state ) {

    for ( auto _ : state ) {
        linkedMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    }
}
BENCHMARK( listSolution );

BENCHMARK( arraySolution );

void parseInput() {
    FILE* f = fopen( "input.txt", "r" );
    char buff[1024];
    int i = 0;

    enum { NAME,
           EXTRAS,
           SIZE,
           VALS } state = NAME;

    while ( fgets( buff, sizeof( buff ), f ) ) {
        *strchr( buff, '\n' ) = '\0';

        switch ( state ) {
            case NAME:
                state = EXTRAS;
                break;

            case EXTRAS:
                extraStudents = atoll( buff );
                state         = SIZE;
                break;

            case SIZE:
                classesSize = atoll( buff );
                matrix      = (int**) malloc( sizeof( int* ) * classesSize );
                state       = VALS;
                break;

            case VALS:
                matrix[i] = (int*) malloc( sizeof( int ) * 2 );
                strtok( buff, "," );
                matrix[i][0] = atol( buff );
                strtok( NULL, "," );
                matrix[i++][1] = atol( buff );

            default:
                break;
        }
    }
}

int main( int argc, char** argv ) {
    benchmark::Initialize( &argc, argv );
    parseInput();
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}