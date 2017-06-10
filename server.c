/*Includes*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>


/*Function to display error which takes message as an arguement*/
void error(char* msg){
  perror(msg);
  exit(1);
}

/*This function encodes the packet by adding number 5 to every element*/
void encode_packet(char *buffer){
  int i;
  for(i=0;i<12;i++){
    *(buffer+i)=*(buffer+i)+5;
  }
}


int main( int argc, char* argv[]){
  
  /*Variables*/
  int sock,newsock, portno, client_len;
  char *touch=NULL;
  char *piezo=NULL;
  char buffer[100];
  struct sockaddr_in server_addr, client_addr;
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

  /*Set pins as input and output*/
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
  sock= socket(AF_INET, SOCK_STREAM, 0);
  
  /*If sock returns -1, there was some error*/
  if(sock<0){
    error("Failed to create socket");
  }

  /*Zero out the value*/
  bzero((char*)&server_addr, sizeof(server_addr));

  /*Take port number as command line argument*/
  portno=atoi(argv[1]);
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port=htons(portno);


  /*bind*/
  if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
    error("bind failed");
  }
  
  /*Listen*/
  listen(sock,5);
 

  client_len=sizeof(client_addr);

  /*accept connection*/
  newsock=accept(sock,(struct sockaddr*)&client_addr, &client_len);

  /*If value returned is -1 then there is some error in accept*/
  if(newsock<0){
    error("Error on accept");
  }
 
  while(1){

    printf("Sending Touch Sensor Data \n");
   
    /*zero out the value of buffer at start of every transfer*/
    bzero(buffer,100);

    /*Read the value of touch sensor and piezo sensor*/
    Touch_Read = digitalRead(Touch_Pin);
    Piezo_Value= digitalRead(Piezo_Pin);
   
 
    if(Piezo_Value==1){
     piezo="PIEZO1";
    }
    else{
     piezo="PIEZO0";
    }
       
    if (Touch_Read == 1){

      touch="TOUCH1";
      strcpy(buffer,piezo);
      strcat(buffer,touch);

      /*Encode the packet before transfer*/ 
      encode_packet((char*)&buffer);

      /*Write the packet*/
      n=write(newsock,buffer,100);     
      
      digitalWrite(LED_Pin, 1);
      
      /*Stepper Motor*/
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
      touch="TOUCH0";
      strcpy(buffer,piezo);
      strcat(buffer,touch);

      /*Encode the packet before transfer*/
      encode_packet((char*)&buffer);

      /*Write the packet*/
      n=write(newsock,buffer,100);

      digitalWrite(LED_Pin, 0);
  
    } 


    if(n<0){
      error("Error writing to socket");
    }
  
  delay(3000);
  }
  return 0;  
}

