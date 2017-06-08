#include <stdio.h>
#include <wiringPi.h>
#include <stdint.h>

int main(void){

  uint8_t Touch_Pin = 8;
  uint8_t Touch_Read = 0;
  uint8_t LED_Pin = 7;
  uint8_t LED_Read = 0;
  uint8_t IN1_Pin = 9;
  uint8_t IN2_Pin = 0;
  uint8_t IN3_Pin = 2;
  uint8_t IN4_Pin = 3;
  uint16_t gradient = 360;
  uint16_t i,j;  

  if(wiringPiSetup() == -1)
    return 1;

  pinMode(Touch_Pin, INPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(IN1_Pin, OUTPUT);
  pinMode(IN2_Pin, OUTPUT);
  pinMode(IN3_Pin, OUTPUT);
  pinMode(IN4_Pin, OUTPUT);
  j= (128*gradient)/90;
 
  while(1){
    Touch_Read = digitalRead(Touch_Pin);
   // delay (250);
   // printf("Touch read = %d\n",Touch_Read);
    if (Touch_Read == 1){
     // printf("Touch detected\n");
     // printf("LED ON\n");
      digitalWrite(LED_Pin, 1);
     // LED_Read = digitalRead(LED_Pin);
     // printf(" LED is %d\n", LED_Read);
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
     // printf("Touch not detected\n");
     // printf("LED OFF\n");
      digitalWrite(LED_Pin, 0);
     // LEDread = digitalRead(LED_Pin);
     // printf("LED is %d\n", LED_Read); 
    }
 // delay (250);
  }
  return 0;
}
