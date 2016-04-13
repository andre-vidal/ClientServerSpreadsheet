#include <stdio.h>
#include <sys/types.h>	/* system type defintions */
#include <sys/socket.h>	/* network system functions */
#include <netinet/in.h>	/* protocol & struct definitions */
#include <stdlib.h>	/* exit() warnings */
#include <string.h>	/* memset warnings */


#define BUF_SIZE	1024
#define LISTEN_PORT	60000

int main(int argc, char *argv[]){
    int	sock_listen,sock_recv[2];
    struct sockaddr_in	my_addr,recv_addr[2];
    int i,addr_size,bytes_received;
    fd_set readfds;
    struct timeval	timeout={0,0};
    int	incoming_len;
    struct sockaddr	remote_addr;
    int	recv_msg_size;
    char buf[BUF_SIZE];
    int	select_ret;


    char text[80],buf2[BUF_SIZE];
    int send_len,bytes_sent;

    while(1){
    /* create socket for listening */
    sock_listen=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_listen < 0){
        printf("socket() failed\n");
        exit(0);
    }
    /* make local address structure */
    memset(&my_addr, 0, sizeof (my_addr));	/* zero out structure */
    my_addr.sin_family = AF_INET;	/* address family */
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* current machine IP */

    my_addr.sin_port = htons((unsigned short)LISTEN_PORT);
    /* bind socket to the local address */
    i=bind(sock_listen, (struct sockaddr *) &my_addr, sizeof (my_addr));
    if (i < 0){
        printf("bind() failed\n");
        exit(0);
    }
    /* listen ... */
    i=listen(sock_listen, 5);
    if (i < 0){
        printf("listen() failed\n");
        exit(0);
    }

    /* get new socket to receive data on */
    addr_size=sizeof(recv_addr[i]);
    sock_recv[0]=accept(sock_listen, (struct sockaddr *) &recv_addr[0], &addr_size);
    sock_recv[1]=accept(sock_listen, (struct sockaddr *) &recv_addr[1], &addr_size);
   
    while (1){
        //ACCEPT FROM CLIENT
        bytes_received=recv(sock_recv[0],buf,BUF_SIZE,0);
        buf[bytes_received]=0;
        printf("Received : %s\n",buf);

        //SEND RESPONSE
        strcpy(buf2,"Response : ");
        strcat(buf2,buf);
        send_len=strlen(buf2);
        bytes_sent=send(sock_recv[0],buf2,send_len,0);

    if (strcmp(buf,"shutdown") == 0)
        break;
    }

    close(sock_recv);
    close(sock_listen);
}



