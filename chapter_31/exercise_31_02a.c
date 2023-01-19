/* exercise_31_02a.c */

/*********************************************************************
Use thread-specific data to write thread-safe versions of dirname()
and basename() (Section 18.14).

This file implements dirname.
*********************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "tlpi_hdr.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t dirname_key;

#define MAX_DIRNAME 256

static void delete_i_from_str(char *str, int i)
{
  memmove(&str[i], &str[i + 1], strlen(str) - i);
}

static void destructor(void *buf)
{
  free(buf);
}

static void create_key(void)
{
  int s;

  s = pthread_key_create(&dirname_key, destructor);
  if (s != 0)
    errExitEN(s, "pthread_key_create");
}

char *dirname_tsd(char *pathname)
{
  int s, i, islash;
  char *buf;
  
  /* Create a key. Only needs to be done in first thread to call.  */

  s = pthread_once(&once, create_key);
  if (s != 0)
    errExitEN(s, "pthread_once");

  /* Get buffer location */
  
  buf = pthread_getspecific(dirname_key);

  /* Or allocate and save location if first call from this thread */
  
  if (buf == NULL) {
    buf = malloc(MAX_DIRNAME);
    if (buf == NULL)
      errExit("malloc");

    s = pthread_setspecific(dirname_key, buf);
    if (s != 0)
      errExitEN(s, "pthread_setspecific");
  }

  /* Handle null and empty case */
  
  if (pathname == NULL || strcmp(pathname, "") == 0) {
    snprintf(buf, 2, "%s", ".");
    return buf;
  }

  /* Copy to buf to prevent cross-thread contamination */
  
  snprintf(buf, MAX_DIRNAME, "%s", pathname);

  /* Consolidate multi-slashes */
  
  i = 1;
  while (i < strlen(buf)) {
    if (buf[i] == '/' && buf[i - 1] == '/')
      delete_i_from_str(buf, i);
    else
      i++;
  }

  /* Handle root directory pathname */
  
  if (strcmp(buf, "/") == 0)
    return buf;

  /* Remove possible trailing slash */

  i = strlen(buf) - 1;
  if (buf[i] == '/')
    delete_i_from_str(buf, i);

  /* Find last slash and split  */
  
  i = 0;
  islash = -1;
  for (i = 0; i < strlen(buf); i++)
    if (buf[i] == '/')
      islash = i;

  if (islash == -1) /* No slash found */
    snprintf(buf, 2, "%s", ".");
  else
    buf[islash] = '\0';
  
  return buf;
}

int main(int argc, char *argv[])
{
  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pathname\n", argv[0]);

  printf("dirname_tsd = %s\n", dirname_tsd(argv[1]));

  /* Tests from table 18-3 */
  
  assert(strcmp(dirname_tsd("/"), "/") == 0);
  assert(strcmp(dirname_tsd("/usr/bin/zip"), "/usr/bin") == 0);
  assert(strcmp(dirname_tsd("/etc/passwd////"), "/etc") == 0);
  assert(strcmp(dirname_tsd("/etc////passwd"), "/etc") == 0);
  assert(strcmp(dirname_tsd("etc/passwd"), "etc") == 0);
  assert(strcmp(dirname_tsd("passwd"), ".") == 0);
  assert(strcmp(dirname_tsd("passwd/"), ".") == 0);
  assert(strcmp(dirname_tsd(".."), ".") == 0);
  assert(strcmp(dirname_tsd(NULL), ".") == 0);
  
  exit(EXIT_SUCCESS);
}
