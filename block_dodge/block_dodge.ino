#include <Tiny85_TLC5940.h>
#define TICK 50
#define WIDTH 3
#define HEIGHT 5

unsigned long last_tick;

int key_stroke = 0; //0 = nothing, 1 = left, 2 = up, 3 = right

short mapping[] = {15, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 14};

bool frame[HEIGHT][WIDTH];

bool frame_buffer[HEIGHT][WIDTH];

void setup() {
  tlc5940.init();
  pinMode(PB3, INPUT);

  last_tick = millis();
}


void loop () {
  if (millis() - last_tick < TICK) {
    paint();
  } else {
    last_tick = millis();
    refreshBuffer();
    paint();
  }
}

void paint() {
  for (int i = 0 ; i < HEIGHT; i++) {
    for (int j = 0 ; j < WIDTH; j++) {
      if (frame[i][j]) {
        setLED(i, j, true);
      } else {
        setLED(i, j, false);
      }
    }
  }
  tlc5940.update();
}
void refreshBuffer() {
  for (int i = 0 ; i < HEIGHT; i++) {
    for (int j = 0 ; j < WIDTH; j++) {
      frame[i][j] = frame_buffer[i][j];
    }
  }
}
void setLED(int x, int y, bool value) {
  int index = x * WIDTH + y;
  index = mapping[index];
  if (value) {
    tlc5940.set(index, 2000);
  } else {
    tlc5940.set(index, 0);
  }
}
bool valid(int x, int y) {
  if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
    return false;
  }
  return true;
}

int buttonStatus() {
  int value = analogRead(PB3);
  value = value / (1024 / 15);
  int key = 0;
  switch (value) {
    case 11:
      key = 2;
      break;
    case 13:
      key = 1;
      break;
    case 12:
      key = 3;
  }
  return key;
}

