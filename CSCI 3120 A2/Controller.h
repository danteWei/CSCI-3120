#ifndef _Controller_h_
#define _Controller_h_

#define MAXLINE 80
#define MAXVIEW 3


void read_input();
int exitProg(pthread_t *model, pthread_t views[MAXVIEW]);
int exitThread(pthread_t *thread);
void *Controller();

#endif
