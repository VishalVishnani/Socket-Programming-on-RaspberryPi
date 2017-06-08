#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(char* msg){
  perror(msg);
  exit(0);
}

int main(int argc , char *argv[]){
  int sockfd,portno,n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char serverIp[]="10.0.0.42";
  int buffer[256];
  
  if(argc<3){
    fprintf(stderr,"usage %s hostname port\n",argv[0]);
    exit(0);
  }

  portno=atoi(argv[2]);

  sockfd = socket(AF_INET,SOCK_STREAM, 0);
  
  if(sockfd<0){
    error("socket failed");
  }

  server=gethostbyname(serverIp);

  if(server==NULL){
    fprintf(stderr,"Error, no such host\n");
    exit(0);
  }

  bzero((char*)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family=AF_INET;
  
  bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr,server->h_length);

  serv_addr.sin_port=htons(portno);
  
  
 if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
    error("connect failed");
  }
  
  while(1){
    bzero(buffer,256);
    n=read(sockfd,buffer,255);
  
    if(n<0){
      error("Error reading from socket");
    } 

    if(buffer[0]==1){
      printf("\nTouch detected");
    }
    else{
      printf("\nTouch Not DETECTED");
    }

    if(buffer[1]==1){
      printf("\nVibration detected");
    }
    else{
      printf("\nVibration Not DETECTED");
    }

  }
  return 0;
}

