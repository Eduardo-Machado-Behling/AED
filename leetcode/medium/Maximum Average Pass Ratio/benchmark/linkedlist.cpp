#include <climits>
#include <cstdint>
#include <cstdlib>

struct entry {
    int students;
    int passed;
    double change;
};

struct node {
    struct node* next;
    struct entry val;
};

static void appendMin( struct node* head,
                       struct entry entry ) {
    struct node* prev = head;
    struct node* it   = head->next;

    while ( it && it->val.change > entry.change ) {
        prev = it;
        it   = it->next;
    }

    prev->next       = (struct node*) malloc( sizeof( struct node ) );
    prev->next->val  = entry;
    prev->next->next = it;
}

static double getNextChange( int passed, int students ) {
    return ( (double) ( passed + 1 ) / ( students + 1 ) );
}

static double getChange( int passed, int students ) {
    return ( (double) ( passed + 1 ) / ( students + 1 ) ) - (double) ( passed ) / ( students );
}

static struct node* popHead( struct node* head ) {
    struct node* ret = head->next;
    head->next       = ret->next;
    return ret;
}

double linkedMaxAverageRatio( int** classes, int classesSize, int* classesColSize,
                              int extraStudents ) {
    struct node head = { 0 };

    int used   = 0;
    double res = 0;

    for ( int i = 0; i < classesSize; i++ ) {
        double change = (double) ( classes[i][0] ) / ( classes[i][1] );
        res += change;
        appendMin( &head,
                   ( struct entry ){ .students = classes[i][1],
                                     .passed   = classes[i][0],
                                     .change   = getNextChange( classes[i][0], classes[i][1] ) - change } );
    }

    for ( int i = 0; i < extraStudents; i++ ) {
        struct node* max =
            popHead( &head );
        res += max->val.change;
        max->val.change = getChange( ++max->val.passed, ++max->val.students );
        appendMin( &head, max->val );
        free( max );
    }

    return res / classesSize;
}
