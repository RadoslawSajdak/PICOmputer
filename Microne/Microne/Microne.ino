
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
    ENCODER_A_PIN   = 2,
    ENCODER_B_PIN   = 4,
    ENCODER_BUTTON  = 3,
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
};

static void refresh_oled(bool *funcions, uint8_t cursor);

static Encoder encoder((int)ENCODER_A_PIN, (int)ENCODER_B_PIN, (int)ENCODER_BUTTON);
static Adafruit_SSD1305 display(128, 32, &SPI, OLED_DC, OLED_RESET, OLED_CS, 7000000UL);

static const uint8_t *heat_icons[] = {heat_off, heat_on};
static const uint8_t **icon_array[] = {heat_icons, heat_icons, heat_icons, heat_icons, heat_icons, heat_icons}; 

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  encoder.init();

  if ( ! display.begin(0x3C) ) 
  {
    Serial.println("Unable to initialize OLED");
    while (1) yield();
  }

  display.display(); // show splashscreen
  delay(1000);
  display.clearDisplay();   // clears the screen and buffer

  display.drawBitmap(4,4,heat_on, 16, 23, BLACK, WHITE);
  display.display();
}

void loop() {
  static bool refresh = false;
  static uint8_t cursor = 0;
  static bool function_array[MAX_IMGS] = {0};

  switch (encoder.get_direction())
  {
    case -1:
      Serial.println("<-");
      if (cursor > 0)
        cursor--;
      delay(500);
      refresh = true;
      break;
    
    case 1:
      Serial.println("->");
      if (cursor < MAX_IMGS)
        cursor++;
      delay(500);
      refresh = true;
      break;
  }

  if (encoder.get_button_pressed())
  {
    Serial.println("O");
    delay(500);
    function_array[cursor] = !function_array[cursor];
    refresh = true;
  }

  if (refresh)
  {
    refresh_oled(function_array, cursor);
    refresh = false;
  }
}

static void refresh_oled(bool *funcions, uint8_t cursor)
{
  static uint8_t x = X_OFFSET;

  display.clearDisplay();   // clears the screen and buffer

  for (int i = 0; i < MAX_IMGS; i++)
  {
    display.drawBitmap(X_OFFSET +  i * SINGLE_IMG_WIDTH + i * X_SPACING, \
                        Y_OFFSET, \
                        icon_array[i][funcions[i]], \
                        SINGLE_IMG_WIDTH, SINGLE_IMG_HEIGHT, \
                        BLACK, WHITE);
  }

  display.drawRect(x + (SINGLE_IMG_WIDTH + X_SPACING) * cursor, 32 - BOTTOM_OFFSET, SINGLE_IMG_WIDTH, 2, WHITE);
  display.display();
}
