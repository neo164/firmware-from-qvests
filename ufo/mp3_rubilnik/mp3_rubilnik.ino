/*	2014-01-07 by lisper <leyapin@gmail.com>
 *	control mp3 module by seirial
 *	use mp3TF.h	
 *	test on UNO
 */

#include <mp3TF.h>


unsigned long pusk=0;
int led = 13;
int randNumber;

mp3TF mp3tf = mp3TF ();




//
void setup () {
  mp3tf.init (&Serial);
  Serial.begin (9600);
  pinMode(led, OUTPUT);
  randomSeed(analogRead(3));

  //Serial1.begin (9600);
  delay(100);
  mp3tf.setVolume (25);
  delay(100);
  mp3tf.play (1);
}

//
void loop () {
  randNumber = random(50);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(randNumber);       
  randNumber = random(4000);  // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(randNumber);               // wait for a second
  
  pusk++;
  if (pusk==10) {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  mp3tf.play (1);
  }

if (pusk==200) {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  mp3tf.play (2);
  }
if (pusk==300)pusk=0;

}


