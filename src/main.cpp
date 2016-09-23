#include "mbed.h"

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
int checkAdc(void);
void lightUp(void);
void toggleLED(void);
void runningLED(void);

int main() {
  // led ON is 0; OFF is 1;
  myled1 = 1;
  myled2 = 1;
  myled3 = 1;

  led.period_ms(10);

  // attach interrupts
  button2.rise(&toggleLED);
  runningLED();
  while(true) {
    if (button) {
      lightUp();
    }
  }
}

void toggleLED(void) {
  myled3 = !myled3;
  printf("Interrupted");
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

void lightUp(void) {
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
  printf("raw: %3.3f\n", ain.read());
  printf("percentage: %3.3f%%\n", ain.read()*100.0f);
  printf("normalized: 0x%04X \n", ain.read_u16());
  //led.pulsewidth_ms(ain.read()*1000);
  // FIXME: PWM writing ADC reading is not stable!
  led.write(adc);
  //led.pulsewidth_ms(3);
  wait(0.05f);
}

void runningLED(void) {
  for(int i=0; i<16; i++) {
    myleds = i;
    wait(0.25f);
  }
}
