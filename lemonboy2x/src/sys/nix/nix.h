/*
 * nix.c
 *
 * System interface for *nix systems.
 */


#ifndef _NIX_H
#define _NIX_H

#include <sys/time.h>

void *sys_timer();
int sys_elapsed(struct timeval*);
void sys_checkdir(char *path, int wr);
void sys_initpath();
void sys_sanitize(char *s);
void sys_sleep(int);

#endif