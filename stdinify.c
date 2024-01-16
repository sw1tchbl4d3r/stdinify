#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int TTY_FD = -1;

int (*original_close)(int);
int (*original_open)(const char*, int, mode_t);
ssize_t (*original_read)(int, void*, size_t);

int open(const char* path, int oflag, mode_t mode) {
    if (!original_open) {
        original_open = dlsym(RTLD_NEXT, "open");
        if (!original_open)
            fprintf(stderr, "Something bad happened in dlsym: %s\n", dlerror());
    }

    int fd = original_open(path, oflag, mode);

    if (fd < 0)
        return fd;

    if (strcmp(path, "/dev/tty") == 0)
        TTY_FD = fd;

    return fd;
}

int close(int fd) {
    if (!original_close) {
        original_close = dlsym(RTLD_NEXT, "close");
        if (!original_close)
            fprintf(stderr, "Something bad happened in dlsym: %s\n", dlerror());
    }

    if (fd == TTY_FD)
        TTY_FD = -1;

    return original_close(fd);
}

ssize_t read(int fildes, void *buf, size_t nbyte) {
    if (!original_read) {
        original_read = dlsym(RTLD_NEXT, "read");
        if (!original_read)
            fprintf(stderr, "Something bad happened in dlsym: %s\n", dlerror());
    }

    if (TTY_FD == -1 || fildes != TTY_FD)
        return original_read(fildes, buf, nbyte);

    return original_read(0, buf, nbyte);
}
