#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */


int main(int argc, char **argv)
{
	pid_t p_ID;
	pid_t child_ID;

	if (argc!=1) {
		fprintf(stderr,"Usage: %s <exe_name> <newExe_name> <chuakala>\n",argv[0]);
		exit(1);
	}

	p_ID = getpid();
	printf("proc. PID = %d comienza \n", p_ID);
	child_ID = fork();
	printf("proc. PID = %d, pidC = %d \n", p_ID, child_ID);


	if(child_ID > 0){
		p_ID = getpid();
		printf("proc. PID = %d sigue \n", p_ID);
		child_ID = fork();
		printf("proc. PID = %d, pidC = %d \n", p_ID, child_ID);
	}
	
	if(child_ID > 0){
		const char *args_[] = {NULL};
		execvp(argv[1], args_);
		waitpid(getppid(),-1, 0);
	}

	return 0;
}
