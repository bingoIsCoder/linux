#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

int main()
{
    int fd;
    int len;
    char buf[1024];

    if (mkfifo("fifo1", 0666) < 0) {
        perror("mkfifo");
    }

    if ((fd = open("fifo1", O_RDONLY)) < 0 && errno != EEXIST)
    {
        perror("open fifo1");
        return EXIT_FAILURE;
    }

    while ((len = read(fd, buf, 1024)) > 0) {
        printf("Read message: %s\n", buf);
    }

    close(fd);
    return 0;
}
