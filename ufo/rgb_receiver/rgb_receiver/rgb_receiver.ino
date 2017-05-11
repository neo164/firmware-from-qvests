#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // https://github.com/maniacbug/RF24

const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"

RF24 radio(9, 10); // CE, CSN

int ledPins[] = { 6, 3, 5 };
int inputPin = 0;
int hueValue = 0;
int data = 0;
void setup() {
  Serial.begin(9600);

  pinMode(inputPin, INPUT);
  for (int i = 0; i < 3; i++)
    pinMode(ledPins[i], OUTPUT);

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

  if (radio.available()) { // проверяем не пришло ли чего в буфер.
    radio.read(&data, sizeof(data)); // читаем данные, указываем сколько байт читать

    Serial.print("data: ");
    Serial.println(data);
    hueValue = data;

    //hueValue = map(data, 0, 1024, 0, 360);
    setHueValue(hueValue);

  }
}


void setHueValue(int hueValue)
{
  setColor(ledPins, hsvToRgb(hueValue, 1, 1));
}

void setColor(int* led, const byte* color)
{
  for (int i = 0; i < 3; i++)
    analogWrite(led[i], 255 - color[i]);
}

byte rgb[3];

byte* hsvToRgb(int h, double s, double v)
{
  // Make sure our arguments stay in-range
  h = max(0, min(360, h));
  s = max(0, min(1.0, s));
  v = max(0, min(1.0, v));
  if (s == 0)
  {
    // Achromatic (grey)
    rgb[0] = rgb[1] = rgb[2] = round(v * 255);
    return rgb;
  }
  double hs = h / 60.0; // sector 0 to 5
  int i = floor(hs);
  double f = hs - i; // factorial part of h
  double p = v * (1 - s);
  double q = v * (1 - s * f);
  double t = v * (1 - s * (1 - f));
  double r, g, b;
  switch (i)
  {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default: // case 5:
      r = v;
      g = p;
      b = q;
  }
  rgb[0] = round(r * 255.0);
  rgb[1] = round(g * 255.0);
  rgb[2] = round(b * 255.0);
  return rgb;
}
