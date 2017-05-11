/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 12;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(2, OUTPUT);      
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);     
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == LOW) {     
    // turn LED on:    
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);

    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);  

    digitalWrite(5, HIGH);
    delay(1000);
    digitalWrite(5, LOW);

    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(3, LOW);

    digitalWrite(7, HIGH);
    delay(1000);
    digitalWrite(7, LOW);

    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);

    digitalWrite(6, HIGH);
    delay(1000);
    digitalWrite(6, LOW);

    digitalWrite(8, HIGH);
    delay(1000);
    digitalWrite(8, LOW);


  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    Serial.print("\t output = ");      
    Serial.println("outputValue");
    
  }
}


