
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include "CyberLib.h"
#include <Servo.h>

const int buttonPin = 11;
int buttonState = 0;
Servo myservo;
int val;
const int analogInPin = A0;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
const int analogInPin1 = A1;
int sensorValue1 = 0;        // value read from the pot
int outputValue1 = 0;        // value output to the PWM (analog out)
const int analogInPin2 = A2;
int sensorValue2 = 0;        // value read from the pot
int outputValue2 = 0;        // value output to the PWM (analog out)

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
int encoderSwitchPin = 4; //push button switch
int led = 13;

int k1 = 8, k2 = 7, k3 = 6, k4 = 5;

boolean flag1 = false, flag2 = false, flag3 = false, flag4 = false;


#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  pinMode(led, OUTPUT);
  pinMode(k1, OUTPUT);
  pinMode(k2, OUTPUT);
  pinMode(k3, OUTPUT);
  pinMode(k4, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  myservo.attach(9);
  Serial.println("DHTxx test!");
  dht.begin();
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();



  display.display();

  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);

  pinMode(encoderSwitchPin, INPUT);


  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  digitalWrite(encoderSwitchPin, HIGH); //turn pullup resistor on


  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  delay(1000);
  display.clearDisplay();
  encoderValue = ReadEEPROM_Byte(0);
  if(encoderValue >=140)encoderValue=139;
  digitalWrite(k1, LOW);
  digitalWrite(k2, LOW);
  digitalWrite(k3, LOW);
  digitalWrite(k4, LOW);
  
}
int hygr = 0, temp = 0;

void loop() {

  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  sensorValue1 = analogRead(analogInPin1);
  // map it to the range of the analog out:
  outputValue1 = map(sensorValue1, 0, 1023, 0, 255);
  sensorValue2 = analogRead(analogInPin2);
  // map it to the range of the analog out:
  outputValue2 = map(sensorValue2, 0, 1023, 0, 255);

  display.clearDisplay();
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("enc:");
  display.print(encoderValue, DEC);
  display.setCursor(0, 10);
  display.setTextSize(2);
  display.print(outputValue, DEC);
  display.setCursor(0, 25);
  display.setTextSize(2);
  display.print(outputValue1, DEC);
  display.setCursor(0, 40);
  display.setTextSize(2);
  display.print(outputValue2, DEC);
  display.display();

  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(k1, LOW);
    digitalWrite(k2, LOW);
    digitalWrite(k3, LOW);
    display.setCursor(50, 10);
    display.setTextSize(1);
    display.print("_");
    display.setCursor(50, 25);
    display.setTextSize(1);
    display.print("_");
    display.setCursor(50, 40);
    display.setTextSize(1);
    display.print("_");
    display.display();
  }

  if (outputValue <= encoderValue) {
    digitalWrite(k1, HIGH);
    display.setCursor(50, 10);
    display.setTextSize(1);
    display.print("*");
    flag1 = true;
    delay(200);
    digitalWrite(k1, HIGH);
  }
  else {
    display.setCursor(50, 10);
    display.setTextSize(1);
    display.print("_");
  }

  //else digitalWrite(k1,HIGH);

  if (outputValue1 <= encoderValue) {
    digitalWrite(k2, HIGH);
    display.setCursor(50, 25);
    display.setTextSize(1);
    display.print("*");
    flag2 = true;
    delay(200);
    digitalWrite(k2, HIGH);

  }
  else {
    display.setCursor(50, 25);
    display.setTextSize(1);
    display.print("_");
  }

  //else digitalWrite(k2,HIGH);

  if (outputValue2 <= encoderValue) {
    digitalWrite(k3, HIGH);
    display.setCursor(50, 40);
    display.setTextSize(1);
    display.print("*");
    flag3 = true;
    delay(200);
    digitalWrite(k3, HIGH);
  }
  else {
    display.setCursor(50, 40);
    display.setTextSize(1);
    display.print("_");
  }
  //else digitalWrite(k3,HIGH);

  display.display();
  myservo.write(encoderValue);
  //Do stuff here
  if (digitalRead(encoderSwitchPin)) {
    //button is not being pushed
  }
  else {
    WriteEEPROM_Byte(0, encoderValue);
    display.setCursor(0, 17);
    display.print("ok");
    display.display();
    delay(1000);
    display.setCursor(0, 17);
    display.print("  ");
    display.display();
    //encoderValue=0;
    //button is being pushed
  }
  if (encoderValue <= 0) encoderValue = 1;

  if ((flag1) && (flag2) && (flag3)) {

    digitalWrite(led, HIGH);
    digitalWrite(k4, HIGH);
    delay(5000);
    digitalWrite(led, LOW);
    digitalWrite(k4, LOW);
    digitalWrite(k3, LOW);
    digitalWrite(k2, LOW);
    digitalWrite(k1, LOW);
    flag1=flag2=flag3=false;
  }

}







void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  for (uint8_t i = 0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
}

void updateEncoder() {
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}





