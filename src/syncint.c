#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

enum {
    INC_TO = 1000000
};

int global_int;

pid_t gettid(void)
{
    return syscall(SYS_gettid);
}

void *incrementer(void *arg)
{
    int i;
    int proc_num = *(int *)arg;
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(proc_num, &set);

    if (sched_setaffinity(gettid(), sizeof(cpu_set_t), &set)) {
        perror("sched_setaffinity");
        return NULL;
    }

    for (i = 0; i < INC_TO; i++) {
        global_int++;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    long nproc = 0;
    long i;
    pthread_t *threads;

    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }
    
    nproc = sysconf(_SC_NPROCESSORS_ONLN);
    if (nproc < 0) {
        perror("sysconf");
        return 1;
    }

    threads = malloc(sizeof(pthread_t) * nproc);
    if (threads == NULL) {
        perror("malloc");
        return 1;
    }

    printf("Starting %ld threads...\n", nproc);

    for (i = 0; i < nproc; i++) {
        if (pthread_create(&threads[i], NULL, incrementer, (void *)&i)) {
            perror("pthread_create");
            nproc = i;
            break;
        }
    }

    for (i = 0; i < nproc; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);

    printf("global_int actual value:   %d\n", global_int);
    printf("global_int expected value: %d\n", (int)(INC_TO * nproc));
    return 0;
}
