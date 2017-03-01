#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd[2];
    pid_t pid;
    char buf[20];

    if (pipe(fd) < 0)
    {
        perror("pipe");
    }

    if ((pid = fork()) < 0)
        perror("fork");
    else if (pid > 0)
    {
        close(fd[0]);
        write(fd[1], "hello world\n", 13);
    }
    else
    {
        close(fd[1]);
        read(fd[0], buf, 20);
        printf("%s", buf);
    }

    return 0;
}
