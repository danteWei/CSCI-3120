
#ifndef _service_h_
#define _service_h_

#include "constants.h"

typedef struct {
  char name[NAME_LEN+1];
  char secret[SECRET_LEN+1];
} service_info_t;

void *add_agent( void *param );
void *sub_agent( void *param );
void *mult_agent( void *param );

#endif

