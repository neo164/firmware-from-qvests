#include <mp3TF.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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



mp3TF mp3tf = mp3TF ();

//-------замки---------out
int zamok_vhod=1;
int zamok_garaj=1;
int zamok_kitchen=1;
int zamok_holod=1;
int zamok_ritual_in=1;
int zamok_ritual_out=1;

//-----рубильники-------in
const int rub_1 = 22;
const int rub_2 = 24;
const int rub_3 = 26;
const int rub_4 = 28;
const int rub_5 = 30;
const int rub_6 = 32;
const int rub_7 = 34;
const int rub_8 = 36;
const int rub_9 = 38;
//   лампы рубильника   oout
const int 
ledrub_1 = 40, 
ledrub_2 = 42,
ledrub_3 = 44,
ledrub_4 = 46,
ledrub_5 = 48;   

//-----блок реле--------
const int 
relay_1 = 23, //вход М
relay_2 = 25, //дверь на улицу Щ
relay_3 = 27, //дверь на кухню М
relay_4 = 29, //дверь в рубильники Щ
relay_5 = 31, //дверь в ритуальную М 220
relay_6 = 33, //выход из ритуальной
relay_7 = 35, //свет кухня 220В
relay_8 = 37,
relay_9 = 39, //потай магнит
relay_10 = 41, //потай вверх вних
relay_11 = 43, //саркофаг шасси
relay_12 = 45, //откл усилка гараж
relay_13 = 47,
relay_14 = 49, //красный
relay_15 = 51, //синий    ритуальная RGB 
relay_16 = 53; //зеленый



char incoming1Byte;

unsigned long time;

int rub1_st = 0, rub2_st = 0, rub3_st = 0, rub4_st = 0, rub5_st = 0, rub6_st = 0, rub7_st = 0, rub8_st = 0, rub9_st = 0;

const int buttonPin = 2;     //выезд саркофага
const int vodaPin = 3;       //в раковине поплавок-геркон
const int rebusPin = 8;     //саркофф ребус
const int rebusPin1 = 9;
const int rebusPin2 = 10;

const int kitchenPin = 5;     //саркофф ребус *******************
const int veaponPin = 5; //оружие             *******************
const int ledPin =  13;      // the number of the LED pin
const int rubPin =  6; //если что-то с рубильниками
const int start = 7; //start button

// variables will change:
int buttonState = 0;         // кнопки напольные выезд саркофага
int vodaState = 0;           //вода в раковину слливалась
boolean rebusState =1;
boolean rebusState1 =1;
boolean rebusState2 =1;

int rubState = 0; // статус рубильников 

boolean flagholod = false;
boolean flagrebus = false;
boolean svetflag = false;
boolean kitchenFlag = false;
boolean rubflag = false;
boolean one = false,  two = false, three = false, four = false, five = false;
boolean music1 = false, music2 = false, music3 = false, music4 = false, music5 = false, music6 = false, music7 = false;
boolean start_st = false;
boolean starter = false;

int track = 0;
void setup() {

  mp3tf.init (&Serial);

  Serial1.begin(38400);

  Serial.begin(9600);
  //88888888888888888888888888888888888888888888888888888888888888888888  
  delay(500);
  mp3tf.setVolume (15);
  delay(100);
  mp3tf.play (1); //звук улицы
  delay(50);
  //88888888888888888888888888888888888888888888888888888888888888888888


  //******************** OLED **********************
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)init done
  display.display();
  delay(100);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("VNEDRENIYE");
  display.setCursor(0,25);
  display.print("   2014   ");
  display.display(); 
  track=1;
  //******************** OLED **********************

  pinMode(start, INPUT_PULLUP);



  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT); 
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
  //светодиоды рубильника - подсказки
  pinMode(ledrub_1, OUTPUT);   
  pinMode(ledrub_2, OUTPUT);   
  pinMode(ledrub_3, OUTPUT);   
  pinMode(ledrub_4, OUTPUT);
  pinMode(ledrub_5, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(vodaPin, INPUT);
  pinMode(kitchenPin, INPUT);
  pinMode(rub_1, INPUT);
  pinMode(rub_2, INPUT);
  pinMode(rub_3, INPUT);
  pinMode(rub_4, INPUT);
  pinMode(rub_5, INPUT);
  pinMode(rub_6, INPUT);
  pinMode(rub_7, INPUT);
  pinMode(rub_8, INPUT);
  pinMode(rub_9, INPUT); //геркон на тайник в гараже
  pinMode(rubPin, INPUT); // если что-то не так с рубильником

  initrel();
  digitalWrite(rebusPin, INPUT_PULLUP);
  digitalWrite(rebusPin1, INPUT_PULLUP);
  digitalWrite(rebusPin2, INPUT_PULLUP);

  digitalWrite(buttonPin, HIGH);
  digitalWrite(kitchenPin, HIGH);
  digitalWrite(rubPin, HIGH);

  digitalWrite(relay_14, LOW);//красный
  digitalWrite(relay_9, LOW); //магнит потай
  digitalWrite(relay_6, LOW); //магнит выход из ритуальной ВКЛ
  digitalWrite(relay_1, LOW); //магнит входной ВКЛ
  digitalWrite(relay_3, LOW); //магнит вход на кухню

  digitalWrite(relay_8, LOW); //разрешить саркофаг
  delay(30000);
  digitalWrite(relay_8, HIGH);
}

void loop(){

  //******************** TIMER **********************
  display.clearDisplay();
  int z=millis()/1000;

  if(starter){
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
    display.setCursor(0,25);
    display.print("Track:");
    display.print(track,DEC);
    display.display();
    
  }

  else{
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("START     BLADE?");
    display.display(); 
  }

  //******************** TIMER **********************
  //-------------------------------start button
  start_st = digitalRead(start);
  if (start_st == LOW){

    if(!starter){
      timer0_millis = 0;  
      mp3tf.stop();
      delay(30);
      mp3tf.setVolume (20);
      delay(30);
      mp3tf.play (2);//ВТОРОЙ ТРЕК
      track = 2;
      starter=true; 
    } 
  }





  time = millis();
  mainbt();
  rubi();
  kitchenFlag = digitalRead(kitchenPin);
  //digitalWrite(ledrub_1, HIGH);
  //digitalWrite(ledPin, HIGH);

  //  Serial1.println('1');
  //  Serial.println(time);
  //  delay(500);
  //  Serial1.println('0');
  //  delay(500);
  //
  //  if (Serial1.available() > 0) {  //если пришли данные
  //    incoming1Byte = Serial1.read(); // считываем байт
  //    if(incoming1Byte == '1') {
  //      //digitalWrite(relay_3, HIGH); //дверь на кухню
  //      //digitalWrite(relay_7, LOW); //свет кухня
  //      Serial1.println("Veapon inside case");  // и выводим обратно сообщение
  //    }
  //    if(incoming1Byte == '0') {
  //
  //      Serial1.println("Not veapons");
  //    }
  //  }
  //  

  if (!kitchenFlag){
    digitalWrite(relay_3, HIGH); //дверь на кухню
    digitalWrite(relay_7, LOW); //свет кухня
    //8888888888888888888888888888888888888---------------------kitchen is opened....---------------------------
    if(music3==false){
      delay(100);
      mp3tf.setVolume (14);
      delay(100);
      mp3tf.play (3); //звук улицы
      track = 3;
      delay(100);
      music3=true;
    }
    //88888888888888888888888888888888888888888888888888888888888888888888
  }

}

//*******************************************************
void rubi(){

  rub1_st = digitalRead(rub_1);
  rub2_st = digitalRead(rub_2);
  rub3_st = digitalRead(rub_3);
  rub4_st = digitalRead(rub_4);
  rub5_st = digitalRead(rub_5);
  rub6_st = digitalRead(rub_6);
  rub7_st = digitalRead(rub_7);
  rub8_st = digitalRead(rub_8);
  rubflag = digitalRead(rubPin);

  //-------------------------------

  //     led1
  if ((((rub1_st && rub3_st) && (!rub4_st && rub8_st) && rub7_st && (rub5_st == HIGH) && (rub6_st == LOW) && !rub2_st) == HIGH) || ((rubflag) == LOW))
  {
    digitalWrite(ledrub_1, HIGH);
    delay(300);

    flagholod = true;

    if (!two) {
      //888888888888888888888888888888888888888-------------------rubilniki is true------------------
      if(music5==false){
        mp3tf.setVolume (20);
        delay(100);
        mp3tf.play (5);
        track = 5;
        delay(100);
        digitalWrite(relay_12, LOW);
        delay(100);
        digitalWrite(relay_5, LOW); 
        music5=true;
      }
    } // вход ритуальную

    two=true;
  }  
  else
  {
    digitalWrite(ledrub_1, LOW); 

  }



  //        led2
  if ((rub1_st && rub3_st && !rub2_st) == HIGH) {     
    // turn LED on:    
    digitalWrite(ledrub_2, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledrub_2, LOW); 
  }


  //     led3
  if ((!rub4_st && rub8_st && !rub2_st) == HIGH) {     
    // turn LED on:    
    digitalWrite(ledrub_3, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledrub_3, LOW); 
  }  


  //    led4
  if ((rub7_st && !rub2_st)  == HIGH) {     
    // turn LED on:    
    digitalWrite(ledrub_4, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledrub_4, LOW); 
  }


  //    led5
  if ((rub5_st == HIGH) && (rub6_st == LOW) && !rub2_st) {     
    // turn LED on:    
    digitalWrite(ledrub_5, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledrub_5, LOW); 
  }



}

//************************************************
void mainbt(){
  // read the state of the pushbutton value:

  rub9_st = digitalRead(rub_9); //геркон на тайник в гараже
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if ((rub9_st == HIGH) && (time >= 10000)) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
    digitalWrite(relay_9, HIGH);
    delay(500);
    digitalWrite(relay_10, LOW);
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }

  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:

  if ((buttonState == LOW) && (!three && two)) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);
    svetRGB();
    //88888888888888888888888888888888888888888888888888888888888888888888    
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.setVolume (21);
    delay(100);
    mp3tf.play (6); //выезд саркофага /*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*
    track = 6;
    //88888888888888888888888888888888888888888888888888888888888888888888
    //*************************************************выезд саркофага
    delay(1000);  
    digitalWrite(relay_11, LOW);
    digitalWrite(relay_8, LOW);
    delay(35000);
    digitalWrite(relay_8, HIGH);
    //digitalWrite(relay_10, LOW);
    three=true;
  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }
  //--------------------------------vater in tank-----------------------------------------------------------------------------
  vodaState =  digitalRead (vodaPin);
  if ((vodaState == HIGH) && (!flagholod)) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH);
    digitalWrite(relay_4, LOW);
    delay(2000);  
    digitalWrite(relay_4, HIGH);
    delay(3000);
    //88888888888888888888888888888888888888888888888888888888888888888888
    if(music4==false){
      delay(100);
      mp3tf.stop();
      delay(100);
      mp3tf.setVolume (23);
      delay(100);
      mp3tf.play (4);
      track = 4;
      music4=true;
    }
    //88888888888888888888888888888888888888888888888888888888888888888888
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }

  if (flagholod) digitalWrite(relay_7,HIGH); //свет  на кухне


  rebusState = digitalRead(rebusPin); //герконы
  rebusState1 = digitalRead(rebusPin1);
  rebusState2 = digitalRead(rebusPin2);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if ((!flagrebus) && (two==true)&&(three==true)){//**************************************************************
    if ((rebusState && rebusState1 && rebusState2) == LOW) {     
      // turn LED on:    
      digitalWrite(ledPin, HIGH);  
      digitalWrite(relay_6, HIGH);
      digitalWrite(relay_1, HIGH);
      delay(500);

      //    if (!four) mp3tf.play (4); //выезд саркофага
      //      //    four=true;
      //      delay(5);
      //      mp3tf.setVolume (20);
      //      delay(5);
      //88888888888888888888888888888888888888888888888888888888888888888888


      mp3tf.play (7);//*************************************------------------final track------------------------------
      track = 7;
      illumination();
      digitalWrite(relay_13, LOW);
      flagrebus = true;
      //digitalWrite(relay_13, LOW);//******************************************************************************************************************************************
      //88888888888888888888888888888888888888888888888888888888888888888888  
    } 
    else {
      // turn LED off:
      digitalWrite(ledPin, LOW); 
    }
  }



}
//*************************************************
void svetRGB(){
  digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);

}


void illumination(){
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(200);
  digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(200);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  digitalWrite(relay_14, LOW);
  //digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(100);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  //digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(50);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(200);
  digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  //digitalWrite(relay_16, LOW);
  delay(200);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  //digitalWrite(relay_14, LOW);
  //digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(300);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  digitalWrite(relay_14, LOW);
  //digitalWrite(relay_15, LOW);
  //digitalWrite(relay_16, LOW);
  delay(500);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(200);
  digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(400);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  //digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  //digitalWrite(relay_16, LOW);
  delay(300);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(250);
  digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(200);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(200);
  digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(200);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  digitalWrite(relay_14, LOW);
  //digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(100);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  //digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);
  delay(50);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(200);
  digitalWrite(relay_14, LOW);
  digitalWrite(relay_15, LOW);
  //digitalWrite(relay_16, LOW);
  delay(200);
  digitalWrite(relay_14, HIGH);
  digitalWrite(relay_15, HIGH);
  digitalWrite(relay_16, HIGH);
  delay(100);
  //digitalWrite(relay_14, LOW);
  //digitalWrite(relay_15, LOW);
  digitalWrite(relay_16, LOW);


}


void initrel(){
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

}


















