int ledPins[] = { 11, 9, 10 };
int inputPin = 0;

void setup()
{
  pinMode(inputPin, INPUT);
  for (int i = 0; i < 3; i++)
    pinMode(ledPins[i], OUTPUT);
}

void loop()
{
  int inputValue = analogRead(inputPin);
  int hueValue = map(inputValue, 0, 1024, 0, 360);

  setHueValue(hueValue);
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
