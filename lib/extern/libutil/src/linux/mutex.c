/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <gs/util/mutex.h>
#include <errno.h>
#include <stdlib.h>

gs_error_t gs_mutex_create(gs_mutex_t * mutex)
{
  if (mutex == NULL) {
    return GS_ERROR_ARG;
  }

  *mutex = malloc(sizeof(pthread_mutex_t));
  if (*mutex == NULL) {
    return GS_ERROR_ALLOC;
  }

  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  int res = pthread_mutex_init(*mutex, &attr);
  if (res < 0) {
    res = gs_error(errno);
    free(*mutex);
  }

  return res;
}

gs_error_t gs_mutex_destroy(gs_mutex_t mutex)
{
  int res = GS_OK;
  if (mutex) {
    res = pthread_mutex_destroy(mutex);
    if (res < 0) {
      res = gs_error(errno);
    }
    free(mutex);
  }
  return res;
}

gs_error_t gs_mutex_lock(gs_mutex_t mutex)
{
  int res = pthread_mutex_lock(mutex);
  if (res < 0) {
    res = gs_error(errno);
  }
  return res;
}

gs_error_t gs_mutex_unlock(gs_mutex_t mutex)
{
  int res = pthread_mutex_unlock(mutex);
  if (res < 0) {
    res = gs_error(errno);
  }
  return res;
}
