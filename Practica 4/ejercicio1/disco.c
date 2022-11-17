#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

int current_in = 0;

pthread_mutex_t disco, ticket_Vips, ticket_NoVips;
pthread_t tID[100];

void enter_normal_client(int id)
{
	while (current_in >= CAPACITY && tId_Vips[0] == NULL);
	pthread_mutex_lock(&disco);
	pthread_mutex_lock(&ticket_NoVips);
	current_in++;
	pthread_mutex_unlock(&disco);
}

void enter_vip_client(int id)
{
	while (current_in >= CAPACITY);
	pthread_mutex_lock(&disco);
	pthread_mutex_lock(&ticket_Vips);
	current_in++;
	pthread_mutex_unlock(&disco);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{	
	pthread_mutex_lock(&disco);
	current_in--;


	if(isvip)
		pthread_mutex_unlock(&ticket_Vips);
	else
		pthread_mutex_unlock(&ticket_NoVips);

	pthread_mutex_unlock(&disco);
}

void *client(void *arg)
{
	pid_t p_ID =  (struct)atoi(*arg).id;// arg[0];
	int* isVip = (struct)atoi(*arg).vip;	//arg[1]

	if (isVip){
		enter_vip_client(p_ID);
	}
	else{
		enter_normal_client(p_ID);
	}

	dance(p_ID, isVip);
	exit_client(p_ID, isVip);
}

int main(int argc, char *argv[])
{
	FILE* file = NULL;
	char c;
	int clients = 0;

	if (pthread_mutex_init(&disco, NULL) != 0 || pthread_mutex_init(&ticket_Vips, NULL) != 0 || pthread_mutex_init(&ticket_NoVips, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	fscanf(file, "%d", clients);
	for(int i = 0; i < clients; ++i){
		int vip;
		fscanf(file, "%d", vip);
		//Crear un struct con el valor del pid y si es vip
		pthread_create(&(tID[i]), NULL, &client, struct);
	}

	fclose(file);

	return 0;
}
