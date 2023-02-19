/* exercise_44_03.c */

/*********************************************************************
The server in Listing 44-7 ( fifo_seqnum_server.c ) always starts
assigning sequence numbers from 0 each time it is started. Modify the
program to use a backup file that is updated each time a sequence
number is assigned. (The open() O_SYNC flag, described in Section
4.3.1, may be useful.) At startup, the program should check for the
existence of this file, and if it is present, use the value it
contains to initialize the sequence number. If the backup file can’t
be found on startup, the program should create a new file and start
assigning sequence numbers beginning at 0. (An alternative to this
technique would be to use memory-mapped files, described in Chapter
49.)
*********************************************************************/

#include "tlpi_hdr.h"

#include <signal.h>
#include "exercise_44_03.h"

static int update_backup(int fd, int val)
{
    /* Erase contents */
    
    if (ftruncate(fd, 0) != 0)
        return -1;

    lseek(fd, 0, SEEK_SET);
    if (write(fd, &val, sizeof(int)) == -1)
        return -1;

    return val;
}

int main(int argc, char *argv[])
{
    int server_fd, dummy_fd, client_fd, backup_fd;
    char client_fifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    struct stat sb;
    int seq_num;
    
    umask(0);
    
    if (stat(SEQUENCE_BACKUP, &sb) == 0) { /* file exists */
        backup_fd = open(SEQUENCE_BACKUP, O_RDWR | O_SYNC);
        if (backup_fd == -1)
            errExit("open");
        
        /* Initialize sequence number with value from backup */
        
        if (read(backup_fd, &seq_num, sizeof(int)) == -1)
            errExit("read");
    }
    else {
        backup_fd = open(SEQUENCE_BACKUP, O_RDWR | O_CREAT | O_SYNC,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (backup_fd == -1)
            errExit("open");
        
        seq_num = SEQUENCE_START; /* default value */
        
        if (write(backup_fd, &seq_num, sizeof(int)) == -1)
            errExit("write");
    }
    
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1
            && errno != EEXIST)
        errExit("mkfifo %s", SERVER_FIFO);
    server_fd = open(SERVER_FIFO, O_RDONLY);
    if (server_fd == -1)
        errExit("open %s", SERVER_FIFO);

    dummy_fd = open(SERVER_FIFO, O_WRONLY);
    if (dummy_fd == -1)
        errExit("open %s", SERVER_FIFO);

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)    errExit("signal");

    for (;;) {
        if (read(server_fd, &req, sizeof(struct request))
                != sizeof(struct request)) {
            fprintf(stderr, "Error reading request; discarding\n");
            continue;
        }

        snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
                (long) req.pid);
        client_fd = open(client_fifo, O_WRONLY);
        if (client_fd == -1) {
            errMsg("open %s", client_fifo);
            continue;
        }

        resp.seq_num = seq_num;
        if (write(client_fd, &resp, sizeof(struct response))
                != sizeof(struct response))
            fprintf(stderr, "Error writing to FIFO %s\n", client_fifo);
        if (close(client_fd) == -1)
            errMsg("close");

        seq_num = update_backup(backup_fd, seq_num + req.seq_len);
        /* seq_num += req.seq_len; */
    }
}
