
#ifndef __USE_GNU
#define __USE_GNU 1
#endif /* __USE_GNU */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

void daemonize_server(char *lock_file, __sighandler_t signal_handler) {
  int pid;
  int lockfp;
  char str[10];

  if (getppid() == 1) {
    return;
  }
  pid = fork();
  if (pid < 0) {
    exit(1);
  }
  if (pid > 0) {
    exit(0);
  }

  /* Try to become root, but ignore if we can't */
  setuid((uid_t) 0);

  setsid();
  for (pid = getdtablesize(); pid>=0; --pid) {
    close(pid);
  }
  pid = open("/dev/null", O_RDWR); dup(pid); dup(pid);
  umask((mode_t) 022);
  lockfp = open(lock_file, O_RDWR | O_CREAT, 0640);
  if (lockfp < 0) {
    syslog(LOG_ERR, "Could not serialize PID to lock file");
    exit(1);
  }
  if (lockf(lockfp, F_TLOCK,0)<0) {
    syslog(LOG_ERR, "Could not create lock, bailing out");
    exit(0);
  }
  sprintf(str, "%d\n", getpid());
  write(lockfp, str, strlen(str));
  close(lockfp);

  /* Signal handling */
  signal(SIGCHLD, signal_handler);
  signal(SIGTSTP, signal_handler);
  signal(SIGTTOU, signal_handler);
  signal(SIGTTIN, signal_handler);
  signal(SIGHUP , signal_handler);
  signal(SIGTERM, signal_handler);
}

