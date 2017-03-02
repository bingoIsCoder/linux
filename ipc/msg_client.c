#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#define MSG_FILE "/etc/passwd"

struct msg_form {
    long mtype;
    char mtext[256];
};

int main ()
{
    int msqid;
    key_t key;
    struct msg_form msg;

    if ((key = ftok(MSG_FILE, 'z')) < 0)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    printf("Message Queue - client key is: %d.\n", key);

    if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("My msqid is: %d\n", msqid);
    printf("My pid is: %d\n", getpid());

    msg.mtype = 888;
    sprintf(msg.mtext, "hello, I'm client %d", getpid());
    msgsnd(msqid, &msg, sizeof(msg.mtext), 0);

    msgrcv(msqid, &msg, 256, 999, 0);
    printf("Client receive msg.mtext is: %s.\n", msg.mtext);
    printf("Client receive msg.mtype is: %d.\n", msg.mtype);
    return 0;
}
