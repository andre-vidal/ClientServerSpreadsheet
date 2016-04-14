/*
Author : Andre Vidal
ID num : 620077449
Assignment : 1 Part 2
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */
#include <arpa/inet.h>


#define BUF_SIZE	2000
#define SERVER_IP	"127.0.0.1"
#define	SERVER_PORT	60010

int main(int argc, char *argv[]){
    int			sock_send;
    struct      sockaddr_in	addr_send;
    char		text[80],buf[BUF_SIZE];
    char        response[BUF_SIZE];
    int			send_len,bytes_sent;
    int         recv_msg_size;


    struct      sockaddr_in server_addr;
    fd_set  readfds,active_fd_set,read_fd_set;
    int         incoming_len;
    int         select_ret;



/* create socket for sending data */
    sock_send=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock_send < 0){
        printf("socket() failed\n");
        exit(0);
    }

    /* fill the address structure for sending data */
    memset(&addr_send, 0, sizeof(addr_send));  /* zero out structure */
    addr_send.sin_family = AF_INET;  /* address family */
    addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr_send.sin_port = htons((unsigned short)SERVER_PORT);

    FD_ZERO(&readfds);      /* zero out socket set */
    FD_SET(sock_send,&readfds); /* add socket to listen to */

    printf("\n<Send \"???\" to see server options>\n<Send \"shutdown\" to shutdown server>\n<Send \"quit\" to close client>\n\n");

    while(1){

        printf(">> ");
        scanf("%s",text);

        strcpy(buf,text);

        if (strcmp(buf,"quit") == 0){
            break;
        
        }else{

            send_len=strlen(text);
            bytes_sent=sendto(sock_send, buf, send_len, 0,(struct sockaddr *) &addr_send, sizeof(addr_send));
              

            //RESPONSE FROM SERVER
            read_fd_set = active_fd_set;
            select_ret=select(sock_send+1,&readfds,NULL,NULL,NULL);


            if (select_ret > 0){/* anything arrive on any socket? */
                incoming_len=sizeof(server_addr);/* who sent to us? */
                recv_msg_size=recvfrom(sock_send,response,BUF_SIZE,0,(struct sockaddr *)&server_addr,&incoming_len);

                if (recv_msg_size > 0){ /* what was sent? */
                    response[recv_msg_size]='\0';
                    printf("\n<Received from Server %s> \n %s\n\n",inet_ntoa(server_addr.sin_addr),response);
                }
            }   
        }   
    }
    close(sock_send);
}