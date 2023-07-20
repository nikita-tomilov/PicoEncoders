#include <Arduino.h>  
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

uint16_t colors[12] = {
    0x0000, // чёрный
    0xFFFF, // белый
    0xBDF7, // светло серый
    0x7BEF, // серый
    0xF800, // красный
    0xFFE0, // жёлтый
    0xFBE0, // оранжевый
    0x79E0, // коричневый
    0x7E0,  // зелёный
    0x07FF, // голубой
    0x001F, // синий
    0xF81F  // розовый
};

#include <Keyboard.h>

#include <EncButton2.h>
EncButton2<EB_ENCBTN> enc1(INPUT, 1, 0, 2); //A, B, button 
EncButton2<EB_ENCBTN> enc2(INPUT, 14, 15, 13); //A, B, button 

void print(String line) {
  tft.setTextFont(4);
  tft.setCursor(0, 0);
  tft.println(line);
}

void normalPress(uint8_t keycode) {
  Keyboard.press(keycode);
  delay(10);
  Keyboard.release(keycode);
}

void consumerPress(uint16_t keycode) {
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
  Serial.println("Init screen done");

  Keyboard.begin();
  Serial.println("Init USB done");

  byte color = 0;
  for(byte y=0; y<2; y++){
      for(byte x=0; x<6; x++){
          tft.fillRect(2+x*26, 2+y*38, 26, 38, colors[color]);
          color++;
      }
  }
}

void loop() {
  delay(1);
  enc1.tick();
  enc2.tick();

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
