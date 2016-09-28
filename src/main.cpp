#include "mbed.h"
// #include "USBMouse.h"
//
// USBMouse mouse(ABS_MOUSE);

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
AnalogIn ain(PTB1);

// Interrupt only support on PORT A/D
DigitalIn button(PTC7);
InterruptIn button2(PTA13);

// Busout
BusOut myleds(LED1, LED2, LED3);

// PWM
PwmOut led(PTD5);

double delay = 0.5;

// Function prototypes
int checkAdc();
void lightUp(void);
void toggleLED(void);
void runningLED(void);
void flash(int n);

// Study create a Flasher class
class Flasher {
private:
  DigitalOut _pin;

public:
  Flasher(PinName pin) : _pin(pin) {

  }

  void flash(int n) {
    for(int i=0; i<n*2; i++) {
      _pin = !_pin;
      wait(0.2);
    }
  }
};

// Instanlize
Flasher flash1(LED2);
Flasher flash2(LED3);

int main() {
  // mouse testing
  // uint16_t x_center = (X_MAX_ABS - X_MIN_ABS)/2;
  // uint16_t y_center = (Y_MAX_ABS - Y_MIN_ABS)/2;
  // uint16_t x_screen = 0;
  // uint16_t y_screen = 0;
  //
  // uint32_t x_origin = x_center;
  // uint32_t y_origin = y_center;
  // uint32_t radius = 5000;
  // uint32_t angle = 0;

  // led ON is 0; OFF is 1;
  myled1 = 1;
  myled2 = 1;
  myled3 = 1;

  led.period_ms(10);

  // attach interrupts
  button2.rise(&toggleLED);
  //runningLED();

  flash1.flash(5);
  flash2.flash(2);

  while(true) {
    if (button) {
      // x_screen = x_origin + cos((double)angle*3.14/180.0)*radius;
      // y_screen = y_origin + sin((double)angle*3.14/180.0)*radius;
      //
      // mouse.move(x_screen, y_screen);
      // angle += 3;
      lightUp();
    }
  }
}


// No printf in ISR
void toggleLED() {
  myled3 = !myled3;
  //printf("Interrupted");
}

int checkAdc() {
  if (ain >= 0.6f) {
    return 3;
  }
  if (ain >= 0.3f && ain < 0.6f) {
    return 2;
  }
  else {
    return 1;
  }
}

void lightUp() {
  float adc = ain.read();
  switch (checkAdc()) {
    case 1:
      myled1 = 0;
      myled2 = 1;
      myled3 = 1;
      break;
    case 2:
      myled1 = 1;
      myled2 = 0;
      myled3 = 1;
      break;
    case 3:
      myled1 = 1;
      myled2 = 1;
      myled3 = 0;
      break;
    default:
      break;
  }
  printf("raw: %3.3f\n", adc);
  printf("percentage: %3.3f%%\n", adc*100.0f);
  printf("normalized: 0x%04X \n", ain.read_u16());
  //led.pulsewidth_ms(ain.read()*1000);
  // FIXME: PWM writing ADC reading is not stable!
  led.write(adc);
  //led.pulsewidth_ms(3);
  wait(0.5f);
}

void runningLED() {
  for(int i=0; i<16; i++) {
    myleds = i;
    wait(0.25f);
  }
}
