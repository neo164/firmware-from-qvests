//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/
#include <Shifter.h>
#include <SPI.h>
#include <Wire.h>
//#include "CyberLib.h"
#include <Bounce2.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define PIN 5
#define count_led 64 // количество светодиодов 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800); //first number change does distance between colors
int randNumber;
int randColorR, randColorB, randColorG;
//_______________________________Shifer______________________
#define SER_Pin 11 //SER_IN
#define RCLK_Pin 12 //L_CLOCK
#define SRCLK_Pin 13 //CLOCK
#define NUM_REGISTERS 8 //how many registers are in the chain 8*8=64
//initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS);
//_______________________________Shifer______________________
//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
int encoderSwitchPin = 4; //push button switch
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

const int dly_k = 150;

const int
led_1 = 0,//green
led_2 = 0,
led_3 = 0,

led_4 = 0,//white
led_5 = 0,
led_6 = 0,
led_7 = 0,

led_8 = 0,//blue
led_9 = 0,
led_10 = 0,
led_11 = 0,
led_12 = 0,

led_13 = 0,//red
led_14 = 0,
led_15 = 0,
led_16 = 0,
led_17 = 0,
led_18 = 0;

const int color = 0,
          RGB_R = 6,
          RGB_G = 7,
          RGB_B = 8;

boolean flag_1 = false, flag_2 = false, flag_3 = false, flag_4 = false;

//-----input------------
const int
din_1 = 22,//green a
din_2 = 23,
din_3 = 24,

din_4 = 25,//white b
din_5 = 26,
din_6 = 27,
din_7 = 28,

din_8 = 29,//blue c
din_9 = 30,
din_10 = 31,
din_11 = 32,
din_12 = 33,

din_13 = 34,//red d
din_14 = 35,
din_15 = 36,
din_16 = 37,
din_17 = 38,
din_18 = 39;

int a1 = 0, a2 = 0, a3 = 0,
    b1 = 0, b2 = 0, b3 = 0, b4 = 0,
    c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0,
    d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0;

int din_1_st = 0, din_2_st = 0, din_3_st = 0, din_4_st = 0, din_5_st = 0, din_6_st = 0, din_7_st = 0, din_8_st = 0, din_9_st = 0, din_10_st = 0, din_11_st = 0,
    din_12_st = 0, din_13_st = 0, din_14_st = 0, din_15_st = 0, din_16_st = 0, din_17_st = 0, din_18_st = 0;

int v = 0; 
//-----input------------

//------output-RGB-status-mission-----
const int
led_mission_1_R = 42,
led_mission_1_G = 43,
led_mission_1_B = 44,

led_mission_2_R = 45,
led_mission_2_G = 46,
led_mission_2_B = 47,

led_mission_3_R = 48,
led_mission_3_G = 49,
led_mission_3_B = 50,

led_mission_4_R = 51,
led_mission_4_G = 52,
led_mission_4_B = 53,

led_mission_1 = A0,
led_mission_2 = A1,
led_mission_3 = A2,
led_mission_4 = A3;



boolean fl_enc = false, fl_changer = false;
//------output-RGB-status-mission-----

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
  Serial.begin (9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();


  pixels.begin();
  pixels.show(); // Устанавливаем все светодиоды в состояние "Выключено"

  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  pinMode(encoderSwitchPin, INPUT_PULLUP);
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
  //  encoderValue = ReadEEPROM_Byte(0);

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
  pinMode(din_17, INPUT_PULLUP);
  pinMode(din_18, INPUT_PULLUP);
  //----------------------output------------------
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);

  pinMode(led_mission_1_R, OUTPUT);
  pinMode(led_mission_1_G, OUTPUT);
  pinMode(led_mission_1_B, OUTPUT);

  pinMode(led_mission_2_R, OUTPUT);
  pinMode(led_mission_2_G, OUTPUT);
  pinMode(led_mission_2_B, OUTPUT);

  pinMode(led_mission_3_R, OUTPUT);
  pinMode(led_mission_3_G, OUTPUT);
  pinMode(led_mission_3_B, OUTPUT);

  pinMode(led_mission_4_R, OUTPUT);
  pinMode(led_mission_4_G, OUTPUT);
  pinMode(led_mission_4_B, OUTPUT);

  pinMode(led_mission_1, OUTPUT);
  pinMode(led_mission_2, OUTPUT);
  pinMode(led_mission_3, OUTPUT);
  pinMode(led_mission_4, OUTPUT);

  //  digitalWrite(led_mission_1_G, HIGH);
  //  delay(1000);
  //  digitalWrite(led_mission_1_G, LOW);
  //
  //  digitalWrite(led_mission_1_B, HIGH);
  //  delay(500);
  //  digitalWrite(led_mission_1_B, LOW);
  //
  //
  //  digitalWrite(led_mission_1_G, HIGH);
  //  delay(500);
  //  digitalWrite(led_mission_2_G, HIGH);
  //  delay(500);
  //  digitalWrite(led_mission_3_G, HIGH);
  //  delay(500);
  //  digitalWrite(led_mission_4_G, HIGH);




  //  test();
  //  symbol_1();
  //  delay(1000);
  //  pixels.clear();
  //  symbol_2();
  //  delay(1000);
  //  pixels.clear();
  //  symbol_3();
  //  delay(1000);
  //  pixels.clear();
  //  symbol_4();
  //  delay(1000);
  //  pixels.clear();
  //  symbol_5();
  //  delay(1000);
  //  pixels.clear();

  //  digitalWrite(led_mission_1_R, HIGH);
  //  delay(500);
  //  digitalWrite(led_mission_2_R, HIGH);
  //  delay(500);
  //  digitalWrite(led_mission_3_R, HIGH);
  //  delay(500);
  //  digitalWrite(led_mission_4_R, HIGH);


}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void loop() {



  updateDI();
  rgb();
  switcher();


  screen();
  save();

  if (!fl_enc) {
    perebor();
  }
  
   
  
  if((encoderValue != 0) && (encoderValue != 4) && (encoderValue != 8) && (encoderValue != 12)){
  standby_r(50,encoderValue);
  }


  Serial.println(encoderValue);

  //111111111111111111111111111111111111111111111111111
  if (flag_1) { //  r b g

    //symbol_1();
    digitalWrite(led_mission_1_B, LOW);
    //delay(10);
    digitalWrite(led_mission_1_G, HIGH);
  }
  else {
    digitalWrite(led_mission_1_B, HIGH);
    //delay(10);
    digitalWrite(led_mission_1_G, LOW);
    //standby(155, 20); //(random,delay)
  }
  //111111111111111111111111111111111111111111111111111
  
  //222222222222222222222222222222222222222222222222222222222222222222222222222222222222
  if (flag_2) { //  g b r y

    digitalWrite(led_mission_2_B, LOW);
    digitalWrite(led_mission_2_G, HIGH);
    //  symbol_2();
  }
  else {

    digitalWrite(led_mission_2_B, HIGH);
    digitalWrite(led_mission_2_G, LOW);
    //    standby(155, 20); //(random,delay)
  }
  //222222222222222222222222222222222222222222222222222222222222222222222222222222222222

  //33333333333333333333333333333333333333333333333333333333333333333333333333333333333
  if (flag_3) { //  b b r p y

    digitalWrite(led_mission_3_B, LOW);
    digitalWrite(led_mission_3_G, HIGH);
    //    symbol_3();
  }
  else {

    digitalWrite(led_mission_3_B, HIGH);
    digitalWrite(led_mission_3_G, LOW);
    //      standby(155, 20); //(random,delay)

  }
  //333333333333333333333333333333333333333333333333333333333333333333333333333333333333


  //444444444444444444444444444444444444444444444444444444444444444444444444444444444
  if (flag_4) { //
    digitalWrite(led_mission_4_B, LOW);
    digitalWrite(led_mission_4_G, HIGH);
    //    symbol_4();
  }
  else {
    digitalWrite(led_mission_4_B, HIGH);
    digitalWrite(led_mission_4_G, LOW);
    //      standby(155, 20); //(random,delay)
  }
  //444444444444444444444444444444444444444444444444444444444444444444444444444444444


}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void rgb() {

  //green
  if (!din_1_st) {
    a1++;
    delay(dly_k);
    if (a1 >= 6) a1 = 1;
  }
  if (!din_2_st) {
    a2++;
    delay(dly_k);
    if (a2 >= 6) a2 = 1;
  }
  if (!din_3_st) {
    a3++;
    delay(dly_k);
    if (a3 >= 6) a3 = 1;
  }
  
  //white
  if (!din_4_st) {
    b1++;
    delay(dly_k);
    if (b1 >= 6) b1 = 1;
  }
  if (!din_5_st) {
    b2++;
    delay(dly_k);
    if (b2 >= 6) b2 = 1;
  }
  if (!din_6_st) {
    b3++;
    delay(dly_k);
    if (b3 >= 6) b3 = 1;
  }
  if (!din_7_st) {
    b4++;
    delay(dly_k);
    if (b4 >= 6) b4 = 1;
  }

  //blue
  if (!din_8_st) {
    c1++;
    delay(dly_k);
    if (c1 >= 6) c1 = 1;
  }
  if (!din_9_st) {
    c2++;
    delay(dly_k);
    if (c2 >= 6) c2 = 1;
  }
  if (!din_10_st) {
    c3++;
    delay(dly_k);
    if (c3 >= 6) c3 = 1;
  }
  if (!din_11_st) {
    c4++;
    delay(dly_k);
    if (c4 >= 6) c4 = 1;
  }
  if (!din_12_st) {
    c5++;
    delay(dly_k);
    if (c5 >= 6) c5 = 1;
  }

  //red
  if (!din_13_st) {
    d1++;
    delay(dly_k);
    if (d1 >= 6) d1 = 1;
  }
  if (!din_14_st) {
    d2++;
    delay(dly_k);
    if (d2 >= 6) d2 = 1;
  }
  if (!din_15_st) {
    d3++;
    delay(dly_k);
    if (d3 >= 6) d3 = 1;
  }
  if (!din_16_st) {
    d4++;
    delay(dly_k);
    if (d4 >= 6) d4 = 1;
  }
  if (!din_17_st) {
    d5++;
    delay(dly_k);
    if (d5 >= 6) d5 = 1;
  }
  if (!din_18_st) {
    d6++;
    delay(dly_k);
    if (d6 >= 6) d6 = 1;
  }


}



void switcher() {

  if (encoderValue == 8) {

    switch (a1) {

      case 1:
        shifter.setPin(0, 1);//r
        shifter.setPin(1, 0);//g
        shifter.setPin(2, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(0, 0);//r
        shifter.setPin(1, 1);//g
        shifter.setPin(2, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(0, 0);//r
        shifter.setPin(1, 0);//g
        shifter.setPin(2, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(0, 1);//r
        shifter.setPin(1, 1);//g
        shifter.setPin(2, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(0, 1);//r
        shifter.setPin(1, 0);//g
        shifter.setPin(2, 1);//b
        shifter.write();

        break;
    }

    switch (a2) {

      case 1:
        shifter.setPin(3, 1);//r
        shifter.setPin(4, 0);//g
        shifter.setPin(5, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(3, 0);//r
        shifter.setPin(4, 1);//g
        shifter.setPin(5, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(3, 0);//r
        shifter.setPin(4, 0);//g
        shifter.setPin(5, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(3, 1);//r
        shifter.setPin(4, 1);//g
        shifter.setPin(5, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(3, 1);//r
        shifter.setPin(4, 0);//g
        shifter.setPin(5, 1);//b
        shifter.write();

        break;
    }
    switch (a3) {

      case 1:
        shifter.setPin(6, 1);//r
        shifter.setPin(7, 0);//g
        shifter.setPin(8, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(6, 0);//r
        shifter.setPin(7, 1);//g
        shifter.setPin(8, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(6, 0);//r
        shifter.setPin(7, 0);//g
        shifter.setPin(8, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(6, 1);//r
        shifter.setPin(7, 1);//g
        shifter.setPin(8, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(6, 1);//r
        shifter.setPin(7, 0);//g
        shifter.setPin(8, 1);//b
        shifter.write();

        break;
    }

    //111111111111111111111111111111111111111111111111111
    if ((a1 == 1) && (a2 == 3) && (a3 == 2)) { //  r b g

      symbol_1();
      flag_1 = true;
    }
    else {
      flag_1 = false,
      standby(155, 20); //(random,delay)
    }
    //111111111111111111111111111111111111111111111111111



  }
  //****************************
  if (encoderValue == 12) {

    switch (b1) {


      case 1:
        shifter.setPin(9, 1);//r
        shifter.setPin(10, 0);//g
        shifter.setPin(11, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(9, 0);//r
        shifter.setPin(10, 1);//g
        shifter.setPin(11, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(9, 0);//r
        shifter.setPin(10, 0);//g
        shifter.setPin(11, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(9, 1);//r
        shifter.setPin(10, 1);//g
        shifter.setPin(11, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(9, 1);//r
        shifter.setPin(10, 0);//g
        shifter.setPin(11, 1);//b
        shifter.write();
        break;
    }

    switch (b2) {

      case 1:
        shifter.setPin(12, 1);//r
        shifter.setPin(13, 0);//g
        shifter.setPin(14, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(12, 0);//r
        shifter.setPin(13, 1);//g
        shifter.setPin(14, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(12, 0);//r
        shifter.setPin(13, 0);//g
        shifter.setPin(14, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(12, 1);//r
        shifter.setPin(13, 1);//g
        shifter.setPin(14, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(12, 1);//r
        shifter.setPin(13, 0);//g
        shifter.setPin(14, 1);//b
        shifter.write();

        break;
    }
    switch (b3) {

      case 1:
        shifter.setPin(15, 1);//r
        shifter.setPin(16, 0);//g
        shifter.setPin(17, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(15, 0);//r
        shifter.setPin(16, 1);//g
        shifter.setPin(17, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(15, 0);//r
        shifter.setPin(16, 0);//g
        shifter.setPin(17, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(15, 1);//r
        shifter.setPin(16, 1);//g
        shifter.setPin(17, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(15, 1);//r
        shifter.setPin(16, 0);//g
        shifter.setPin(17, 1);//b
        shifter.write();
        break;
    }
    switch (b4) {

      case 1:
        shifter.setPin(18, 1);//r
        shifter.setPin(19, 0);//g
        shifter.setPin(20, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(18, 0);//r
        shifter.setPin(19, 1);//g
        shifter.setPin(20, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(18, 0);//r
        shifter.setPin(19, 0);//g
        shifter.setPin(20, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(18, 1);//r
        shifter.setPin(19, 1);//g
        shifter.setPin(20, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(18, 1);//r
        shifter.setPin(19, 0);//g
        shifter.setPin(20, 1);//b
        shifter.write();
        break;
    }

    //222222222222222222222222222222222222222222222222222222222222222222222222222222222222
    if ((b1 == 2) && (b2 == 3) && (b3 == 1) && (b4 == 4)) { //  g b r y
      flag_2 = true;
      symbol_2();
    }
    else {
      flag_2 = false;
      standby(155, 20); //(random,delay)
    }
    //222222222222222222222222222222222222222222222222222222222222222222222222222222222222


  }
  //******************
  if (encoderValue == 0) {
    switch (c1) {

      case 1:
        shifter.setPin(21, 1);//r
        shifter.setPin(22, 0);//g
        shifter.setPin(23, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(21, 0);//r
        shifter.setPin(22, 1);//g
        shifter.setPin(23, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(21, 0);//r
        shifter.setPin(22, 0);//g
        shifter.setPin(23, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(21, 1);//r
        shifter.setPin(22, 1);//g
        shifter.setPin(23, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(21, 1);//r
        shifter.setPin(22, 0);//g
        shifter.setPin(23, 1);//b
        shifter.write();
        break;

        break;
    }
    switch (c2) {
      case 1:
        shifter.setPin(24, 1);//r
        shifter.setPin(25, 0);//g
        shifter.setPin(26, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(24, 0);//r
        shifter.setPin(25, 1);//g
        shifter.setPin(26, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(24, 0);//r
        shifter.setPin(25, 0);//g
        shifter.setPin(26, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(24, 1);//r
        shifter.setPin(25, 1);//g
        shifter.setPin(26, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(24, 1);//r
        shifter.setPin(25, 0);//g
        shifter.setPin(26, 1);//b
        shifter.write();
        break;
        break;
    }
    switch (c3) {
      case 1:
        shifter.setPin(27, 1);//r
        shifter.setPin(28, 0);//g
        shifter.setPin(29, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(27, 0);//r
        shifter.setPin(28, 1);//g
        shifter.setPin(29, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(27, 0);//r
        shifter.setPin(28, 0);//g
        shifter.setPin(29, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(27, 1);//r
        shifter.setPin(28, 1);//g
        shifter.setPin(29, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(27, 1);//r
        shifter.setPin(28, 0);//g
        shifter.setPin(29, 1);//b
        shifter.write();
        break;

        break;
    }
    switch (c4) {
      case 1:
        shifter.setPin(32, 1);//r
        shifter.setPin(33, 0);//g
        shifter.setPin(34, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(32, 0);//r
        shifter.setPin(33, 1);//g
        shifter.setPin(34, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(32, 0);//r
        shifter.setPin(33, 0);//g
        shifter.setPin(34, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(32, 1);//r
        shifter.setPin(33, 1);//g
        shifter.setPin(34, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(32, 1);//r
        shifter.setPin(33, 0);//g
        shifter.setPin(34, 1);//b
        shifter.write();
        break;

        break;
    }
    switch (c5) {
      case 1:
        shifter.setPin(35, 1);//r
        shifter.setPin(36, 0);//g
        shifter.setPin(37, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(35, 0);//r
        shifter.setPin(36, 1);//g
        shifter.setPin(37, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(35, 0);//r
        shifter.setPin(36, 0);//g
        shifter.setPin(37, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(35, 1);//r
        shifter.setPin(36, 1);//g
        shifter.setPin(37, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(35, 1);//r
        shifter.setPin(36, 0);//g
        shifter.setPin(37, 1);//b
        shifter.write();
        break;

        break;
    }
    //33333333333333333333333333333333333333333333333333333333333333333333333333333333333
    if ((c1 == 3) && (c2 == 3) && (c3 == 1) && (c4 == 5) && (c5 == 4)) { //  b b r p y
flag_3 = true;
      symbol_3();
    }
    else {
flag_3 = false,
      standby(155, 20); //(random,delay)

    }
    //333333333333333333333333333333333333333333333333333333333333333333333333333333333333


  }
  //******************
  if (encoderValue == 4) {

    switch (d1) {

      case 1:
        shifter.setPin(38, 1);//r
        shifter.setPin(39, 0);//g
        shifter.setPin(40, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(38, 0);//r
        shifter.setPin(39, 1);//g
        shifter.setPin(40, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(38, 0);//r
        shifter.setPin(39, 0);//g
        shifter.setPin(40, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(38, 1);//r
        shifter.setPin(39, 1);//g
        shifter.setPin(40, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(38, 1);//r
        shifter.setPin(39, 0);//g
        shifter.setPin(40, 1);//b
        shifter.write();
        break;

        break;
    }
    switch (d2) {
      case 1:
        shifter.setPin(41, 1);//r
        shifter.setPin(42, 0);//g
        shifter.setPin(43, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(41, 0);//r
        shifter.setPin(42, 1);//g
        shifter.setPin(43, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(41, 0);//r
        shifter.setPin(42, 0);//g
        shifter.setPin(43, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(41, 1);//r
        shifter.setPin(42, 1);//g
        shifter.setPin(43, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(41, 1);//r
        shifter.setPin(42, 0);//g
        shifter.setPin(43, 1);//b
        shifter.write();
        break;

        break;
    }
    switch (d3) {
      case 1:
        shifter.setPin(44, 1);//r
        shifter.setPin(45, 0);//g
        shifter.setPin(46, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(44, 0);//r
        shifter.setPin(45, 1);//g
        shifter.setPin(46, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(44, 0);//r
        shifter.setPin(45, 0);//g
        shifter.setPin(46, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(44, 1);//r
        shifter.setPin(45, 1);//g
        shifter.setPin(46, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(44, 1);//r
        shifter.setPin(45, 0);//g
        shifter.setPin(46, 1);//b
        shifter.write();
        break;

        break;
    }
    switch (d4) {
      case 1:
        shifter.setPin(47, 1);//r
        shifter.setPin(48, 0);//g
        shifter.setPin(49, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(47, 0);//r
        shifter.setPin(48, 1);//g
        shifter.setPin(49, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(47, 0);//r
        shifter.setPin(48, 0);//g
        shifter.setPin(49, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(47, 1);//r
        shifter.setPin(48, 1);//g
        shifter.setPin(49, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(47, 1);//r
        shifter.setPin(48, 0);//g
        shifter.setPin(49, 1);//b
        shifter.write();
        break;

        break;
    }
    switch (d5) {
      case 1:
        shifter.setPin(50, 1);//r
        shifter.setPin(51, 0);//g
        shifter.setPin(52, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(50, 0);//r
        shifter.setPin(51, 1);//g
        shifter.setPin(52, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(50, 0);//r
        shifter.setPin(51, 0);//g
        shifter.setPin(52, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(50, 1);//r
        shifter.setPin(51, 1);//g
        shifter.setPin(52, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(50, 1);//r
        shifter.setPin(51, 0);//g
        shifter.setPin(52, 1);//b
        shifter.write();
        break;


        break;
    }
    switch (d6) {
      case 1:
        shifter.setPin(53, 1);//r
        shifter.setPin(54, 0);//g
        shifter.setPin(55, 0);//b
        shifter.write();
        break;
      case 2:
        shifter.setPin(53, 0);//r
        shifter.setPin(54, 1);//g
        shifter.setPin(55, 0);//b
        shifter.write();
        break;
      case 3:
        shifter.setPin(53, 0);//r
        shifter.setPin(54, 0);//g
        shifter.setPin(55, 1);//b
        shifter.write();
        break;
      case 4:
        shifter.setPin(53, 1);//r
        shifter.setPin(54, 1);//g
        shifter.setPin(55, 0);//b
        shifter.write();
        break;
      case 5:
        shifter.setPin(53, 1);//r
        shifter.setPin(54, 0);//g
        shifter.setPin(55, 1);//b
        shifter.write();
        break;

        break;
    }

    //444444444444444444444444444444444444444444444444444444444444444444444444444444444
    if ((d1 == 5) && (d2 == 3) && (d3 == 1) && (d4 == 4) && (d5 == 2) && (d6 == 3)) { //
      flag_4 = true;
      symbol_4();
    }
    else {
      flag_4 = false;
      standby(155, 20); //(random,delay)
    }
    //444444444444444444444444444444444444444444444444444444444444444444444444444444444

  }




}

void updateDI() {
  din_1_st = digitalRead(din_1);
  din_2_st = digitalRead(din_2);
  din_3_st = digitalRead(din_3);
  din_4_st = digitalRead(din_4);
  din_5_st = digitalRead(din_5);
  din_6_st = digitalRead(din_6);
  din_7_st = digitalRead(din_7);
  din_8_st = digitalRead(din_8);
  din_9_st = digitalRead(din_9);
  din_10_st = digitalRead(din_10);
  din_11_st = digitalRead(din_11);
  din_12_st = digitalRead(din_12);
  din_13_st = digitalRead(din_13);
  din_14_st = digitalRead(din_14);
  din_15_st = digitalRead(din_15);
  din_16_st = digitalRead(din_16);
  din_17_st = digitalRead(din_17);
  din_18_st = digitalRead(din_18);

}


//****************************************************************************************************************************
void screen() {
  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("enc:");
  display.print(encoderValue, DEC);
  display.display();
}

void save() {
  if (digitalRead(encoderSwitchPin)) {
    //button is not being pushed
  }
  else {
    encoderValue = 0;
    //    WriteEEPROM_Byte(0, encoderValue);
    Serial.println("ok");
    Serial.println(encoderValue);
    delay(200);
  }

}

void perebor() {
  for (int i = 0; i <= 63; i++) {
    shifter.setPin(i, HIGH);
    shifter.write();
    //    if (encoderValue < 0) encoderValue = 0;
    //    if (encoderValue > 3) encoderValue = 3;
    delay(encoderValue);
    shifter.clear();
    shifter.write();
  }

}

void test() {

  digitalWrite(led_mission_1_R, HIGH);
  delay(500);
  digitalWrite(led_mission_2_R, HIGH);
  delay(500);
  digitalWrite(led_mission_3_R, HIGH);
  delay(500);
  digitalWrite(led_mission_4_R, HIGH);
  delay(1000);
  digitalWrite(led_mission_1_R, LOW);
  delay(500);
  digitalWrite(led_mission_2_R, LOW);
  delay(500);
  digitalWrite(led_mission_3_R, LOW);
  delay(500);
  digitalWrite(led_mission_4_R, LOW);

  digitalWrite(led_mission_1_G, HIGH);
  delay(500);
  digitalWrite(led_mission_2_G, HIGH);
  delay(500);
  digitalWrite(led_mission_3_G, HIGH);
  delay(500);
  digitalWrite(led_mission_4_G, HIGH);
  delay(1000);
  digitalWrite(led_mission_1_G, LOW);
  delay(500);
  digitalWrite(led_mission_2_G, LOW);
  delay(500);
  digitalWrite(led_mission_3_G, LOW);
  delay(500);
  digitalWrite(led_mission_4_G, LOW);

  for (int i = 0; i <= 63; i++) {
    shifter.setPin(i, HIGH);
    shifter.write();
    //    if (encoderValue < 0) encoderValue = 0;
    //    if (encoderValue > 3) encoderValue = 3;
    delay(50);
    shifter.write();
  }
  shifter.clear();

  digitalWrite(led_mission_1, HIGH);
  delay(250);
  digitalWrite(led_mission_2, HIGH);
  delay(250);
  digitalWrite(led_mission_3, HIGH);
  delay(250);
  digitalWrite(led_mission_4, HIGH);
  delay(500);
  digitalWrite(led_mission_1, LOW);
  delay(250);
  digitalWrite(led_mission_2, LOW);
  delay(250);
  digitalWrite(led_mission_3, LOW);
  delay(250);
  digitalWrite(led_mission_4, LOW);

  symbol_1();
  delay(1000);
  pixels.clear();
  symbol_2();
  delay(1000);
  pixels.clear();
  symbol_3();
  delay(1000);
  pixels.clear();
  symbol_4();
  delay(1000);
  pixels.clear();
  delay(1000);
  pixels.clear();
  symbol_5();
  delay(1000);
  pixels.clear();

}


//==========================led 8x8================================

int standby(int y, int d) {

  randNumber = random(64);
  randColorR = random(y);
  randColorG = random(y);
  randColorB = random(y);
  pixels.setPixelColor(randNumber, pixels.Color(randColorR, randColorG, randColorB));
  pixels.show();
  delay(d);
  pixels.clear();
}


int standby_r(int y, int v) {
  
  randColorR = 0;
  randColorG = y;
  randColorB = 0;
  pixels.setPixelColor(v, pixels.Color(randColorR, randColorG, randColorB));
  if(v>=0){
  pixels.setPixelColor(0, pixels.Color(50, 0, 50));
  
}
  if(v>=4)
{
  pixels.setPixelColor(4, pixels.Color(50, 0, 50));

}
  if(v>=8)
 {
  pixels.setPixelColor(8, pixels.Color(50, 0, 50));
 
 
 }
  if(v>=12)
  {
  pixels.setPixelColor(12, pixels.Color(50, 0, 50));
  }
  
  pixels.show();
  delay(20);
  pixels.clear();
}


void symbol_1() {
  /*
  * ****
   * ***
   *   *
   *   *
   *   *
   *****
  */
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, 50));
  pixels.setPixelColor(1, pixels.Color(0, 0, 50));
  pixels.setPixelColor(2, pixels.Color(0, 0, 50));
  pixels.setPixelColor(3, pixels.Color(0, 0, 50));
  pixels.setPixelColor(4, pixels.Color(0, 0, 50));
  pixels.setPixelColor(10, pixels.Color(0, 0, 50));
  pixels.setPixelColor(12, pixels.Color(0, 0, 50));
  pixels.setPixelColor(18, pixels.Color(0, 0, 50));
  pixels.setPixelColor(20, pixels.Color(0, 0, 50));
  pixels.setPixelColor(24, pixels.Color(0, 0, 50));
  pixels.setPixelColor(25, pixels.Color(0, 0, 50));
  pixels.setPixelColor(26, pixels.Color(0, 0, 50));
  pixels.setPixelColor(27, pixels.Color(0, 0, 50));
  pixels.setPixelColor(28, pixels.Color(0, 0, 50));

    pixels.setPixelColor(32, pixels.Color(0,0,50));
    pixels.setPixelColor(33, pixels.Color(0,0,50));
    pixels.setPixelColor(34, pixels.Color(0,0,50));
    pixels.setPixelColor(35, pixels.Color(0,0,50));
    pixels.setPixelColor(36, pixels.Color(0,0,50));
    pixels.setPixelColor(37, pixels.Color(0,0,50));
    pixels.setPixelColor(38, pixels.Color(0,0,50));
    //pixels.setPixelColor(38, pixels.Color(0,0,50));
    //pixels.setPixelColor(39, pixels.Color(0,0,50));
  pixels.show();

}


//****************************************************




void symbol_2() {
  /*


  */
  pixels.clear();
  pixels.setPixelColor(63, pixels.Color(0, 0, 50));
  pixels.setPixelColor(55, pixels.Color(0, 0, 50));
  pixels.setPixelColor(47, pixels.Color(0, 0, 50));
  pixels.setPixelColor(39, pixels.Color(0, 0, 50));
  pixels.setPixelColor(31, pixels.Color(0, 0, 50));
  pixels.setPixelColor(30, pixels.Color(0, 0, 50));
  pixels.setPixelColor(29, pixels.Color(0, 0, 50));
  pixels.setPixelColor(28, pixels.Color(0, 0, 50));
  pixels.setPixelColor(36, pixels.Color(0, 0, 50));
  pixels.setPixelColor(44, pixels.Color(0, 0, 50));
  pixels.setPixelColor(43, pixels.Color(0, 0, 50));
  pixels.setPixelColor(42, pixels.Color(0, 0, 50));
  pixels.setPixelColor(34, pixels.Color(0, 0, 50));
  pixels.setPixelColor(26, pixels.Color(0, 0, 50));
  pixels.setPixelColor(18, pixels.Color(0, 0, 50));
  pixels.setPixelColor(19, pixels.Color(0, 0, 50));
  pixels.setPixelColor(27, pixels.Color(0, 0, 50));

  pixels.show();
}

void symbol_3() {
  /*


  */
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, 50));
  pixels.setPixelColor(1, pixels.Color(0, 0, 50));
  pixels.setPixelColor(2, pixels.Color(0, 0, 50));
  pixels.setPixelColor(3, pixels.Color(0, 0, 50));
  pixels.setPixelColor(4, pixels.Color(0, 0, 50));
  pixels.setPixelColor(12, pixels.Color(0, 0, 50));
  pixels.setPixelColor(20, pixels.Color(0, 0, 50));
  pixels.setPixelColor(28, pixels.Color(0, 0, 50));
  pixels.setPixelColor(27, pixels.Color(0, 0, 50));
  pixels.setPixelColor(26, pixels.Color(0, 0, 50));
  pixels.setPixelColor(18, pixels.Color(0, 0, 50));
  pixels.setPixelColor(10, pixels.Color(0, 0, 50));
  pixels.setPixelColor(45, pixels.Color(0, 0, 50));
  pixels.setPixelColor(46, pixels.Color(0, 0, 50));
  pixels.setPixelColor(53, pixels.Color(0, 0, 50));
  pixels.setPixelColor(54, pixels.Color(0, 0, 50));


  pixels.show();
}

void symbol_4() {
  /*


  */
  pixels.clear();
  pixels.setPixelColor(25, pixels.Color(0, 0, 50));
  pixels.setPixelColor(26, pixels.Color(0, 0, 50));
  pixels.setPixelColor(34, pixels.Color(0, 0, 50));
  pixels.setPixelColor(42, pixels.Color(0, 0, 50));
  pixels.setPixelColor(43, pixels.Color(0, 0, 50));
  pixels.setPixelColor(44, pixels.Color(0, 0, 50));
  pixels.setPixelColor(45, pixels.Color(0, 0, 50));
  pixels.setPixelColor(37, pixels.Color(0, 0, 50));
  pixels.setPixelColor(29, pixels.Color(0, 0, 50));
  pixels.setPixelColor(21, pixels.Color(0, 0, 50));
  pixels.setPixelColor(13, pixels.Color(0, 0, 50));
  pixels.setPixelColor(12, pixels.Color(0, 0, 50));
  pixels.setPixelColor(11, pixels.Color(0, 0, 50));
  pixels.setPixelColor(46, pixels.Color(0, 0, 50));
  pixels.setPixelColor(54, pixels.Color(0, 0, 50));
  pixels.setPixelColor(53, pixels.Color(0, 0, 50));


  pixels.show();
}

void symbol_5() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, 50));
  pixels.setPixelColor(1, pixels.Color(0, 0, 50));
  pixels.setPixelColor(3, pixels.Color(0, 0, 50));
  pixels.setPixelColor(4, pixels.Color(0, 0, 50));
  pixels.setPixelColor(17, pixels.Color(0, 0, 50));
  pixels.setPixelColor(18, pixels.Color(0, 0, 50));
  pixels.setPixelColor(19, pixels.Color(0, 0, 50));
  pixels.setPixelColor(25, pixels.Color(0, 0, 50));
  pixels.setPixelColor(26, pixels.Color(0, 0, 50));
  pixels.setPixelColor(27, pixels.Color(0, 0, 50));
  pixels.setPixelColor(17, pixels.Color(0, 0, 50));
  pixels.setPixelColor(53, pixels.Color(0, 0, 50));
  pixels.setPixelColor(54, pixels.Color(0, 0, 50));
  pixels.setPixelColor(45, pixels.Color(0, 0, 50));
  pixels.setPixelColor(46, pixels.Color(0, 0, 50));
  pixels.setPixelColor(37, pixels.Color(0, 0, 50));
  pixels.setPixelColor(38, pixels.Color(0, 0, 50));


  pixels.show();
}
//==========================led 8x8=================================


void updateEncoder() {
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time

  if (encoderValue < 0) encoderValue = 0;
  if (encoderValue > 12) encoderValue = 12;

  switch (encoderValue) {
    case 0:
      digitalWrite(led_mission_1, HIGH);
      digitalWrite(led_mission_2, LOW);
      digitalWrite(led_mission_3, LOW);
      digitalWrite(led_mission_4, LOW);
      fl_enc = true;
      break;

    case 4:
      digitalWrite(led_mission_1, LOW);
      digitalWrite(led_mission_2, HIGH);
      digitalWrite(led_mission_3, LOW);
      digitalWrite(led_mission_4, LOW);
      fl_enc = true;
      break;

    case 8:
      digitalWrite(led_mission_1, LOW);
      digitalWrite(led_mission_2, LOW);
      digitalWrite(led_mission_3, HIGH);
      digitalWrite(led_mission_4, LOW);
      fl_enc = true;
      break;

    case 12:
      digitalWrite(led_mission_1, LOW);
      digitalWrite(led_mission_2, LOW);
      digitalWrite(led_mission_3, LOW);
      digitalWrite(led_mission_4, HIGH);
      fl_enc = true;
      break;

    default:
      //fl_enc=false;

      break;
   }
   
   v = encoderValue; 

}
//****************************************************************************************************************************






















