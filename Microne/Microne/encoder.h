#include <stdint.h>


class Encoder
{
  private:
    int pina__, pinb__, pinc__;
    int8_t direction;

    void **button_pressed_cb;
    void **direction_cb;

    void handle_direction(void);

  public:
    Encoder(int pin_a, int pin_b, int button);
    void init(void);
    bool get_button_pressed(void);
    int set_direction_callback(void);

};
