#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, 8);

String dataIn = "";
char charIn = 'a';
String received = "";
String varName = "";
String data = "";
int split = 0;

int c = 0, b = 0, d = 0, p = 0;

int aR = 0, aG = 0, aB = 0;

void setup() {
  Serial.begin(9600);
  
  strip.begin();
  strip.clear();
}

void loop() {
  processIn();

  if(c == 0) { //Calculate Alliance RGB Values
    aR = 255;
    aG = 0;
    aB = 0;
  }
  if(c == 1) {
    aR = 0;
    aG = 0;
    aB = 255;
  }

  if(b == 1) { //Browned out? If so, blink green.
    pulse(3, 0, 255, 0);
  }
  else if(d == 1) { //Disabled? If so, monoBead alliance colors.
    monoBeads(1);
  }
  else {
    if(p == 0) {
      fastWipe(strip.Color(255, 0, 0));
    }
    if(p == 1) {
      fastWipe(strip.Color(0, 255, 0));
    }
    if(p == 2) {
      fastWipe(strip.Color(0, 0, 255));
    }
    if(p == 3) {
      fastWipe(strip.Color(255, 255, 255));
    }
  }
}

//-------------------------------------------------------Serial Code----------------------------------------------------------------------------------------------------

void processIn() {
  while(Serial.available()) {
    charIn = Serial.read();

    if(charIn == '|') {
      dataIn = "";
      received = "";
    }
    else if(charIn == '/') {
      received = dataIn;
      split = received.indexOf(':');
      varName = received.substring(0, split);
      data = received.substring(split + 1);

      processVariables();
    }
    else {
      dataIn += charIn;
    }
  }  
}

void processVariables() {
  if(varName == "c") { //Alliance Color: 0 = Red; 1 = Blue
      c = data.toInt();
  }
  if(varName == "b") { //Brownout? (Boolean)
      b = data.toInt();
  }
  if(varName == "d") { //Disabled? (Boolean)
      d = data.toInt();
  }
  if(varName == "p") { //Position: 0 = down, 1 = low, 2 = mid, 3 = high
      p = data.toInt();
  }
}

//-------------------------------------------------------LED Code----------------------------------------------------------------------------------------------------

void pulse(uint8_t wait, uint8_t r, uint8_t g, uint8_t b) {
  for(uint8_t i = 100; i > 0; i--) {
    strip.clear();
    for(uint8_t j = 0; j < strip.numPixels(); j++) {
      strip.setPixelColor(j, (int)(r*(i/100.0)), (int)(g*(i/100.0)), (int)(b*(i/100.0)));
    }
    strip.show();
    delay(wait);
  }
  for(uint8_t i = 0; i < 100; i++) {
    strip.clear();
    for(uint8_t j = 0; j < strip.numPixels(); j++) {
      strip.setPixelColor(j, (int)(r*(i/100.0)), (int)(g*(i/100.0)), (int)(b*(i/100.0)));
    }
    strip.show();
    delay(wait);
  }
}

void referenceRainbow() {
  double con = 255/strip.numPixels();
  strip.clear();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel((int)(10*i)));
  }
  strip.show();
}

void beads(uint8_t wait) {
  double con = 255/strip.numPixels();
  for(uint16_t j = 0; j < strip.numPixels(); j++) {
    for(uint16_t i = 0; i < (strip.numPixels() - j); i++) {
      strip.clear();
      //Draw End Piece
      for(uint16_t k = 59; k > (strip.numPixels() - j); k--) {
        strip.setPixelColor(k, Wheel((int)(con*k)));
      }
      //Draw dot
      strip.setPixelColor(i, Wheel((int)(con*(60-j))));

      strip.show();
      delay(wait*((j)/20));
    }
  }
}

void monoBeads(uint8_t wait) {
  double con = 255/strip.numPixels();
  for(uint16_t j = 0; j < strip.numPixels(); j++) {
    for(uint16_t i = 0; i < (strip.numPixels() - j); i++) {
      strip.clear();
      //Draw End Piece
      for(uint16_t k = 59; k > (strip.numPixels() - j); k--) {
        strip.setPixelColor(k, strip.Color(aR, aG, aB));
      }
      //Draw dot
      strip.setPixelColor(i, strip.Color(aR, aG, aB));

      strip.show();
      delay(wait*((j)/20));
    }
  }
}

void skittles(uint8_t wait) {
  int randint, color;
  randint = random(0,60);
  color = random(0, 255);
  strip.setPixelColor(randint, Wheel(color));
  strip.show();
  delay(wait);
}

void ant(uint8_t wait) { //Send a dot down to the end, morphing through the rainbow as it goes
  double con = 255/strip.numPixels();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.clear();
    strip.setPixelColor(i, Wheel((int)(con*i)));
    strip.show();
    delay(wait);
  }
}

void fastWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);  
  }
  strip.show();
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

