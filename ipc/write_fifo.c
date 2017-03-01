#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    int fd;
    int n, i;
    char buf[1024];
    time_t tp;

    printf("I am %d process.\n", getpid());

    if ((fd = open("fifo1", O_WRONLY)) < 0)
    {
        perror("open fifo1");
        return EXIT_FAILURE;
    }

    for (i = 0; i < 10; i++)
    {
        time(&tp);
        n = sprintf(buf, "Process %d's time is %s", getpid(), ctime(&tp));
        printf("Send message: %s", buf);
        if (write(fd, buf, n+1) < 0) {
            perror("write fifo");
            close(fd);
            exit(1);
        }
        sleep(1);
    }
    close(fd);
    return 0;
}
