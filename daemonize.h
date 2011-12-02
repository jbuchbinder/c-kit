
#ifndef __DAEMONIZE_H
#define __DAEMONIZE_H 1

void daemonize_server(char *lock_file, __sighandler_t signal_handler);

#endif /* __DAEMONIZE_H */

