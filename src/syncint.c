#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

enum {
    INC_TO = 10000000
};

int global_int;

#if defined _USE_MUTEX
pthread_mutex_t mutex;
#elif defined _USE_SPIN
pthread_spinlock_t spinlock;
#endif

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
#if defined _USE_ATOMIC
        __sync_fetch_and_add(&global_int, 1);
#elif defined _USE_GTM 
        __transaction_atomic {
            global_int++;
        }
#elif defined _USE_MUTEX
        pthread_mutex_lock(&mutex);
        global_int++;
        pthread_mutex_unlock(&mutex);
#elif defined _USE_SPIN
        pthread_spin_lock(&spinlock);
        global_int++;
        pthread_spin_unlock(&spinlock);
#else
        global_int++;
#endif
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

#if defined _USE_MUTEX
    pthread_mutex_init(&mutex, NULL);
#elif defined _USE_SPIN
    pthread_spin_init(&spinlock, 0);
#endif

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

#if defined _USE_MUTEX
    pthread_mutex_destroy(&mutex);
#elif defined _USE_SPIN
    pthread_spin_destroy(&spinlock);
#endif

    return 0;
}
