#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>


#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            4
const int buttonPin = 2;
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
boolean flag = false, svet = false, fl_strip = false, rounder_flag = false, pressure_st = false, key = false;

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      58

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 15; // delay for half a second


const int analogInPin0 = A0, analogInPin1 = A1, analogInPin2 = A2, analogInPin3 = A3;

int sensorValue0 = 0, sensorValue1 = 0, sensorValue2 = 0, sensorValue3 = 0;        // value read from the pot
int outputValue0 = 0, outputValue1 = 0, outputValue2 = 0, outputValue3 = 0;        // value output to the PWM (analog out)



const int led23 = 23, led25 = 25, led27 = 27, led29 = 29, led31 = 31, led33 = 33, led35 = 35, led37 = 37;


const int btn39 = 39, btn41 = 41, btn43 = 43, btn45 = 45, btn47 = 47, btn49 = 49, btn51 = 51, btn53 = 53;

const int rly1 = 22, rly2 = 24, rly3 = 26, rly4 = 28, rly5 = 30, rly6 = 32, rly7 = 34, rly8 = 7, pressure = 52; //7 is pwm!!!



const int sw9 = 38, sw10 = 40, sw11 = 42, sw12 = 44;

boolean 
st_sw9 = false,
st_sw10 = false,
st_sw11 = false,
st_sw12 = false;


int btn39_st = 0, btn41_st = 0, btn43_st = 0, btn45_st = 0, btn47_st = 0, btn49_st = 0, btn51_st = 0, btn53_st = 0;

int dly = 150;
// the setup routine runs once when you press reset:
void setup() {

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pinMode(buttonPin, INPUT_PULLUP);
  pixels.begin(); // This initializes the NeoPixel library.
  delay(100);
  Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(led23, OUTPUT);
  pinMode(led25, OUTPUT);
  pinMode(led27, OUTPUT);
  pinMode(led29, OUTPUT);
  pinMode(led31, OUTPUT);
  pinMode(led33, OUTPUT);
  pinMode(led35, OUTPUT);
  pinMode(led37, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(rly1, OUTPUT);
  pinMode(rly2, OUTPUT);
  pinMode(rly3, OUTPUT);
  pinMode(rly4, OUTPUT);
  pinMode(rly5, OUTPUT);
  pinMode(rly6, OUTPUT);
  pinMode(rly7, OUTPUT);
  // pinMode(rly8, OUTPUT);
  pinMode(pressure, INPUT_PULLUP);
  pinMode(btn39, INPUT_PULLUP);
  pinMode(btn41, INPUT_PULLUP);
  pinMode(btn43, INPUT_PULLUP);
  pinMode(btn45, INPUT_PULLUP);
  pinMode(btn47, INPUT_PULLUP);
  pinMode(btn49, INPUT_PULLUP);
  pinMode(btn51, INPUT_PULLUP);
  pinMode(btn53, INPUT_PULLUP);

  pinMode(sw9, INPUT_PULLUP);
  pinMode(sw10, INPUT_PULLUP);
  pinMode(sw11, INPUT_PULLUP);
  pinMode(sw12, INPUT_PULLUP);


  digitalWrite(rly4, HIGH);
  delay(1000);

  digitalWrite(rly1, LOW);
  digitalWrite(rly2, HIGH);
  //digitalWrite(rly4,HIGH);
  digitalWrite(rly3, HIGH);
  digitalWrite(rly5, HIGH);
  delay(2500);
  digitalWrite(rly1, LOW);
  digitalWrite(rly2, LOW);
  digitalWrite(rly4, LOW);
  digitalWrite(rly3, LOW);
  test();
  Timer1.initialize(300000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

//================================================================================================================
void loop() {

  sensorValue0 = analogRead(analogInPin0);  
  sensorValue1 = analogRead(analogInPin1);
  sensorValue2 = analogRead(analogInPin2);
  sensorValue3 = analogRead(analogInPin3);

  outputValue0 = map(sensorValue0, 0, 1023, 0, 255);
  outputValue1 = map(sensorValue1, 0, 1023, 0, 255);
  outputValue2 = map(sensorValue2, 0, 1023, 0, 255);
  outputValue3 = map(sensorValue3, 0, 1023, 0, 255);

  //--------oper-----------
  st_sw9 = digitalRead(sw9); //pressure
  st_sw10 = digitalRead(sw10); //roll_changer
  st_sw11 = digitalRead(sw11); //figures
  st_sw12 = digitalRead(sw12); //...
  //--------oper-----------

  Serial.print("sensor 0 = " );                       
  Serial.println(outputValue0); 
  Serial.print("sensor 1 = " );                       
  Serial.println(outputValue1);
  Serial.print("sensor 2 = " );                       
  Serial.println(outputValue2);
  Serial.print("sensor 3 = " );                       
  Serial.println(outputValue3);

  //----------------открытие пьедистала от 4-х фигурок----------------------
  if((outputValue0 >=120 &&  outputValue1 >=130 && outputValue2 >=128 && outputValue3 <= 100 ) || (!st_sw11)) {

    //digitalWrite( 13,HIGH);
    openped();

    if (!svet) {
      //включение света
      pwm_on();
    }

    svet = true;
    key = true;


  } 
  //----------------открытие пьедистала от 4-х фигурок----------------------



  //------------датчик давления-----------
  if ((pressure_st) || (!st_sw9)) {
    digitalWrite(rly4, HIGH);
    fl_strip = true;
    digitalWrite(8, HIGH);
  }

  if (fl_strip) {
    ledstrip();

  }
  //------------датчик давления-----------

  //----------------отмена кругового вращения-------------------
  if (digitalRead(led23) || digitalRead(led25) || digitalRead(led27) || digitalRead(led29) || digitalRead(led31) || digitalRead(led33) || digitalRead(led35) || digitalRead(led37)) 
  { 
    rounder_flag = true;
  }

  if (!rounder_flag && fl_strip) {
    rounder();
  }
  //----------------отмена кругового вращения-------------------


  //----------------последовательное нажатие--------------------

  //----------------последовательное нажатие--------------------


  //-------------если все зажжены то открываем дверь-------------
  if ((digitalRead(led23) && digitalRead(led25) && digitalRead(led27) && digitalRead(led29) && digitalRead(led31) && digitalRead(led33) && digitalRead(led35) && digitalRead(led37)) || (!st_sw10))//roll_changer
  {
    if (!key) {
      //открытие двери
      digitalWrite(rly5, LOW);
      delay(100);

    }
    

    //и еще что-то...

  }
  //-------------если все зажжены то открываем дверь-------------

}
//================================================================================================================



void pwm_on() {
  for (int fadeValue =  0; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(rly8, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(50);
  }
}
void openped() {
  digitalWrite(rly1, HIGH);
  digitalWrite(rly2, HIGH);
  delay(2500);
  digitalWrite(rly1, LOW);
  digitalWrite(rly2, LOW);
}

void ledstrip() {
  // {
  {
    flag = true;
  }
  //else {
  {
    for (int i = NUMPIXELS; i >= 0; i--) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 0 , 0)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).

    }
    pixels.clear();

  }
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.


  if (flag) {

    // turn LED on:
    for (int i = NUMPIXELS; i >= 0; i--) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(100, 0 , 100)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).

    }

    for (int i = NUMPIXELS; i >= 0; i--) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 0 , 0)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).

    }
    pixels.clear();

  }

  else {

    pixels.clear();
    delay(delayval);
    pixels.show();


  }


}



void test() {

  // turn LED on:
  for (int i = NUMPIXELS; i >= 0; i--) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(100, 0 , 100)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }

  for (int i = NUMPIXELS; i >= 0; i--) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0 , 0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }
  pixels.clear();

}



void changer() {

  if (!btn39_st) {
    digitalWrite( led23, digitalRead( led23 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(!led25)&&digitalRead(!led27)&&digitalRead(!led29)&&digitalRead(!led31)&&digitalRead(!led33)&&digitalRead(!led35)&&digitalRead(!led37));//11111111111111
    else ledreset();  

    delay(dly);
    //digitalWrite(led23, HIGH);
  }
  else {
    //digitalWrite(led23, LOW);
  }

  if (!btn41_st) {
    digitalWrite( led25, digitalRead( led25 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(led25)&&digitalRead(!led27)&&digitalRead(led29)&&digitalRead(!led31)&&digitalRead(led33)&&digitalRead(led35)&&digitalRead(led37));//66666666666666666666
    else ledreset(); 

    delay(dly);
    //digitalWrite(led25, HIGH);
  }
  else {
    //digitalWrite(led25, LOW);
  }
  if (!btn43_st) {
    digitalWrite( led27, digitalRead( led27 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(led25)&&digitalRead(led27)&&digitalRead(led29)&&digitalRead(led31)&&digitalRead(led33)&&digitalRead(led35)&&digitalRead(led37));//88888888888888888888
    else ledreset(); 

    delay(dly);
    //digitalWrite(led27, HIGH);
  }
  else {
    //digitalWrite(led27, LOW);
  }
  if (!btn45_st) {
    digitalWrite( led29, digitalRead( led29 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(!led25)&&digitalRead(!led27)&&digitalRead(led29)&&digitalRead(!led31)&&digitalRead(led33)&&digitalRead(led35)&&digitalRead(led37));//5555555555555555
    else ledreset(); 

    delay(dly);
    //digitalWrite(led29, HIGH);
  }
  else {
    //digitalWrite(led29, LOW);
  }
  if (!btn47_st) {
    digitalWrite( led31, digitalRead( led31 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(led25)&&digitalRead(!led27)&&digitalRead(led29)&&digitalRead(led31)&&digitalRead(led33)&&digitalRead(led35)&&digitalRead(led37));//777777777777777777
    else ledreset(); 

    delay(dly);
    //digitalWrite(led31, HIGH);
  }
  else {
    //digitalWrite(led31, LOW);
  }
  if (!btn49_st) {
    digitalWrite( led33, digitalRead( led33 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(!led25)&&digitalRead(!led27)&&digitalRead(!led29)&&digitalRead(!led31)&&digitalRead(led33)&&digitalRead(led35)&&digitalRead(!led37));//33333333333333333
    else ledreset(); 

    delay(dly);
    //digitalWrite(led33, HIGH);
  }
  else {
    //digitalWrite(led33, LOW);
  }
  if (!btn51_st) {
    digitalWrite( led35, digitalRead( led35 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(!led25)&&digitalRead(!led27)&&digitalRead(!led29)&&digitalRead(!led31)&&digitalRead(!led33)&&digitalRead(led35)&&digitalRead(!led37));//22222222222222222
    else ledreset();  

    delay(dly);
    //digitalWrite(led35, HIGH);
  }
  else {
    //digitalWrite(led35, LOW);
  }
  if (!btn53_st) {

    digitalWrite( led37, digitalRead( led37 ) ^ 1 );

    if(digitalRead(led23)&&digitalRead(!led25)&&digitalRead(!led27)&&digitalRead(!led29)&&digitalRead(!led31)&&digitalRead(led33)&&digitalRead(led35)&&digitalRead(led37));//44444444444444444444
    else ledreset(); 

    delay(dly);
    //digitalWrite(led37, HIGH);
  }
  else {
    //digitalWrite(led37, LOW);
  }



}


void ledreset(){
  for (int j = 23; j < 39; j += 2) {
    digitalWrite(j, LOW);
    //delay();
  }
}

void rounder() {

  for (int j = 23; j < 39; j += 2) {
    digitalWrite(j, HIGH);
    delay(100);
  }


  for (int j = 23; j < 39; j += 2) {
    digitalWrite(j, LOW);
    delay(100);
  }
}


//*************interrup**********************************************************
void timerIsr()
{

  buttonState = digitalRead(buttonPin);
  pressure_st = digitalRead(pressure);
  btn39_st = digitalRead(btn39);
  btn41_st = digitalRead(btn41);
  btn43_st = digitalRead(btn43);
  btn45_st = digitalRead(btn45);
  btn47_st = digitalRead(btn47);
  btn49_st = digitalRead(btn49);
  btn51_st = digitalRead(btn51);
  btn53_st = digitalRead(btn53);
  //digitalWrite( 13, digitalRead( 13 ) ^ 1 );
  if (!buttonState) {
    digitalWrite(rly3, HIGH);
  }

  changer();

}
//*************interrup**********************************************************






