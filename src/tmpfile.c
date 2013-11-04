#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "rwall.h"

#ifndef __O_TMPFILE
#define __O_TMPFILE	020000000
#endif
#define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#define O_TMPFILE_MASK (__O_TMPFILE | O_DIRECTORY | O_CREAT)      


int main(int argc, char *argv[])
{
    int fd;
    const char *buf = "Hello temporary file!";
    size_t buffer_length = strlen(buf);

    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    fd = open(".", O_TMPFILE | O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    printf("Temporary file fd: %d\n", fd);

    if (buffer_length != writeall(fd, buf, buffer_length)) {
        perror("writeall");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
