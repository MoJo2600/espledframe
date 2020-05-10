#define NUM_LEDS 64
#define DATA_PIN 13
#define CLOCK_PIN 14
const int rows = 8;

CRGB leds[NUM_LEDS];

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);

  Serial.begin(38400);

  // ## Clocked (SPI) types ##
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);  // BGR ordering is typical
  FastLED.setBrightness(46);
}

const long test01[] =
{
  CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red
};

const long test02[] =
{
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

void show_frame(const long *img) {
  int lines = NUM_LEDS / 8;
  int startIndex = 0;
  int index = 0;

  FastLED.clear();
  for (int line = 0; line < lines; line++) {
    startIndex = line * 8;
    // Every second row
    if (line % 2 == 0) {
      for (int column = 0; column < 8; column++) {
        leds[index] = img[startIndex+column];
        index++;
      }
    } else {
      for (int column = 7; column >= 0; column--) {
        leds[index] = img[startIndex+column];
        index++;
      }
    }
  }
  FastLED.show();
}


void show_animation(const long *img1, const long *img2, long interval, long duration) {

  int frame = 0;
  unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  unsigned long startMillis = currentMillis;

  while (currentMillis - startMillis < duration) {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (frame == 0) {
        Serial.print("frame == 0");
        show_frame(img1);
        frame = 1;
      } else {
        Serial.print("frame == 1");
        show_frame(img2);
        frame = 0;
      }
    }
  }
}


void loop() {
  Serial.print("animation started");
  show_animation(test01, test02, 500, 5000);
  Serial.print("animation ended");
}