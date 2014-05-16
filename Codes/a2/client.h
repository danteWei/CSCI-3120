
#ifndef _client_h_
#define _client_h_

#include <pthread.h>
#include "list.h"
#include "constants.h"

typedef struct {
  char name[NAME_LEN+1];
  pthread_t service_id;

  char *certificate;
} client_services_t;

typedef struct {
  pthread_t auth_service;
  pthread_t ticket_service;
  List_t services;
} client_info_t;

void *client_agent( void *param );

#endif

