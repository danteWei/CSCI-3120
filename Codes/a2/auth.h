
#ifndef _auth_h_
#define _auth_h_

#include "constants.h"
#include "list.h"

#define FILENAME_LEN (80)
#define USER_LEN (80)
#define PASS_LEN (80)

typedef struct {
  char ticket_secret[SECRET_LEN+1];
  char auth_filename[FILENAME_LEN+1];
} auth_info_t;

typedef struct {
  char user[USER_LEN+1];
  char password[PASS_LEN+1];
} user_t;

void *authenticator( void *param );

#endif

