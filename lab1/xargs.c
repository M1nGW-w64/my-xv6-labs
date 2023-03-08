#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MAX_ARGS_LEN 1024

int main(int argc, char *argv[]) {
    int buf_index = 0;
    char buf, stdin_args[MAX_ARGS_LEN], *xargs_args[MAXARG];
    
    if (argc < 2) {
        fprintf(2, "usage: xargs <command>.\n");
        exit(1);
    }

    // read xargs's parameter
    for (int i = 1; i < argc; i++)
        xargs_args[i - 1] = argv[i];

    // read parameter from stdin
    while (read(0, &buf, sizeof(buf))) {
        if (buf == '\n') {
            stdin_args[buf_index] = 0;

            // child
            if (fork() == 0) {
                xargs_args[argc - 1] = stdin_args;
                xargs_args[argc] = 0;
                exec(xargs_args[0], xargs_args);
            }
            // parent
            else {
                wait(0); 
                buf_index = 0;
            }
        }
        else stdin_args[buf_index++] = buf;
    }
    exit(0);
}