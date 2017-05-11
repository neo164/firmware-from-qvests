#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mp3TF.h>
#include <Bounce2.h>
#include <SPI.h>
#include "DHT.h"
#include "CyberLib.h"
#include <wiring.c>
#include <Bounce2.h>
#include <TimerOne.h>

#define BUTTON_PIN 12 //onboard button

// Instantiate a Bounce object
Bounce debouncer = Bounce();


int led = 13;
int R = 5, G = 6, B = 7;
int RGB = 0;
boolean flag = false, start_st = false;

int
din1_st = 0, din2_st = 0, din3_st = 0, din4_st = 0, din5_st = 0, din6_st = 0, din7_st = 0, din8_st = 0,
din9_st = 0, din10_st = 0, din11_st = 0,  din12_st = 0,  din13_st = 0,  din14_st = 0,  din15_st = 0,  din16_st = 0;

mp3TF mp3tf = mp3TF ();


#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

uint8_t bell[8]  = {
  0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4
};
uint8_t note[8]  = {
  0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0
};
uint8_t clock[8] = {
  0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0
};
uint8_t heart[8] = {
  0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0
};
uint8_t duck[8]  = {
  0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0
};
uint8_t check[8] = {
  0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0
};
uint8_t cross[8] = {
  0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0
};
uint8_t retarrow[8] = {
  0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4
};

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display


//-----блок реле--------
const int
relay_1 = 23,
relay_2 = 22,
relay_3 = 25,
relay_4 = 24,
relay_5 = 27,
relay_6 = 26,
relay_7 = 29,
relay_8 = 28,
relay_9 = 31,
relay_10 = 30,
relay_11 = 33,
relay_12 = 32,
relay_13 = 35,
relay_14 = 34,
relay_15 = 37,
relay_16 = 36;

//-----блок реле--------


//-----входа------------
const int
din_1 = 38,
din_2 = 39,
din_3 = 40,
din_4 = 41,
din_5 = 42,
din_6 = 43,
din_7 = 44,
din_8 = 45,
din_9 = 46,
din_10 = 47,
din_11 = 48,
din_12 = 49,
din_13 = 50,
din_14 = 51,
din_15 = 52,
din_16 = 53;
//-----входа------------









void setup()
{
  Timer1.initialize(300000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  mp3tf.init (&Serial);
  Serial.begin (9600);
  pinMode(led, OUTPUT);

  // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, bell);
  lcd.createChar(1, note);
  lcd.createChar(2, clock);
  lcd.createChar(3, heart);
  lcd.createChar(4, duck);
  lcd.createChar(5, check);
  lcd.createChar(6, cross);
  lcd.createChar(7, retarrow);
  lcd.home();
  //---------------------output------------------
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  pinMode(relay_5, OUTPUT);
  pinMode(relay_6, OUTPUT);
  pinMode(relay_7, OUTPUT);
  pinMode(relay_8, OUTPUT);
  pinMode(relay_9, OUTPUT);
  pinMode(relay_10, OUTPUT);
  pinMode(relay_11, OUTPUT);
  pinMode(relay_12, OUTPUT);
  pinMode(relay_13, OUTPUT);
  pinMode(relay_14, OUTPUT);
  pinMode(relay_15, OUTPUT);
  pinMode(relay_16, OUTPUT);

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
  pinMode(din_9, INPUT_PULLUP);
  pinMode(din_10, INPUT_PULLUP);
  pinMode(din_11, INPUT_PULLUP);
  pinMode(din_12, INPUT_PULLUP);
  pinMode(din_13, INPUT_PULLUP);
  pinMode(din_14, INPUT_PULLUP);
  pinMode(din_15, INPUT_PULLUP);
  pinMode(din_16, INPUT_PULLUP);
  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(15); // interval in ms

  initrel();


  lcd.setCursor(0, 0);
  lcd.print("|------------------|");
  lcd.setCursor(0, 1);
  lcd.print("|    VNEDRENIE     |");
  lcd.printByte(5);
  lcd.print(" |");
  lcd.setCursor(0, 2);
  lcd.print("|   SARATOV 2016   |");
  lcd.setCursor(0, 3);
  lcd.print("|------------------|");
  
  digitalWrite(relay_15, 1);
delay(10000);
digitalWrite(relay_15, 0);

  delay(1777);
  lcd.clear();

  mp3tf.setVolume (20);
  delay(50);
  mp3tf.play (1);
  delay(50);
}

// display all keycodes
//void displayKeyCodes(void) {
//  uint8_t i = 0;
//  while (1) {
//    lcd.clear();
//    lcd.print("Codes 0x");
//    lcd.print(i, HEX);
//    lcd.print("-0x");
//    lcd.print(i+16, HEX);
//    lcd.setCursor(0, 1);
//    for (int j=0; j<16; j++) {
//      lcd.printByte(i+j);
//    }
//    i+=16;
//
//    delay(4000);
//  }
//}

void loop()
{
  // Update the Bounce instance :
  debouncer.update();

  // Get the updated value :
  int value = debouncer.read();

  if ( value == LOW ) {
    digitalWrite(R, 1);
    digitalWrite(G, 1);
    digitalWrite(B, 1);
    delay(500);
    mp3tf.next();
  }

  //******************** TIMER **********************
  lcd.clear();
  int z = millis() / 1000;


  //  if(flag){
  int time = millis() / 1000;
  if (time / 60 / 60 < 10) {
    Serial.print ("0");
  }
  Serial.print (time / 60 / 60);
  Serial.print (":");
  if (time / 60 % 60 < 10) {
    Serial.print ("0");
  }
  Serial.print ((time / 60) % 60);
  Serial.print (":");
  if (time % 60 < 10) {
    Serial.print ("0");
  }
  Serial.println (time % 60);

  // draw time

  lcd.setCursor(0, 0);
  lcd.print("B:");
  lcd.print(time / 60 / 60, DEC);
  lcd.print(":");
  lcd.print((time / 60) % 60, DEC);
  lcd.print(":");
  lcd.print(time % 60, DEC);



  delay(500);
  // Toggle LED
  digitalWrite( led, digitalRead( led ) ^ 1 );



}

void timerIsr()
{
  update_DI();


  if (RGB >= 3) {
    RGB = 0;
  }

  if (RGB == 0) digitalWrite(R, 1);

  else digitalWrite(R, 0);

  if (RGB == 1)digitalWrite(G, 1);
  else digitalWrite(G, 0);

  if (RGB == 2)digitalWrite(B, 1);
  else digitalWrite(B, 0);

  RGB++;

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

  din9_st = digitalRead(din_9);
  if (din9_st) digitalWrite(relay_9, 1);
  else digitalWrite(relay_9, 0);

  din10_st = digitalRead(din_10);
  if (din10_st) digitalWrite(relay_10, 1);
  else digitalWrite(relay_10, 0);

  din11_st = digitalRead(din_11);
  if (din11_st) digitalWrite(relay_11, 1);
  else digitalWrite(relay_11, 0);

  din12_st = digitalRead(din_12);
  if (din12_st) digitalWrite(relay_12, 1);
  else digitalWrite(relay_12, 0);

  din13_st = digitalRead(din_13);
  if (din13_st) digitalWrite(relay_13, 1);
  else digitalWrite(relay_13, 0);

  din14_st = digitalRead(din_14);
  if (din14_st) digitalWrite(relay_14, 1);
  else digitalWrite(relay_14, 0);

  din15_st = digitalRead(din_15);
  if (din15_st) digitalWrite(relay_15, 1);
  else digitalWrite(relay_15, 0);

  din16_st = digitalRead(din_16);
  if (din16_st) digitalWrite(relay_16, 1);
  else digitalWrite(relay_16, 0);
}





void initrel() {               // обратное вкл
  digitalWrite(relay_1, HIGH);
  digitalWrite(relay_2, HIGH);
  digitalWrite(relay_3, HIGH);
  digitalWrite(relay_4, HIGH);
  digitalWrite(relay_5, HIGH);
  digitalWrite(relay_6, HIGH);
  digitalWrite(relay_7, HIGH);
  digitalWrite(relay_8, HIGH);
  digitalWrite(relay_9, HIGH);
  digitalWrite(relay_10, HIGH);
  digitalWrite(relay_11, HIGH);
  digitalWrite(relay_12, HIGH);
  digitalWrite(relay_13, HIGH);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  digitalWrite(R, HIGH);
  delay(200);
  digitalWrite(G, HIGH);
  delay(200);
  digitalWrite(B, HIGH);
  delay(200);
  digitalWrite(R, 0);
  digitalWrite(G, 0);
  digitalWrite(B, 0);
}

















