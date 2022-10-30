#include "encoder.h"
#include <stdint.h>
#include <Arduino.h>

enum
{
    DEBOUNCE_TIME   = 500,
};

void blink(void)
{
  static bool state = false;
  digitalWrite(LED_BUILTIN, state);
  state = !state;
}
void debounced_irq(void)
{
  static uint32_t lastTime = 0U;
  uint32_t currentTime = millis();
  if (currentTime - lastTime > DEBOUNCE_TIME)
  {
    blink();
  }
  lastTime = currentTime;
}

void Encoder::handle_direction(void)
{
  if (digitalRead(this->pinb__))
  {
    this->direction = -1;
  }
  else 
  {
    this->direction = 1;
  }

}

Encoder::Encoder(int pinA, int pinB, int button)
{
  this->pina__ = pinA;
  this->pinb__ = pinB;
  this->pinc__ = button;
}

void Encoder::init()
{
  pinMode(this->pina__, INPUT_PULLUP);
  pinMode(this->pinb__, INPUT_PULLUP);
  pinMode(this->pinc__, INPUT_PULLUP);

  TCCR1A = 0;
  TCCR1B = (1<<CS12) | (1<<WGM12);
  OCR1A = 31249;
  TIMSK1 = (1<<OCIE1A);

  attachInterrupt(digitalPinToInterrupt(this->pinc__), debounced_irq, LOW);
}

bool Encoder::get_button_pressed(void)
{
  return true;  
}

int Encoder::set_direction_callback(void)
{
  return 0;
}

ISR(TIMER1_COMPA_vect)
{
  static bool state = false;
  digitalWrite(LED_BUILTIN, state);
  state = !state;
}
