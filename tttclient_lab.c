#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
	int socket_id;
	struct sockaddr_in server_address;
	int i=0,j=0,k=0,c=0,m=0,r=0,col=0,dc=0,idc=0;
	//int i=0,j=0,k=0;
	char buffer[100];int bytes_recv, row , column;
	int position=0;
	int flag=0;
	char board_size_buffer;
	char msg, coordinates_buffer[2],choice[]="X",other_choice[]="Y",sign;
	int b_size=3; 
	if((socket_id=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("Error\n");
		exit(0);
	}
	
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_address.sin_port=htons(5003);

	if((connect(socket_id,(struct sockaddr*)&server_address, sizeof(server_address)))<0)
	{
		printf("unable to connect\n");
		exit(0);
	}
		bytes_recv=recv(socket_id,&board_size_buffer,sizeof(board_size_buffer),0);
	if(bytes_recv == -1)
	{
		printf("\nCould not fetch size of board....Trying Again...\n");
		return 1;
	}
	printf("\n!!...Welcome to Tic Tac Toe...!!\n");
	b_size = board_size_buffer-'0';
	printf("\nYou will play in board size %d x %d :\n", b_size, b_size);
	bytes_recv=recv(socket_id,choice,sizeof(choice),0);
	if(bytes_recv == -1)
	{
		printf("\n Could not fetch choice\n ");
		return 1;
	}
	if(strcmp(choice,"X")==0)
	{
		sign='X';
		strcpy(other_choice,"Y");
		printf("\nYou are Player 1\n");
		printf("\nYou will play with 'X' and Player 2 will play with 'Y'\n");
	}
	else
	{
		sign='Y';
		strcpy(other_choice,"X");
		printf("\nYou are Player 2\n");
		printf("\nYou will play with 'Y' and Player 1 will play with 'X'\n");
	}
	
	for(i=0;i<100;++i)buffer[i]='\0';
	while(strcmp(choice,buffer)!=0)
	{	
		//system("clear");
		recv(socket_id,buffer,100,0);
		//printf("\nrecived %s\n",buffer);
		if(strcmp(buffer,other_choice)==0)
		{	printf("\n....You Lost....\n");
			strcpy(buffer,choice);
			
			send(socket_id, buffer,strlen(buffer)+1,0);
			break;
		}
		//system("clear");
		for(j=0;j<b_size;++j)
		{
			printf("|");
			for(k=0;k<b_size;++k)
			{
				printf("\t%c\t|",buffer[j*b_size+k]);
			}
			printf("\n");
		}
		
		
		printf("\n available positions are as follows :- ");
	
		for(j=0;j<(b_size*b_size);++j)
		{
			if(buffer[j]=='0')
			{
				flag=1;
				printf("%d,",j+1);
			}
		}
		if(flag==0)
		{
			printf("\ndraw\n");
			strcpy(buffer,"-1");
			break;
		}
		flag=0;
		printf("\nenter the position - ");
		scanf("%d",&position);
		while(buffer[position-1]!='0')
		{
			printf("\nenter valid position - ");
			scanf("%d",&position);
		}
		buffer[position-1]=sign;
		row = (position-1)/b_size;
		column = (position-1)%b_size;
		//send(socket_id, buffer,strlen(buffer)+1,0);
		/*memset(coordinates_buffer,0,sizeof(coordinates_buffer));
		coordinates_buffer[0]=row+'0';
		coordinates_buffer[1]=column+'0';
		send(socket_id, &coordinates_buffer,sizeof(coordinates_buffer),0);
		*/
	//	recv(socket_id,buffer,100,0);
		for(j=0;j<b_size;++j)
		{
			printf("|");
			for(k=0;k<b_size;++k)
			{
				printf("\t%c\t|",buffer[j*b_size+k]);
			}
			printf("\n");
		}
		
		c=row*b_size;
		r=0;col=0;dc=0;idc=0;
		//checking columns
		for(i=0;i<b_size-1;i++)
		{
			if(buffer[column]==buffer[column+b_size] && buffer[column]!='0')
			{
				col++;
				column+=b_size;
			}
			else
				break;
			
		}
		//checking rows
		for(i=0;i<b_size-1;i++)
		{
			if(buffer[c]==buffer[c+1] && buffer[c]!='0')
			{
				r++;
				c+=1;
			}
			else
				break;
			
		}
		//checking diagonal
		if(row==column)
		{
			j=0;
			for(i=0;i<b_size-1;i++)
			{
				if(buffer[j]==buffer[j+b_size+1] && buffer[j]!='0')
				{
					dc++;
					j+=b_size+1;
				}
				else
					break;
				
			}
		}
		//checking other diagonal
		if((row+column)==b_size-1)
		{
			k=b_size-1;
			for(i=0;i<b_size-1;i++)
			{
				if(buffer[k]==buffer[k+b_size-1] && buffer[k]!='0')
				{
					idc++;
					k+=b_size-1;
				}
				else
					break;
			}
		}
		if(col == b_size-1 || r == b_size-1 || dc == b_size-1 || idc == b_size-1)
		{
			//msg='1';
			//send(socket_id,&msg,sizeof(msg),0);
			//recv(socket_id,&msg,sizeof(msg),0);
			//if (msg=='1')
			//{
			printf("\n...You won...\n");
			strcpy(buffer,choice);
			//printf("\nsend %s\n",buffer);
			//strcpy(winner,choice);
			send(socket_id, buffer,strlen(buffer)+1,0);
			//send(socket_id,winner,sizeof(winner),0);
			continue;
		}
		//printf("\nsend %s\n",buffer);
		//print("\n%s\n",buffer);
		//checking for draw
		send(socket_id, buffer,strlen(buffer)+1,0);
		//recv(socket_id,&winner,sizeof(winner),0);
		printf("\n Wait for your turn\n");
		for(m=0;m<b_size*b_size;++m)
		{
			if(buffer[m]=='0')
			{
				flag=1;
			}
		}
		if(flag==0)
		{
			printf("\ndraw\n");
			strcpy(buffer,choice);
            strcpy(buffer,"Draw");
            send(socket_id, buffer,strlen(buffer)+1,0);
			break;
		}
		flag=0;
	}

	close(socket_id);
	return 0;
}
