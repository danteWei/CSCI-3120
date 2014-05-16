
Example of using fork to spawn a heavyweight process to then run a
different executable.

fork.c runs ls, but it only runs lists the current directory contents.

fork2.c tries to provide a path for where to list.  Unfortunately,
it didn't create the set of parameters for command invocation properly
(the first element of argv should be the program name"

fork3.c now lists the directory correctly.

