


#define BUTTON_PIN 12 //onboard button


//-----------------onboard UNL---------------
const int u1 = A0, u2 = A1, u3 = A2, u4 = A3, u5 = A4, u6 = A5, u7 = A6, un8 = A7,

s1 = A8, s2 = A9, s3 = A10, s4 = A11, s5 = A12, s6 = A13, s7 = A14;

int s1_st = 0, s2_st = 0, s3_st = 0, s4_st = 0, s5_st = 0, s6_st = 0;

//-----------------onboard UNL---------------



int dly = 250;

int y = 0, x = 0;

int led = 13;
int R = 5, G = 6, B = 7;
int RGB = 0;
boolean flag = false, start_st = false,

box1 = false,
box2 = false,
box3 = false,
box4 = false;

int
din1_st = 0, din2_st = 0, din3_st = 0, din4_st = 0, din5_st = 0, din6_st = 0, din7_st = 0, din8_st = 0,
din9_st = 0, din10_st = 0, din11_st = 0,  din12_st = 0,  din13_st = 0,  din14_st = 0,  din15_st = 0,  din16_st = 0;


//-----блок реле--------
const int
relay_1 = 53,
relay_2 = 52,
relay_3 = 51,
relay_4 = 50,
relay_5 = 49,
relay_6 = 48,
relay_7 = 47,
relay_8 = 46,
relay_9 = 45,
relay_10 = 44,
relay_11 = 43,
relay_12 = 42,
relay_13 = 41,
relay_14 = 40,
relay_15 = 39,
relay_16 = 38;

//-----блок реле--------


//-----входа------------
const int
din_1 = 37,
din_2 = 36,
din_3 = 35,
din_4 = 34,
din_5 = 33,
din_6 = 32,
din_7 = 31,
din_8 = 30,
din_9 = 29,
din_10 = 28,
din_11 = 27,
din_12 = 26,
din_13 = 25,
din_14 = 24,
din_15 = 23,
din_16 = 22;
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
  pinMode(relay_9, OUTPUT);
  pinMode(relay_10, OUTPUT);
  pinMode(relay_11, OUTPUT);
  pinMode(relay_12, OUTPUT);
  pinMode(relay_13, OUTPUT);
  pinMode(relay_14, OUTPUT);
  pinMode(relay_15, OUTPUT);
  pinMode(relay_16, OUTPUT);

  pinMode(u1, OUTPUT); //up
  pinMode(u2, OUTPUT);
  pinMode(u3, OUTPUT);
  pinMode(u4, OUTPUT); //down
  pinMode(u5, OUTPUT); //table

  pinMode(u6, OUTPUT);


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

  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT_PULLUP);
  pinMode(s6, INPUT_PULLUP);

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  delay(500);
  digitalWrite( u1, 1);
  digitalWrite( u2, 1);
  digitalWrite( u3, 1);
  digitalWrite( u4, 1);
  digitalWrite( u5, 1);


}

//*************************************************************
void loop()
{
  Serial.print(y,HEX);
  x = random(1000,10000);
  y++;

  update_DI();


  // Toggle LED
  //digitalWrite( led, digitalRead( led ) ^ 1 );
  symbol();
  //  digitalWrite( u1, digitalRead( u1 ) ^ 1 );
  //  digitalWrite( u2, digitalRead( u2 ) ^ 1 );
  //  digitalWrite( u3, digitalRead( u3 ) ^ 1 );
  //  digitalWrite( u4, digitalRead( u4 ) ^ 1 );
 
  //if((s1_st)&&(s2_st)&&(s3_st)&&(s4_st)) {

  if(y>(x-300)){//500
    digitalWrite( u6, 1);
    Serial.println();
    Serial.print("_DATE_ID_COLOR___31.10.2015___");
    Serial.print("04.10.2015");
    Serial.print("___06.07.2016___DATE_ID_COLOR_");
    Serial.println(); 
}
  
  else 
    digitalWrite( u6, 0);
  if(y > x) y = 0;
  // }
  delay(20);//от




}
//*************************************************************

//------------symbol--------------
void symbol() {

  // it's all
  if (digitalRead(relay_1) && digitalRead(relay_2) && digitalRead(relay_3) && digitalRead(relay_4) && digitalRead(relay_5) && digitalRead(relay_6) && digitalRead(relay_7) && digitalRead(relay_8) &&
    digitalRead(relay_9) && digitalRead(relay_10) && digitalRead(relay_11) && digitalRead(relay_12) && digitalRead(relay_13) && digitalRead(relay_14) && digitalRead(relay_15) && digitalRead(relay_16))  {
    Serial.println("");
    Serial.println("Opps! Seen selection, it may be hacking...");
    delay(5000);
    Serial.println("ERROR! INVALID PASSWORD! SYSTEM IS BLOCKED!");
    reset_relay();
    
    delay(10000);
    
    Serial.println("ERROR! INVALID PASSWORD! SYSTEM IS BLOCKED!");
    delay(10000);
    
    Serial.println("ERROR! INVALID PASSWORD! SYSTEM IS BLOCKED!");
    delay(10000);

    digitalWrite( led, 1);

  }

  else digitalWrite( led, 0);

  // it's all

  //     S1
  if(!box1 && !digitalRead(relay_1) && !digitalRead(relay_2) && !digitalRead(relay_8)){
    
  if ((!digitalRead(relay_1) && !digitalRead(relay_2) && digitalRead(relay_3) && digitalRead(relay_4) && !digitalRead(relay_5) && !digitalRead(relay_6) && !digitalRead(relay_7) && !digitalRead(relay_8) &&
    digitalRead(relay_9) && digitalRead(relay_10) && !digitalRead(relay_11) && !digitalRead(relay_12) && digitalRead(relay_13) && digitalRead(relay_14) && digitalRead(relay_15) && !digitalRead(relay_16)))
  {
    box1 = true;
    digitalWrite( u1, 0);
    delay(1000);
    reset_relay();
  }

  else ;//digitalWrite( u1, 1);
  }
  //     S1

    //     S2
    if((box1) && (!box2)){
  if ((digitalRead(relay_1) && digitalRead(relay_2) && digitalRead(relay_3) && digitalRead(relay_4) && !digitalRead(relay_5) && digitalRead(relay_6) && digitalRead(relay_7) && !digitalRead(relay_8) &&
    !digitalRead(relay_9) && digitalRead(relay_10) && digitalRead(relay_11) && !digitalRead(relay_12) && digitalRead(relay_13) && digitalRead(relay_14) && digitalRead(relay_15) && digitalRead(relay_16)))
  {
    box2 = true;
    digitalWrite( u2, 0);
    delay(1000);
    reset_relay();
  }

  else ;//digitalWrite( u2, 1);
    }
  //     S2

  //      S3
  if(box1 && box2 && !box3){
  if ((digitalRead(relay_1) && !digitalRead(relay_2) && !digitalRead(relay_3) && digitalRead(relay_4) && digitalRead(relay_5) && !digitalRead(relay_6) && !digitalRead(relay_7) && digitalRead(relay_8) &&
    !digitalRead(relay_9) && digitalRead(relay_10) && digitalRead(relay_11) && !digitalRead(relay_12) && digitalRead(relay_13) && !digitalRead(relay_14) && !digitalRead(relay_15) && digitalRead(relay_16)))
  { 
    box3 = true;
    digitalWrite( u3, 0);
    delay(1000);
    reset_relay();
  }

  else ;//digitalWrite( u3, 1);
  }
  //      S3

  //      S4
  if(box1 && box2 && box3 && !box4){
  if ((digitalRead(relay_1) && digitalRead(relay_2) && digitalRead(relay_3) && digitalRead(relay_4) && digitalRead(relay_5) && !digitalRead(relay_6) && !digitalRead(relay_7) && digitalRead(relay_8) &&
    !digitalRead(relay_9) && !digitalRead(relay_10) && !digitalRead(relay_11) && digitalRead(relay_12) && digitalRead(relay_13) && digitalRead(relay_14) && digitalRead(relay_15) && digitalRead(relay_16)))
  { 
    box4 = true;
    digitalWrite( u4, 0);
    delay(1000);
    reset_relay();

  }

  else ;// digitalWrite( u4, 1);
  }
  //      S4  
}
//------------symbol--------------


void update_DI() {

  s1_st = digitalRead(s1);
  s2_st = digitalRead(s2);
  s3_st = digitalRead(s3);
  s4_st = digitalRead(s4);
  s5_st = digitalRead(s5);
  s6_st = digitalRead(s6);

  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*  table
  if((s1_st)&&(s2_st)&&(s3_st)&&(s4_st)) {
    digitalWrite( u5, 0);

  }
  else {
    digitalWrite( u5, 1);
  }
  //*-*-**-*-*-*-*-*-*-*-*-*-*-*- table

  //----------------------------

  din1_st = digitalRead(din_1);
  if (!din1_st) {
    digitalWrite( relay_1, digitalRead( relay_1 ) ^ 1 );
    Serial.println("pressed 1");
    delay(dly);
  }
  din2_st = digitalRead(din_2);

  if (!din2_st) {
    digitalWrite( relay_2, digitalRead( relay_2 ) ^ 1 );
    Serial.println("pressed 2");
    delay(dly);
  }
  din3_st = digitalRead(din_3);
  if (!din3_st) {
    digitalWrite( relay_3, digitalRead( relay_3 ) ^ 1 );
    Serial.println("pressed 3");
    delay(dly);
  }
  din4_st = digitalRead(din_4);
  if (!din4_st) {
    digitalWrite( relay_4, digitalRead( relay_4 ) ^ 1 );
    Serial.println("pressed 4");
    delay(dly);
  }
  din5_st = digitalRead(din_5);
  if (!din5_st) {
    digitalWrite( relay_5, digitalRead( relay_5 ) ^ 1 );
    Serial.println("pressed 5");
    delay(dly);
  }
  din6_st = digitalRead(din_6);
  if (!din6_st) {
    digitalWrite( relay_6, digitalRead( relay_6 ) ^ 1 );
    Serial.println("pressed 6");
    delay(dly);
  }
  din7_st = digitalRead(din_7);
  if (!din7_st) {
    digitalWrite( relay_7, digitalRead( relay_7 ) ^ 1 );
    Serial.println("pressed 7");
    delay(dly);
  }
  din8_st = digitalRead(din_8);
  if (!din8_st) {
    digitalWrite( relay_8, digitalRead( relay_8 ) ^ 1 );
    Serial.println("pressed 8");
    delay(dly);
  }
  din9_st = digitalRead(din_9);
  if (!din9_st) {
    digitalWrite( relay_9, digitalRead( relay_9 ) ^ 1 );
    Serial.println("pressed 9");
    delay(dly);
  }
  din10_st = digitalRead(din_10);
  if (!din10_st) {
    digitalWrite( relay_10, digitalRead( relay_10 ) ^ 1 );
    Serial.println("pressed 10");
    delay(dly);
  }
  din11_st = digitalRead(din_11);
  if (!din11_st) {
    digitalWrite( relay_11, digitalRead( relay_11 ) ^ 1 );
    Serial.println("pressed 11");
    delay(dly);
  }
  din12_st = digitalRead(din_12);
  if (!din12_st) {
    digitalWrite( relay_12, digitalRead( relay_12 ) ^ 1 );
    Serial.println("pressed 12");
    delay(dly);
  }
  din13_st = digitalRead(din_13);
  if (!din13_st) {
    digitalWrite( relay_13, digitalRead( relay_13 ) ^ 1 );
    Serial.println("pressed 13");
    delay(dly);
  }
  din14_st = digitalRead(din_14);
  if (!din14_st) {
    digitalWrite( relay_14, digitalRead( relay_14 ) ^ 1 );
    Serial.println("pressed 14");
    delay(dly);
  }

  din15_st = digitalRead(din_15);
  if (!din15_st) {
    digitalWrite( relay_15, digitalRead( relay_15 ) ^ 1 );
    Serial.println("pressed 15");
    delay(dly);
  }
  din16_st = digitalRead(din_16);
  if (!din16_st) {
    digitalWrite( relay_16, digitalRead( relay_16 ) ^ 1 );
    Serial.println("pressed 16");
    delay(dly);
  }

}





void reset_relay(){

  digitalWrite(relay_1, 0);
  digitalWrite(relay_2, 0);
  digitalWrite(relay_3, 0);
  digitalWrite(relay_4, 0);
  digitalWrite(relay_5, 0);
  digitalWrite(relay_6, 0);
  digitalWrite(relay_7, 0);
  digitalWrite(relay_8, 0);
  digitalWrite(relay_9, 0);
  digitalWrite(relay_10, 0);
  digitalWrite(relay_11, 0);
  digitalWrite(relay_12, 0);
  digitalWrite(relay_13, 0);
  digitalWrite(relay_14, 0);
  digitalWrite(relay_15, 0);
  digitalWrite(relay_16, 0);
}





























