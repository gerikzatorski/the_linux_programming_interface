#ifndef EXERCISE_57_01_H
#define EXERCISE_57_01_H

#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 20 /* Maximum size of messages exchanged
                       between client to server */

#define SV_SOCK_PATH "/tmp/ud_ucase"

#endif /* EXERCISE_57_01_H */
