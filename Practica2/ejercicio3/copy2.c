#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void copy_regular(char *orig, char *dest)
{
	char c;
	/* Read file byte by byte */
	while (fread(&c, 1 , 1,orig)) {
		/* Print byte to dest */
		if (!fwrite(&c,1,1, dest)){
			fclose(orig);fclose(dest);
			err(3,"fwrite failed!!");
		}
	}

	fclose(orig);fclose(dest);
}

void copy_link(char *orig, char *dest)
{
	struct stat file;
	lstat(orig, &file);
	int count = file.st_size;
	char c [count + 1];
	readlink(orig, c, count - 1);
	c[count]='\0';
	symlink(c,dest);
}


int main(int argc, char *argv[])
{
	struct stat file;


	if (argc!=3) {
		fprintf(stderr,"Usage: %s <source_filename> <destfile_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if (fopen(argv[1], "r") != NULL){
		 stat(argv[1], &file);
		switch(file.st_mode & S_IFMT){
			case S_IFREG:
				copy_regular(argv[1],argv[2]);
				break;

			case S_IFLNK:
				copy_link(argv[1],argv[2]);
				break;

			default:
			err(2,"The input file %s could not be copied",argv[1]);
				break;
		}
	}
		
	else
		err(2,"The input file %s could not be opened",argv[1]);


	return 0;
}
