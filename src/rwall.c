#include "rwall.h"

size_t readall(int fd, char *buf, size_t len)
{
    size_t left = len;

    while (left != 0) {
        ssize_t ret = read(fd, buf, left);
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            return len - left;
        }

        left -= ret;
        buf += ret;
    }
    return len;
}

size_t writeall(int fd, const char *buf, size_t len)
{
    size_t left = len;

    while (left != 0) {
        ssize_t ret = write(fd, buf, left);
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            return len - left;
        }

        left -= ret;
        buf += ret;
    }
    return len;
}
