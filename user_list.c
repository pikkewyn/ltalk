#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "user_list.h"

struct User_list* list_init()
{
    struct User_list* list = malloc( sizeof( struct User_list ) );
    list->head = NULL;
    pthread_mutex_init( &( list->mutex ), NULL );

    return list;
}

void list_free( struct User_list* list )
{
    if( list == NULL )
    {
        return;
    }
        
    pthread_mutex_destroy( &list->mutex );
    
    struct User_node* node = list->head;
    struct User_node* tmp = NULL;
    
    while( node != NULL )
    {
        tmp = node;
        node = node->next;
        node_free( tmp );
    }
    
    free( list );
    list = NULL;
}

void list_append_node( struct User_list* list, struct User_node* unode )
{
    assert( list );
    assert( unode );
    
    unode->next = list->head;
    list->head = unode;
}

bool list_user_exists( struct User_list* list, char const* nick )
{
    assert( list );
    assert( nick );
    
    struct User_node* unode = list->head;
    
    while( unode != NULL )
    {
        if( strcmp( unode->nick, nick ) == 0 )
            return true;
        else
            unode = unode->next;
    }
    
    return false;
}

void list_send_foreach( struct User_list* list, char const* nick, char const* message )
{
    assert( list );
    assert( nick );
    assert( message );
    
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [ 8 ] = {'\0'};
    
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    
    strftime( buffer, 8, " %H:%M ", timeinfo );
    
    int const full_message_len = 19 + strlen( nick ) + strlen( message ) + 1;
    char* full_message = malloc( full_message_len );
    memset( full_message, '\0', full_message_len );
    
    strcpy( full_message, "[" );
    strcat( full_message, nick );
    strcat( full_message, buffer );
    strcat( full_message, "]: " );
    strcat( full_message, message );
    strcat( full_message, ">>>> " );
    
   
    struct User_node* unode = list->head;
    
    while( unode != NULL )
    {
        puts( "broadcasting" );
        if( strcmp( unode->nick, nick ) == 0 )
        {
            unode = unode->next;
            continue;
        }
        node_send_data( unode, full_message, full_message_len );
        unode = unode->next;
    }
    
    free( full_message );
}

void list_del_user( struct User_list* list, char const* nick )
{
    struct User_node* prev = NULL;
    struct User_node* node = list->head;
    
    while( node != NULL )
    {
        if( strcmp( node->nick, nick ) == 0 )
        {
            pthread_mutex_lock( &list->mutex );
            if( prev == NULL )
            {
                list->head = node->next;
            }
            else
            {
                prev->next = node->next;
            }
            node_free( node );
            pthread_mutex_unlock( &list->mutex );
            break;
        }
        
        prev = node;
        node = node->next;
    }
}
