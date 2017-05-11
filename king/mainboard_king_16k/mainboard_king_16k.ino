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

//------------------------------------------------------------------------------------------------------------------------------
#if 0
#define DEBUG_BUTTON16(a) (a)
#else
#define DEBUG_BUTTON16(a)
#endif

#if 1
#define DEBUG_STATUS(a) (a)
#else
#define DEBUG_STATUS(a)
#endif


#pragma pack(push, 0)

// TBD: Replace with proper interrupt pin macros. It does not seem to be defined for atmega328p or I am incapable of finding it
#ifndef INT0_PIN
#ifdef __AVR_ATmega328P__
#define INT0_PIN 2
#else
#warning Define INT0_PIN for this microcontroller to use interrupt
#endif
#endif

#ifndef INT1_PIN 
#ifdef __AVR_ATmega328P__
#define INT1_PIN 3
#else
#warning Define INT1_PIN for this microcontroller to use interrupt
#endif
#endif


#ifndef INT4_PIN 
#ifdef __AVR_ATmega2560__
#define INT4_PIN 2
#else
#warning Define INT4_PIN for this microcontroller to use interrupt
#endif
#endif

uint32_t g_intCount = 0;

struct CTtp229ButtonEvent
{
uint8_t ButtonNumber : 
  5;         // ButtonNumber != 0 : Event is present. if equals 0, no event is received
uint8_t IsButtonReleased : 
  1;     // True = Button is pressed. False = Button is released
};

class CTtP229TouchButton
{
  struct CTtp229Prop
  {
uint16_t  SclPin : 
    6;
uint16_t  SdoPin : 
    6;
uint16_t  Is16Button : 
    1;
#if defined(INT0_PIN) || defined(INT4_PIN)
uint16_t  HasPendingInterrupt : 
    1;
uint16_t  IgnoreNextEvent : 
    1;            // When reading 16th key and if it is pressed, SDO stays low for 2ms.
    // If we enable interrupt before that, then it will trigger after 2ms, only to find the same condition.
    // To make things worse, at the end of reading the pin will stay low and generate another interrupt.
    // TBD: One possible fix is to send more pulses to make it roll over to HIGH. Have to find out if all 16 keys can be pressed in multi-key scenario (NOT supported yet).    
    uint16_t UnhandledButtonPresses;
#endif
    uint16_t PreviousButtonValue;
  };

  static CTtp229Prop g_prop;

  //
  //    Internal function that captures the data from TTP229 on which key is pressed.
  //  Currently, this function only supports one key being pressed. Multi-key press needs to be added later.
  // 
  //  Return Value : Bit field of buttons pressed
  //
  static uint16_t GetPressedButton()
  {
    DEBUG_BUTTON16(Serial.println("GetPressedButton : Enter "));
    uint16_t buttonsPressed = 0;
    // Need to generate the LOW and then HIGH on the clock and read the value from data when clock is back high.
    // As per the specification, the TTP229 chip can support 512Khz. This translates to approximately 2us for a cycle. So introducing clock with fall and raise each of 1us.
    uint8_t maxCnt = g_prop.Is16Button ? 16 : 8;
    for(uint8_t ndx = 0; ndx < maxCnt; ndx++ )
    {
      digitalWrite(g_prop.SclPin, LOW);
      delayMicroseconds(1);
      digitalWrite(g_prop.SclPin, HIGH);

      int val = digitalRead(g_prop.SdoPin);

      delayMicroseconds(1);  // Technically this can be moved after the if for most atmel microcontrollers. But if there is a really fast one (now/future) and the next call to GetPressedButton made immediately, we might overclock TTP229. Being safe here

      if( LOW == val )
      {
        buttonsPressed |= (1 << ndx);
      }      
    }

    DEBUG_BUTTON16(Serial.print("GetPressedButton : Exit. Return Value : ")); 
    DEBUG_BUTTON16(Serial.println(buttonsPressed));

    return buttonsPressed;
  }

#if defined(INT0_PIN) || defined(INT4_PIN)
  // Detaching the interrupt after receiving the data can cause problem in sleeping. If the interrupt is not properly dispatched, it can lead to permanent sleep and can't wake up from button
  static void HandleButtonEvent()
  {
    if( g_prop.IgnoreNextEvent )
    {
      // We ignored an event. Now we will accept the event
      g_prop.IgnoreNextEvent = false;
    }
    else
    {
      g_prop.HasPendingInterrupt = true;
      g_intCount++;
    }
  }

  static void SetInterruptHandler()
  {
#ifdef INT0_PIN    
    if( INT0_PIN == g_prop.SdoPin ) 
    {
      DEBUG_BUTTON16(Serial.println("Configure : With interrupt 0"));
      EIFR = 0x01; // Clear INTF0 flag
      attachInterrupt(0, HandleButtonEvent, RISING); // The pin goes down for 93us and then raises that is when the device is ready (technically after 10us)      
    }
#endif 

#ifdef INT4_PIN    
    if( INT4_PIN == g_prop.SdoPin ) 
    {
      DEBUG_BUTTON16(Serial.println("Configure : With interrupt 1"));
      EIFR = 0x02; // Clear INTF1 flag
      attachInterrupt(1, HandleButtonEvent, RISING); // The pin goes down for 93us and then raises that is when the device is ready (technically after 10us)      
    }
#endif 
  }

  static void RemoveInterruptHandler()
  {
#ifdef INT0_PIN
    if( INT0_PIN == g_prop.SdoPin ) 
    {
      detachInterrupt(0);
    }
#endif

#ifdef INT4_PIN 
    if( INT4_PIN == g_prop.SdoPin ) 
    {
      detachInterrupt(1);
    }
#endif
  }
#endif

  //
  //    Returns button number being pressed. High bit indicates more changes present
  //
  static uint8_t GetButtonNumberFromFlag(uint16_t buttonsChanged)
  {
    uint16_t flag = 1;
    for(uint8_t ndx = 1; ndx <=16; ndx++, flag <<= 1)
    {
      if( (buttonsChanged & flag) != 0 )
      {
        if( (buttonsChanged & ~flag) != 0 )
        {
          // Some other bit is present
          ndx |= 0x80;
        }

        return ndx;
      }
    }

    return 0;
  }

public:
  //
  //    Setup the TTP229 Touch button on this input.
  //
  // Inputs:
  //     sclPin  - Clock Pin of the button (3rd from left on button, connected to arduino's digital pin number)
  //     sdoPin  - Data pin to read from the button (4th pin from left on button, connected to arduino's digital pin number)
  //     is16Button - true = 16 buttons board. false = 8 button board
  //
  static void Configure(int sclPin, int sdoPin, bool is16Button = true)
  {
    DEBUG_BUTTON16(Serial.println("Configure : Enter")); 

    g_prop.SclPin = sclPin;
    g_prop.SdoPin = sdoPin;
    g_prop.Is16Button = is16Button;

    g_prop.PreviousButtonValue = 0;

    // Configure clock as output and hold it high
    pinMode( sclPin, OUTPUT ); 
    digitalWrite(sclPin, HIGH);    

    // Configure data pin as input
    pinMode( sdoPin, INPUT);

    DEBUG_BUTTON16(Serial.print("Button Configuration\n\rSCL Pin : "));
    DEBUG_BUTTON16(Serial.println(sclPin));
    DEBUG_BUTTON16(Serial.print("SDO Pin : "));
    DEBUG_BUTTON16(Serial.println(sdoPin));
    DEBUG_BUTTON16(Serial.print("Number of Keys : "));
    DEBUG_BUTTON16(Serial.println(is16Button ? 16 : 8));

#if defined(INT0_PIN) || defined(INT4_PIN)
    g_prop.UnhandledButtonPresses = 0;
    g_prop.HasPendingInterrupt = false;
    g_prop.IgnoreNextEvent = false;
    SetInterruptHandler();
#endif

    DEBUG_BUTTON16(Serial.println("Configure : Exit"));
  }

  //
  //    Get the current status from the 16 button touch device
  //
  //   Return Value : Returns the bitflag of the keys pressed. returns 0, if no key is pressed.
  //
  static uint16_t GetButtonStatus()
  {
#if defined(INT0_PIN) || defined(INT4_PIN)
    g_prop.HasPendingInterrupt = 0;
#endif

    uint16_t returnValue = GetPressedButton();

#if defined(INT0_PIN) || defined(INT4_PIN)
    returnValue |= g_prop.UnhandledButtonPresses;    // and also include any data that was received that we have not sent yet.
    g_prop.UnhandledButtonPresses = 0;
#endif

    g_prop.PreviousButtonValue = returnValue;

    return returnValue; 
  }

  //
  //    Gets the event from the button. This is useful for monitoring press and release only. 
  // Each button press will generate max 2 events, one for press and another for release. When the button is press and held, this method will return no event.
  // If the calls were not made often enough, the events could be missed. For instance, you might get 2 pressed, followed by 4 pressed, which automatically means 2 released in single key mode.
  //
  // Return Value : if ButtonNumber is 0, then no event
  //
  static CTtp229ButtonEvent GetButtonEvent()
  {
    CTtp229ButtonEvent returnValue = {
      0, 0        };
    uint8_t buttonNumber;

    DEBUG_BUTTON16(Serial.print("Old Value  : "));
    DEBUG_BUTTON16(Serial.println(g_prop.PreviousButtonValue));


#if defined(INT0_PIN) || defined(INT4_PIN)
    if(
#if defined(INT0_PIN)
    INT0_PIN == g_prop.SdoPin 
#endif
#if defined(INT0_PIN) && defined(INT4_PIN)
      ||
#endif
#if defined(INT4_PIN)
      INT4_PIN == g_prop.SdoPin 
#endif
      ) 
    {
      // Interrupts are used. Check if we have interrupt
      if( g_prop.HasPendingInterrupt )
      {
        RemoveInterruptHandler();                 // From this point upto SetInterruptHandler is called, ensure there is no return path that will leave without SetInterruptHandler
      }
      else
      {
        DEBUG_BUTTON16(Serial.println("GetButtonEvent: No interrupt pending"));
        return returnValue;
      }
    }
#endif

    uint16_t currValue = GetPressedButton();

#if defined(INT0_PIN) || defined(INT4_PIN)
    currValue |= g_prop.UnhandledButtonPresses; // Get any previously returned but not returned now values also into the mix
#endif

    uint16_t changes = g_prop.PreviousButtonValue ^ currValue;
    uint16_t pressed = (changes & currValue);
    uint16_t released = (changes & g_prop.PreviousButtonValue);

    // First check if any key is that is pressed and generate press event
    if( 0 != pressed )
    {
      buttonNumber = GetButtonNumberFromFlag(pressed);
      returnValue.ButtonNumber = (buttonNumber & 0x7F);

      uint16_t mask = (1 << (returnValue.ButtonNumber -1));
      // set the new notified button into prev
      g_prop.PreviousButtonValue |= mask;

#if defined(INT0_PIN) || defined(INT4_PIN)
      g_prop.UnhandledButtonPresses = currValue;
      g_prop.UnhandledButtonPresses = currValue & ~g_prop.PreviousButtonValue;    // clear unhandled for this bit, just in case
#endif
    }
    else if(0 != released)
    {
      buttonNumber = GetButtonNumberFromFlag(released);
      returnValue.ButtonNumber = (buttonNumber & 0x7F);

      // The unmatching bit whose previous value of 1 means, it is released
      returnValue.IsButtonReleased = true;

      // clear the notified release button
      g_prop.PreviousButtonValue &= ~(1 << (returnValue.ButtonNumber -1));
    }


#if defined(INT0_PIN) || defined(INT4_PIN)

    if(((!returnValue.IsButtonReleased || (0 == pressed))   // We handle release but no pending press
    && ((buttonNumber & 0x80) == 0 )) // or more button changes are detected
    || (returnValue.ButtonNumber == 0) )    // safety in case interrupt and data mismatch or code bug
    {
      // No more button notification pending
      g_prop.HasPendingInterrupt = false;
    }
    else
    {
      DEBUG_BUTTON16(Serial.println("not Clearing interrupt"));
    }

    g_prop.IgnoreNextEvent = digitalRead(g_prop.SdoPin) == LOW; // If the pin is still low at the end of reading, ignore next event which is for data finished raise
    DEBUG_BUTTON16(Serial.print(g_prop.IgnoreNextEvent ? "Ignoring next event\n\r" : "Not ignoring\n\r"));

    // All the data has been read. Now reactivate the interrupt
    SetInterruptHandler();
#endif

    DEBUG_BUTTON16(Serial.print("currValue : "));
    DEBUG_BUTTON16(Serial.println(currValue));
    DEBUG_BUTTON16(Serial.print("Changes    : "));
    DEBUG_BUTTON16(Serial.println(changes));
    DEBUG_BUTTON16(Serial.print("Button N   : "));
    DEBUG_BUTTON16(Serial.println(buttonNumber));
    DEBUG_BUTTON16(Serial.print("Unhandled  : "));
    DEBUG_BUTTON16(Serial.println(g_prop.UnhandledButtonPresses));
    DEBUG_BUTTON16(Serial.print("ButtonRelease : "));
    DEBUG_BUTTON16(Serial.println(returnValue.IsButtonReleased));
    DEBUG_BUTTON16(Serial.print("buttonNumber : "));
    DEBUG_BUTTON16(Serial.println(buttonNumber));
    DEBUG_BUTTON16(Serial.print("Pending interrupts :"));
    DEBUG_BUTTON16(Serial.println(g_prop.HasPendingInterrupt));

    return returnValue;
  }

#if defined(INT0_PIN) || defined(INT4_PIN)
  static bool HasEvent()
  {
#if defined(INT0_PIN)
    if( INT0_PIN == g_prop.SdoPin )
    {
      return g_prop.HasPendingInterrupt;
    }
#endif

#if defined(INT4_PIN)
    if( INT4_PIN == g_prop.SdoPin )
    {
      return g_prop.HasPendingInterrupt;
    }
#endif

    return true;
  }
#endif

};

CTtP229TouchButton::CTtp229Prop CTtP229TouchButton::g_prop;
CTtP229TouchButton g_ttp229Button;

#define TTP16Button g_ttp229Button
#pragma pack(pop)


void TestStatus()
{
  uint8_t buttonNumber = TTP16Button.GetButtonStatus();

  if( 0 != buttonNumber )
  {
    DEBUG_STATUS(Serial.print("Button Pressed : "));
    DEBUG_STATUS(Serial.println(buttonNumber));
  }

  delayMicroseconds(2500); // TTP229 document says it will reset the output if 2ms idle + little bit safety
}

void TestEvent()
{
#if defined(INT0_PIN) || defined(INT4_PIN)
  if( TTP16Button.HasEvent())
#endif
  {
    CTtp229ButtonEvent buttonEvent = TTP16Button.GetButtonEvent();

    if( 0 != buttonEvent.ButtonNumber )
    {
      if( buttonEvent.IsButtonReleased )
      {
        DEBUG_STATUS(Serial.print("Button Released: "));
      }
      else
      {
        DEBUG_STATUS(Serial.print("Button Pressed : "));
      }
      DEBUG_STATUS(Serial.println(buttonEvent.ButtonNumber));
    }
    else
    {
#if defined(INT0_PIN) || defined(INT4_PIN)
      DEBUG_STATUS(Serial.println("Are you not using interrupt? Should never come here for interrupt based system."));
#endif
    }

    //Serial.print("CurrentTime : "); Serial.println(millis());
    delayMicroseconds(2500); // TTP229 document says it will reset the output if 2ms idle + little bit safety. Not required if using interrupts
  }

}

//------------------------------------------------------------------------------------------------------------------------------



#define BUTTON_PIN 12 //onboard button

// Instantiate a Bounce object
Bounce debouncer = Bounce();

const int u1 = A0, u2 = A1, u3 = A2, u4 = A3, u5 = A4, u6 = A5, u7 = A6, un8 = A7,

s1 = A8, s2 = A9, s3 = A10, s4 = A11, s5 = A12, s6 = A13, s7 = A14, s8 = A15;


int led = 13;
int R = 5, G = 6, B = 7;
int RGB = 0;
boolean flag = false, start_st = false;

int
s1_state = 0, s2_state = 0, s3_state = 0, s4_state = 0, s5_state = 0, s6_state = 0, s7_state = 0, s8_state = 0;

int
din1_st = 0, din2_st = 0, din3_st = 0, din4_st = 0, din5_st = 0, din6_st = 0, din7_st = 0, din8_st = 0,
din9_st = 0, din10_st = 0, din11_st = 0,  din12_st = 0,  din13_st = 0,  din14_st = 0,  din15_st = 0,  din16_st = 0;

mp3TF mp3tf = mp3TF ();


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
  pinMode(relay_3, OUTPUT); 
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
  pinMode(din_13, INPUT_PULLUP);
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
  pinMode(s8, INPUT_PULLUP);

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  //--------------------------------------------------------------------------------------------------------
  TTP16Button.Configure(8, 2);
  //--------------------------------------------------------------------------------------------------------
  
  initrel();
  unl_HL(7, true); //полы дракон
  digitalWrite(relay_4, LOW); //бочка вкл  
  digitalWrite(relay_11, LOW);



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

  set_light_dragon_on();

  //delay(5000);
  //opustit_lebedka();
  //delay(5000);
  //podnyat_lebedka();

  debouncer.attach(BUTTON_PIN);
  debouncer.interval(15); // interval in ms

  Timer1.initialize(300000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

}


void loop()
{
  timer();//считаем, рисуем время
  update_DI();


  // Update the Bounce instance :
  debouncer.update();

  // Get the updated value :
  int value = debouncer.read();

  if ( value == LOW ) {
    digitalWrite(R, 1);
    digitalWrite(G, 1);
    digitalWrite(B, 1);
    delay(500);
    mp3tf.next();
  }




  delay(500);
  // Toggle LED
  digitalWrite( led, digitalRead( led ) ^ 1 );



}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


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



  //lcd.setCursor(0, 2);
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

  //**********морда-бочка*************
  if (din10_st) {  //1
    lcd.setCursor(7, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(7, 1);
    lcd.print("1");
  }

  if (din11_st) {  //1
    lcd.setCursor(8, 1);
    lcd.print("0");
  }
  else {
    lcd.setCursor(8, 1);
    lcd.print("2");
  }

  if((din10_st && din11_st) == false) digitalWrite(relay_4, HIGH);
  else digitalWrite(relay_4, LOW);
  //&&(din11_st)
  //**********морда-бочка*************

  //***********трон-поджопник*********
  if (din12_st) {  //1
    lcd.setCursor(0, 3);
    lcd.print("the king does notsit");
  }
  else {
    lcd.setCursor(0, 3);
    lcd.print("king sits");
  }

  //***********трон-поджопник*********

  //************череп*****************
  if (din9_st) {  //1
    lcd.setCursor(12, 1);
    lcd.print("_");
  }
  else {
    lcd.setCursor(12, 1);
    lcd.printByte(5);
    //    lcd.print("1");
  }
  if (!din9_st) digitalWrite(relay_11, HIGH);
  //************череп*****************

  //**********щит**********
  if (s8_state) {
    lcd.setCursor(8, 2);
    lcd.print("0");
  }
  else {
    lcd.setCursor(8, 2);
    lcd.print("8");
  }
  //************щит*********
  //**************клык***************
  if (din16_st) {
    lcd.setCursor(10, 2);
    lcd.print("k");
    unl_HL(7, true);
  }
  else {
    lcd.setCursor(10, 2);
    lcd.print("K");
    unl_HL(7, false);    
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
  TestEvent();

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


void faster() {
  din5_st = digitalRead(din_5);
  if (din5_st) digitalWrite(relay_5, 1);
  else digitalWrite(relay_5, 0);

  din6_st = digitalRead(din_6);
  if (din6_st) digitalWrite(relay_6, 1);
  else digitalWrite(relay_6, 0);

  din7_st = digitalRead(din_7);
  if (din7_st) digitalWrite(relay_7, 1);
  else digitalWrite(relay_7, 0);

  din8_st = digitalRead(din_8);
  if (din8_st) digitalWrite(relay_8, 1);
  else digitalWrite(relay_8, 0);
}





void update_DI() {
  s1_state = digitalRead(s1);
  s2_state = digitalRead(s2);
  s3_state = digitalRead(s3);
  s4_state = digitalRead(s4);
  s5_state = digitalRead(s5);
  s6_state = digitalRead(s6);
  s7_state = digitalRead(s7);
  s8_state = digitalRead(s8);



  din1_st = digitalRead(din_1);
  if (din1_st) digitalWrite(relay_1, 1);
  else digitalWrite(relay_1, 0);

  din2_st = digitalRead(din_2);
  if (din2_st) digitalWrite(relay_2, 1);
  else digitalWrite(relay_2, 0);

  din3_st = digitalRead(din_3);
  if (din3_st) digitalWrite(relay_3, 1);
  else digitalWrite(relay_3, 0);

  din4_st = digitalRead(din_4);
  //  if (din4_st) digitalWrite(relay_4, 1);
  //  else digitalWrite(relay_4, 0);


  //*************************

  din9_st = digitalRead(din_9);
  //  if (din9_st) digitalWrite(relay_9, 1);
  //  else digitalWrite(relay_9, 0);

  din10_st = digitalRead(din_10);
  //  if (din10_st) digitalWrite(relay_10, 1);
  //  else digitalWrite(relay_10, 0);
  //
  din11_st = digitalRead(din_11);
  //  if (din11_st) digitalWrite(relay_11, 1);
  //  else digitalWrite(relay_11, 0);

  din12_st = digitalRead(din_12);
  if (din12_st) digitalWrite(relay_12, 1);
  else digitalWrite(relay_12, 0);

  din13_st = digitalRead(din_13);
  if (din13_st) digitalWrite(relay_13, 1);
  else digitalWrite(relay_13, 0);

  din14_st = digitalRead(din_14);
  if (din14_st) digitalWrite(relay_14, 1);
  else digitalWrite(relay_14, 0);

  din15_st = digitalRead(din_15);
  if (din15_st) digitalWrite(relay_15, 1);
  else digitalWrite(relay_15, 0);

  din16_st = digitalRead(din_16);
  if (din16_st) digitalWrite(relay_16, 1);
  else digitalWrite(relay_16, 0);
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
















