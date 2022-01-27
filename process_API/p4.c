#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char buf;

    if(pipe(pipefd) == -1){
        fprintf(stderr, "pipe failure\n");
        exit(1);
    }

    cpid = fork();

    if(cpid < 0)
    {
        fprintf(stderr, "fork failure\n");
        exit(1);
    }else if(cpid == 0){
        close(pipefd[1]);
        while(read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        exit(1);
    }else{
        close(pipefd[0]);
        write(pipefd[1], "hello there", 11);
        close(pipefd[1]);
        wait(NULL);
        exit(1);
    }
    return 0;
}
