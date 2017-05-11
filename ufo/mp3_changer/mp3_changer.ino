
#include <mp3TF.h>


unsigned long pusk=0;
int led = 13, di_2 = 2, di_3 = 3, di_4 = 4, di_5 = 5, di_6 = 6, di_7 = 7, di_8 = 8, di_9 = 9;
int randNumber;

boolean di_2_st = false, di_3_st = false, di_4_st = false, di_5_st = false, di_6_st = false, di_7_st = false,
di_8_st = false, di_9_st = false;

int track = 0;

mp3TF mp3tf = mp3TF ();


//
void setup () {
  
  Serial.begin (9600);
  mp3tf.init (&Serial);
  
  pinMode(led, OUTPUT);
  pinMode(di_2, INPUT_PULLUP);
  pinMode(di_3, INPUT_PULLUP);
  pinMode(di_4, INPUT_PULLUP);
  pinMode(di_5, INPUT_PULLUP);
  pinMode(di_6, INPUT_PULLUP);
  pinMode(di_7, INPUT_PULLUP);
  pinMode(di_8, INPUT_PULLUP);
  pinMode(di_9, INPUT_PULLUP);

  randomSeed(analogRead(3));

  
  delay(4000);
  mp3tf.stop();
  delay(100);
  mp3tf.setVolume (25);
  delay(100);
  mp3tf.play (1);
}

//
void loop () {

  di_updater();

  if((!di_2_st) && (track != 2)){
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    mp3tf.setVolume (25);
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.play (2);
    track = 2;
  }


  if((!di_3_st) && (track != 3)){
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(10);
    mp3tf.setVolume (25);
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.play (3);
    track = 3;
  }

  if((!di_4_st) && (track != 4)){
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    mp3tf.setVolume (25);
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.play (4);
    track = 4;
  }

  if((!di_5_st) && (track != 5)){
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    mp3tf.setVolume (25);
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.play (5);
    track = 5;
  }

  if((!di_6_st) && (track != 6)){
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    mp3tf.setVolume (25);
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.play (6);
    track = 6;
  }

  if((!di_7_st) && (track != 7)){
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    mp3tf.setVolume (25);
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.play (7);
    track = 7;
  }
  if(!di_8_st){

  }

  if((!di_9_st) && (track != 1)){
    
    delay(100);
    mp3tf.stop();
    delay(100);
    mp3tf.play (1);
    track = 1;
  }






  pusk++;

  //  if (pusk==1000) {
  //    //mp3tf.play (1);
  //    digitalWrite(led, HIGH);
  //    delay(1000);
  //    digitalWrite(led, LOW);
  //
  //  }


  //  if (pusk==100) {
  //    leder();
  //  }
  if (pusk >= 2000)pusk=0;

  delay(100);

}


void di_updater(){


  di_2_st = digitalRead(di_2);
  di_3_st = digitalRead(di_3);
  di_4_st = digitalRead(di_4);
  di_5_st = digitalRead(di_5);
  di_6_st = digitalRead(di_6);
  di_7_st = digitalRead(di_7);
  di_8_st = digitalRead(di_8);
  di_9_st = digitalRead(di_9);

}

void leder(){

  switch(track){
  case 1:
    {
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
    }
  case 2:
    {
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
    }

  case 3:
    {
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
    }

  case 4:
    {
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
    }

  case 5:
    {
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
      digitalWrite(led, HIGH);
      delay(20);
      digitalWrite(led, LOW);
      delay(50);
    }


  }

}







