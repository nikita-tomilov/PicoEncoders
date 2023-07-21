#include <Arduino.h>  
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

uint16_t colors[12] = {
    0x0000,
    0xFFFF, 
    0xBDF7, 
    0x7BEF, 
    0xF800, 
    0xFFE0, 
    0xFBE0, 
    0x79E0, 
    0x7E0,  
    0x07FF, 
    0x001F, 
    0xF81F 
};

#include <Keyboard.h>

#include <EncButton2.h>
EncButton2<EB_ENCBTN> enc1(INPUT, 1, 0, 2); //A, B, button 
EncButton2<EB_ENCBTN> enc2(INPUT, 14, 15, 13); //A, B, button 

int backlightPin = 25;
int backlightBrightness = 1024;

long lastKeyPressTimestamp = millis();
String previousLine = "";

void resetScreen() {
  byte color = 0;
  for(byte y=0; y<2; y++){
      for(byte x=0; x<6; x++){
          tft.fillRect(2+x*26, 2+y*38, 26, 38, colors[color]);
          color++;
      }
  }
}

void print(String line) {
  if (line.compareTo(previousLine) != 0) {
    resetScreen();
    tft.setTextFont(4);
    tft.setCursor(0, 0);
    tft.println(line);
  }
  previousLine = line;
}

void keyPressDetected() {
  lastKeyPressTimestamp = millis();
}

void normalPress(uint8_t keycode) {
  keyPressDetected();
  Keyboard.press(keycode);
  delay(10);
  Keyboard.release(keycode);
}

void consumerPress(uint16_t keycode) {
  keyPressDetected();
  Keyboard.consumerPress(keycode);
  delay(10);
  Keyboard.consumerRelease();
}

void setup(void) {
  Serial.begin(115200);

  Serial.println("Init screen");
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  print("Hello!");
  Serial.println("Init screen done");

  Keyboard.begin();
  Serial.println("Init USB done");

  analogWriteFreq(1000);
  analogWriteRange(1024);
  pinMode(backlightPin, OUTPUT);
}

void loop() {
  delay(1);
  enc1.tick();
  enc2.tick();

  analogWrite(backlightPin, backlightBrightness);
  if (millis() - lastKeyPressTimestamp < 3000) {
    backlightBrightness = 1024;
  } else {
    if (backlightBrightness > 0) {
      backlightBrightness -= 1;
    }
  }

  if (enc1.press() || enc2.press()) {
    keyPressDetected();
  }

  if (enc1.click()) {
    print("Press 1");
    consumerPress(KEY_PLAY_PAUSE);
  }

  if (enc1.left()) {
    print("Left 1 ");
    consumerPress(KEY_VOLUME_DECREMENT);
  }

  if (enc1.right()) {
    print("Right 1");
    consumerPress(KEY_VOLUME_INCREMENT);
  }

  if (enc2.click()) {
    print("Press 2");
    normalPress(KEY_RETURN);
  }

  if (enc2.left()) {
    print("Left 2 ");
    normalPress(KEY_UP_ARROW);
  }

  if (enc2.right()) {
    print("Right 2");
    normalPress(KEY_DOWN_ARROW);
  }
}
