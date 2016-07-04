#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

pid_t read_pid(const char *pid_file)
{
    FILE *fp = fopen(pid_file, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    pid_t p;
    if (1 != fscanf(fp, "%d", &p)) {
        fprintf(stderr, "%s has no pid value\n", pid_file);
        exit(EXIT_FAILURE);
    }
    return p;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "usage: %s <pid_file> [signal_number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int signo = SIGTERM;
    if (argc == 3) {
        const char *sig_str = argv[2];
        signo = atoi(sig_str);
        if (0 == signo) {
            fprintf(stderr, "Invalid signal number: %s", sig_str);
            exit(EXIT_FAILURE);
        }
    }
    pid_t pid = read_pid(argv[1]);
    kill(pid, signo);
    exit(EXIT_SUCCESS);
}
