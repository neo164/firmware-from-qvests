#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mp3TF.h>
#include <Bounce2.h>
#include <SPI.h>
#include "DHT.h"
//#include "CyberLib.h"
#include <wiring.c>
#include <Bounce2.h>
#include <TimerOne.h>
#include <Shifter.h>

//******************
#define SER_Pin 4 //SER_IN
#define RCLK_Pin 3 //L_CLOCK
#define SRCLK_Pin 2 //CLOCK

#define NUM_REGISTERS 1 //how many registers are in the chain
//initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS);
//******************

//-------------кнопка----------------
#define BUTTON_PIN 12 //onboard button

// Instantiate a Bounce object
Bounce debouncer = Bounce();
int value = 0;

//-------------кнопка----------------

//-------------аналоговый вход----------------

int analogInPin = A15;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

//-------------аналоговый вход----------------


//-----------------onboard UNL---------------
const int u1 = A0, u2 = A1, u3 = A2, u4 = A3, u5 = A4, u6 = A5, u7 = A6, un8 = A7,

s1 = A8, s2 = A9, s3 = A10, s4 = A11, s5 = A12, s6 = A13, s7 = A14;
//-----------------onboard UNL---------------

//-----------------onboard LEDs--------------
const int led = 13;
const int R = 5, G = 6, B = 7;
const int f1 = 8, f2 = 9, f3 = 10, f4 = 11;
int RGB = 0;
//-----------------onboard LEDs--------------


//-----------------flag mission--------------

boolean fl_bochka = false, fl_shlem = false, fl_shit = false, fl_clock = false, fl_eggs = false, fl_fonari = false, fl_gerby = false, fl_lebedka = false, fl_klyk = false, fl_tron = false, fl_fakel = false, flag = false, start_st = false;

boolean track_1 = false, track_2 = false, track_3 = false, track_4 = false, track_5 = false, track_6 = false, track_7 = false, track_8 = false, track_9 = false, track_10 = false, track_11 = false, track_12 = false, track_13 = false, track_14 = false, track_15 = false;

//-----------------flag mission--------------
int
s1_state = 0, s2_state = 0, s3_state = 0, s4_state = 0, s5_state = 0, s6_state = 0, s7_state = 0, s8_state = 0;
int
f1_state = 0, f2_state = 0, f3_state = 0, f4_state = 0;
int
din1_st = 0, din2_st = 0, din3_st = 0, din4_st = 0, din5_st = 0, din6_st = 0, din7_st = 0, din8_st = 0,
din9_st = 0, din10_st = 0, din11_st = 0,  din12_st = 0,  din13_st = 0,  din14_st = 0,  din15_st = 0,  din16_st = 0;

mp3TF mp3tf = mp3TF ();

//----------symbols----------------------

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

//----------symbols----------------------

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
  pinMode(relay_3, OUTPUT); //трон замок
  pinMode(relay_4, OUTPUT); //бочка замок
  pinMode(relay_5, OUTPUT); //down
  pinMode(relay_6, OUTPUT); //medium
  pinMode(relay_7, OUTPUT); //up
  pinMode(relay_8, OUTPUT); //дверь входная
  pinMode(relay_9, OUTPUT); //стол замок
  pinMode(relay_10, OUTPUT); //сундук яйца
  pinMode(relay_11, OUTPUT); //потолок ключ
  pinMode(relay_12, OUTPUT); //комод левый
  pinMode(relay_13, OUTPUT); //комод правый
  pinMode(relay_14, OUTPUT);
  pinMode(relay_15, OUTPUT); //тайник меч или красный свет
  pinMode(relay_16, OUTPUT); //красные глаза дракона

  pinMode(u1, OUTPUT); //RED DRAGON ROOM
  pinMode(u2, OUTPUT); //GREEN DRAGON ROOM
  pinMode(u3, OUTPUT); //BLUE DRAGON ROOM
  pinMode(u4, OUTPUT); //lebadka inversiya (1-close) (0-open)
  pinMode(u5, OUTPUT); //lebedka 220
  pinMode(u6, OUTPUT); //рез реле
  pinMode(u7, OUTPUT); //полы дракон
  pinMode(un8, OUTPUT);

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
  pinMode(din_13, INPUT_PULLUP); //щит
  pinMode(din_14, INPUT_PULLUP);
  pinMode(din_15, INPUT_PULLUP);
  pinMode(din_16, INPUT_PULLUP);

  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  pinMode(s5, INPUT_PULLUP);
  pinMode(s6, INPUT_PULLUP);
  pinMode(s7, INPUT_PULLUP);
  //pinMode(s8, INPUT_PULLUP);

  pinMode(f1, INPUT_PULLUP);
  pinMode(f2, INPUT_PULLUP);
  pinMode(f3, INPUT_PULLUP);
  pinMode(f4, INPUT_PULLUP);

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :

  //--------------all_relay_off-----------------
  initrel();

  delay(100);
  test_shifer();
  //--------------all_relay_off-----------------

  lcd.setCursor(0, 0);
  lcd.print("|------------------|");
  lcd.setCursor(0, 1);
  lcd.print("|    VNEDRENIE     |");
  lcd.printByte(5);
  lcd.print("| ");
  lcd.setCursor(0, 2);
  lcd.print("|   SARATOV 2016   |");
  lcd.setCursor(0, 3);
  lcd.print("|------------------|");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("|------------------|");
  lcd.setCursor(0, 1);
  lcd.print("| Game of Thrones  |");
  lcd.setCursor(0, 2);
  lcd.print("|                  |");
  lcd.setCursor(0, 3);
  lcd.print("|------------------|");
  delay(500);
  lcd.clear();

  //-----SET_FIRST_TRACK--------
  mp3tf.setVolume (20);
  delay(50);
  mp3tf.play (1);
  delay(50);
  //-----SET_FIRST_TRACK--------

  //podnyat_lebedka();
  delay(100);

  unl_HL(7, true); //полы дракон
  unl_HL(6, false);
  digitalWrite(relay_3, LOW); //трон замок
  digitalWrite(relay_4, LOW); //бочка вкл
  digitalWrite(relay_9, LOW); //стол замок
  digitalWrite(relay_10, LOW); //сундук яйца
  digitalWrite(relay_11, LOW); //потолок-ключ
  digitalWrite(relay_12, LOW); //комод левый
  digitalWrite(relay_13, LOW); //комод правый


  //digitalWrite(relay_11, LOW); //

  //delay(5000);
  //opustit_lebedka();
  //delay(5000);
  //-----SET_FIRST_TRACK--------
  if(!track_1){
    delay(10);
    mp3tf.setVolume (20);
    delay(50);
    mp3tf.play (1);
    delay(50);
    track_1 = true;
  }
  //-----SET_FIRST_TRACK--------

  debouncer.attach(BUTTON_PIN);
  debouncer.interval(10); // interval in ms

  Timer1.initialize(300000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

//========================================================================================================================================================================================================================
void loop()
{

  timer();//считаем, рисуем время
  update_DI();


  //--------------start------------
  // Update the Bounce instance :
  debouncer.update();

  // Get the updated value :
  value = debouncer.read();

  if ( value == LOW ) {

    timer0_millis = 0;
    digitalWrite(led, HIGH);

    //-----SET_FIRST_TRACK--------
    if(!track_2){
      delay(10);
      mp3tf.setVolume (20);
      delay(50);
      mp3tf.play (2);
      delay(50);
      track_2 = true;
    }
    //-----SET_FIRST_TRACK--------

    delay(500);

    if(!flag){
      set_light_dragon_off(); 
      opustit_lebedka();
    }

    flag = true;
    //mp3tf.next();
  }
  //--------------start------------




  delay(1000);
  // Toggle LED
  //digitalWrite( led, digitalRead( led ) ^ 1 );



}
//========================================================================================================================================================================================================================


//******************** TIMER **********************
void timer() {


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

  //-------------anolog read-----------------
  lcd.setCursor(12, 0);
  lcd.print(outputValue, DEC);
  //-------------anolog read-----------------

  //------------гербы-------------
  if (s1_state) {
    lcd.setCursor(0, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(0, 2);
    lcd.print("1");
  }

  if (s2_state) {
    lcd.setCursor(1, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(1, 2);
    lcd.print("2");
  }

  if (s3_state) {
    lcd.setCursor(2, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(2, 2);
    lcd.print("3");
  }

  if (s4_state) {
    lcd.setCursor(3, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(3, 2);
    lcd.print("4");
  }

  if (s5_state) {
    lcd.setCursor(4, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(4, 2);
    lcd.print("5");
  }
  if (s6_state) {
    lcd.setCursor(5, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(5, 2);
    lcd.print("6");
  }

  if (s7_state) {
    lcd.setCursor(6, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(6, 2);
    lcd.print("7");
  }
  //  if (!s1_state && !s2_state && !s3_state && !s4_state && !s5_state && !s6_state && !s7_state){
  //  fl_gerby=true;
  //  }
  //------------гербы-------------


  //**************часы***************
  if (!din1_st) {  //1
    lcd.setCursor(14, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(14, 1);
    lcd.print("1");
  }

  if (!din2_st) {  //2
    lcd.setCursor(15, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(15, 1);
    lcd.print("2");
  }

  if (!din3_st) {  //3
    lcd.setCursor(16, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(16, 1);
    lcd.print("3");
  }

  if (!din4_st) {  //4
    lcd.setCursor(17, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(17, 1);
    lcd.print("4");
  }

  //**************часы***************


  //*************фонари**************
  if (f1_state) {  //4
    lcd.setCursor(0, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("1");
  }

  if (f2_state) {  //4
    lcd.setCursor(1, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(1, 1);
    lcd.print("2");
  }

  if (f3_state) {  //4
    lcd.setCursor(2, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(2, 1);
    lcd.print("3");
  }

  if (f4_state) {  //4
    lcd.setCursor(3, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(3, 1);
    lcd.print("4");
  }

  if (fl_fonari) {
    lcd.setCursor(4, 1);
    lcd.printByte(7);
  }
  else {
    lcd.setCursor(5, 1);
    lcd.print("_");
  }
  //*************фонари**************


  //**********морда-бочка*************
  if (!din10_st) {  //1
    lcd.setCursor(7, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(7, 1);
    lcd.print("1");
  }

  if (!din11_st) {  //1
    lcd.setCursor(8, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(8, 1);
    lcd.print("2");
  }

  if (!din10_st && !din11_st) {
    fl_bochka = true;
    unl_HL(6, true);
  }
  else {
    fl_bochka = false;
    unl_HL(6, false);
  }

  //**********морда-бочка*************


  //***********трон-поджопник*********
  if ((!din12_st) && (outputValue >= 5)) {  //1
    lcd.setCursor(0, 3);
    lcd.printByte(6);
    shifter.setPin(0, HIGH);
    shifter.write();
    fl_tron = true;
  }
  else {
    lcd.setCursor(0, 3);
    lcd.print("_");
    shifter.setPin(0, LOW);
    shifter.write();
    fl_tron = false;
  }

  //***********трон-поджопник*********


  //************череп*****************
  if (!din9_st) {  //1
    lcd.setCursor(12, 1);
    lcd.printByte(5);
    fl_shlem = true;
  }
  else {
    lcd.setCursor(12, 1);
    lcd.print("_");
    fl_shlem = false;
    //    lcd.print("1");
  }

  //************череп*****************


  //**********щит**********
  if (din13_st) {
    lcd.setCursor(8, 2);
    lcd.print("0");
    fl_shit = false;
  }
  else {
    lcd.setCursor(8, 2);
    lcd.print("8");
    fl_shit = true;
  }
  //************щит*********

  //**************клык***************

  if (din16_st) {
    lcd.setCursor(10, 2);
    lcd.print("k");
  }
  else {
    lcd.setCursor(10, 2);
    lcd.print("K");
    fl_klyk=true;  
  }
  //**************клык***************

  //**************факел***************
  if (din15_st) {
    lcd.setCursor(12, 2);
    lcd.print("f");
  }
  else {
    lcd.setCursor(12, 2);
    lcd.print("F");
  }
  //**************факел***************

  //**************дверь***************
  if (din7_st) {  //верхняя цепь
    lcd.setCursor(14, 2);
    lcd.print("u");
  }
  else {
    lcd.setCursor(14, 2);
    lcd.print("U");
  }

  if (din6_st) {  //средняя цепь
    lcd.setCursor(15, 2);
    lcd.print("m");
  }
  else {
    lcd.setCursor(15, 2);
    lcd.print("M");
  }

  if (din5_st) {  //нижняя цепь
    lcd.setCursor(16, 2);
    lcd.print("d");
  }
  else {
    lcd.setCursor(16, 2);
    lcd.print("D");
  }

  if (din8_st) {  //замок первой двери
    lcd.setCursor(18, 2);
    lcd.print("O");
    
     //-----SET7_TRACK--------
    if(!track_7){
      delay(10);
      mp3tf.setVolume (20);
      delay(50);
      mp3tf.play (7);
      delay(50);
      track_7 = true;
    }
    //-----SET_6_TRACK--------
    
  }
  else {
    lcd.setCursor(18, 2);
    lcd.print("C");
  }

  //**************дверь***************

}
//******************** TIMER **********************


//*************interrup**********************************************************
void timerIsr()
{

  faster();


  //*********RGB_Changed************
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
  //*******************************

}
//*************interrup**********************************************************

//--------------------------НЕ ЗАМЕДЛИТЕЛЬНО--------------------
void faster() {
  din5_st = digitalRead(din_5);
  if (din5_st) {
    digitalWrite(relay_5, 1);

  }
  else digitalWrite(relay_5, 0);

  din6_st = digitalRead(din_6);
  if (din6_st) {
    digitalWrite(relay_6, 1);

  }
  else digitalWrite(relay_6, 0);

  din7_st = digitalRead(din_7);
  if (din7_st){
    digitalWrite(relay_7, 1);
  }
  else digitalWrite(relay_7, 0);

  din8_st = digitalRead(din_8);
  if (din8_st){ 
    digitalWrite(relay_8, 1);

  }
  else digitalWrite(relay_8, 0);
}
//--------------------------НЕ ЗАМЕДЛИТЕЛЬНО--------------------



void update_DI() {

  din1_st = digitalRead(din_1);
  din2_st = digitalRead(din_2);
  din3_st = digitalRead(din_3);
  din4_st = digitalRead(din_4);
  din9_st = digitalRead(din_9);
  din10_st = digitalRead(din_10);
  din11_st = digitalRead(din_11);
  din12_st = digitalRead(din_12);
  din13_st = digitalRead(din_13);
  din14_st = digitalRead(din_14);
  din15_st = digitalRead(din_15);

  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 99);


  //клык
  din16_st = digitalRead(din_16);

  if (din16_st) {
    digitalWrite(relay_16, 1); 
    unl_HL(7, true);
  }
  else {
    digitalWrite(relay_16, 0);
    unl_HL(7, false);
  }
  //клык

  s1_state = digitalRead(s1);
  s2_state = digitalRead(s2);
  s3_state = digitalRead(s3);
  s4_state = digitalRead(s4);
  s5_state = digitalRead(s5);
  s6_state = digitalRead(s6);
  s7_state = digitalRead(s7);
  //s8_state = digitalRead(s8);

  f1_state = digitalRead(f1);
  f2_state = digitalRead(f2);
  f3_state = digitalRead(f3);
  f4_state = digitalRead(f4);

  // --4asy
  if (din1_st && din2_st && din3_st && din4_st) fl_clock = true;


  //-------------fonari
  if (fl_tron) { //----------------1 зажигает трон

  }
  else {
    //shifter.setPin(0, LOW);
    //shifter.write();
  }


  if (!f1_state && fl_tron) { //----------------2 зажигает 3-й
    shifter.setPin(1, HIGH);
    shifter.write();
  }
  else {
    shifter.setPin(1, LOW);
    shifter.write();
  }

  if (!f2_state && !f1_state && fl_tron) { //----------------3 зажигает 4-й
    shifter.setPin(2, HIGH);
    shifter.write();

  }
  else {
    shifter.setPin(2, LOW);
    shifter.write();
  }

  if (!f3_state && !f2_state && !f1_state && fl_tron) { //----------------хз
    shifter.setPin(3, HIGH);
    shifter.write();
    digitalWrite(relay_3, HIGH); //трон замок
    fl_fonari = true;
  }
  else {
    shifter.setPin(3, LOW);
    shifter.write();
  }

  //--------------fonari

  //         дезактивация****************************
  if (!s1_state && !s2_state && !s3_state && !s4_state && !s5_state && !s6_state && !s7_state) {
    if(!fl_gerby){

      //-----SET_5_TRACK--------
      if(!track_5){
        delay(10);
        mp3tf.setVolume (20);
        delay(50);
        mp3tf.play (5);
        delay(50);
        track_5 = true;
      }
      //-----SET_5_TRACK--------

      podnyat_lebedka();
      set_light_dragon_on();
      fl_gerby = true;
    }

  }


  if(fl_klyk && fl_gerby) {

    //-----SET6_TRACK--------
    if(!track_6){
      delay(10);
      mp3tf.setVolume (20);
      delay(50);
      mp3tf.play (6);
      delay(50);
      track_6 = true;
    }
    //-----SET_6_TRACK--------

  }


  //полы дракон

  if (fl_shit){
    digitalWrite(relay_9, 1);
  }
  if (fl_fonari)digitalWrite(relay_12, 1); // левый
  if (fl_clock) digitalWrite(relay_13, 1); // левый
  if (fl_bochka) {
    unl_HL(6, true);
    delay(2000);

    //-----SET_3_TRACK--------
    if(!track_3){
      delay(10);
      mp3tf.setVolume (20);
      delay(50);
      mp3tf.play (3);
      delay(50);
      track_3 = true;
    }
    //-----SET_3_TRACK--------

    digitalWrite(relay_4, 1);
  }

  else digitalWrite(relay_4, 0);

  if (fl_shlem) {
    digitalWrite(relay_11, 1);

    //-----SET_4_TRACK--------
    if(!track_4){
      delay(10);
      mp3tf.setVolume (20);
      delay(50);
      mp3tf.play (4);
      delay(50);
      track_4 = true;
    }
    //-----SET_4_TRACK--------  

  }
  else digitalWrite(relay_11, 0);
  if (fl_shit) digitalWrite(relay_9, 1);
  else digitalWrite(relay_9, 0);

  //         дезактивация****************************


  if (din1_st )

    if (din14_st == true) digitalWrite(relay_14, 1);
  if (din14_st == false) digitalWrite(relay_14, 0);
  //
  //
  //  if (din2_st) digitalWrite(relay_2, 1);
  //  else digitalWrite(relay_2, 0);
  //
  //
  //  if (din3_st) digitalWrite(relay_3, 1);
  //  else digitalWrite(relay_3, 0);


  //  if (din4_st) digitalWrite(relay_4, 1);
  //  else digitalWrite(relay_4, 0);
}

//************** UNL onboard ***************************************
int unl_HL(int setnum, boolean stat) {
  switch (setnum) {

  case 1:
    digitalWrite(u1, stat);
    break;

  case 2:
    digitalWrite(u2, stat);
    break;

  case 3:
    digitalWrite(u3, stat);
    break;

  case 4:
    digitalWrite(u4, stat);
    break;

  case 5:
    digitalWrite(u5, stat);
    break;

  case 6:
    digitalWrite(u6, stat);
    break;

  case 7:
    digitalWrite(u7, stat);
    break;

  case 8:
    digitalWrite(un8, stat);
    break;

  }

}
//************** UNL onboard ***************************************





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
//--------------------------TEST SHIFER--------------------
void test_shifer() {
  shifter.clear(); //set all pins on the shift register chain to LOW
  shifter.write(); //send changes to the chain and display them

  for (int d = 0; d < 8; d++) {
    shifter.setPin(d, HIGH);
    shifter.write();
    delay(200);
  }

  delay(500);

  shifter.clear(); //set all pins on the shift register chain to LOW
  shifter.write(); //send changes to the chain and display them


}
//--------------------------TEST SHIFER--------------------

//--------------------------TEST UNL--------------------
void testunl() {
  for (int i = 1; i < 9; i++) {
    unl_HL(i, true);
    delay(1000);
  }
  for (int i = 1; i < 9; i++) {
    unl_HL(i, false);
    delay(1000);
  }
}



void set_light_dragon_on() {
  unl_HL(1, true);
  unl_HL(2, true);
  unl_HL(3, true);
}

void set_light_dragon_off() {
  unl_HL(1, false);
  unl_HL(2, false);
  unl_HL(3, false);
}


void opustit_lebedka() {
  unl_HL(5, true);
  delay(200);
  unl_HL(4, true);
  delay(7000);
  unl_HL(5, false);
  unl_HL(4, false);

}


void podnyat_lebedka() {
  unl_HL(5, true);
  delay(200);
  unl_HL(4, false);
  delay(7000);
  unl_HL(5, false);
  unl_HL(4, false);
}

//--------------------------TEST UNL--------------------

































