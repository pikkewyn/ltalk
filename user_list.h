#ifndef USER_LIST_H
#define USER_LIST_H

#include <pthread.h>
#include <stdbool.h>

#include "user_node.h"

struct User_list
{
    pthread_mutex_t mutex;
    struct User_node* head;
};

struct User_list* list_init(); 
void list_free( struct User_list* );
void list_append_node( struct User_list*, struct User_node* );
void list_del_user( struct User_list*, char const* );
bool list_user_exists( struct User_list*, char const* );
void list_send_foreach( struct User_list*, char const*, char const* );

#endif
