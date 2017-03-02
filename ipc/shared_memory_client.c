#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

struct msg_form {
    long mtype;
    char mtext;
};

union semun {
    int             val;
    struct semid_ds *buf;
    unsigned short  *array;
};

int sem_p(int sem_id)
{
    struct sembuf sbuf;
    sbuf.sem_num = 0;
    sbuf.sem_op = -1;
    sbuf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sbuf, 1) == -1)
    {
        perror("p operation error");
        return -1;
    }
    return 0;
}

int sem_v(int sem_id)
{
    struct sembuf sbuf;
    sbuf.sem_num = 0;
    sbuf.sem_op = 1;
    sbuf.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sbuf, 1) == -1)
    {
        perror("v operation error");
        return -1;
    }
    return 0;

}

int main()
{
    int semid, shmid, msqid;
    key_t key;
    char *shm;
    struct msg_form msg;
    int flag = 1;

    if ((key = ftok(".", 'z')) < 0)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    if ((shmid = shmget(key, 1024,0)) == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    shm = (char*)shmat(shmid, 0, 0);
    if ((int)shm == -1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    if ((msqid = msgget(key, 0)) == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if ((semid = semget(key, 0, 0)) == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    printf("*************************************\n");
    printf("*               IPC                 *\n");
    printf("*   Input r to send data to server. *\n");
    printf("*   input q to quit.                *\n");
    printf("*************************************\n");

    while (flag)
    {
        char c;
        printf("Please input command: ");
        fflush(stdout);
        scanf("%c", &c);
        int rd_nu;
        switch(c)
        {
            case 'r':
                printf("Data to send: ");
                fflush(stdout);
                sem_p(semid);
                rd_nu = read(STDIN_FILENO, shm, 1024);
                shm[rd_nu - 1] = 0;
                sem_v(semid);
                while ((c = getchar()) != '\n' && c!= EOF);
                msg.mtype = 888;
                msg.mtext = 'r';
                msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
                break;
            case 'q':
                msg.mtype = 888;
                msg.mtext = 'q';
                msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
                flag = 0;
                break;
            default:
                printf("Wrong in put!\n");
                while ((c = getchar()) != '\n' && c!= EOF);
        }
    }
    shmdt(shm);
    return 0;
}

