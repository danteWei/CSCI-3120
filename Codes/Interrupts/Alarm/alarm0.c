#include <stdio.h>
#include <signal.h>

void
alarm_bells( int the_signal )
{
  printf ("tick\n");
}


int
main( int argc, char **argv )
{
  int return_code = 0;

  if (signal( SIGALRM, alarm_bells ) == SIG_ERR) {
    printf ("Unable to install handler\n");
    return_code = 1;
  } else {
    /* Start the timer and then wait! */

    alarm( 1 );
    for (;;);
  }

  return return_code;
}

