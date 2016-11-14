#include <stddef.h>

typedef int ssize_t;

void close(int fd)
{
}

ssize_t read(int fd, void *buf, size_t count)
{
    return -9; /* EBADF */
}

char *getcwd(char *buf)
{
    return NULL;
}