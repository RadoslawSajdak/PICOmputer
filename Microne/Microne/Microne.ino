
#include "encoder.h"
#include "icons.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>

enum
{
  OLED_CLK          = 13,
  OLED_MOSI         = 11,
  OLED_CS           = 10,
  OLED_DC           = 8,
  OLED_RESET        = 9,
};

enum
{
  ENCODER_A_PIN   = 3,
  ENCODER_B_PIN   = 4,
  ENCODER_BUTTON  = 2,
};

enum
{
  PWM0_PIN = 6,
  PWM1_PIN = 5,
  DIGITAL0 = A0,
  DIGITAL1 = A1, 
};

enum
{
  MAX_IMGS          = 6,  
  SINGLE_IMG_WIDTH  = 16,
  SINGLE_IMG_HEIGHT = 23,
  X_OFFSET          = 4,
  X_SPACING         = 4,
  Y_OFFSET          = 4,
  BOTTOM_OFFSET     = 4,
  PWM_STEP          = 5
};

static void refresh_oled(uint8_t *funcions, uint8_t cursor);

static Encoder encoder((int)ENCODER_A_PIN, (int)ENCODER_B_PIN, (int)ENCODER_BUTTON);
static Adafruit_SSD1305 display(128, 32, &SPI, OLED_DC, OLED_RESET, OLED_CS, 7000000UL);

static const uint8_t *heat_icons[] = {heat_off, heat_on};
static const uint8_t **icon_array[] = {heat_icons, heat_icons, heat_icons, heat_icons, heat_icons, heat_icons}; 

static uint8_t map_cursor_to_pin(uint8_t cursor)
{
  switch (cursor) {
    case 0:
      return DIGITAL0;
    case 1:
      return DIGITAL1;
    case 2:
      return PWM0_PIN;
    case 3:
      return PWM1_PIN;
    default:
      return 200;
  }  

}
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DIGITAL0, OUTPUT);
  pinMode(DIGITAL1, OUTPUT);
  encoder.init();

  if ( ! display.begin(0x3C) ) 
  {
    Serial.println("Unable to initialize OLED");
    while (1) yield();
  }
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer
  display.display();
}

void loop() {
  static bool refresh = true;
  static bool direction_blocked = false;
  static uint8_t cursor = 0;
  static uint8_t function_array[MAX_IMGS] = {0};

  switch (encoder.get_direction())
  {
    case -1:
      if (!direction_blocked)
      {
        if (cursor > 0)
          cursor--;
      }
      else {
        if (function_array[cursor] >= PWM_STEP)
          function_array[cursor] -= PWM_STEP;
        analogWrite(map_cursor_to_pin(cursor), function_array[cursor]);
      }
      Serial.print("<- / pos:");
      Serial.print(cursor);
      delay(500);
      refresh = true;
      break;
    
    case 1:
      if (!direction_blocked)
      {
        if (cursor < MAX_IMGS - 1)
          cursor++;
      }
      else {
        if (function_array[cursor] <= 250 - PWM_STEP)
          function_array[cursor] += PWM_STEP;
        analogWrite(map_cursor_to_pin(cursor), function_array[cursor]);
      }
      Serial.print("-> / pos:");
      Serial.print(cursor);
      delay(500);
      refresh = true;
      break;
  }

  if (encoder.get_button_pressed())
  {
    Serial.println("O");
    delay(500);
    if (cursor <= 1)
    {
      function_array[cursor] = !function_array[cursor];
      digitalWrite(map_cursor_to_pin(cursor), function_array[cursor]);
    }
    else 
    {
      direction_blocked = !direction_blocked;
    }
    refresh = true;
  }

  if (refresh)
  {
    Serial.print(" Val: ");
    Serial.println(function_array[cursor]);
    refresh_oled(function_array, cursor);
    refresh = false;
  }
}

static void refresh_oled(uint8_t *funcions, uint8_t cursor)
{
  display.clearDisplay();   // clears the screen and buffer

  for (int i = 0; i < MAX_IMGS; i++)
  {
    display.drawBitmap(X_OFFSET +  i * SINGLE_IMG_WIDTH + i * X_SPACING, \
                        Y_OFFSET, \
                        icon_array[i][funcions[i]], \
                        SINGLE_IMG_WIDTH, SINGLE_IMG_HEIGHT, \
                        BLACK, WHITE);
  }

  display.drawRect(X_OFFSET + (SINGLE_IMG_WIDTH + X_SPACING) * cursor, 32 - BOTTOM_OFFSET, SINGLE_IMG_WIDTH, 2, WHITE);
  display.display();
}
