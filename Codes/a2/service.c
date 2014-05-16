
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "service.h"
#include "list.h"
#include "pmessages.h"

#define LINE_LEN (200)

int
validate_operation( service_info_t *myinfo, char *message, int *p1, int *p2 ) {
  int ok_to_go = FAILED;
  char *username;
  char *servicename;
  char *valid_id;
  char *ticket_user;
  char *ticket_service;
  char *secret;
  char *p1_str, *p2_str;
  char *place;

  if ((myinfo != NULL) && (message != NULL)) {

    /* Pull out the basic message. */
 
    place = NULL;
    if ( (username = strtok_r( message, ":\n", &place)) && 
         (servicename = strtok_r( NULL, ":\n", &place)) && 
         (p1_str = strtok_r( NULL, ":\n", &place)) && 
         (p2_str = strtok_r( NULL, ":\n", &place)) ) {

      *p1 = atoi( p1_str );
      *p2 = atoi( p2_str );

      if (strncmp(servicename, myinfo->name, strlen(servicename)) == 0) {

        /* They've asked for my service.  Now check the authentication information. */

        if ( (valid_id = strtok_r( NULL, ":\n", &place )) &&
             (ticket_user = strtok_r( NULL, ":\n", &place )) &&
             (ticket_service = strtok_r( NULL, ":\n", &place )) &&
             (secret = strtok_r( NULL, ":\n", &place ))) {

          /* Validate the ticket from the ticket agent. */

          if ( (strncmp(valid_id, "1", 2) == 0) &&
               (strncmp(ticket_user, username, strlen(username)) == 0) &&
               (strncmp(ticket_service, servicename, strlen(servicename)) == 0) &&
               (strncmp(secret, myinfo->secret, strlen(secret)) == 0)) {

            /* Everhthing checks out so allow the operation. */

            ok_to_go = OK;
          } 
        }
      }
    }
  }

  return ok_to_go;
}


void *
add_agent( void *param )
{
  service_info_t *info = (service_info_t *)param;

  pthread_t sender;
  char *message;
  int message_size;

  int p1, p2;
  int result;

  char line[LINE_LEN+1];

  if (param != NULL) {

    /* Wait for requests. */

    while (1) {
      if (receive_message( &sender, &message, &message_size ) ) {

        /* Break the message apart. */

        if (validate_operation( info, message, &p1, &p2 ) == OK) {
          result = p1 + p2;
          sprintf (line, "1:%s:%d", info->name, result );
        } else {
          sprintf (line, "0:%s:0", info->name );
        }

        /* Send the answer. */

        if (send_message_to_thread( sender, line, strlen(line)+1 ) == FAILED) {
          printf ("Send from auth thread failed\n");
        }

        /* Get rid of the message that was created for me. */
          
        free( message );
        message = NULL;

      } else {
        printf ("Message receive failed in service.\n");
      }
    }
  }

  free( param );

  return NULL;
}

void *
sub_agent( void *param )
{
  service_info_t *info = (service_info_t *)param;

  pthread_t sender;
  char *message;
  int message_size;

  int p1, p2;
  int result;

  char line[LINE_LEN+1];

  if (param != NULL) {

    /* Wait for requests. */

printf ("Service %s started with secret %s\n", info->name, info->secret );
    while (1) {
      if (receive_message( &sender, &message, &message_size ) ) {

        /* Break the message apart. */

        if (validate_operation( info, message, &p1, &p2 ) == OK) {
          result = p1 - p2;
          sprintf (line, "1:%s:%d", info->name, result );
        } else {
          sprintf (line, "0:%s:0", info->name );
        }

        /* Send the answer. */

        if (send_message_to_thread( sender, line, strlen(line)+1 ) == FAILED) {
          printf ("Send from auth thread failed\n");
        }

        /* Get rid of the message that was created for me. */
          
        free( message );
        message = NULL;

      } else {
        printf ("Message receive failed in service.\n");
      }
    }
  }

  free( param );

  return NULL;
}

void *
mult_agent( void *param )
{
  service_info_t *info = (service_info_t *)param;

  pthread_t sender;
  char *message;
  int message_size;

  int p1, p2;
  int result;

  char line[LINE_LEN+1];

  if (param != NULL) {

    /* Wait for requests. */

printf ("Service %s started with secret %s\n", info->name, info->secret );
    while (1) {
      if (receive_message( &sender, &message, &message_size ) ) {

        /* Break the message apart. */

        if (validate_operation( info, message, &p1, &p2 ) == OK) {
          result = p1 * p2;
          sprintf (line, "1:%s:%d", info->name, result );
        } else {
          sprintf (line, "0:%s:0", info->name );
        }

        /* Send the answer. */

        if (send_message_to_thread( sender, line, strlen(line)+1 ) == FAILED) {
          printf ("Send from auth thread failed\n");
        }

        /* Get rid of the message that was created for me. */
          
        free( message );
        message = NULL;

      } else {
        printf ("Message receive failed in service.\n");
      }
    }
  }

  free( param );

  return NULL;
}

