// developed by abhi velaga
// github.com/avelaga/drum-led

#include <FastLED.h>

#define NUM_LEDS 40
#define DATA_PIN 3
const int knockSensor = A0;
const int threshold = 500;
const int maxRate = 75;
const int hitIncrease = 15;
const int restingHue = 232;
const int activeHue = 70;

CRGB leds[NUM_LEDS];
int levelArray[NUM_LEDS];
int level = 0;
int sensorReading = 0;
int rate;
int frames;
int hue;
boolean on = true;

void setup() {
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(255);
  frames = 0;
  rate = maxRate;
  hue = restingHue;
  for (int i = 0; i < NUM_LEDS; i++) {
    levelArray[i] = 0;
  }
  FastLED.show();
}

void loop() {
  frames++;
  // toggle on and off to create the seperated light strips
  if (frames % 5 == 0) {
    on = !on;
  }

  for (int a = NUM_LEDS - 1; a > 0; a--) {
    levelArray[a] = levelArray[a - 1];
  }

  sensorReading = analogRead(knockSensor);

  if (sensorReading > threshold) {
    if (rate > hitIncrease) {
      rate -= hitIncrease;
      hue = map(rate, 1, 75, activeHue, restingHue);
    } else { // delay is at shortest possible
      rate = 1;
      hue = map(rate, 1, 75, activeHue, restingHue);
    }
  } else { // if not hit, increase delay until it's at max
    if (rate < maxRate) {
      rate++;
      hue = map(rate, 1, 75, activeHue, restingHue);
    }
  }

  if (on) {
    levelArray[0] = 255;
  } else {
    levelArray[0] = 0;
  }

  setLeds();
  FastLED.show();
  delay(rate);
}

void setLeds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, levelArray[i]);
  }
}
