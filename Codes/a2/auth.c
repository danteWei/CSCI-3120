
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "auth.h"
#include "list.h"
#include "pmessages.h"

#define LINE_LEN (160)

int
read_auth_file(char *filename, List_t *users)
{
  int return_code = FAILED;
  char line[LINE_LEN+1];
  FILE *info_file;
  user_t *user_info;
  char *username;
  char *password;
  char *place;

  if ((filename != NULL) && (strlen(filename) > 0) && ((info_file = fopen( filename, "r")) != NULL)) {

    return_code = OK;

    /* Read in the file contents. */

    while (fgets( line, LINE_LEN, info_file ) != NULL) {
      line[LINE_LEN] = '\0';
      user_info = (user_t *)malloc(sizeof(user_t));
      if (user_info != NULL) {

        /* Pull out the username and password for the linked list. */

        place = NULL;
        username = strtok_r( line, ":\n", &place );
        password = strtok_r( NULL, ":\n", &place );

        strncpy( user_info->user, username, USER_LEN );
        strncpy( user_info->password, password, PASS_LEN );

        if (List_add_tail( users, (void *)user_info ) == FAILED) {
          printf ("Unable to add new user to list. \n");
          return_code = FAILED;
        }
      } else {
        printf ("Unable to allocate space for new user. \n");
        return_code = FAILED;
      }
    }

    fclose( info_file );
  } 

  return return_code;
}

void *
authenticator( void *param )
{
  auth_info_t *info = (auth_info_t *)param;
  List_t users;
  char line[LINE_LEN+1];
  user_t *user_info;
  void *context;
  pthread_t sender;
  char *message;
  int message_size;
  char *username;
  char *password;
  int found;
  char *place;

  if ((param != NULL) && (List_init( &users ))){
    /* Initialize with the set of users. */

    if (read_auth_file(info->auth_filename, &users) == OK) {

      /* Now that we have all of the users, wait for requests. */

      while (1) {
        if (receive_message( &sender, &message, &message_size ) ) {
          /* Break the message apart. */
          place = NULL;
          if ( (username = strtok_r( message, ":\n", &place)) &&
               (password = strtok_r( NULL, ":\n", &place)) ) {

            username[USER_LEN] = '\0';
            password[PASS_LEN] = '\0';

            /* Search for the user name in the list of users. */

            found = 0;
            context = NULL;

            while (!found && List_next_node( &users, &context, (void **) &user_info ) && (user_info != NULL)) {
              if (strncmp(username, user_info->user, strlen(username)) == 0) {
                found = 1;
              }
            }

            if (found) {
              if (strncmp( password, user_info->password, strlen(password) ) == 0) {
                /* All checks out. */

                sprintf( line, "1:%s:%s", username, info->ticket_secret );
              } else {
                /* Not authenticated. */
                sprintf( line, "0:%s:0", username );
              }
            }
          } else {
            printf ("Improper message to authenticator.\n");
            sprintf ( line, "0:nobody:0" );
          }

          if (send_message_to_thread( sender, line, strlen(line)+1 ) == FAILED) {
            printf ("Send from auth thread failed\n");
          }

          /* Get rid of the message that was created for me. */

          free( message );
          message = NULL;
        } else {
          printf ("Message receive failed in authenticator.\n");
        }
      }

      /* In the off case that we can leave the loop, clean up the space
         that was allocated. */

      context = NULL;
      while (List_next_node( &users, &context, (void **) &user_info ) && (user_info != NULL)) {
        free( user_info );
      }

      List_destroy( &users );
    }
  }

  return NULL;
}

