
#include "encoder.h"

enum
{
    ENCODER_A_PIN   = 2,
    ENCODER_B_PIN   = 4,
    ENCODER_BUTTON  = 3,
};


static Encoder encoder((int)ENCODER_A_PIN, (int)ENCODER_B_PIN, (int)ENCODER_BUTTON);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  encoder.init();
}

void loop() {
  switch (encoder.get_direction())
  {
    case -1:
      Serial.println("<-");
      delay(500);
      break;
    
    case 1:
      Serial.println("->");
      delay(500);
      break;
  }

  if (encoder.get_button_pressed())
  {
    Serial.println("O");
    delay(500);
  }
}

