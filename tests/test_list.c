#include <malloc.h>
#include <stdio.h>
#include <pthread.h>

#include "user_list.h"

struct TestA
{
    int a;
    int b;
};

struct Test 
{
    struct TestA* test;
    pthread_mutex_t mutex;
};

struct Test* test_init()
{
    struct Test* test = malloc( sizeof( struct Test ) );
    test->test = NULL;
    pthread_mutex_init( &test->mutex, NULL );
    
    return test;
}

void test_free( struct Test* test )
{
    pthread_mutex_destroy( &test->mutex );
    free( test );
}

int main( int argc, char** argv )
{
    struct User_node* unode1 = node_init( 0, "ala" );
    struct User_node* unode2 = node_init( 1, "ma" );
    struct User_node* unode3 = node_init( 2, "kota" );
    
    struct User_list* list = list_init();
    
    list_append_node( list, unode1 );
    list_append_node( list, unode2 );
    list_append_node( list, unode3 );
    
    list_free( list );

    return 0;
}

