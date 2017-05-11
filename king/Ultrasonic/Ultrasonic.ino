//
// Ultrasonic HR-SC04 sensor test
//
// http://robocraft.ru
//

#include "Ultrasonic.h"

// sensor connected to:
// Trig - 12, Echo - 13
Ultrasonic ultrasonic(8, 7);

int ledPin = 9;

void setup()
{
  Serial.begin(9600); 						// start the serial port
}

void loop()
{ int val, old_val, y, x;

  float dist_cm = ultrasonic.Ranging(CM); 	// get distance
  Serial.println(dist_cm); 					// print the distance
  //  Serial.println("x:");
  //  Serial.println(x);
  val = dist_cm;
  val = map(dist_cm, 20, 220, 255, 0);

//  if ((dist_cm==0)&&(dist_cm <=150)) {
//    
//     analogWrite(9, 255);
//    }
    analogWrite(9, val);

   
   // delay(50);
  


  //  if (val != old_val) {
  //    old_val = val;
  //    y=0;
  //
  //
  //
  //  }
  //  if (val == old_val){
  //    y++;
  //    }
  //
  //
  //if(y>=100){
  //  for(x=255;x<=0;x--){
  //    analogWrite(9, x);
  //    }
  //  }

  delay(50); 								// arbitary wait time.
}
