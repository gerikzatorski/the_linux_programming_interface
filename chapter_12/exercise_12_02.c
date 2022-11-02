/* exercise_12_02.c */

/*********************************************************************
Write a program that draws a tree showing the hierarchical
parent-child relationships of all processes on the system, going all
the way back to init. For each process, the program should display the
process ID and the command being executed. The output of the program
should be similar to that produced by pstree(1), although it does need
not to be as sophisticated. The parent of each process on the system
can be found by inspecting the PPid: line of all of the
/proc/PID/status files on the system. Be careful to handle the
possibility that a process’s parent (and thus its /proc/PID directory)
disappears during the scan of all /proc/PID directories.
 *********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <limits.h>

#define PROC_DIR_PATH "/proc/"
#define MAX_CMD 128
#define MAX_CHILDREN 128
#define MAX_LINE 128

struct proc_node
{
    char cmd[MAX_CMD];
    pid_t pid;
    pid_t ppid;
    struct proc_node * parent;
    size_t num_children;
    struct proc_node * children[MAX_CHILDREN];
};

static struct proc_node *processes;

/* Similar to Listing 12-1 */

long get_pid_max()
{
    long pid_max;
    char line[MAX_LINE];
    int fd;
    ssize_t n;

    fd = open("/proc/sys/kernel/pid_max", O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    
    n = read(fd, line, MAX_LINE);
    if (n == -1) {
        return -1;
    }

    pid_max = atol(line);
    if (pid_max == 0) {
        return -1;
    }

    return pid_max;
}

void append_proc(const pid_t pid, size_t proc_i)
{
    char filepath[PATH_MAX + 1];
    FILE *fp;
    char *line = NULL;
    size_t num_read;
    pid_t pid_proc, ppid_proc;
    char cmd_proc[MAX_CMD];
    
    sprintf(filepath, "/proc/%d/status", pid);

    fp = fopen(filepath, "r");
    if (fp == 0) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    /* Parse status files defensively */
    
    while (getline(&line, &num_read, fp) != -1) {
        if (strncmp(line, "Name", 4) == 0) {
            sscanf(line, "%*s %s", cmd_proc);
        }
        if (strncmp(line, "Tgid", 4) == 0) {
            sscanf(line, "%*s %d", &pid_proc);
        }
        if (strncmp(line, "PPid", 3) == 0) {
            sscanf(line, "%*s %d", &ppid_proc);
        }
    }

    strcpy(processes[proc_i].cmd, cmd_proc);
    processes[proc_i].pid = pid_proc;
    processes[proc_i].ppid = ppid_proc;
    processes[proc_i].num_children = 0;
    
    if (fclose(fp)) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
    
    free(line);
    
    return;
}

int record_procs()
{
    DIR *dirp;
    struct dirent *dp;
    pid_t pid;
    size_t num_procs;

    dirp = opendir("/proc/");
    if (dirp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    num_procs = 0;
    while (1) {
        errno = 0;
        dp = readdir(dirp);
        if (dp == NULL) {
            break;
        }

        pid = atoi(dp->d_name);
        if (pid == 0) {
            continue;  /* Skip non-numerical subdirs */
        }

        append_proc(pid, num_procs);
        num_procs++;
    }

    if (errno != 0) {
        perror("readdir");
        return -1;
    }
    
    if (closedir(dirp) == -1) {
        perror("closedir");
        return -1;
    }

    return num_procs;
}

void build_tree(size_t n)
{
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (processes[j].pid == processes[i].ppid) {
                processes[j].children[processes[j].num_children] = &processes[i];
                processes[j].num_children++;
            }
        }
    }
    
    return;
}

void print_tree(struct proc_node *root, int level)
{
    int i;
    struct proc_node *node;
        
    for (i = 0; i < level; i++) {
        printf("    ");
    }

    printf("%s, %d, %d\n", root->cmd, root->pid, root->ppid);

    i = 0;
    while (1) {
        node = root->children[i];
        if (node == NULL) {
            break;
        }
        print_tree(node, level + 1);  /* recursion on children */
        i++;
    }

    return;
}

int main(int argc, char *argv[])
{
    long pid_max;
    size_t num_procs;
    int i;

    pid_max = get_pid_max();
    if (pid_max == -1) {
        perror("get_pid_max");
        exit(EXIT_FAILURE);
    }

    processes = malloc(pid_max * sizeof(struct proc_node));

    num_procs = record_procs();
    build_tree(num_procs);

    /* Find the root of all roots (systemd) */
    
    for (i = 0; i < num_procs; i++) {
        if (processes[i].pid == 1) {
            break;
        }
    }
    
    print_tree(&processes[i], 0);

    exit(EXIT_FAILURE);
}
