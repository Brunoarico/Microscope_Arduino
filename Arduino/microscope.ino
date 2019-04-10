#include <Stepper.h>

#define STEPS 32
#define ONE_REV 2048.0
#define SCREW_DIST 1.0
#define RES 0.01
// create an instance of the stepper class using the steps and pins
Stepper stepper(STEPS, 8, 10, 9, 11);

float val = 0;
float led = 0;

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  setPwmFrequency(3, 1);
}

void off() {
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

int getcommand() {
  int ax = map (analogRead(A1), 0, 1023, -2, 3);
  led += map (analogRead(A0), 0, 1023, -1, 2)/100.;
  if(led > 254) led = 254;
  if(led < 0) led = 0;
  analogWrite(3, led);
  stepper.setSpeed(abs(ax)*200);
  if(ax < 0) val = -RES;
  else if(ax > 0) val = RES;
  else val = 0;
  return ax;
}

void loop() {
  getcommand();
  if(val !=0 ) {
    Serial.println((val/SCREW_DIST) * ONE_REV);
    stepper.step((val/SCREW_DIST) * ONE_REV);
    Serial.write("ok\n");
  }
  else off();
}
