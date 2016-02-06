#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "user_node.h"

struct User_node* node_init( int const connection, char const* nick )
{
    struct User_node* unode = malloc( sizeof( struct User_node ) );
    unode->connection = connection;
    unode->connection_status = true;
    unode->nick = strdup( nick );
    pthread_mutex_init( &unode->mutex, NULL );
    unode->next = NULL;
    
    return unode;
}

void node_free( struct User_node* unode )
{
    free( unode->nick );
    pthread_mutex_destroy( &unode->mutex );
    free( unode );
    unode = NULL;
}

void node_send_data( struct User_node* const unode, char const* message, int const message_len )
{
    pthread_mutex_lock( &unode->mutex );
    if( write( unode->connection, message, message_len ) == -1 )
    {
        perror( "node_send_data" );
        unode->connection_status = false;
    }
    pthread_mutex_unlock( &unode->mutex );
}
