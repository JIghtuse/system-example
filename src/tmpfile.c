#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "rwall.h"

int main(int argc, char *argv[])
{
    int fd = -1;
    int retval = 1;
    const char *buf = "Hello temporary file!";
    size_t buffer_length = strlen(buf);

    if (argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        goto out;
    }

    fd = open(".", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        goto out;
    }
    printf("Temporary file fd: %d\n", fd);

    if (buffer_length != writeall(fd, buf, buffer_length)) {
        perror("writeall");
        goto out;
    }

    retval = 0;
out:
    if (fd > 0)
        close(fd);
    return retval;
}
