//client.c
/*Includes*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>


/*Error function which takes message as arguement*/
void error(char* msg){
  perror(msg);
  exit(0);
}


/*Function to decode packet*/
void decode_packet(char *buffer){
  int i;
  for(i=0;i<12;i++){
    *(buffer+i)=*(buffer+i)-5;
  }
}

/*Function to analyse packet*/
void analyse_packet(char *buffer){
  if(*(buffer+5)=='1')
    printf("\nVibration detected");
  else
    printf("\nNo Vibration");

  if(*(buffer+11)=='1')
    printf("   Touch Detected");
  else
    printf("   No touch");
}


int main(int argc , char *argv[]){
  int sock,portno,n;
  struct sockaddr_in server_addr;
  struct hostent *server;
  char serverIp[]="10.0.0.42"; //Ip address of raspberryPi
  char buffer[100];
  
  if(argc<3){
    fprintf(stderr,"usage %s hostname port\n",argv[0]);
    exit(0);
  }

  /*Take port number as command line arguement*/
  portno=atoi(argv[2]);

  /*Create socket*/
  sock = socket(AF_INET,SOCK_STREAM, 0);
  
  /*If value returned is -1, display error*/
  if(sock<0){
    error("socket failed");
  }

  server=gethostbyname(serverIp);

  if(server==NULL){
    fprintf(stderr,"Error, no such host\n");
    exit(0);
  }

  bzero((char*)&server_addr, sizeof(server_addr));

  server_addr.sin_family=AF_INET;
  
  bcopy((char*)server->h_addr, (char*)&server_addr.sin_addr.s_addr,server->h_length);

  server_addr.sin_port=htons(portno);
  
 /*connect*/
 if(connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr))<0){
    error("connect failed");
  }
  
  while(1){
   
    /*Zero out the elements of buffer at start*/
    bzero(buffer,100);

    /*Read the packet*/
    n=read(sock,buffer,99);
  
    /*If value returned is -1, display error*/
    if(n<0){
      error("Error reading from socket");
    } 

    printf("\n\nString received: %s",buffer);

    /*Decode the received packet*/
    decode_packet((char*)&buffer);
    printf("    Decoded Packet: %s",buffer);

    /*Analyse packet*/
    analyse_packet((char*)&buffer);
  
    }
  return 0;

}

