
#include "encoder.h"

enum
{
    ENCODER_A_PIN   = 2,
    ENCODER_B_PIN   = 4,
    ENCODER_BUTTON  = 3,
};


static Encoder encoder((int)ENCODER_A_PIN, (int)ENCODER_B_PIN, (int)ENCODER_BUTTON);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  encoder.init();
  // TCCR1A = 0;
  // TCCR1B = (1<<CS12) | (1<<WGM12);
  // OCR1A = 31249;
  // TIMSK1 = (1<<OCIE1A);
}

void loop() {
  encoder.get_button_pressed();
}

