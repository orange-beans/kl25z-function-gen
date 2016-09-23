#include "mbed.h"

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
AnalogIn ain(PTB1);

DigitalIn button(PTC7);

double delay = 0.5;

int checkAdc(void);

int main() {
  // led ON is 0; OFF is 1;
  myled1 = 1;
  myled2 = 1;
  myled3 = 1;
  while(true) {
    if (button) {
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
      printf("raw: %3.3f\n", ain.read());
      printf("percentage: %3.3f%%\n", ain.read()*100.0f);
      printf("normalized: 0x%04X \n", ain.read_u16());
      wait(0.2f);
    }
  }
}

int checkAdc(void) {
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
