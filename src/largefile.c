#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "files.h"

int create_large_file(char *fname, off64_t sz);

int main(int argc, char *argv[])
{
    char *fname;
    off64_t kilobyte = 1024;
    off64_t megabyte = kilobyte * kilobyte;
    off64_t terabyte = megabyte * megabyte;
    off64_t offset = 15 * terabyte;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <new_file_name>\n", argv[0]);
        return 1;
    }
    fname = argv[1];

    if (create_large_file(fname, offset) != -1) {
        off_t fsz = get_filesize(fname);
        printf("Success!\n");
        if (fsz > 0) {
            printf("Created file '%s' with size %ld bytes\n", fname, fsz);
        }
    }
    return 0;
}

int create_large_file(char *fname, off64_t sz)
{
    int retval = -1;

    int fd = open(fname, O_WRONLY | O_CREAT | O_LARGEFILE, 0644);
    if (fd < 1) {
        perror("open");
        goto out;
    }

    if (lseek64(fd, sz, SEEK_END) < 1) {
        perror("lseek");
        goto out;
    }

    if (write(fd, "0", 1) < 1) {
        perror("write");
        goto out;
    }

    retval = 0;
out:
    if (fd > 0)
        close(fd);
    return retval;
}
