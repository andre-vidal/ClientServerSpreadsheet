#include <stdio.h>
#include <sys/types.h>	/* system type defintions */
#include <sys/socket.h>	/* network system functions */
#include <netinet/in.h>	/* protocol & struct definitions */
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <arpa/inet.h>
#include "Assignment1.c"

#define BUF_SIZE	1024
#define LISTEN_PORT	60007

int main(int argc, char *argv[]){
    int			sock_recv;
    struct sockaddr_in	my_addr;
    struct sockaddr_in  remote_addr;
    int			i, x, count = 0;
    fd_set	    readfds,active_fd_set,read_fd_set;
    struct      timeval		timeout={0,0};
    int			incoming_len;
    int			recv_msg_size;
    char	    buf[BUF_SIZE];
    int			select_ret;
    char        **clients = malloc(5*sizeof(char*));
    char        *current;   

    int         send_len,bytes_sent;

    //VARIABLES ADDED TO SERVER FROM ASSIGNMENT ONE//
	
	struct cell grid[COL][ROW],currCell;
	int choice=0,col,row;
    char cellname[BUF_SIZE],input[BUF_SIZE];
	
	Initialize(grid,&currCell,input);

    /*for select cell*/
    struct cell temp;
    //VARIABLES ADDED TO SERVER FROM ASSIGNMENT ONE//

    /* create socket for receiving */
    sock_recv=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_recv < 0){
        printf("socket() failed\n");
        exit(0);
    }

    /* make local address structure */
    memset(&my_addr, 0, sizeof (my_addr));	/* zero out structure */
    my_addr.sin_family = AF_INET;	/* address family */
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* current machine IP */
    my_addr.sin_port = htons((unsigned short)LISTEN_PORT);
    
    /* bind socket to the local address */
    i=bind(sock_recv, (struct sockaddr *) &my_addr, sizeof (my_addr));
    if (i < 0){
        printf("bind() failed\n");
        exit(0);
    }

    FD_ZERO(&readfds);		/* zero out socket set */
    FD_SET(sock_recv,&readfds);	/* add socket to listen to */
    openWorksheet(grid);

    //Instantiate the queue for the clients. Starts with 5 spaces
    for (x = 0; x < 5; x++){
        clients[x] = malloc(16*sizeof(char));
    }

    /* listen ... */
    while (1){
                
        col = ((int)currCell.col)-65;//current column in grid
        row = currCell.row-1;//current row in grid

        system("clear");

        //display the current cell and its contents
        printf("\nCURRENT CELL: \t%c%d\nCONTENT : \t",currCell.col,currCell.row);

        if(grid[col][row].type == 0){printf("%s",grid[col][row].text);}
        if(grid[col][row].type == 1){printf("%d",grid[col][row].value);}

        DisplayGrid(grid);
        DisplayMenu();

        read_fd_set = active_fd_set;
        select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);

        if (select_ret > 0){/* anything arrive on any socket? */
            incoming_len=sizeof(remote_addr);/* who sent to us? */
            recv_msg_size=recvfrom(sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);
            
            if (recv_msg_size > 0){	/* what was sent? */
                buf[recv_msg_size]='\0';                    
                //printf("From %s received: %s\n",inet_ntoa(remote_addr.sin_addr),buf);  
                
                if(current != NULL){
                    current = inet_ntoa(remote_addr.sin_addr);
                    printf("Current set to %s\n", inet_ntoa(remote_addr.sin_addr));
                }
    
                //printf("Current is %d while addr is  %s\n", current, inet_ntoa(remote_addr.sin_addr));                
                
                if(strcmp(current, inet_ntoa(remote_addr.sin_addr)) != 0){
                    if(count < 5){
                        clients[count - 1] = inet_ntoa(remote_addr.sin_addr);
                    }else{
                        //Make Space for one more
                    } 
                    count++;
                    printf("Added to queue : %s at position : %d\n", inet_ntoa(remote_addr.sin_addr), count - 1);
                }else{
                    printf("Accepted\n");
                    printf("From %s received: %s\n",inet_ntoa(remote_addr.sin_addr),buf);
                }                                  
            }
        }

        send_len=strlen("RECIEVED BY SERVER");
        bytes_sent=sendto(sock_recv, "RECIEVED BY SERVER", send_len, 0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));
        
        if(strcmp(buf, "finish")){
            //empty current
            //pop from begining
            //save
        }else if (strcmp(buf,"shutdown") == 0){
            break;
        }

		choice = (int)strtol(buf,(char**)NULL,10);
		switch(choice){
			case 1: //send prompt to client - printf("\nChoose a cell: ");
                    read_fd_set = active_fd_set;
                    select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
                    incoming_len=sizeof(remote_addr);/* who sent to us? */
                    recv_msg_size=recvfrom(sock_recv,cellname,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);/*recieve here*/
                    cellname[recv_msg_size]='\0';/*clear end of string*/
                    
                    send_len=strlen("RECIEVED BY SERVER");
                    bytes_sent=sendto(sock_recv, "RECIEVED BY SERVER", send_len, 0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));
        
                    currCell = *FindCell(grid,toupper(cellname[0]),(int)cellname[1]-48);
                    break;
			case 2: //AcceptInput(grid,currCell.col,currCell.row,input);
                    //send prompt to client - printf("Input Data: ");
                    read_fd_set = active_fd_set;
                    select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
                    incoming_len=sizeof(remote_addr);/* who sent to us? */
                    recv_msg_size=recvfrom(sock_recv,input,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);/*recieve here*/
                    input[recv_msg_size]='\0';/*clear end of string*/
                    
                    send_len=strlen("RECIEVED BY SERVER");
                    bytes_sent=sendto(sock_recv, "RECIEVED BY SERVER", send_len, 0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));
        
                    TestInput(grid,currCell.col,currCell.row,input);//have function to check for the type of input enteredbreak;
			case 3: saveWorksheet(grid);
                    break;
			default : break;
		}
    }

    close(sock_recv);
}
