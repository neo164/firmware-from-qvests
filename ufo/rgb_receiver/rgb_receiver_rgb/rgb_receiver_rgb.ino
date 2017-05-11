#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // https://github.com/maniacbug/RF24

const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"

RF24 radio(9, 10); // CE, CSN


int data = 0;
int lastdata = 0;

const int pin_out = 2;
const int pinB = 3;
const int pinG = 5;
const int pinR = 6;
const int led = 4;
int r = 0;
int g = 0;
int b = 0;
int maximo = 100;

void setup() {
  Serial.begin(9600);

  pinMode(pin_out, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinR, OUTPUT);
  pinMode(led, OUTPUT);

  radio.begin();
  delay(2);
  radio.setChannel(9); // канал (0-127)

  // скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
  // RF24_250KBPS на nRF24L01 (без +) неработает.
  // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);

  // мощьность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);

  radio.openReadingPipe(1, pipe); // открываем первую трубу с индитификатором "pipe"
  radio.startListening(); // включаем приемник, начинаем слушать трубу
}

//  radio.stopListening(); // останавливает приём (нужно перед началом передачи)


void loop()
{
  int data;
  int heading;
  if (radio.available()) { // проверяем не пришло ли чего в буфер.
    radio.read(&data, sizeof(data)); // читаем данные, указываем сколько байт читать

    Serial.print("data: ");
    Serial.println(data);
    heading = data;


    if ((heading >= 0) && (heading < 90)) {
      r = int(map(heading, 0, 90, maximo, 0));
      g = int(map(heading, 0, 90, 0, maximo));
      b = 0;
    } else if ((heading >= 90) && (heading < 135)) {
      r = 0;
      g = maximo;
      b = int(map(heading, 90, 135, 0, maximo));
    } else if ((heading >= 135) && (heading < 180)) {
      r = int(map(heading, 135, 180, 0, maximo));
      g = maximo;
      b = maximo;
    } else if ((heading >= 180) && (heading < 225)) {
      r = int(map(heading, 180, 225, maximo, 0));
      g = int(map(heading, 180, 225, maximo, 0));
      b = maximo;
    } else if ((heading >= 225) && (heading <= 360)) {
      r = int(map(heading, 225, 360, 0, maximo));
      g = 0;
      b = int(map(heading, 225, 360, maximo, 0));
    }

    analogWrite(pinR, r);
    analogWrite(pinG, g);
    analogWrite(pinB, b);

    if (data >= 286 && data <= 290) {
      digitalWrite(pin_out, HIGH);
    }
    else digitalWrite(pin_out, LOW);


    }
}



