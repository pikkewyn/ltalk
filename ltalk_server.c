#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "user_list.h"

#define MAX_MESSAGE 2000
#define THREAD_NUM 100

static int const DEFAULT_PORT = 8888;
static int const MAX_CONNECTIONS_QUEUE_LEN = 5;
static struct User_list* user_list = NULL;
static bool run = true;

struct Thread_Array
{
    pthread_t* threads;
    int size;
    int capacity;
};

void thread_array_enlarge( struct Thread_Array** thread_array )
{
    if( *thread_array == NULL )
    {
        *thread_array = malloc( sizeof( struct Thread_Array ) );
        ( *thread_array )->size = 0;
        ( *thread_array )->capacity = 10;
        ( *thread_array )->threads = malloc( ( *thread_array )->capacity * sizeof( pthread_t ) );
    }
    else
    {
        ( *thread_array )->capacity *= 2;
        ( *thread_array )->threads = realloc( ( *thread_array )->threads, ( *thread_array )->capacity );
    }
}

void thread_array_free( struct Thread_Array* thread_array )
{
    free( thread_array->threads );
    free( thread_array );
    thread_array = NULL;
}

void *connection_handler( void * );

void sigint_handler( int const );

void sigint_handler( int const signum )
{
    run = false;
}

void* connection_handler( void* socket_desc )
{
    int connection_socket = *( int* )socket_desc;
    char *message;
    char user_message[ MAX_MESSAGE ];

    message = ">>>> WELCOME <<<<\n";
    write( connection_socket, message, strlen( message ) );

    message = ">>>> Please use /nick [nick] to set Your nick\n>>>> ";
    write( connection_socket, message, strlen( message ) );


    bool registered = false;
    struct User_node* unode = NULL;
    int read_size = 0;

    while( ( read_size = recv( connection_socket, user_message, MAX_MESSAGE, 0 ) ) > 0 && run )
    {
        if( ! registered )
        {
            user_message[ read_size - 2 ] = '\0';

            if( strstr( user_message, "/nick " ) == NULL )
            {
                message = ">>>> Please use /nick [nick] to set Your nick\n>>>> ";
                write( connection_socket, message , strlen( message ) );
            }
            else
            {
                char* nick = strchr( user_message, ' ' ) + 1;
                if( list_user_exists( user_list, nick ) )
                {
                    message = ">>>> This nick is already in use\n>>>> ";
                    write( connection_socket, message , strlen( message ) );
                }
                else
                {
                    message = "thanks for registering\n>>>> ";
                    write( connection_socket, message, strlen( message ) );

                    unode = node_init( connection_socket, nick );
                    list_append_node( user_list, unode );

                    registered = true;
                    puts( "user registered" );
                }
            }
        }
        else
        {
            message = ">>>> ";
            write( connection_socket, message , strlen( message ) );

            if( read_size > 2 )
            {
                list_send_foreach( user_list, unode->nick, user_message );
            }
        }

        memset( user_message, '\0', MAX_MESSAGE );
    }

    if( read_size == 0 )
    {
        puts( "user disconnected" );
    }
    else if( read_size == -1 )
    {
        perror( "recv failed" );
    }

    if( unode != NULL )
        list_del_user( user_list, unode->nick );

    free( socket_desc );

    pthread_exit( NULL );
    return 0;
}

bool set_sigint_action()
{
    struct sigaction sigint_action;
    sigint_action.sa_handler = sigint_handler;
    sigint_action.sa_flags = 0;
    sigemptyset( &sigint_action.sa_mask );

    if( sigaction( SIGINT, &sigint_action, NULL ) == -1 )
    {
        perror( "sigaction" );
        return false;
    }

    return true;
}

bool set_sigpipe_action()
{
    struct sigaction sigpipe_action;
    sigpipe_action.sa_handler = SIG_IGN;
    sigpipe_action.sa_flags = 0;
    sigemptyset( &sigpipe_action.sa_mask );

    if( sigaction( SIGPIPE, &sigpipe_action, NULL ) == -1 )
    {
        perror( "sigaction" );
        return false;
    }

    return true;
}

int prepare_listening_socket()
{
    int listening_socket = socket( AF_INET , SOCK_STREAM , 0 );
    if( listening_socket == -1 )
    {
        perror( "socket" );
        return -1;
    }

    int opt_val = 1;
    setsockopt( listening_socket, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof( opt_val ) );

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( DEFAULT_PORT );

    if( bind( listening_socket, ( struct sockaddr * ) &server, sizeof( server ) ) < 0 )
    {
        perror( "bind" );
        return -1;
    }

    puts( "bind done" );

    return listening_socket;
}

int main( int argc , char *argv[] )
{
    if( ! set_sigint_action() )
        return EXIT_FAILURE;

    if( ! set_sigpipe_action() )
        return EXIT_FAILURE;

    int listening_socket = 0;
    if( ( listening_socket = prepare_listening_socket() ) == -1 )
        return EXIT_FAILURE;

    if( listen( listening_socket, MAX_CONNECTIONS_QUEUE_LEN ) == -1 )
    {
        perror( "listen" );
        return EXIT_FAILURE;
    }

    struct Thread_Array* connection_threads = NULL;
    thread_array_enlarge( &connection_threads );

    //pthread_t* connection_threads = malloc( 10 * sizeof( pthread_t ) );
    user_list = list_init();

    int tmp_socket;
    puts( "waiting for incoming connections..." );

    while( ( tmp_socket = accept( listening_socket, NULL, 0 ) ) >= 0  && ( run == true ) )
    {
        puts( "connection accepted" );

        int* incoming_connection = malloc( sizeof( int ) );
        *incoming_connection = tmp_socket;

        if( connection_threads->size == connection_threads->capacity )
            thread_array_enlarge( &connection_threads );

        if( pthread_create( &connection_threads->threads[ connection_threads->size++ ], NULL, connection_handler, ( void* ) incoming_connection ) < 0 )
        {
            perror( "could not create thread" );
            return 1;
        }


        puts( "handler assigned" );
        puts( "waiting for incoming connections..." );
    }


    if( tmp_socket < 0 )
    {
        perror( "accept failed" );
    }

    for( int i = 0; i < connection_threads->size; ++i )
        pthread_join( connection_threads->threads[ i ], NULL );

    thread_array_free( connection_threads );
    list_free( user_list );
    close( listening_socket );

    return 0;
}


