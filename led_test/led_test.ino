#include <Tiny85_TLC5940.h>
#define tick 50
int c_led = 0;
short m[] = {15, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 14};

void setup() {
  tlc5940.init();
  pinMode(PB3,INPUT);
}


void loop () {
  setPost(0,0);
}

void setPost (int x, int y) {
    int value = analogRead(PB3);
    value = value/(1024/15);
    value=value%15;
    tlc5940.set(m[value], 4000);
    for (int j = 0 ; j < 15; j++) {
      if (value != j) {
        tlc5940.set(m[j], 0);
      }
    }
    tlc5940.update();
}

