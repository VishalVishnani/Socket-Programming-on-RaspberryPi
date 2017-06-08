#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

void error(char* msg){
  perror(msg);
  exit(1);
}

int main( int argc, char* argv[]){
  
  /*Variables*/
  int sockfd,newsockfd, portno, clilen;
  int buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  
  uint8_t Touch_Pin = 8;
  uint8_t Touch_Read = 0;
  uint8_t LED_Pin = 7;
  uint8_t LED_Read = 0;
  uint8_t IN1_Pin = 9;
  uint8_t IN2_Pin = 0;
  uint8_t IN3_Pin = 2;
  uint8_t IN4_Pin = 3;
  uint16_t gradient = 180;
  uint16_t i,j;
  uint8_t Piezo_Pin= 29;
  uint8_t Piezo_Value=0;  
 
  if(wiringPiSetup() == -1)
    return 1;

  pinMode(Touch_Pin, INPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(IN1_Pin, OUTPUT);
  pinMode(IN2_Pin, OUTPUT);
  pinMode(IN3_Pin, OUTPUT);
  pinMode(IN4_Pin, OUTPUT);
  pinMode(Piezo_Pin, INPUT);
  j= (128*gradient)/90;

 
  if(argc<2){
    fprintf(stderr,"Error, no port provided\n");
    exit(1);
  }
  /*Create socket*/

  sockfd= socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd<0){
    error("Failed to create socket");
  }

  bzero((char*)&serv_addr, sizeof(serv_addr));

  portno=atoi(argv[1]);
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port=htons(portno);


  /*bind*/
  if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
    error("bind failed");
  }
  
  /*Listen*/
  listen(sockfd,5);
 
  clilen=sizeof(cli_addr);

  newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr, &clilen);

  if(newsockfd<0){
    error("Error on accept");
  }
 
  while(1){

    printf("Sending Touch Sensor Data \n");
    bzero(buffer,256);
    Touch_Read = digitalRead(Touch_Pin);
    buffer[0]=Touch_Read;
    Piezo_Value= digitalRead(Piezo_Pin);
    buffer[1]= Piezo_Value;
    n=write(newsockfd,buffer,255);
    if (Touch_Read == 1){
      digitalWrite(LED_Pin, 1);
      
      for(i=0; i<j; i++){

        digitalWrite(IN1_Pin,HIGH);
        digitalWrite(IN2_Pin,LOW);
        digitalWrite(IN3_Pin,LOW);
        digitalWrite(IN4_Pin,LOW);
        delay(10);

        digitalWrite(IN1_Pin,LOW);
        digitalWrite(IN2_Pin,HIGH);
        digitalWrite(IN3_Pin,LOW);
        digitalWrite(IN4_Pin,LOW);
        delay(10);

        digitalWrite(IN1_Pin,LOW);
        digitalWrite(IN2_Pin,LOW);
        digitalWrite(IN3_Pin,HIGH);
        digitalWrite(IN4_Pin,LOW);
        delay(10);

        digitalWrite(IN1_Pin,LOW);
        digitalWrite(IN2_Pin,LOW);
        digitalWrite(IN3_Pin,LOW);
        digitalWrite(IN4_Pin,HIGH);
        delay(10);
      }

      digitalWrite(IN1_Pin,LOW);
      digitalWrite(IN2_Pin,LOW);
      digitalWrite(IN3_Pin,LOW);
      digitalWrite(IN4_Pin,LOW);
      delay(1000);

    }
    
 
   else{
      digitalWrite(LED_Pin, 0);
  
  }

  if(n<0){
    error("Error writing to socket");
  }
  delay(2000);
  }
  return 0;  
}

