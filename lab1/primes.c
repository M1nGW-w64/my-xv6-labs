#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READ 0
#define WRITE 1

__attribute__((noreturn)) 
void sieveProc(int *left) {
    close(left[WRITE]);

    int temp, right[2];
    // from fd, read num to var
    if (read(left[READ], &temp, sizeof(int)) == 0) {
        close(left[READ]);
        exit(0);
    }

    pipe(right);
    if (fork() == 0) {
        sieveProc(right);
    }
    else {
        close(right[READ]);
        printf("prime %d\n", temp);
        // keep reading num from pipe-file
        int prime = temp;
        while (read(left[READ], &temp, sizeof(int))) {
            if (temp % prime != 0) {
                write(right[WRITE], &temp, sizeof(int));
            }
        }
        close(right[WRITE]);
        wait(0);
        exit(0);
    }
}

void main(int argc, char **argv) {
    int fd_pair[2];
    pipe(fd_pair);
    if (fork() == 0) {
        sieveProc(fd_pair);
    }
    else {
        close(fd_pair[READ]);
        // parent: from fd, write all num to pipe-file
        for (int i = 2; i <= 35; ++i) 
            write(fd_pair[WRITE], &i, sizeof(int));
        close(fd_pair[WRITE]);
        wait(0);
        exit(0);
    }
}