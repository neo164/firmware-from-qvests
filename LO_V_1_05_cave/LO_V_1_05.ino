#include <mp3TF.h>
#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include "CyberLib.h"
#include <wiring.c>


//******************** OLED **********************
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//******************** OLED **********************


int stul_st=0, flag = 0;


mp3TF mp3tf = mp3TF ();


#define LEDPIN 13        // Вывод светодиода
#define veterPIN 3        //int5 2
#define vodaPIN 2         //int4 3
#define me4PIN 18         //int3
#define tmpPIN 19         //int2

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
relay_10 = 30, //
relay_11 = 33, //6
relay_12 = 32, //5
relay_13 = 35, //4
relay_14 = 34, //3
relay_15 = 37, //2
relay_16 = 36,

relay_17 = 7,
relay_18 = 6,
relay_19 = 5,
relay_20 = 4; //220v

//********************
int rub1_st = 1, rub2_st = 1, rub3_st = 1, rub4_st = 1, rub5_st = 1, rub6_st = 1, rub7_st = 0, rub8_st = 0, rub9_st = 0, rub10_st = 0, rub11_st = 0,
rub12_st = 0, rub13_st = 0, rub14_st = 0, rub15_st = 0, rub16_st = 0, rub17_st = 0;

const int rub_1 = 38;
const int rub_2 = 39;
const int rub_3 = 40;
const int rub_4 = 41;
const int rub_5 = 42;
const int rub_6 = 43;
const int rub_7 = 44; //меч 
const int rub_8 = 45;
const int rub_9 = 46;
const int rub_10 = 47; //рука л
const int rub_11 = 48; //рука п

const int rub_12 = 49; //кнопка стул
const int rub_13 = 50;
const int rub_14 = 51;
const int rub_15 = 52;
const int rub_16 = 53;
const int rub_17 = 8;

int cnt_0=0, cnt_1=0;
boolean stone_1=false, stone_2=false, peshera_in=false, peshera_out=false, mech=false, start_st=false, flag1=false,flag2=false,flag3=false,flag4=false, 
flag_5=false;
//********************

//-----------------------------------------
volatile int count = 0;  // Переменная счётчика (volatile означает указание компилятору не оптимизировать код её чтения,
// поскольку её значение изменяется внутри обработчика прерывания)

ISR(INT4_vect)
{
  count = 7;           // Инициализировать счётчик
}

// Режимы вызова прерывания INT0
#define INT4_SENSE_LOW_LEVEL     0        // Прерывание при низком уровне на выводе
#define INT4_SENSE_LEVEL_CHANGE  1        // Прерывание при изменении уровня
#define INT4_SENSE_FALLING_EDGE  2        // Прерывание по фронту на спад (когда 1 переходит в 0)
#define INT4_SENSE_RISING_EDGE   3        // Прерывание по фронту на подъём (когда 0 переходит в 1)

// Управляющая функция для прерывания INT0
//    mode    - режим вызова прерывания
//    enable  - разрешить/запретить прерывание
void int4Control (uint8_t mode, bool enable){
  EIMSK &= ~ (1 << INT4);                    // Запретить прерывание (так как следующая команда устанавливает режим INT0_SENSE_LOW_LEVEL)
  EICRA &= ~ (1 << ISC00) | (1 << ISC01);    // Обнуляем биты ISC00 и ISC01 в регистре EICRA
  EICRA |= mode;                             // Устанавливаем режим вызова прерывания INT0
  if (enable)
    EIMSK |= (1 << INT4);                    // Разрешить прерывание
}
//-----------------------------------------

void setup(){

  //вкл фоновую музыку
  mp3tf.init (&Serial);
  Serial.begin(9600);
  delay(500);
  mp3tf.play (1);
  delay(100);
  mp3tf.setVolume (21);

  //-------------***************************************************************************
  pinMode(LEDPIN, OUTPUT);                      // Вывод светодиода в режим вывода
  pinMode(veterPIN, INPUT_PULLUP);    // Вывод кнопки в режим ввода
  pinMode(vodaPIN, INPUT_PULLUP);

  delay(100);
  int4Control(INT4_SENSE_FALLING_EDGE, true);    // Разрешить прерывание Прерывание при изменении уровня (в данном случае при нажатии на кнопку)
  interrupts();   // Разрешить прерывания глобально

  //-----------******************************************************************************
  //pinMode(stul, INPUT_PULLUP);
  //******************** OLED **********************
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)init done
  display.display();
  delay(100);
  display.clearDisplay();
  //******************** OLED **********************


  //      блок реле
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

  pinMode(relay_17, OUTPUT);   
  pinMode(relay_18, OUTPUT);
  pinMode(relay_19, OUTPUT);
  pinMode(relay_20, OUTPUT);

  pinMode(rub_1, INPUT_PULLUP);
  pinMode(rub_2, INPUT_PULLUP);
  pinMode(rub_3, INPUT_PULLUP);
  pinMode(rub_4, INPUT_PULLUP);
  pinMode(rub_5, INPUT_PULLUP);
  pinMode(rub_6, INPUT_PULLUP);
  pinMode(rub_7, INPUT_PULLUP);
  pinMode(rub_8, INPUT_PULLUP);
  pinMode(rub_9, INPUT_PULLUP); //start
  pinMode(rub_10, INPUT_PULLUP);
  pinMode(rub_11, INPUT_PULLUP);
  pinMode(rub_12, INPUT_PULLUP);
  pinMode(rub_13, INPUT_PULLUP);
  pinMode(rub_14, INPUT_PULLUP);
  pinMode(rub_15, INPUT_PULLUP);
  pinMode(rub_16, INPUT_PULLUP);
  pinMode(rub_17, INPUT_PULLUP);
  initrel();
}

void loop(){
  //*******************************************
  //******************** TIMER **********************
  display.clearDisplay();
  int z=millis()/1000;
  ;

  if(flag){
    int time=millis()/1000;
    if (time/60/60<10) { 
      Serial.print ("0"); 
    }
    Serial.print (time/60/60);
    Serial.print (":");
    if (time/60%60<10) { 
      Serial.print ("0"); 
    }
    Serial.print ((time/60)%60);
    Serial.print (":");
    if (time%60<10) { 
      Serial.print ("0"); 
    }
    Serial.println (time%60);

    // draw time
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("B:");
    display.print(time/60/60,DEC);
    display.print(":");
    display.print((time/60)%60,DEC);
    display.print(":");
    display.print(time%60,DEC);
  }

  else{
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("START THE CAVE TRUE?"); 
  }

  //******************** TIMER **********************

  start_st = digitalRead(rub_9);
  if (start_st == LOW){
    timer0_millis=0; 
    digitalWrite(LEDPIN,HIGH);
    mp3tf.stop();
    delay(30);
    mp3tf.setVolume (18);
    digitalWrite(LEDPIN,LOW);
    delay(30);
    mp3tf.play (2);//ВТОРОЙ ТРЕК
    flag=true; 
  } 
  else digitalWrite(LEDPIN, LOW); 

  //******************** OLED DRAW **********************



  display.setTextSize(2);
  display.setCursor(0,17);
  if (z%2==0)
    display.print("1|2|3|4|5>");
  //display.setCursor(0,17);
  if(z%2>0)
    display.print("1|2|3|4|5-");

  //1
  display.setCursor(0,32);
  if(flag2){
    display.print("y");

  }
  else{ 
    display.print("n");
  }
  //2
  display.setCursor(23,32);
  if(peshera_in){
    display.print("y");
    Serial.println ("1");
  }
  else{ 
    display.print("n");
    Serial.println ("0");
  }
  //3
  display.setCursor(47,32);
  if(stone_1){
    display.print("y");
  }
  else{ 
    display.print("n");
  }
  //4
  display.setCursor(72,32);
  if(stone_2){
    display.print("y");
  }
  else{ 
    display.print("n");
  }

  //5
  display.setCursor(97,32);//ruki
  if(flag3){//flag3
    display.print("y");
  }
  else{ 
    display.print("n");
  }

  //----------------test
  display.setTextSize(1);
  display.setCursor(0,50);
  if(!rub1_st){
    display.print("1-1");

  }
  else{ 
    display.print("1-0");
  }
  //2
  display.setCursor(20,50);
  if(!rub2_st){
    display.print("2-1");
  }
  else{ 
    display.print("2-0");
  }
  //3
  display.setCursor(40,50);
  if(!rub3_st){
    display.print("3-1");
  }
  else{ 
    display.print("3-0");
  }
  //4
  display.setCursor(60,50);
  if(!rub4_st){
    display.print("4-1");
  }
  else{ 
    display.print("4-0");
  }
  //5
  display.setCursor(80,50);//
  if(!rub5_st){
    display.print("5-1");
  }
  else{ 
    display.print("5-0");
  }
  //6
  display.setCursor(100,50);
  if(!rub6_st){
    display.print("6-1");
  }
  else{ 
    display.print("6-0");
  }
  //******************** OLED DRAW **********************
  display.display();
  //*******************************************

  //-----------
  update_DI();
}


void update_DI() {
  // ********************** морды *********************
  rub1_st = digitalRead(rub_1);
  if (rub1_st) digitalWrite(relay_16, 1);
  else digitalWrite(relay_16, 0);

  rub2_st = digitalRead(rub_2);
  if (rub2_st) digitalWrite(relay_15, 1);
  else digitalWrite(relay_15, 0);

  rub3_st = digitalRead(rub_3);
  if (rub3_st) digitalWrite(relay_14, 1);
  else digitalWrite(relay_14, 0);

  rub4_st = digitalRead(rub_4);
  if (rub4_st) digitalWrite(relay_13, 1);
  else digitalWrite(relay_13, 0); 

  rub5_st = digitalRead(rub_5);
  if (rub5_st) digitalWrite(relay_12, 1);
  else digitalWrite(relay_12, 0);

  rub6_st = digitalRead(rub_6);
  if (rub6_st) digitalWrite(relay_11, 1);
  else digitalWrite(relay_11, 0);
  // ********************** морды *********************

  // условие открытия пещеры ------------------------переделано

  if (!(rub6_st || rub5_st || rub4_st || rub3_st || rub2_st || rub1_st)){//222222222222222222222222


    if(!peshera_in){
      count=0;
      digitalWrite(relay_17, LOW); //220 откл

      mp3tf.stop();
      delay(30);
      mp3tf.setVolume (18);
      digitalWrite(LEDPIN,HIGH);
      delay(30);
      mp3tf.play (4); // ОТКРЫТИЕ ПЕЩЕРЫ
      peshera_in=true;
      digitalWrite(LEDPIN,LOW);
      delay(10);
      count=0;
    }
    //else 

  }


  rub7_st = digitalRead(rub_7);  //меч - выход из пещеры
  if (rub7_st && peshera_in && flag3) {

    if(!peshera_out || flag3){   //*************************************************-*-*-*-*-*-*-*-*-*-*-*-*
      digitalWrite(relay_10, 1);
      digitalWrite(relay_3, 0);//зел голем
      digitalWrite(relay_1, 1);//красн голем 
      digitalWrite(relay_6, 0);
      digitalWrite(relay_7, 1);

if (!flag4){
      digitalWrite(LEDPIN,HIGH);
      mp3tf.stop();
      delay(30);
      mp3tf.setVolume (23);
      digitalWrite(LEDPIN,LOW);
      delay(30);
      mp3tf.play (5); //ФИНАЛЬНЫЙ ТРЕК
      peshera_out=true;
      flag4=true;
}
    } 
  }
  else {
    digitalWrite(relay_10, 0);
    digitalWrite(relay_3, 1);//зел голем
    digitalWrite(relay_6, 1);
    digitalWrite(relay_7, 0);
  }


  //******************************** ветер камень ***************************************
  rub8_st = digitalRead(rub_8); //ветер 
  if (rub8_st) cnt_0++;
  if (cnt_0 >= 25){
    if(!stone_1 || peshera_in){  //***********************************************************-*-*--*-**-*-*-*-*-*-*-*
      if(!flag_5){
      digitalWrite(relay_9, 0);
      delay(300);
      digitalWrite(relay_9, 1);
      delay(300);
      digitalWrite(relay_1, 0); //красн голем включить
      stone_1=true;
      flag_5=true;
      }
    }
    cnt_0=0;
  }
  //**************************************************************************************


  //*********************************** вода камень **************************************
  //rub9_st = digitalRead(vodaPIN);  // вода

  if(count==0) {       //******************************************************-*-*-*-*-*-*-*-*-*-*-*-*-*                     
    digitalWrite(LEDPIN, LOW);        // Выключить светодиод, если счётчик равен 0...
    digitalWrite(relay_8, HIGH); // 
  }

  else {
    if(peshera_in && !stone_2){
      if(stone_2==false){
      digitalWrite(LEDPIN, HIGH);       // ... иначе включить светодиод,
      digitalWrite(relay_8, LOW); //выбросить 2-й камень
      delay(100);
      digitalWrite(relay_8, HIGH);
      delay(100);
      --count;                         // и уменьшить счётчик на 1.
      stone_2=true;
      }  
  }
  }
  if (!stone_2){

  }
  //***************************************************************************

  //************************** камни в руки ***********************************
  rub10_st = digitalRead(rub_10);
  rub11_st = digitalRead(rub_11);//руки 

  if (!rub10_st || !rub11_st) {

  }

  else { 
    flag3=true;
  }
  //***************************************************************************


  //************************** кнопки оператора *******************************
  // превью
  rub12_st = digitalRead(rub_12);
  if (!rub12_st) {
    digitalWrite(LEDPIN,HIGH);
    mp3tf.stop();
    delay(30);
    mp3tf.setVolume (15);
    digitalWrite(LEDPIN,LOW);
    delay(30);
    mp3tf.play (3);  //включить 3-й трек ИСТОРИЯ
    flag2=true;

  }


  rub13_st = digitalRead(rub_13);
  if (!rub13_st) {
    digitalWrite(LEDPIN,HIGH);
    mp3tf.stop();
    delay(30);
    mp3tf.setVolume (15);
    digitalWrite(LEDPIN,LOW);
    delay(30);
    mp3tf.play (4); 

    digitalWrite(relay_17, LOW); //220 откл
    peshera_in=true;


  }

  rub14_st = digitalRead(rub_14);
  if (!rub14_st) {

    digitalWrite(relay_9, 0);
    delay(300);
    digitalWrite(relay_9, 1);
    delay(300);
    digitalWrite(relay_1, 0); //красн голем включить
    stone_1=true;  

  }
  //
  rub15_st = digitalRead(rub_15);
  if (!rub15_st) {

    digitalWrite(relay_8, LOW); //выбросить 2-й камень
    delay(300);
    digitalWrite(relay_8, HIGH); //выбросить 2-й камень
    delay(300);
    count=0;                         // и уменьшить счётчик на 1.
    stone_2=true;

  }

  rub16_st = digitalRead(rub_16);
  if (!rub16_st) { //***********************************************-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


    //????????????????????????????????????????????????????????????????????????
if(!peshera_out || flag3)
    digitalWrite(relay_10, 1);
    digitalWrite(relay_3, 0);//зел голем
    digitalWrite(relay_1, 1);//красн голем 
    digitalWrite(relay_6, 0);
    digitalWrite(relay_7, 1);

    digitalWrite(LEDPIN,HIGH);
    mp3tf.stop();
    delay(30);
    mp3tf.setVolume (23);
    digitalWrite(LEDPIN,LOW);
    delay(30);
    mp3tf.play (5); //ФИНАЛЬНЫЙ ТРЕК
    peshera_out=true; 
      
  }

  rub17_st = digitalRead(rub_17);
  if (!rub17_st) {

    flag3=true;

  }
  //*****************************************************************************
  if(flag3)digitalWrite(relay_4, 0);

}


void initrel(){                // обратное вкл
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

  digitalWrite(relay_17, HIGH); //220 прямое вкл
  digitalWrite(relay_18, LOW);
  digitalWrite(relay_19, LOW);
  digitalWrite(relay_20, LOW);
}
































