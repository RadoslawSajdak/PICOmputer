#include "encoder.h"
#include <stdint.h>
#include <Arduino.h>

enum
{
    DEBOUNCE_TIME   = 500,
    ENCODER_DEBOUNCE_TIME = 100,
};

Encoder *encoder_pointer;

static void debounced_irq(void)
{
  static uint32_t lastTime = 0U;
  uint32_t currentTime = millis();
  if (currentTime - lastTime > DEBOUNCE_TIME)
  {
    encoder_pointer->handle_press();    
  }
  lastTime = currentTime;
}

static void encoder_irq(void)
{
  static uint32_t lastTime = 0U;
  uint32_t currentTime = millis();
  if (currentTime - lastTime > ENCODER_DEBOUNCE_TIME)
  {
    encoder_pointer->handle_direction();
  }
  lastTime = currentTime;
}

static void clear_direction(void)
{
  encoder_pointer->clear();
}

void Encoder::clear(void)
{
  this->direction = 0;
  this->pressed = false;
}

void Encoder::handle_direction(void)
{
  if (digitalRead(this->pinb__))
  {
    this->direction = 1;
  }
  else 
  {
    this->direction = -1;
  }

}

void Encoder::handle_press(void)
{
  this->pressed = true;
}

Encoder::Encoder(int pinA, int pinB, int button)
{
  this->pina__ = pinA;
  this->pinb__ = pinB;
  this->pinc__ = button;
}

void Encoder::init(void)
{
  pinMode(this->pina__, INPUT_PULLUP);
  pinMode(this->pinb__, INPUT_PULLUP);
  pinMode(this->pinc__, INPUT_PULLUP);

  TCCR1A = 0;
  TCCR1B = (1<<CS12) | (1<<WGM12);
  OCR1A = 6250;
  TIMSK1 = (1<<OCIE1A);
  encoder_pointer = this;
  attachInterrupt(digitalPinToInterrupt(this->pinc__), debounced_irq, LOW);
  attachInterrupt(digitalPinToInterrupt(this->pina__), encoder_irq, FALLING);
}

bool Encoder::get_button_pressed(void)
{
  return this->pressed;  
}

int Encoder::get_direction()
{
  return this->direction;
}

ISR(TIMER1_COMPA_vect)
{
  clear_direction();
}
