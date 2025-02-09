#include <benchmark/benchmark.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

int extraStudents;
int classesSize;
int classesColSize = 2;
int** matrix;
std::vector<std::vector<int>> m;

#include "array_updated.h"
#include "best_solution.h"
#include "inplace.h"
#include "linkedlist.h"
#include "passed0.h"
#include "passed1.h"
extern "C" {
#include "solution.h"
}

static void mainSolution( benchmark::State& state ) {
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

static void arrayUpdatedSolution( benchmark::State& state ) {

    for ( auto _ : state ) {
        arrayUpdatedMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    }
}

static void inplaceSolution( benchmark::State& state ) {

    for ( auto _ : state ) {
        inplaceMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    }
}

static void passed1Solution( benchmark::State& state ) {

    for ( auto _ : state ) {
        passed1MaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    }
}

static void passed0Solution( benchmark::State& state ) {

    for ( auto _ : state ) {
        passed0MaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    }
}

static void bestCSolution( benchmark::State& state ) {
    for ( auto _ : state ) {
        bestMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    }
}

BENCHMARK( bestCSolution );
BENCHMARK( passed0Solution );
BENCHMARK( passed1Solution );
BENCHMARK( mainSolution );
// BENCHMARK( listSolution );
// BENCHMARK( inplaceSolution );
// BENCHMARK( arrayUpdatedSolution );

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