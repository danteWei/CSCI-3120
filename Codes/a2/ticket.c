
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "ticket.h"
#include "list.h"
#include "pmessages.h"

#define LINE_LEN (200)

void *
ticket_agent( void *param )
{
  ticket_info_t *info = (ticket_info_t *)param;

  ticket_service_t *ticket_info;
  void *context;
  pthread_t sender;
  char *message;
  int message_size;

  char *username;
  char *servicename;
  char *valid_id;
  char *user_auth;
  char *secret;
  int found;
  char *place;

  char line[LINE_LEN+1];
  char *who;

  if (param != NULL) {

    /* Now that we have all of the users, wait for requests. */

    while (1) {
      if (receive_message( &sender, &message, &message_size ) ) {
        /* Break the message apart. */

        place = NULL;
        if ( (username = strtok_r( message, ":\n", &place)) && 
             (servicename = strtok_r( NULL, ":\n", &place)) ) {

          if ( (valid_id = strtok_r( NULL, ":\n", &place)) && 
               (user_auth = strtok_r( NULL, ":\n", &place)) && 
               (secret = strtok_r( NULL, ":\n", &place)) ) {

            /* Make sure that this is a correct person. */
  
            if((strncmp( valid_id, "1", 2) == 0) && 
               (strncmp( username, user_auth, strlen(username)) == 0) &&
               (strncmp( secret, info->ticket_secret, strlen(secret)) == 0)){

              /* The authenticator says that this user checks out.  See what the
                 user is allowed to do. */

              /* Search for the service name in the list of services. */
  
              found = 0;
              context = NULL;
    
              while (!found && List_next_node( &(info->services), &context, (void **) &ticket_info ) && (ticket_info != NULL)) {
                if (strncmp(servicename, ticket_info->name, strlen(servicename)) == 0) {
                  found = 1;
                }
              }
  
              if (found) {
                /* See if this user is allowed to use the service. */

                found = 0;
                context = NULL;
  
                while (!found && List_next_node( &(ticket_info->users), &context, (void **) &who ) && (who != NULL)) {
                  if (strncmp(username, who, strlen(username)) == 0) {
                    found = 1;
                  }
                }
 
                if (found) {
                  /* Allow the request. */
                  sprintf( line, "1:%s:%s:%s", username, servicename, ticket_info->secret );
                } else {
                  /* Deny the request. */
                  sprintf( line, "0:%s:%s:0", username, servicename );
                }
              } else {
                printf ("Bad auth ticket: %s %s\n", username, servicename);
                sprintf( line, "0:%s:%s:0", username, servicename );
              }
            } else {
              printf ("Bad service requested\n");
              sprintf( line, "0:%s:%s:0", username, servicename );
            }
          } else {
            printf ("Bad auth ticket\n");
            sprintf ( line, "0:nobody:nothing:0" );
          }
        } else {
          printf ("Improper message to authenticator.\n");
          sprintf ( line, "0:nobody:nothing:0" );
        }

        if (send_message_to_thread( sender, line, strlen(line)+1 ) == FAILED) {
          printf ("Send from auth thread failed\n");
        }
        
        /* Get rid of the message that was created for me. */
          
        free( message );
        message = NULL;

      } else {
        printf ("Message receive failed in ticket agent.\n");
      }
    }
  }

  return NULL;
}

