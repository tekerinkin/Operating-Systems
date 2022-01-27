#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int create_ancestor(int *pipefd)
{
    pid_t cpid;

    cpid = fork();

    if(cpid < 0){
        fprintf(stderr, "fork failure\n");
        return 1;
    }else if(cpid == 0){
        close(pipefd[0]);
        write(pipefd[1], "hello there", 11);
        close(pipefd[1]);
        wait(NULL);
        return 0;
    }
    return 0;
}


int main(int argc, char *argv[]){
    int pipefd[2];
    char buf;
    pid_t cpid1;
    
    if(pipe(pipefd) == -1){
        fprintf(stderr, "pipe failure\n");
        exit(1);
    }

    cpid1 = fork();

    if(cpid1 < 0){
        fprintf(stderr, "fork failure\n");
    }else if(cpid1 == 0) {
        close(pipefd[1]);

        while(read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        exit(1);
    }else{
        if(create_ancestor(&pipefd[0]) == 1)
            exit(1);
        else{
            wait(NULL);
            exit(1);
        }
    }

    return 0;

}
