#include <benchmark/benchmark.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>

int extraStudents;
int classesSize;
int classesColSize = 2;
int** matrix;
double expected;

#include "array_updated.h"
#include "best_solution.h"
#include "heap_recursive.h"
#include "inplace.h"
#include "linkedlist.h"
#include "passed0.h"
#include "passed1.h"
extern "C" {
#include "solution.h"
}

const double EPSILON = 1e-6; // Adjust based on required precision

bool almostEquals( double a, double b ) {
    return std::abs( a - b ) < EPSILON;
}

static void mainSolution( benchmark::State& state ) {
    for ( auto _ : state ) {
        // Code inside this loop is measured repeatedly
        double res = maxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "maxAverageRatio produced wrong output" );
        }
    }
}
// Register the function as a benchmark

static void listSolution( benchmark::State& state ) {

    for ( auto _ : state ) {
        double res = linkedMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "linkedMaxAverageRatio produced wrong output" );
        }
    }
}

static void arrayUpdatedSolution( benchmark::State& state ) {

    for ( auto _ : state ) {
        double res = arrayUpdatedMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "arrayUpdatedMaxAverageRatio produced wrong output" );
        }
    }
}

static void inplaceSolution( benchmark::State& state ) {

    for ( auto _ : state ) {
        double res = inplaceMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "inplaceMaxAverageRatio produced wrong output" );
        }
    }
}

static void passed1Solution( benchmark::State& state ) {

    for ( auto _ : state ) {
        double res = passed1MaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "passed1MaxAverageRatio produced wrong output" );
        }
    }
}

static void passed0Solution( benchmark::State& state ) {

    for ( auto _ : state ) {
        double res = passed0MaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "passed0MaxAverageRatio produced wrong output" );
        }
    }
}

static void bestCSolution( benchmark::State& state ) {
    for ( auto _ : state ) {
        double res = bestMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "bestMaxAverageRatio produced wrong output" );
        }
    }
}

static void heapRSolution( benchmark::State& state ) {
    for ( auto _ : state ) {
        double res = heapRMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
        if ( !almostEquals( res, expected ) ) {
            state.SkipWithError( "heapRMaxAverageRatio produced wrong output" );
        }
    }
}

BENCHMARK( bestCSolution );
BENCHMARK( passed0Solution );
BENCHMARK( passed1Solution );
BENCHMARK( mainSolution );
BENCHMARK( listSolution );
BENCHMARK( inplaceSolution );
BENCHMARK( arrayUpdatedSolution );
BENCHMARK( heapRSolution );

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
    expected =
        bestMaxAverageRatio( matrix, classesSize, &classesColSize, extraStudents );
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}