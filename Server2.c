/*
Author : Andre Vidal
ID num : 620077449
Assignment : 1 Part 2
*/

#include <stdio.h>
#include <sys/types.h>	/* system type defintions */
#include <sys/socket.h>	/* network system functions */
#include <netinet/in.h>	/* protocol & struct definitions */
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <arpa/inet.h>
#include "Assignment1.c"

#define BUF_SIZE	2000
#define LISTEN_PORT	60010

int main(int argc, char *argv[]){
    int			sock_recv;
    struct sockaddr_in	my_addr;
    struct sockaddr_in  remote_addr;
    int			i;
    fd_set	    readfds,active_fd_set,read_fd_set;
    struct      timeval		timeout={0,0};
    int			incoming_len;
    int			recv_msg_size;
    char	    buf[BUF_SIZE],response[BUF_SIZE],curr[BUF_SIZE];
    int			select_ret;

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
    /* listen ... */
    while (1){
                
        col = ((int)currCell.col)-65;//current column in grid
        row = currCell.row-1;//current row in grid
/*
        system("clear");

        //display the current cell and its contents
        
        DisplayGrid(grid);
        DisplayMenu();
*/

        sprintf(curr,"\nCURRENT CELL: \t%c%d\nCONTENT : \t",currCell.col,currCell.row);

        if(grid[col][row].type == 0){sprintf(curr,"%s%s",curr,grid[col][row].text);}
        if(grid[col][row].type == 1){sprintf(curr,"%s%d",curr,grid[col][row].value);}

        read_fd_set = active_fd_set;
        select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);

        if (select_ret > 0){/* anything arrive on any socket? */        
            
            incoming_len=sizeof(remote_addr);/* who sent to us? */
            recv_msg_size=recvfrom(sock_recv,buf,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);

            if (recv_msg_size > 0){	/* what was sent? */
                buf[recv_msg_size]='\0';
                printf("<Message from Client %s> %s\n",inet_ntoa(remote_addr.sin_addr),buf);
            }
        }


        choice = (int)strtol(buf,(char**)NULL,10);

        if(strcmp(buf,"???")==0){
           //printf("Sending Options to %s",inet_ntoa(remote_addr.sin_addr));
            strcpy(response,"\n<--SERVER OPTIONS-->\n[1] Select Cell\n[2] Input Data\n[3] Display Sheet\n[4] Save Sheet");
        }else if(strcmp(buf,"shutdown")==0){
            //printf("Sending Shutdown message to %s",inet_ntoa(remote_addr.sin_addr));
            strcpy(response,"...SERVER SHUTTING DOWN...");
        }else{
            switch(choice){
                case 1:strcpy(response,"Enter cell name [eg. B4]:");
                       //printf("Sending \"Select cell\" prompt to %s",inet_ntoa(remote_addr.sin_addr));break;
                case 2:strcpy(response,"[RAW INPUT - just type data]\n [KEYWORDS - SUM , AVG , RNG]\n [FORMATTING - eg. SUM(A1:A4)]\n Enter data: ");
                       //printf("Sending \"Input Data\" prompt to %s",inet_ntoa(remote_addr.sin_addr));break;
                case 3:strcpy(response,stringGrid(grid));
                       strcat(response,curr);break;
                case 4:strcpy(response,"...SHEET SAVED...");
                       //printf("Sending \"Saved Sheet\" Notification to %s",inet_ntoa(remote_addr.sin_addr));break;
                default:break;
            }
        } 

        send_len=strlen(response);
        bytes_sent=sendto(sock_recv,response, send_len, 0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));          
        
		switch(choice){
			case 1: //Receive cell name from client
                    read_fd_set = active_fd_set;
                    select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
                    incoming_len=sizeof(remote_addr);/* who sent to us? */
                    recv_msg_size=recvfrom(sock_recv,cellname,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);/*recieve here*/
                    cellname[recv_msg_size]='\0';/*clear end of string*/
                    printf("<Message from Client %s> %s\n",inet_ntoa(remote_addr.sin_addr),cellname);

                    //Select the cell
                    currCell = *FindCell(grid,toupper(cellname[0]),(int)cellname[1]-48);

                    //send message back to client
                    strcat(cellname," Selected");
                    send_len=strlen(cellname);
                    bytes_sent=sendto(sock_recv, cellname, send_len, 0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));
        
                    break;

			case 2: 
                    read_fd_set = active_fd_set;
                    select_ret=select(sock_recv+1,&readfds,NULL,NULL,NULL);
                    incoming_len=sizeof(remote_addr);/* who sent to us? */
                    recv_msg_size=recvfrom(sock_recv,input,BUF_SIZE,0,(struct sockaddr *)&remote_addr,&incoming_len);/*recieve here*/
                    input[recv_msg_size]='\0';/*clear end of string*/
                    printf("<Message from Client %s> %s\n",inet_ntoa(remote_addr.sin_addr),input);
                    //accept input into cell                
                    TestInput(grid,currCell.col,currCell.row,input);

                    //send message back to client
                    strcat(input," Accepted");
                    send_len=strlen(input);
                    bytes_sent=sendto(sock_recv, input, send_len, 0,(struct sockaddr *) &remote_addr, sizeof(remote_addr));
        
                    break;

			case 3: //printf("%s",stringGrid(grid));
                    break;
            case 4: saveWorksheet(grid);
                    break;
			default : break;
		}

        if (strcmp(buf,"shutdown") == 0){

            break;
        }
    }

    close(sock_recv);
}