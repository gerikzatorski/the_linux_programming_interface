#ifndef EXERCISE_57_03_H
#define EXERCISE_52_03_H

#include <sys/un.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"

#define SERVER_SOCK "xyz"
#define BACKLOG 5

struct request {
    int seq_len;
};

struct response {
    int seq_num;
};

#endif /* EXERCISE_57_03_H */
