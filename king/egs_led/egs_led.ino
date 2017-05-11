/**
   ----------------------------------------------------------------------------
   This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
   for further details and other examples.

   NOTE: The library file MFRC522.h has a lot of useful info. Please read it.

   Released into the public domain.
   ----------------------------------------------------------------------------
   This sample shows how to read and write data blocks on a MIFARE Classic PICC
   (= card/tag).

   BEWARE: Data will be written to the PICC, in sector #1 (blocks #4 to #7).


   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS 1    SDA(SS)      10            53        D10        10               10
   SPI SS 2    SDA(SS)      2             53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN        10           // Configurable, see typical pin layout above

#define SS_1_PIN        5          // Configurable, see typical pin layout above
#define SS_2_PIN        6          // Configurable, see typical pin layout above
#define SS_3_PIN        7

#define NR_OF_READERS  3
int tmp = 0, x = 0, y = 0, z = 0, k = 0;;
boolean egg1 = false, egg2 = false, egg3 = false, eggall = false;
byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

/**
   Initialize.
*/
void setup() {
  pinMode(14, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(9600); // Initialize serial communications with the PC
  delay(250);
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  delay(250);
  SPI.begin();        // Init SPI bus
  delay(250);
  
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
  }
  delay(250);
  Serial.println("Version 1.0");
}

/**
   Main loop.
*/
void loop() {
  tmp = 0;


  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards
    tmp = 0;
    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader: "));
      Serial.print(reader);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(" Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));
      // Halt PICC
      // mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();

      if ((reader == 0) && (tmp == 148)) {//137
        x = tmp;
        egg1 = true;
        digitalWrite(2, HIGH);
      }
      
      if ((reader == 1) && (tmp == 137)) {//148
        y = tmp;
        egg2 = true;
        digitalWrite(3, HIGH);
      }

      if ((reader == 2) && (tmp == 100)) {//100
        z = tmp;
        egg3 = true;
        digitalWrite(4, HIGH);
      }

      if ((egg1 == true) && (egg2 == true) && (egg3 == true)) {

        digitalWrite(14, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(100);              // wait for a second

      }
      else{
        
        
        }

    }
  }

  Serial.print("x:");
  Serial.print(x);
  Serial.print(" y:");
  Serial.print(y);
  Serial.print(" z:");
  Serial.println(z);
  if (k > 10) {
    digitalWrite(14, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    egg1 = false;
    egg2 = false;
    egg3 = false;
    k = 0;
  }
  k++;

}

/**
   Helper routine to dump a byte array as hex values to Serial.
*/
int dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
    if (i == 0) tmp = buffer[i];
  }

  return tmp;
}
