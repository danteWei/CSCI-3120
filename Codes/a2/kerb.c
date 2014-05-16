
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "pmessages.h"
#include "list.h"
#include "constants.h"
#include "auth.h"
#include "ticket.h"
#include "service.h"
#include "client.h"

#define LINE_LEN (80)

int 
parse_service_file( char *filename, List_t *ticket_services, List_t *client_services )
{
  int ok_to_go = FAILED;
  FILE *file_info = NULL;
  char line[LINE_LEN+1];
  char *operation;
  char *secret;
  char *users;
  char *user;
  char *lasts;
  char *usercopy;
  client_services_t *service_info;
  ticket_service_t *ticket_info;
  char *place;

  if ((ticket_services != NULL) && (client_services != NULL) && (filename != NULL) && ((file_info = fopen(filename, "r")) != NULL)) {
    /* Get the contents of the file. */

    ok_to_go = OK;
    while ((ok_to_go == OK) && fgets( line, LINE_LEN, file_info) ) {
      line[LINE_LEN] = '\0';

      place = NULL;
      if ((operation = strtok_r( line, ":\n", &place)) && 
          (secret = strtok_r( NULL, ":\n", &place)) && 
          (users = strtok_r( NULL, ":\n", &place))) {

        /* Fill in the info for the client and the ticket agent. */

        service_info = (client_services_t *) malloc( sizeof(client_services_t));
        ticket_info = (ticket_service_t *)malloc( sizeof( ticket_service_t ) );
        if ((service_info != NULL) && (ticket_info != NULL)) {

          /* Add the entry to the client list of services. */

          strncpy( service_info->name, operation, strlen(operation) );
          service_info->service_id = 0;
          service_info->certificate = NULL;
          if (!List_add_tail( client_services, (void *)service_info)) {
            ok_to_go = FAILED;
          }

          /* Add the entry to the ticket agent's services. */

          strncpy( ticket_info->name, operation, strlen(operation) );
          strncpy( ticket_info->secret, secret, strlen(secret) );
          if (List_init( &(ticket_info->users) )) {

            /* Add each user to the ones listed for the service. */

            lasts = NULL;
            user = strtok_r ( users, ",\n", &lasts );
            while ((ok_to_go == OK) && (user != NULL)) {
              usercopy = (char *)malloc(strlen(user)+1);
              strncpy( usercopy, user, strlen(user) );
              usercopy[strlen(user)] = '\0';
              if (!List_add_tail( &(ticket_info->users), (void *)usercopy)) {
                ok_to_go = FAILED;
              }
              user = strtok_r ( NULL, ",\n", &lasts );
            }

            if (!List_add_tail( ticket_services, (void *)ticket_info)) {
              ok_to_go = FAILED;
            }
          } else {
            printf ("Unable to initialize list for ticket service\n");
            ok_to_go = FAILED;
          }
        } else {
          ok_to_go = FAILED;
          printf ("Unable to allocate space. \n");
        }
      } else {
        printf ("Unable to parse input line: %s\n", line);
      }
    }
  }

  return ok_to_go; 
}


int
main( int argc, char **argv )
{
  auth_info_t auth_info;
  ticket_info_t ticket_info;
  client_info_t client_info;
  service_info_t *service_info = NULL;
  pthread_attr_t attrib;
  pthread_t client_id;
  client_services_t *client_service;
  void *context;
  void *context2;
  ticket_service_t *ticket_service;

  if (argc < 3) {
    printf ("usage %s <auth_filename> <service_filename>\n", argv[0] );
  } else {
    if ((messages_init() == MSG_OK) && List_init( &(ticket_info.services) ) && List_init( &(client_info.services) ) ) {

      /* Get all of the service information. */

      if (parse_service_file( argv[2], &(ticket_info.services), &(client_info.services) ) == OK) {
        /* Prepare the authentication system. */

        strncpy( auth_info.auth_filename, argv[1], FILENAME_LEN );
        strncpy( auth_info.ticket_secret, tmpnam(NULL), SECRET_LEN );
        auth_info.ticket_secret[SECRET_LEN] = '\0';
        strncpy( ticket_info.ticket_secret, auth_info.ticket_secret, SECRET_LEN );
        ticket_info.ticket_secret[SECRET_LEN] = '\0';

        /* Launch the authenticator and ticket agent. */

        pthread_attr_init( &attrib );
        pthread_attr_setscope( &attrib, PTHREAD_SCOPE_SYSTEM );

        pthread_create( &client_info.auth_service, &attrib, authenticator, (void *)&auth_info );
        pthread_create( &client_info.ticket_service, &attrib, ticket_agent, (void *)&ticket_info );

        /* Launch each of the services. */

        context = NULL;
        while (List_next_node( &(client_info.services), &context, (void **)&client_service) & (client_service != NULL)) {
          service_info = (service_info_t *)malloc(sizeof(service_info_t) );
          if (service_info != NULL) {
            strncpy( service_info->name, client_service->name, NAME_LEN );
    
            /* Find its secret. */

            service_info->secret[0] = '\0';
            context2 = NULL;
            while (List_next_node( &(ticket_info.services), &context2, (void **)&ticket_service) && (ticket_service != NULL) ) {
              if (strcmp( service_info->name, ticket_service->name) == 0) {
                strncpy( service_info->secret, ticket_service->secret, SECRET_LEN );
              }
            }

            if (strncmp("add", service_info->name, 4) == 0) {
              pthread_create( &(client_service->service_id), &attrib, add_agent, (void *)service_info );
            } else if (strncmp("sub", service_info->name, 4) == 0) {
              pthread_create( &(client_service->service_id), &attrib, sub_agent, (void *)service_info );
            } else if (strncmp("mult", service_info->name, 5) == 0) {
              pthread_create( &(client_service->service_id), &attrib, mult_agent, (void *)service_info );
            }
          }
        }

        /* Last, launch the client. */

        pthread_create( &client_id, &attrib, client_agent, (void *)&client_info );

        /* Wait for the authenticator to end.  Without it, the rest of the system can be torn down. */

        pthread_join( client_info.auth_service, NULL );

      } else {
        printf ("Unable to get service info.  Cannot continue.\n");
      }
    } else {
      printf ("Unable to initialize lists\n");
    }

    messages_end();
  }

  return 1;
}

