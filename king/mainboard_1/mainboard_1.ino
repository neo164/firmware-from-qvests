#include "CyberLib.h"
#include <wiring.c>
#include <TimerOne.h>



int led = 13, button = 12;
int R = 5, G = 6, B = 7;
int RGB = 0;
boolean flag = false, start_st = false;

int
din1_st = 0, din2_st = 0, din3_st = 0, din4_st = 0, din5_st = 0, din6_st = 0, din7_st = 0, din8_st = 0, button_st;



#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif



//-----блок реле--------
const int
relay_1 = 22,
relay_2 = 23,
relay_3 = 24,
relay_4 = 25,
relay_5 = 26,
relay_6 = 27,
relay_7 = 28,
relay_8 = 29;


//-----блок реле--------


//-----входа------------
const int
din_1 = 30,
din_2 = 31,
din_3 = 32,
din_4 = 33,
din_5 = 34,
din_6 = 35,
din_7 = 36,
din_8 = 37;

//-----входа------------









void setup()
{
  Serial.begin (9600);
  pinMode(led, OUTPUT);


  //---------------------output------------------
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  pinMode(relay_5, OUTPUT);
  pinMode(relay_6, OUTPUT);
  pinMode(relay_7, OUTPUT);
  pinMode(relay_8, OUTPUT);



  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  //----------------------input------------------
  pinMode(din_1, INPUT_PULLUP);
  pinMode(din_2, INPUT_PULLUP);
  pinMode(din_3, INPUT_PULLUP);
  pinMode(din_4, INPUT_PULLUP);
  pinMode(din_5, INPUT_PULLUP);
  pinMode(din_6, INPUT_PULLUP);
  pinMode(din_7, INPUT_PULLUP);
  pinMode(din_8, INPUT_PULLUP);

  pinMode(button, INPUT_PULLUP);


  initrel();
  
  Timer1.initialize(300000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here


}


//****************************************************************************
void loop()
{
  button_st = digitalRead(button);



  if(button_st){
    delay(500);
    // Toggle LED
    digitalWrite( led, digitalRead( led ) ^ 1 );
  }


}
//****************************************************************************

void timerIsr()
{
  update_DI();


}


void update_DI() {

  din1_st = digitalRead(din_1);
  if (din1_st) digitalWrite(relay_1, 1);
  else digitalWrite(relay_1, 0);

  din2_st = digitalRead(din_2);
  if (din2_st) digitalWrite(relay_2, 1);
  else digitalWrite(relay_2, 0);

  din3_st = digitalRead(din_3);
  if (din3_st) digitalWrite(relay_3, 1);
  else digitalWrite(relay_3, 0);

  din4_st = digitalRead(din_4);
  if (din4_st) digitalWrite(relay_4, 1);
  else digitalWrite(relay_4, 0);

  din5_st = digitalRead(din_5);
  if (din5_st) digitalWrite(relay_5, 1);
  else digitalWrite(relay_5, 0);

  din6_st = digitalRead(din_6);
  if (din6_st) digitalWrite(relay_6, 1);
  else digitalWrite(relay_6, 0);

  din7_st = digitalRead(din_7);
  if (din7_st) digitalWrite(relay_7, 1);
  else digitalWrite(relay_7, 0);

  din8_st = digitalRead(din_8);
  if (din8_st) digitalWrite(relay_8, 1);
  else digitalWrite(relay_8, 0);
}





void initrel() {               
  digitalWrite(relay_1, HIGH);
  delay(1500);
  digitalWrite(relay_2, HIGH);
  delay(1500);
  digitalWrite(relay_3, HIGH);
  delay(1500);
  digitalWrite(relay_4, HIGH);
  delay(1500);
  digitalWrite(relay_5, HIGH);
  delay(1500);
  digitalWrite(relay_6, HIGH);
  delay(1500);
  digitalWrite(relay_7, HIGH);
  delay(1500);
  digitalWrite(relay_8, HIGH);

}



















