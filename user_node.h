#ifndef USER_NODE_H
#define USER_NODE_H

#include <pthread.h>
#include <stdbool.h>

struct User_node
{
    int connection;
    bool connection_status;
    pthread_mutex_t mutex;
    char* nick;
    struct User_node* next;  
};

struct User_node* node_init( int const, char const* );
void node_free( struct User_node* );
void node_send_data( struct User_node* const, char const*, int const );


#endif
