#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// p[0]:read fd
// p[1]:write fd
#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {

    int fdp2c[2], fdc2p[2], pid;
    char p2c[64], c2p[64];

    pipe(fdp2c);
    pipe(fdc2p);

    if ((pid = fork()) < 0) {
        fprintf(2, "fork error ...\n");
        exit(1);
    }
    // child; read info from parent, then write to parent
    else if (pid == 0) {
        close(fdp2c[WRITE]);
        close(fdc2p[READ]);

        read(fdp2c[READ], p2c, sizeof(p2c));
        close(fdp2c[READ]);

        printf("%d: received %s\n", getpid(), p2c);
        write(fdc2p[WRITE], "pong", 4);
        close(fdc2p[WRITE]);

        exit(0);
    }
    // parent; write info to child, read info from child
    else {
        close(fdp2c[READ]);
        close(fdc2p[WRITE]);

        write(fdp2c[WRITE], "ping", 4);
        close(fdp2c[WRITE]);

        read(fdc2p[READ], c2p, sizeof(c2p));
        close(fdc2p[READ]);

        printf("%d: received %s\n", getpid(), c2p);
        exit(0);
    }
}