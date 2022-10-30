#include <stdint.h>


class Encoder
{
  private:
    int pina__, pinb__, pinc__;
    int8_t direction;
    bool pressed;  

  public:
    Encoder(int pin_a, int pin_b, int button);
    void handle_direction(void);
    void handle_press(void);
    void clear(void);
    void init(void);
    bool get_button_pressed(void);
    int get_direction(void);

};
