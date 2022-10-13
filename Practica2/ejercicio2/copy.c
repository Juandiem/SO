#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void copy(int fdo, int fdd)
{
	char c[64];
	int count;
	/* Read file byte by byte */
	while (count = read(fdo, c, 64)) {
		printf("ciao");
		/* Print byte to stdout */
		if (!write(fdd, c, count)){
			close(fdo);close(fdd);
			err(3,"fwrite failed!!");
			}
		}

	close(fdo);close(fdd);
}



int main(int argc, char *argv[])
{
	int fdo, fdd;


	if (argc!=3) {
		fprintf(stderr,"Usage: %s <source_filename> <destfile_name>\n",argv[0]);
		exit(1);
	}

	if((fdo = open(argv[1], O_RDONLY, 0444)) && (fdd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0444)))
		copy(fdo,fdd);
	else
		err(2,"The input file %s could not be opened",argv[1]);


	return 0;
}
