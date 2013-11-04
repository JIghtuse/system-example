#ifndef RWALL_H
#define RWALL_H

#include <errno.h>
#include <unistd.h>

size_t readall(int fd, char *buf, size_t len);
size_t writeall(int fd, const char *buf, size_t len);

#endif /* RWALL_H */
