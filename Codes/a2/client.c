
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "client.h"
#include "list.h"
#include "pmessages.h"

#define LINE_LEN (200)
#define PASS_LEN (80)

int 
have_service_certificate( client_info_t *info, char **ticket_certificate, char *username, char *password, client_services_t *service )
{
  int ok_to_go = FAILED;
  char message[LINE_LEN+1];
  pthread_t ignore;
  int response_size;

  if (service->certificate != NULL) { 
    ok_to_go = OK;
  } else {
    /* Need to get the service ticket. */

    if (*ticket_certificate == NULL) {

      /* We first need to get the ticket agent's certificate. */
      /* First authenticate myself. */

      sprintf (message, "%s:%s", username, password );
      if (send_message_to_thread( info->auth_service, message, strlen(message)+1)) {
        if (receive_message( &ignore, ticket_certificate, &response_size ) == OK) {
          ok_to_go = OK;
        } else {
          printf ("Failed to get certificate from authenticator\n");
        }
      } else {
        printf ("Unable to send message to auth service\n");
      }
    } else {
      ok_to_go = OK;
    }

    if (ok_to_go == OK) {
      ok_to_go = FAILED;

      /* We have the ticket certificate, now get the operation's certificate. */
  
      sprintf (message, "%s:%s:%s", username, service->name, *ticket_certificate);
      if (send_message_to_thread( info->ticket_service, message, strlen(message)+1)) {
        if (receive_message( &ignore, &service->certificate, &response_size ) == OK) { 
          ok_to_go = OK;
        } else {
          printf ("Failed to get message from service\n");
        }
      } else {
        printf ("Unable to send message to ticket service\n");
      }
    }
  } 

  return ok_to_go;
}

void *
client_agent( void *param )
{
  client_info_t *info = (client_info_t *)param;

  client_services_t *service;
  void *context;
  int found;
  char message[LINE_LEN];
  char *response;
  int response_size;

  char username[USER_LEN+1];
  char password[PASS_LEN+1];

  char user_input[LINE_LEN+1];
  char operation[LINE_LEN+1];
  int p1, p2;
  char *result;
  char *return_code;
  char *place;
  char *ignore_op;

  char *ticket_certificate = NULL;

  pthread_t ignore;

  if (param != NULL) {

    /* Start with the basics from the user. */

    printf("Enter your user name\n");
    fgets( username, USER_LEN, stdin );
    username[USER_LEN] = '\0';
    if (username[strlen(username)-1] == '\n') {
      username[strlen(username)-1] = '\0';
    }

    printf("Enter your password\n");
    fgets( password, PASS_LEN, stdin );
    password[PASS_LEN] = '\0';
    if (password[strlen(password)-1] == '\n') {
      password[strlen(password)-1] = '\0';
    }

    printf ("Enter request\n");
    while (fgets( user_input, LINE_LEN, stdin )) {
      user_input[LINE_LEN] = '\0';

      /* Parse the user's request. */

      if (sscanf( user_input, "%s %d %d", operation, &p1, &p2) == 3) {
        operation[LINE_LEN] = '\0';

        /* Find the thread for the operation. */

        found = 0;
        context = NULL;

        while (!found && List_next_node( &(info->services), &context, (void **) &service) && (service != NULL)) {
          if (strncmp( operation, service->name, strlen(operation) ) == 0) {
            found = 1;
          };
        }

        if (found) {
          /* We have the operation.  Get the certificate for the operation. */

          if (have_service_certificate( info, &ticket_certificate, username, password, service ) == OK) {

            /* We now have all our tickets, so make the request. */

            sprintf (message, "%s:%s:%d:%d:%s", username, operation, p1, p2, service->certificate);
            if (send_message_to_thread( service->service_id, message, strlen(message)+1)) {
              if (receive_message( &ignore, &response, &response_size ) ) {
              /* We have a response to share with the user. */
 
              place = NULL;
              if ( (return_code = strtok_r( response, ":\n", &place)) && 
                   (ignore_op = strtok_r( NULL, ":\n", &place)) && 
                   (result = strtok_r( NULL, ":\n", &place)) ) {
                 if (strncmp(return_code, "1", 2) == 0) {
                   printf ("Result: %s\n", result);
                 } else {
                   printf ("Operation denied\n");
                 }
               } else {
                 printf ("Incorrect format of response\n");
               }
  
               free( response );
               response = NULL;
              } else {
                printf ("No response from service\n");
              }
            }  else {
              printf ("Unable to send query to service\n");
            }
          }
        } else {
          printf ("Unknonwn operation: %s\n", operation);
        }
      } else {
        printf ("Bad request\n");
      }
    }

    printf ("Enter next request\n");
  }

  /* On exit, clean up all the data that we've accumulated. */

  free( ticket_certificate );
  ticket_certificate = NULL;

  context = NULL;

  while (!found && List_next_node( &(info->services), &context, (void **) &service) && (service != NULL) ) {
    free(service->certificate);
    service->certificate = NULL;
  }

  return NULL;
}

