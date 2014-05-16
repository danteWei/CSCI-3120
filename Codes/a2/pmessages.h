
#ifndef _pmessages_h_

#include <pthread.h>

/* Standard return codes for this system. */

#define MSG_FAIL (0)
#define MSG_OK (1)

/* Define all of the functions that can be used. */

int messages_init ();
void messages_end ();

/* The message is duplicated by the message passing system, so the sender is responsible for releasing any memory tha they allocated to the sent message. */

int send_message_to_thread( pthread_t thread, char *message, int message_size);

/* Each of thread, messages, and message_size has values returning to the caller, so you need to pass in addresses. */

int receive_message( pthread_t *thread, char **message, int *message_size);

#endif

