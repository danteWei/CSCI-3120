
#ifndef _ticket_h_
#define _ticket_h_

#include "list.h"
#include "constants.h"

typedef struct {
  char name[NAME_LEN+1];
  char secret[SECRET_LEN+1];
  List_t users;
} ticket_service_t;

typedef struct {
  char ticket_secret[SECRET_LEN+1];
  List_t services;
} ticket_info_t;

void *ticket_agent( void *param );

#endif

