/* NAME: ARPAN SAHA ; ROLL NO: 17CS8013
   NAME: SURAJ KUMAR MODI ; ROLL NO: 17CS8020 
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include<semaphore.h>
#include<signal.h>

int main()
{
	int socket_id,newsocket_id,client_address_length,bytes_sent;
	struct sockaddr_in server_address,client_address;
	pid_t newserver;
	int flag=0,b_size=0,flag_player=0;
	int decieder=-1;
	int personal_player_no=0;
	int i=0,x=0,j=0;
	char board_size_buffer,choice1[]="X",choice2[]="Y";

	printf("\n!!.. Welcome to tic-tac-toe..!! \n");                
	int segment_id1 = shmget(IPC_PRIVATE, sizeof(char)*100, S_IRUSR | S_IWUSR);   // Memory Allocations in buffer (Shared Memory)
	char *buffer = (char*)shmat(segment_id1, NULL, 0);
	int segment_id2 = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR | S_IWUSR);   // Memory Allocations in buffer (Shared Memory) 
	int *player_no = (int*)shmat(segment_id2, NULL, 0);
	
	
	int seg1 =shmget(IPC_PRIVATE,sizeof(sem_t),S_IRUSR|S_IWUSR);    // Memory Allocations in buffer (Shared Memory)
	sem_t *tt1 = (sem_t *)shmat(seg1,NULL,0);       // Semaphore declarartion
	sem_init(tt1,1,1);               // Semaphore Initialization
	int seg2 =shmget(IPC_PRIVATE,sizeof(sem_t),S_IRUSR|S_IWUSR);     // Memory Allocations in buffer (Shared Memory)
	sem_t *tt2 = (sem_t *)shmat(seg2,NULL,0);  // Semaphore declarartion
	sem_init(tt2,1,1);              // Semaphore Initialization
	
	
	*player_no=0;
	
	if((socket_id=socket(AF_INET,SOCK_STREAM,0))<0)            //Socket Creation
	{
		printf("socket creation Error\n");
		exit(0);
	}
	
	server_address.sin_family=AF_INET;                  // Address family of server
	server_address.sin_addr.s_addr=INADDR_ANY;              // Address of Server
	server_address.sin_port=htons(5003);                // Port on which server is listening
	if((bind(socket_id,(struct sockaddr*)&server_address, sizeof(server_address)))<0)       //binding of server to the socket
	{
		printf("unable to bind \n");
		exit(0);
	}
	
	sem_wait(tt1);
	sem_wait(tt2);
	listen(socket_id,2);
	printf("\nEnter the size of the board....\n");
	printf("\n For example, enter 4 for 4x4 board....!!!\n");
	scanf("%d",&b_size);
	printf("\n Waiting for Players to join....\n");
	while(1)
	{
		client_address_length=sizeof(client_address);
		newsocket_id=accept(socket_id,(struct sockaddr*)&client_address,&client_address_length);                //Accept of connection from a client
		for(i=0;i<b_size*b_size;++i)
		{
			buffer[i]='0';
		}
		*player_no=(*player_no)+1;
		if(newsocket_id<0)
		{
			printf("accept error\n");
			exit(0);
		}
		personal_player_no=*player_no;
		printf("\nPlayer %d joined\n",*player_no);
		board_size_buffer=b_size+'0';
        do{
        	bytes_sent=send(newsocket_id,&board_size_buffer,sizeof(board_size_buffer),0);
        	if(bytes_sent == -1)
        	{
        		printf("\nCould not send Board size...Trying Again..!!\n");
        	}
        }while(bytes_sent == -1);
		if(*player_no==1 || *player_no & 1)
		{
			send(newsocket_id,choice1,sizeof(choice1),0);
		}
		if(*player_no==2 || !(*player_no & 1))
            {
            send(newsocket_id,choice2,sizeof(choice2),0);
            printf("\n....Both Players have joined...Beginning Match...\n");
			sem_post(tt1);
            }
		if(fork()==0)
		{	
			
			if(strcmp(buffer,"-1")==0)
			{
				label:
                //buffer[9]='k';
				//send(newsocket_id,buffer,strlen(buffer)+1,0);
			    *player_no=(*player_no)-1;                          // Deallocations of all allocation memory and semaphore variables.
				x=close(newsocket_id);
				sem_post(tt1);                                      // " " " " "
				sem_post(tt2);
				shmdt(buffer);
				shmdt(player_no);
				shmdt(tt1);
				shmdt(tt2);
				kill(0,SIGINT);
				exit(0);
			}
			
			if(personal_player_no==1)
			{
				while(1)
				{
					                            
					sem_wait(tt1);                      // Decrease value of Semaphore
					//printf("hello\n");
					send(newsocket_id,buffer,strlen(buffer)+1,0);       // buffer send to server
					//printf("\nsend %s\n",buffer);		
                    recv(newsocket_id,buffer,100,0);            // buffer received from server
					//printf("\nrecieve %s\n",buffer);
                    counter1++;	
                    if(strlen(buffer)==1)
                    {
                            if(flag_player==0)                      // checker algorithm for getting which player won
                            {
                              printf("Game Over player 1 won\n");
                             flag=1;
                             goto label;
                            }
                    }
                    if(strlen(buffer)==4)
                    {
                        printf("Draw\n");
                        goto label;
                    }
					sem_post(tt2);                      // increasing value of Semaphore
				}
			}
			else
			{
				while(1)
				{
					
					sem_wait(tt2);          // Decrease value of Semaphore
					
					send(newsocket_id,buffer,strlen(buffer)+1,0);   // buffer send to server
					//printf("\nsend %s\n",buffer);
					recv(newsocket_id,buffer,100,0);             // buffer received from server
					//printf("\nrecieve %s\n",buffer);
                    counter2++;
                    if(strlen(buffer)==1)
                    {
                            if(flag_player==0)              // checker algorithm for getting which player won
                            {
                              printf("Game Over player 2 won\n");
                             flag=1;
                              goto label;
                            }
                    }
                    if(strlen(buffer)==4)
                    {
                        printf("Draw\n");
                        goto label;
                    }
					sem_post(tt1);      // increasing value of Semaphore
				}

			}
			//goto label;
		}
		close(newsocket_id);   // Closing of connection 
	}
	shmdt(buffer);
	shmdt(player_no);   // Deallocations of all allocation memory and semaphore variables.
	shmdt(tt1);            // " " " "
	shmdt(tt2);         
	sem_destroy(tt1);
	sem_destroy(tt2);
	shmctl(segment_id1, IPC_RMID, NULL);
	shmctl(seg1, IPC_RMID, NULL);
	shmctl(segment_id2, IPC_RMID, NULL);
	shmctl(seg2, IPC_RMID, NULL);
	return 0;
}
