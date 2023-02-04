#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

/** move oldfd to newfd */
static void redirect(int oldfd, int newfd) {
  if (oldfd != newfd) {
    if (dup2(oldfd, newfd) != -1)
      close(oldfd); /* successfully redirected */
    else
      perror("dup2");
  }
}

std::string ExecFile(std::string name, std::string data,std::string argument = ""){
	int fd_stdin[2], fd_stdout[2]; /* stdin/stdout pipes */
    pid_t cpid;

   if (pipe(fd_stdin) == -1 || pipe(fd_stdout) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    /* Child reads from pipe */
        close(fd_stdin[1]); /* Close unused write end */
		close(fd_stdout[0]); /* Close unused read end */

		redirect(fd_stdin[0], STDIN_FILENO);   /* redirect stdin */	
		redirect(fd_stdout[1], STDOUT_FILENO); /* redirect stdout */

        char *args[]={name.data(),argument.data(), NULL};//A null terminated array of character pointers
		execvp(args[0],args);

        _exit(EXIT_SUCCESS);

    } else {            /* Parent writes argv[1] to pipe */
        close(fd_stdin[0]); /* Close unused read end */
		close(fd_stdout[1]); /* Close unused write end */
		
		{ /* send some input to the child */
		  write(fd_stdin[1], data.data(), data.size());
		  close(fd_stdin[1]);
		} 
        
        wait(NULL); /* Wait for child */
        
        /* read it back */
	   char readbuf[512] = {0};
	   ssize_t n = read(fd_stdout[0], readbuf, sizeof readbuf);
	   close(fd_stdout[0]);
	   //printf("got [%.*s]\n", (int)n, readbuf);

        return std::string (readbuf);
    }
	std::cout<<"programm end"<<std::endl;
}
