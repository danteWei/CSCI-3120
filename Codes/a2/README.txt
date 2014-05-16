
Overview
--------

The main program is in kerb.c (compiled to "kerb") for Kerberos.  The program
simulates a simplified version of what Kerberos would do to authorize
access to a service.

The program starts 6 threads that communicate through a non-blocking send / blocking receive system.  The 6 threads are:
 - an authentication thread to validate user names and passwords
 - a ticket granting thread to determine which users can use what services
 - three service threads (add, sub, and mult)
 - a client thread to manage the communication with a user and send requests
   to services.

All threads are in infinite loops to request data.

Data structures
---------------

Authentication thread:  (auth.h, auth.c)
  - auth_info_t -- configuration information to start the thread
  - a linked list of usernames and passwords, each element of type user_t.
    The authentication thread builds this linked list on its own from 
    the authentication file contents.

Ticket thread:  (ticket.h, ticket.c)
  - ticket_info_t -- configuration information to start the thread, which
    includes a linked list of all the services that the ticket thread is
    to manage.  Each service has a name, a secret value shared with the
    ticket thread, and a linked list of usernames who are allowed to use
    the service.

    Both linked lists are created from the contents of the services file
    _before_ the thread is created.

Service threads:  (service.h, service.c)
  - service_info_t -- configuration information for the thread (the
    name of the service and the service's secret value)

Client thread:  (client.h, client.c)
  - client_info_t -- configuration information for the thread.  The
    configuration information tells the client where to find all the
    other threads since all communication in this system originates
    with the client thread.

    The set of services are stored as a linked list that is created _before_
    the client thread is created.  The content comes from the services
    file.

    One limitation of the current structure is that the certificate for each
    service is stored in this same linked list.  This means that the 
    initialization of the client must set the certificates initially to NULL
    and that we would need to copy the list for each client that we might
    want to create.

Thread "secrets"
----------------

The ticket and service threads each have a secret.  The secrets for the
service threads are provided in the service file as input.  The secret for
the ticket thread is not provided in the input.  Consequently, the program
generates a random secret prior to starting the authentication and ticket
threads.  While I could have used a random number generator to get a number
as the secret, I used the tmpnam function, usually used to generate a
temporary file name, to have a string as the secret instead.

Thread creation
---------------

The "main" function creates all the threads.  It is also responsible
for parsing the services file among the ticket thread, service threads,
and client thread information before launching the threads.  By doing
the parsing before we create the threads, we have a better claim that no
secret information is revealed except to the appropriate services since
only the overseeing thread sees all the secrets and that thread does no
work after creating the threads.

One ugly point of the thread creation process is that we have an exhaustive
"if" check to determine which function to launch for each service.  This
hard-coding is done for simplicity.  An alternative would be to place the
service names and a function pointer to the appropriate function in a table
and then iterate through the table to launch each thread.

Client certificates
-------------------

To minimize the creation of certificates, the client only asks for
a certificate when a service is about to be used.  Once it receives a
certificate, it caches it:  service certificates are cached in the linked
list of services and the certificate from the authentication thread to
the ticket thread is cached in a local variable.  All the certificates
are managed in the have_service_certificate function.

Since certificates are supposed to be encrypted, the client thread does
_not_ look at the return values from the authentication and ticket threads
to see if it should proceed.  That return value would be part of what
is encrypted so the best the client can do is present its certificate
to the service and be accepted or denied.  Only at that time will the
client know if it has the permission to use the service.

Service threads
---------------

All the service threads have the same format.  In this example, the services
offered by the threads are simple.  Still, each service is given its own
function to mimic what you would do if you had more complex services to offer.

The common functionality to verify that a user is allowed to use a service
is encapsulated in the function validate_operation.  That function also
returns the two parameters from the user request since it is parsing the 
request anyway.

